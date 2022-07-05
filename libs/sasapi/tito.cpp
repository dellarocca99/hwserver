#include "tito.h"
#include "engconfig.h"
#include "utils.h"

#include <cassert>
#include <cstring>

#define MAX_SYSID_WAIT  10000   // 10s

namespace EngineAPI
{
	void TITO::init(std::shared_ptr<EngineCom>& _eCom)
	{
		assert(_eCom && "_eCom no valido");
		eCom = _eCom;
	}

    /**
     * @brief Envia el ticket impreso al SE. Solo valido en Validation STD
     * 
     * @param _validNbr Numero de validacion calculado por el equipo
     * @param _monto Monto del ticket en centavos
     * @return true Pudo enviar el ticket
     * @return false No pudo enviar el ticket
     */
    bool TITO::sendCashoutTktInfoStd(const uint32_t _validNbr, const uint64_t _monto)
    {
        assert((engConfig::init()->getValidationType() == SAS_VAL_STD) && "Solo permitido con validation STD");
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        valRecord* tkt = reinterpret_cast<valRecord*>(TXCMD(bo)->payload);
        bool sal = false;

        memset(tkt, 0, sizeof(valRecord));
        numToBCD(_monto, tkt->amount, 5);
        numToBCD(_validNbr, tkt->validationNbr, 4);
        TXCMD(bo)->cmd = CMD_SENDTKTSTD;
        TXCMD(bo)->len = sizeof(*tkt);
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendCashoutTktInfoStd(%lu, %llu)->%s", _validNbr, _monto, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Retorna los datos de validacion para generacion de tickets en modo Secure Enchanced
     * 
     * @param _secEn Estructura con los datos. Valida solo si el metodo retorna true. Es importante
     * destacar que hasta no tener estos datos el equipo no debe permanecer deshabilitado.
     * @return true Pudo recuperar los datos
     * @return false No pudo recuperar
     */
    bool TITO::getValidationID(secureEnhValid& _secEn)
    {
        assert((engConfig::init()->getValidationType() == SAS_VAL_SE) && "Solo funciona en Secure Enhanced");

        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_GETVALINF;
        TXCMD(bo)->len = sizeof(secureEnhValid);
        memcpy(TXCMD(bo)->payload, &_secEn, sizeof(secureEnhValid));
        if (eCom->notifyEngine(bo, bi))
        {
            const command_t* pCmd = RXCMD(bi);
            sal = (pCmd->payload[0] == CMD_ACK);
            if (sal) memcpy(&_secEn, &pCmd->payload[1], sizeof(_secEn));
        }

        return sal;
    }

    /**
     * @brief Envia un nuevo registro de validacion al SE
     * 
     * @param _vt Tipo de validacion (ver Tipos de validacion para Registro)
     * @param _amt Monto del ticket en centavos
     * @param _tktNbr Numero de ticket
     * @param _valNbr Validation Number (obtenido mediante calcValidationSE o getValidationSYS)
     * @param _exp Fecha de expiracion impresa:
     * MMDDYYYY -> Fecha o
     * 00000001..00009998 -> Dias hasta expiracion o 
     * 00009999 -> No expira nunca o
     * 00000000 -> No se imprimio ticket o extension de validacion no soportada
     * @return true Pudo enviar y almacenar el registro
     * @return false No pudo reportar el registro
     */
    bool TITO::sendValidationRecord(
            const uint8_t _vt, 
            const uint64_t _amt, 
            const uint16_t _tktNbr,
            const valNumber_t& _valNbr,
            const std::vector<uint8_t>& _exp
        )
    {
        assert((engConfig::init()->getValidationType() != SAS_VAL_STD) && "No valido con validacion STD");

        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        valRecord* vr = reinterpret_cast<valRecord*>(TXCMD(bo)->payload);
        std::string aux;
        bool sal = false;

        memset(vr, 0, sizeof(valRecord));
        vr->type = _vt;
        numToBCD(_amt, vr->amount, 5);
        vr->number = _tktNbr;
        memcpy(vr->validationNbr, _valNbr.data(), 8);
        std::copy(std::begin(_exp), std::end(_exp), std::begin(aux));
        auto expVal = atoll(aux.c_str());
        numToBCD(expVal, vr->expiration, 4);
        vr->poolID = 0x00;
        bcdDateTime(vr->fecha, vr->hora);
        TXCMD(bo)->cmd = CMD_SENDVALRCD;
        TXCMD(bo)->len = sizeof(valRecord);
        if (eCom->notifyEngine(bo, bi, 2000)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG(
        	"sendValidationRecord(%u, %llu, %u, %llu)->%s",
            _vt,
            _amt,
            _tktNbr,
            bcdToNum(vr->validationNbr, 8),
            sal ? "true" : "false"
            );

        return sal;
    };

    /**
     * @brief Calcula el Validation number de un ticket segun el algoritmo STD
     * 
     * @param _monto Monto en centavos del ticket
     * @return uint32_t Validation number
     */
    uint32_t calcValidationStd(const uint64_t _monto)
    {
        uint8_t bcdM[3], bcdT[3], bcd[4];
        const time_t now = time(nullptr);
        tm* lt = localtime(&now);

        // Tomamos los 3 ultimos bytes BCD del monto
        numToBCDR(_monto, bcdM, 3);

        numToBCD(lt->tm_hour, &bcdT[2], 1);
        numToBCD(lt->tm_min, &bcdT[1], 1);
        numToBCD(lt->tm_sec, &bcdT[0], 1);

        // Convertimos y sumamos
        const uint64_t sum1 = bcdToNumR(bcdM, 3);
        const uint64_t sum2 = bcdToNumR(bcdT, 3);
        const uint64_t res = sum1 + sum2;

        // Convertimos
        numToBCDR(res, bcd, 4);

        // Pasamos el LSB al MSB
        bcd[3] = bcd[0];

        // retornamos el valor
        uint32_t* pVal = reinterpret_cast<uint32_t*>(bcd);
        return *pVal;
    }

    /**
     * @brief Calcula el validation number de un ticket segun el algoritmo
     *  Secure Enhanced
     * 
     * @param _secEn Datos secure enhanced
     * @param _valNbr Buffer donde retornar el Validation number en BCD
     * @param _sValNbr String con el validation number
     */
    void calcValidationSE(const secureEnhValid& _secEn, uint8_t* _valNbr)
    {
        union u32to8
        {
            uint32_t u32;
            uint8_t u8[4];
        };
        uint8_t buffA[6], buffB[6], buffC[6];
        int8_t n[20], v[20];

        // Construyo el array A
        buffA[5] = (_secEn.id & 0x00ff0000) >> 16;
        buffA[4] = (_secEn.id & 0x0000ff00) >> 8;
        buffA[3] = (_secEn.id & 0x000000ff);
        buffA[2] = (_secEn.seqNbr & 0x00ff0000) >> 16;
        buffA[1] = (_secEn.seqNbr & 0x0000ff00) >> 8;
        buffA[0] = (_secEn.seqNbr & 0x000000ff);
        //numToBCDR(_secEn.seqNbr, buffA, 3);
        //numToBCDR(_secEn.id, &buffA[3], 3);

        // Construyo array B
        buffB[0] = buffA[0];
        buffB[1] = buffA[1];
        buffB[2] = buffA[2] ^ buffA[0];
        buffB[3] = buffA[3] ^ buffA[1];
        buffB[4] = buffA[4] ^ buffA[0];
        buffB[5] = buffA[5] ^ buffA[1];

        // Construyo el array C
        for (uint32_t i = 0; i < 3; ++i)
        {
            uint16_t crc = sasCRC(&buffB[2 * i], 2, 0);
            uint16_t* p16 = reinterpret_cast<uint16_t*>(&buffC[2 * i]);
            *p16 = crc;
        }

        // Construimos el paso 1 del array V
        u32to8 auxU;
        memset(&n, 0, sizeof(n));

        //primer parte
        auxU.u32 = 0;
        auxU.u8[0] = buffC[3];
        auxU.u8[1] = buffC[4];
        auxU.u8[2] = buffC[5];

        sprintf(reinterpret_cast<char*>(n), "%08u", auxU.u32);

        //segunda parte
        auxU.u32 = 0;
        auxU.u8[0] = buffC[0];
        auxU.u8[1] = buffC[1];
        auxU.u8[2] = buffC[2];
        sprintf(reinterpret_cast<char*>(&n[8]), "%08u", auxU.u32);

        // *** Ejecuta el paso 5 ***
        memcpy(v, n, sizeof(v));

        //calcula v7 (ojo que n ya esta dado vuelta LSB y MSB)
        uint8_t acu = 0;

        for (uint32_t i = 0; i <= 7; ++i)
        {
            acu += (n[i] - '0');
        }
        acu = acu % 5;
        acu = acu << 1;
        v[0] = n[0] | acu;

        //calcula v15 (ojo que n ya esta dado vuelta LSB y MSB)
        acu = 0;
        for (uint32_t i = 8; i <= 15; ++i)
        {
            acu += (n[i] - '0');
        }
        acu = acu % 5;
        acu = acu << 1;
        v[8] = n[8] | acu;

        // ***** Genera la salida ***
        //limpia el buffer de salida
        memset(_valNbr, 0, 8);

        uint8_t bcd;
        uint32_t p;

        //transforma a BCD la cadena
        for (p = 0; p < 8; ++p)
        {
            //nibble alto
            bcd = v[2 * p];
            if ((bcd >= '0') && (bcd <= '9')) bcd -= '0';
            else break;

            //rota 4 bits
            bcd = bcd << 4;

            //salva en la cadena
            _valNbr[p] = bcd;

            //nibble bajo
            bcd = v[(2 * p) + 1];
            if ((bcd >= '0') && (bcd <= '9')) bcd -= '0';
            else break;

            //salva en la cadena
            _valNbr[p] |= bcd;
        }
    }

    /**
     * @brief Obtiene los datos de validacion para System
     *
     * @param _coType Tipo de Cashout (ver Cashout Type Code Values)
     * @param _amnt Monto en centavos del cashout
     * @param _sv Datos de validacion
     * @return true Pudo obtener los datos
     * @return false No pudo obtener los datos
     */
    bool TITO::getValidationID(const uint8_t _coType, const uint32_t _amnt, systemValid& _sv)
    {
        assert((engConfig::init()->getValidationType() == SAS_VAL_SYS) && "Solo funciona con System validation");

        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_GETVALINFSY;
        TXCMD(bo)->len = 5;
        TXCMD(bo)->payload[0] = _coType;
        memcpy(&TXCMD(bo)->payload[1], &_amnt, 4);
        if (eCom->notifyEngine(bo, bi))
        {
            auto rx = RXCMD(bi);
            if (rx->payload[0] != CMD_ACK) return sal;
            const uint32_t start = getOSTick();

            // Ahora a esperar
            while (true)
            {
                // Si vencio el tiempo, chau
                if ((getOSTick() - start) >= MAX_SYSID_WAIT) break;

                // Pido el estado
                TXCMD(bo)->cmd = CMD_GETVALINFST;
                TXCMD(bo)->len = 0;
                if (!eCom->notifyEngine(bo, bi)) break;

                if (rx->payload[0] == CMD_ACK)
                {
                    memcpy(&_sv, &rx->payload[1], sizeof(_sv));
                    sal = true;
                    break;
                }
                else if (rx->payload[0] == CMD_WAIT)
                {
                    osWait(100);
                }
                else
                {
                    break;
                }
            }
        }

        LOG_DEBUG("getValidationID(%u, %u)->%s", _coType, _amnt, sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Solicita al SE que se Redima un ticket
     *
     * @param _vd Codigo de barras del ticket
     * @param _amnt Monto del ticket (0 si no se dispone)
     * @return true Se recibio el ticket
     * @return false No se recibio el ticket dentro del timeout
     */
    bool TITO::redeemTicket(const std::vector<uint8_t>& _vd, const uint32_t _amnt)
    {
        const auto pCfg = engConfig::init();
		assert((pCfg->getValidationType() != SAS_VAL_STD) && "Solo funciona con Validacion");
        assert(pCfg->getMaq()->tktRedemptionEnabled() && "Solo funciona con Ticket Redemption enabled");

        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        ticket_t* tkt = reinterpret_cast<ticket_t*>(TXCMD(bo)->payload);

        tkt->amount = _amnt;
        tkt->status = 0x00;  // Ticket en Escrow
        std::copy(std::begin(_vd), std::end(_vd), tkt->validData);
        TXCMD(bo)->cmd = CMD_REDEEMTKT;
        TXCMD(bo)->len = sizeof(ticket_t);
        if (eCom->notifyEngine(bo, bi, 11000))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("redeemTicket->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Obtiene la autorizacion o rechazo de un ticket
     *  Nota: En caso que el host todavía no haya respondido, el campo
     *        valid de _tktRedeem tendra el valor false y el resto de
     *        los datos es indeterminado.
     * @param _tktRedeem Datos de la operacion
     * @return true Se recibio respuesta del Host
     * @return false No se recibio respuesta
     */
    bool TITO::getRedemptionStatus(ticket_t& _tktRedeem, bool& _valid)
    {
		assert((engConfig::init()->getValidationType() != SAS_VAL_STD) && "Solo funciona con Validacion");

        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        TXCMD(bo)->cmd = CMD_GETREDEEMST;
        TXCMD(bo)->len = 0;
        if (eCom->notifyEngine(bo, bi))
        {
            const command_t* rxCmd = RXCMD(bi);
            switch (rxCmd->payload[0])
            {
            case CMD_ACK:
                _valid = true;
                if ((rxCmd->len == 2) && (rxCmd->payload[1] == 0))
                {
                    // Timeout de espera del Host. El ticket debe retornarse
                    memset(&_tktRedeem, 0, sizeof(_tktRedeem));
                    _tktRedeem.txCode = TX_CODE_UNTOVALID;
                }
                else
                {
                	memcpy(&_tktRedeem, &rxCmd->payload[1], sizeof(_tktRedeem));
                }

                sal = true;
                break;

            case CMD_WAIT:
                _valid = false;
                sal = true;
                break;

            case CMD_NACK:
                break;
            }
        }

        LOG_DEBUG("getRedemptionStatus->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Reporta al SE el resultado final de la operacion del ticket
     * @param _trs Estado final de la operacion
     * @return true Si se pudo reportar
     * @return false Si no se pudo reportar
     */
    bool TITO::ticketRedeemed(const ticket_t& _trs)
    {
		assert((engConfig::init()->getValidationType() != SAS_VAL_STD) && "Solo funciona con Validacion");

        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_TKTREDEEMED;
        TXCMD(bo)->len = sizeof(_trs);
        memcpy(TXCMD(bo)->payload, &_trs, sizeof(_trs));
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("ticketRedeemed->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Convierte un string numerico a un numero BCD
     * @param _str String formado solo por numeros decimales
     * @param _bcd Vector que contiene el numero BCD
     */
    void strToBCD(const std::string& _str, std::vector<uint8_t>& _bcd)
    {
        std::string aux = _str;

        if (aux.length() % 2) aux.insert(std::begin(aux), '0');
        for (uint32_t i = 0; i < aux.length() / 2; ++i)
        {
            uint8_t bcd;

            bcd = (aux[2 * i] - '0') << 4;
            bcd |= (aux[2 * i + 1] - '0');
            _bcd.emplace_back(bcd);
        }
    }
}
