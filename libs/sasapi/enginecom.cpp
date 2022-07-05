#include "enginecom.h"
#include "utils.h"

#if !defined(__linux__)
    #include <process.h>
#endif

#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <climits>

// **********************************************************************
// Logging
//#define _LOG_TX
//#define _LOG_RX

// TIMEOUTS
#define WAITANS_TO      1000    // 1s
#define MAX_RETRIES     3
#define IDLE_SLEEP      5       // 5ms

/**
 * @brief Calcula el CRC CCITT16 con semilla fija 55AAh
 *
 * @param _buffer >Buffer sobre el cual calcular
 * @param _size Cantidad de bytes del buffer a tener en cuenta
 * @return uint16_t Valor del CRC
 */
static uint16_t calcCRC(const uint8_t* _buffer, const uint32_t _size)
{
    uint32_t c, q;
    uint16_t crcVal = 0x55AA;

    for (int32_t ix = 0; ix < _size; ++ix)
    {
        c = _buffer[ix];
        q = (crcVal ^ c) & 017;
        crcVal = (crcVal >> 4) ^ (q * 010201);
        q = (crcVal ^ (c >> 4)) & 017;
        crcVal = (crcVal >> 4) ^ (q * 010201);
    }

    return (crcVal);
}

/**
 * @brief Manda los bytes de un paquete al log
 *
 * @param _dir String libre a interponer a la cadena de bytes
 * @param _buff Buffer a loguear
 * @param _size Cantidad de bytes a loguear
 */
static void logPkt(const char* _dir, const uint8_t* _buff, const uint32_t _size)
{
    std::string logBuff;

    for (uint32_t ix = 0; ix < _size; ++ix)
    {
        char aux[4];

        sprintf(aux, "%02X ", _buff[ix]);
        logBuff += aux;
    }

    LOG_DEBUG(_dir, logBuff.c_str());
}

namespace EngineAPI
{
    /**
     * @brief Inicializa el sistema de comunicaciones con el engine
     * 
     * @param _cfg Estructura de configuracion
     * @return true Si inicializo
     * @return false Si no pudo inicializar
     */
    bool EngineCom::init(const configSE_t& _cfg)
    {
#if defined(__linux__)
        assert(!_cfg.device.empty() && "El puerto serial debe tener un nombre");
#else
        assert(_cfg.portNbr && "El puerto COM debe ser > 0");
#endif

        configSE = _cfg;
        if (configSE.rxTimeOut < WAITANS_TO) configSE.rxTimeOut = WAITANS_TO;

        // Inicializamos el puerto serie
        return initCOMPort();
    }

    /**
     * @brief Desinicializa el sistema de comunicaciones con el engine
     * 
     */
    void EngineCom::deInit()
    {
        deinitCOMPort();
    }

    /**
     * @brief Inicializa el puerto Serial segun la config. No bloqueante
     *
     * @return true Si inicializo
     * @return false Si no inicializo
     */
    bool EngineCom::initCOMPort() noexcept
    {
        // Creamos el ID del device
        char devID[256];
        bool sal = false;

#ifdef __linux__
        strcpy(devID, configSE.device.c_str());
#else
        if (configSE.portNbr < 10)
        {
            sprintf(devID, "COM%u", configSE.portNbr);
        }
        else
        {
            sprintf(devID, "\\\\.\\COM%u", configSE.portNbr);
        }
#endif

        try
        {
            pSerial = new serialPort;
            sal = pSerial->open(devID, configSE.portCfg);
            LOG_DEBUG("sal: %s opening serial port %s : %s", sal ? "true" : "false", devID, configSE.portCfg);
        }
        catch (const std::exception& _ex)
        {
            delete pSerial;
            pSerial = nullptr;
            LOG_ERROR("Error creating serial port: %s", _ex.what());
        }

        return sal;
    }

    /**
     * @brief Desinicializa el puerto serie
     * 
     */
    void EngineCom::deinitCOMPort() noexcept
    {
        try
        {
            if (pSerial)
            {
                pSerial->close();
                delete pSerial;
                pSerial = nullptr;
            }

            LOG_DEBUG("deinitCOMPort");
        }
        catch (...)
        {

        }
    }


    /**
     * @brief Envia el comando Hello al SE. Este comando determina si la config del SE
     * debería ser actualizada o no y el estado de la NVRam. El SE puede tener una config
     * no valida debido a errores en la grabación o a que nunca fue actualizada. Tambien puede tener
     * una config valida, pero desactualizada
     * 
     * @param _validCfg 1 si la config del SE es valida, 0 si no lo es
     * @param _crc CRC CCITT16 de la config del SE
     * @param _nv true si la NVRam esta OK, false si no
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendHello(uint8_t& _validCfg, uint16_t& _crc, bool& _nv)
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_HELLO;
        TXCMD(bo)->len = 0;
        if (notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
            if (sal)
            {
                const command_t* rxCmd = RXCMD(bi);
                _validCfg = rxCmd->payload[1];
                memcpy(&_crc, &rxCmd->payload[2], 2);
                _nv = rxCmd->payload[4] ? true : false;
            }
        }

        if (sal)
            LOG_DEBUG("sendHello-> cfg: %u - CRC: %u - NV: %s", _validCfg, _crc, _nv ? "OK" : "NOK");
        else
            LOG_DEBUG("sendHello->false");

        return sal;
    }

    /**
     * @brief Envia el comando READY al SE. Este comando envia el SEID y el SE lo compara
     * contra el propio. Si la comparacion coincide, los canales SAS son habilitados
     * sino, el equipo se bloquea y deja de responder comandos hasta ser reseteado
     * (power off, power on). Notese que de fallar la comparacion, este comando
     * va a retornar false porque nunca se recibe respuesta.
     * Este comando es rechazado en caso que Hello haya contestado que la NV esta mal.
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendReady()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_READY;
        TXCMD(bo)->len = 3 * sizeof(uint32_t);
        memcpy(TXCMD(bo)->payload, &configSE.SEID, sizeof(configSE.SEID));
        if (notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendReady->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia el comando de reset de NV. Luego ded respondido este comando
     * el equipo debe ser reiniciado
     *
     * @return true Si pudo resetear
     * @return false si no pudo
     */
    bool EngineCom::sendResetNVRam()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_NVRESET;
        TXCMD(bo)->len = 0;
        if (notifyEngine(bo, bi, UINT_MAX)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendResetNVRam->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia un paquete por el puerto serie
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendPkt(commData_t& _cd)
    {
        bool sal = false;

        if (pSerial)
        {
            pSerial->clear();
            if (pSerial->send(_cd.txBuff, _cd.txSize) == eDevError)
            {
                LOG_ERROR("Error sending data to SE");
            }
            else
            {
                sal = true;
            }
        }

        return sal;
    }

    /**
     * @brief Recibe un paquete desde el puerto serial
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::recvPkt(commData_t& _cd)
    {
        bool sal = false;
        bool work = true;

        if (!pSerial) return false;

        uint32_t initTime = getOSTick();

        do
        {
            uint8_t buff[MAX_TXRX];
            uint32_t readed = 0;

            eDevResult res = pSerial->recv(buff, MAX_TXRX, readed);
            switch (res)
            {
            case eDevError:
                sal = false;
                work = false;
                break;

            case eDevOk:
                sal = rxFsm(_cd, buff, readed);
                work = !sal;
                break;

            default:
                osWait(1);
                break;
            }

            if (configSE.rxTimeOut != UINT_MAX)
            {
                // Chequeamos timeout
                if ((getOSTick() - initTime) >= configSE.rxTimeOut)
                {
                    work = false;
                }
            }
        } while(work);

        return sal;
    }

    /**
     * @brief Maquina de estado de recepcion de comandos
     * 
     * @param _buff Buffer RX
     * @param _size Cantidad de bytes en el buffer
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::rxFsm(commData_t& _cd, const uint8_t* _buff, const uint32_t _size)
    {
        for (uint32_t ix = 0; ix < _size; ++ix)
        {
            switch (_cd.decoState)
            {
            case engFSMState::esIdle:
                _cd.size.a[_cd.rxSize++] = _buff[ix];
                if (_cd.rxSize == 2)
                {
                    if (_cd.size.val >= MAX_TXRX)
                    {
                        LOG_DEBUG("%u >= %u", _cd.size.val, MAX_TXRX);
                        // Se descarta el comando
                        _cd.rxSize = 0;
                        return false;
                    }
                    else
                    {
                        // Tengo el tamaño total a recibir
                        _cd.rxSize = 0;
                        _cd.decoState = engFSMState::esCmd;
                    }
                }
                break;

            case engFSMState::esCmd:
                _cd.rxBuff[_cd.rxSize++] = _buff[ix];
                if (_cd.rxSize == _cd.size.val)
                {
                    _cd.rxSize = 0;
                    _cd.decoState = engFSMState::esIdle;
                    return true;
                }
                break;
            }
        }

        return false;
    }

    /**
     * @brief Comprueba la integridad de un comando recibido, chequeando el CRC
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::isValidCmd(commData_t& _cd) noexcept
    {
        const command_t* pCmd = RXCMD(_cd.rxBuff);

        if (pCmd->len)
        {
            const uint16_t crc = sasCRC(pCmd->payload, pCmd->len, 0x55AA);
            return (crc == pCmd->crc);
        }

        return true;
    }

    /**
     * @brief Envia un comando y recibe la respuesta
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendAndRecv(commData_t& _cd)
    {
        std::unique_lock<std::mutex> lock(mtxPort);

        do
        {
            if (!sendPkt(_cd)) return false;
            
            ++_cd.errCount;
            if (!recvPkt(_cd))
            {
                if (_cd.errCount == 3)
                {
                    _cd.errCount = 0;
                    return false;
                }
                else
                {
                    continue;
                }
            }

            _cd.errCount = 0;
            if (!isValidCmd(_cd))
            {
                LOG_DEBUG("Invalid command CRC");
                return false;
            }
            
            return (TXCMD(_cd.txBuff)->cmd == RXCMD(_cd.rxBuff)->cmd);
        } while (true);
    }

    /**
     * @brief Envia el comando al Engine
     * 
     * @param _to Time Out a utilizar. Por default se utiliza el configurado en Init
     * @return true Se envio y el engine lo proceso OK
     * @return false O no se pudo enviar o el engine lo rechazo
     */
    bool EngineCom::notifyEngine(uint8_t* _out, uint8_t* _in, const uint32_t _to)
    {
        auto bkp = configSE.rxTimeOut;
        bool sal = false;
        commData_t cd;

        cd.txBuff = _out;
        cd.rxBuff = _in;

        if (makeFromCmd(cd))
        {
            if (_to) configSE.rxTimeOut = _to;
            sal = sendAndRecv(cd);
            if (_to) configSE.rxTimeOut = bkp;
        }

        return sal;
    }

    /**
     * @brief Genera un paquete de Tx a partir del comando almacenado
     * 
     * @return true si pudo generar el comando, false si no
     */
    bool EngineCom::makeFromCmd(commData_t& _cd)
    {
        command_t* pCmd = TXCMD(_cd.txBuff);
        const uint16_t size = sizeof(command_t) + pCmd->len;

        memcpy(_cd.txBuff, &size, 2);
        if (pCmd->len)
        	pCmd->crc = calcCRC(pCmd->payload, pCmd->len);
        else
            pCmd->crc = 0;
        _cd.txSize = size + 2;
        return true;
    }

    /**
     * @brief Obtiene el SEID (identificador unico de la placa de 96 bits)
     * 
     * @param _seid Buffer donde almacenar el SEID (3 uint32_t)
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::getSEID(uint8_t* _seid)
    {
        assert(_seid && "_seid es NULL");
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_UID;
        TXCMD(bo)->len = 0;
        if (notifyEngine(bo, bi))
        {
            const command_t* pCmd = RXCMD(bi);
            sal = (pCmd->len == 3 * sizeof(uint32_t));
            if (sal) memcpy(_seid, pCmd->payload, pCmd->len);
        }

        LOG_DEBUG("getSEID->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia el comando de Reset (warm reset). Este comando hace un reboot de la placa
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendReset()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_RESET;
        TXCMD(bi)->len = 0;
        if (notifyEngine(bo, bi, 3000)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendReset->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Envia la fecha y hora al SE
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendFyH()
    {
        time_t now = time(nullptr);
        struct tm* lt = localtime(&now);
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_FYH;
        TXCMD(bo)->len = 7;
        TXCMD(bo)->payload[0] = lt->tm_mday;
        TXCMD(bo)->payload[1] = lt->tm_mon + 1;
        TXCMD(bo)->payload[2] = lt->tm_year - 100;
        TXCMD(bo)->payload[3] = lt->tm_hour;
        TXCMD(bo)->payload[4] = lt->tm_min;
        TXCMD(bo)->payload[5] = lt->tm_sec;
        TXCMD(bo)->payload[6] = lt->tm_wday + 1;
        if (notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendFyH->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia una consulta al SE. Este comando permite que el SE reporte a la PC
     * informacion aportada por SAS o solicitudes de este.
     * 
     * @param _cmd Comando retornado por el SE (_cmd.cmd -> CMD_ACK nada que reportar)
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool EngineCom::sendQuery(pcCmd& _cmd)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_QUERY;
        TXCMD(bo)->len = 0;
        if (notifyEngine(bo, bi))
        {
            const command_t* pCmd = RXCMD(bi);
            if (pCmd->len) 
            {
                memcpy(&_cmd, pCmd->payload, pCmd->len);
                sal = true;
            }
        }

        // Lo comente porque como sale varias veces x segundo, genera bardo en el log
        // descomentarlo si es necesario
        //LOG_DEBUG("sendQuery()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Envia una escritura absoluta a NVRam. Usado para testeo.
     * Solo funciona antes del comando Ready
     *
     * @param _arr Array de bytes a enviar
     * @param _size Cantidad de bytes a enviar
     * @param _addr Direccion de memoria donde almacenar
     * @return true Pudo almacenar
     * @return false No pudo almacenar
     */
    bool EngineCom::sendNVData(const uint8_t* _arr, const uint16_t _size, const uint16_t _addr)
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_TESTING;
        TXCMD(bo)->len = _size + 5;
        TXCMD(bo)->payload[0] = SCMD_TST_NVW; // Escribir en NV
        memcpy(&TXCMD(bo)->payload[1], &_addr, 2);
        memcpy(&TXCMD(bo)->payload[3], &_size, 2);
        memcpy(&TXCMD(bo)->payload[5], _arr, _size);

        if (notifyEngine(bo, bi, 5000)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendNVData(xxx, %u, %u)->%s", _size, _addr, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Hace una lectura absoluta desde NVRam. Usado para testeo.
     * Solo funciona antes del comando Ready
     *
     * @param _arr Array de bytes a recibir
     * @param _size Cantidad de bytes a recibir
     * @param _addr Direccion de memoria desde donde recibir
     * @return true Recibio los bytes
     * @return false No recibio los bytes
     */
    bool EngineCom::recvNVData(uint8_t* _arr, const uint16_t _size, const uint16_t _addr)
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_TESTING;
        TXCMD(bo)->len = 5;
        TXCMD(bo)->payload[0] = SCMD_TST_NVR; // Leer desde NV
        memcpy(&TXCMD(bo)->payload[1], &_addr, 2);
        memcpy(&TXCMD(bo)->payload[3], &_size, 2);

        if (notifyEngine(bo, bi, 5000))
        {
        	sal = (RXCMD(bi)->payload[0] == CMD_ACK);
            if (sal)
            {
                memcpy(_arr, &RXCMD(bi)->payload[1], _size);
            }
        }

        LOG_DEBUG("recvNVData(xxx, %u, %u)->%s", _size, _addr, sal ? "true" : "false");

        return sal;
    }


    /**
     * @brief Habilita / deshabilita el log de paquetes Tx y Rx de SAS
     * @param _ena true -> Habilita, false -> Deshabilita
     * @return true Si se pudo enviar el comando
     * @return false Si no se pudo enviar
     */
    bool EngineCom::protocolDebugger(const bool _ena)
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_PROTODEBUG;
        TXCMD(bo)->len = 1;
        TXCMD(bo)->payload[0] = _ena ? 1 : 0;
        if (notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("protocolDebugger(%s)->%s", _ena ? "true" : "false", sal ? "true" : "false");
        return sal;
    }
}

