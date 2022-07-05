//---------------------------------------------------------------------------
#include "engconfig.h"
#include "utils.h"
#include "file_log.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sys/stat.h>
//---------------------------------------------------------------------------

namespace EngineAPI
{
    engConfig* engConfig::cfg = nullptr;

    engConfig* engConfig::init()
    {
        if (cfg) return cfg;

        try
        {
            cfg = new engConfig;
        }
        catch(const std::exception& e)
        {
            LOG_ERROR("Error creating new engConfig: %s", e.what());
        }
        
        return cfg;
    }

    sasCfg::sasCfg() noexcept
    {
        memset(&sas, 0, sizeof(sas));
        sas.cantCanales = 1;
        sas.mainChnl = 1;
        sas.titoChnl = 1;
        sas.aftChnl = 1;
    }

    /**
     * @brief Establece la cantidad de canales SAS habilitados. El canal 2 es para TITO
     * 
     * @param _nc Cantidad de canales (1 o 2)
     */
    void sasCfg::setNumChannels(const uint8_t _nc)
    {
        if (_nc > 2) throw std::invalid_argument("_nc > 2");
        if (_nc == 0) throw std::invalid_argument("_nc == 0");
        sas.cantCanales = _nc;
    }

    /**
     * @brief Obtiene la cantidad de canales SAS habilitados
     * 
     * @return uint8_t Cantidad de canales SAS (1 o 2)
     */
    uint8_t sasCfg::getNumChannels() const noexcept
    {
        return sas.cantCanales;
    }

    /**
     * @brief Establece el ID de SAS.
     * 
     * @param _id Id de SAS. 0 -> desactivado. 1..127 ids validos.
     */
    void sasCfg::setID(const uint8_t _id)
    {
        if (_id > 0x7F) throw std::invalid_argument("_id > 127");
        sas.id = _id;
    }

    /**
     * @brief Obtiene el ID de SAS (0..127)
     * 
     * @return uint8_t ID de SAS
     */
    uint8_t sasCfg::getID() const noexcept
    {
        return sas.id;
    }

    /**
     * @brief Setea el soporte para Legacy Handpay Reporting
     *
     * @param _val true -> Si, false -> No
     */
    void sasCfg::setLegacyHandPay(const bool _val) noexcept
    {
        sas.legacyHP = _val ? 1 : 0;
    }

    /**
     * @brief Indica si el legacy handpay reporting esta soportado
     *
     * @return bool true -> Si, false -> No
     */
    bool sasCfg::getLegacyHandPay() const noexcept
    {
        return (sas.legacyHP == 1) ? true : false;
    }

    /**
     * @brief Setea el canal principal de SAS (por default 1)
     *
     * @param _chnl Canal SAS (1, 2 o 3). Antes debe estar seteada la cantidad de canales
     */
    void sasCfg::setMainChannel(const uint8_t _chnl)
    {
        assert((_chnl > 0) && (_chnl <= sas.cantCanales));
        sas.mainChnl = _chnl;
    }

    /**
     * @brief Obtiene el canal principal de SAS
     *
     * @return Numero de canal (1, 2 o 3)
     */
    uint8_t sasCfg::getMainChannel() const noexcept
    {
        return sas.mainChnl;
    }

    /**
     * @brief Setea el canal TITO de SAS (por default 1)
     *
     * @param _chnl Canal SAS (1, 2 o 3). Antes debe estar seteada la cantidad de canales
     */
    void sasCfg::setTITOChannel(const uint8_t _chnl)
    {
        assert((_chnl > 0) && (_chnl <= sas.cantCanales));
        sas.titoChnl = _chnl;
    }

    /**
     * @brief Obtiene el canal TITO de SAS
     *
     * @return Numero de canal (1, 2 o 3)
     */
    uint8_t sasCfg::getTITOChannel() const noexcept
    {
        return sas.titoChnl;
    }

    /**
     * @brief Setea el canal AFT de SAS (por default 1)
     *
     * @param _chnl Canal SAS (1, 2 o 3). Antes debe estar seteada la cantidad de canales
     */
    void sasCfg::setAFTChannel(const uint8_t _chnl)
    {
        assert((_chnl > 0) && (_chnl <= sas.cantCanales));
        sas.aftChnl = _chnl;
    }

    /**
     * @brief Obtiene el canal AFT de SAS
     *
     * @return Numero de canal (1, 2 o 3)
     */
    uint8_t sasCfg::getAFTChannel() const noexcept
    {
        return sas.aftChnl;
    }

    /**
     * @brief Almacena la configuracion de SAS en un buffer dado
     *
     * @param _buffer Buffer donde almacenar
     * @return true Pudo guardar
     * @return false No pudo guardar
     */
    bool sasCfg::save(std::vector<uint8_t>& _buffer)
    {
        uint8_t* pAux = reinterpret_cast<uint8_t*>(&sas);
        bool sal = false;

        try
        {
            std::copy(pAux, pAux + sizeof(sas), std::back_inserter(_buffer));
            sal = true;
        }
        catch (...)
        {

        }

        return sal;
    }

    /**
     * @brief Almacena la configuracion de SAS en un buffer dado
     *
     * @param _buffer Buffer donde almacenar
     * @param _size Cantidad de bytes almacenados
     */
    void sasCfg::save(uint8_t* _buffer, uint16_t* _size)
    {
        memcpy(_buffer, &sas, sizeof(sas));
        *_size = static_cast<uint16_t>(sizeof(sas));
    }

    /**
     * @brief Carga la configuracion de SAS desde un buffer
     *
     * @param _buffer Buffer desde donde cargar
     * @param _ix Posición dentro del buffer donde comienzan los datos
     * @return uint32_t 0 -> No puede cargar, != 0 -> Nueva posicion de buffer, despues de la config SAS
     */
    uint32_t sasCfg::load(const std::vector<uint8_t>& _buffer, const uint32_t _ix)
    {
        uint32_t pos = 0;

        if ((_buffer.size() - _ix) >= sizeof(sas))
        {
            const cfgSAS* pSAS = reinterpret_cast<const cfgSAS*>(&_buffer[_ix]);
            sas = *pSAS;
            pos = _ix + sizeof(sas);
        }

        return pos;
    }

    /**
     * @brief Calcula el CRC CCITT16 de la config SAS
     * 
     * @param _seed Semilla a utilizar
     * @return uint16_t Valor del CRC
     */
    uint16_t sasCfg::crc(const uint16_t _seed) const
    {
        return sasCRC(reinterpret_cast<const uint8_t*>(&sas), sizeof(sas), _seed);
    }
    //---------------------------------------------------------------------------
    machineCfg::machineCfg() noexcept
    {
        memset(&maq, 0, sizeof(maq));

        // Por default dejamos la mascara de inhibicion de modo que no se puedan tocar
        // los bits reservados y todo lo demas permitido
        maq.extVal.mask = MIM_PRNASCASHOUT | MIM_PRNASHPRECEP | MIM_VALHPANDHPR | MIM_PRINTRESTKT |
            MIM_TKTFORRESAMT | MIM_TKTREDEMPT | MIM_SECENHCFG;
    }


    /**
     * @brief Determina si el ticket rededmption esta activo o no
     *
     * @return true -> Ticket Redemption ON
     */
    bool machineCfg::tktRedemptionEnabled() const
    {
        return (maq.machineFeatures[0] & MFEAT1_TKTREDEMPT) ? true : false;
    }

    /**
     * @brief Setea el token denomination
     * 
     * @param _td Valor del token denomination (0..31 - ver Codigos de denominacion SAS)
     */
    void machineCfg::setTokenDeno(const uint8_t _td)
    {
        if (_td > 0x1F) throw std::invalid_argument("_td > 31");
        maq.tokenDenom = _td;
    }

    /**
     * @brief Retorna el codigo de denominacion de token
     * 
     * @return uint8_t Codigo de denominacion (0..31 - ver Codigos de denominacion SAS)
     */
    uint8_t machineCfg::getTokenDeno() const noexcept
    {
        return maq.tokenDenom;
    }

    /**
     * @brief Setea el numero de serie del equipo
     * 
     * @param _sn Cadena de caracteres del numero de serie (min 1, max 40)
     */
    void machineCfg::setSerialNumber(const std::string& _sn)
    {
        if (_sn.length() > 40) throw std::invalid_argument("_sn must be 40 characters top");
        if (_sn.empty()) throw std::invalid_argument("_sn cant't be empty");
        memset(maq.serialNbr, 0, 41);
        strncpy(maq.serialNbr, _sn.c_str(), 40);
    }

    /**
     * @brief Retorna el numero de serie del equipo
     * 
     * @return std::string Cadena con el nro de serie (min 1, max 40)
     */
    std::string machineCfg::getSerialNumber() const noexcept
    {
        return std::string(maq.serialNbr);
    }

    /**
     * @brief Setea los Machine Features
     * 
     * @param _mf Array de 3 bytes que contiene la mascara de los features. Ver Machine features
     */
    void machineCfg::setMachineFeatures(const uint8_t* _mf) noexcept
    {
        assert(_mf && "_mf no puede ser nullptr");
        memcpy(maq.machineFeatures, _mf, 3);

        // Si se valida System o Secure Enhanced, entonces se debe soportar el polling de 40ms
        if ((maq.machineFeatures[0] & MFEAT1_VALSECENH) || (maq.machineFeatures[0] & MFEAT1_VALSYSTEM))
            maq.machineFeatures[2] |= MFEAT3_MAXPOLLINGRATE;
    }

    /**
     * @brief Retorna un array de 3 bytes con la mascara de los machine features
     * 
     * @return const uint8_t* Array. Ver Machine features
     */
    const uint8_t* machineCfg::getMachineFeatures() const noexcept
    {
        return maq.machineFeatures;
    }

    /**
     * @brief Modifica la mascara que habilita / inhibe la modificacion de parametros de la
     * extended validation.
     * 
     * @param _msk Bit a modificar (ver Constantes de mascara para extended validation status).
     * No se pueden combinar diferentes bits en una misma llamada.
     * @param _val true -> Habilitar modificacion, false -> Inhibir modificacion
     */
    void machineCfg::setModifStatusMask(const uint16_t _msk, const bool _val) 
    {
        if ((_msk > MIM_TKTREDEMPT) && (_msk < MIM_SECENHCFG)) throw std::invalid_argument("Invalid _msk");
        if (_val)
            maq.extVal.mask |= _msk;
        else
            maq.extVal.mask &= ~_msk;
    }

    /**
     * @brief Retorna el estado de los bits de la mascara que habilita / inhibe la modificacion de parametros de la
     * extended validation.
     * 
     * @param _msk Bit a consultar (ver Constantes de mascara para extended validation status).
     * @return true Ese item es modificable
     * @return false Ese item no es modificable
     */
    bool machineCfg::getModifStatusMask(const uint16_t _msk) const
    {
        if ((_msk > MIM_TKTREDEMPT) && (_msk < MIM_SECENHCFG)) throw std::invalid_argument("Invalid _msk");
        return (maq.extVal.mask & _msk) ? true : false;
    }

    /**
     * @brief Setea un bit de estado de configuracion de extended validation
     * 
     * @param _msk Bit a modificar (ver Constantes de mascara para extended validation status).
     * @param _val true -> Permitir accion, false -> Prohibir accion
     */
    void machineCfg::setStatusMask(const uint16_t _msk, const bool _val)
    {
        if ((_msk > MIM_TKTREDEMPT) && (_msk < MIM_SECENHCFG)) throw std::invalid_argument("Invalid _msk");
        
        // Si la mascara de inhibicion permite tocar ese bit...
        if (maq.extVal.mask & _msk)
        {
            if (_val)
                maq.extVal.stBitCtrl |= _msk;
            else
                maq.extVal.stBitCtrl &= ~_msk;
        }
    }

    /**
     * @brief Reemplaza la mascara de configuracion de extended validation. Tiene en cuenta la mascara
     * de inhibicion.
     * 
     * @param _msk Mascara de configuracion completa
     */
    void machineCfg::setFullStatusMask(const uint16_t _msk) noexcept
    {
        maq.extVal.stBitCtrl = _msk & maq.extVal.mask;
    }

    /**
     * @brief Obtiene el estado de un bit de la mascara de configuracion de extended validation
     * 
     * @param _msk Bit a consultar. (ver Constantes de mascara para extended validation status).
     * @return true La caracteristica esta permitida
     * @return false La caracteristica esta prohibida.
     */
    bool machineCfg::getStatusMask(const uint16_t _msk) const
    {
        if ((_msk > MIM_TKTREDEMPT) && (_msk < MIM_SECENHCFG)) throw std::invalid_argument("Invalid _msk");
        return (maq.extVal.stBitCtrl & _msk) ? true : false;
    }

    /**
     * @brief Setea el Asset Number de validacion extendida del equipo
     * 
     * @param _an Asset Number
     */
    void machineCfg::setAssetNumber(const uint32_t _an) noexcept
    {
        maq.extVal.assetNbr = _an;
    }

    /**
     * @brief Retorna el Asset Number de validacion extendida del equipo
     * 
     * @return uint32_t Asset Number
     */
    uint32_t machineCfg::getAssetNumber() const noexcept
    {
        return maq.extVal.assetNbr;
    }

    /**
     * @brief Setea la cantidad de días de expiracion de un ticket casheable o recibo
     * 
     * @param _exp Días hasta la expiracion. 1..9998 o 9999 -> Nunca expira
     */
    void machineCfg::setCashTktAndRecExp(const uint16_t _exp)
    {
        if ((_exp == 0) || (_exp > 9999)) throw std::invalid_argument("0 < _exp <= 9999");
        maq.extVal.cashTktRecExp = _exp;
    }

    /**
     * @brief Obtiene la cantidad de días hasta expirar de un ticket casheable o recibo
     * 
     * @return uint16_t Cantidad de días hasta expirar (1...9998) o 9999 -> Nunca expira
     */
    uint16_t machineCfg::getCashTktAndRecExp() const noexcept
    {
        return maq.extVal.cashTktRecExp;
    }

    /**
     * @brief Setea la cantidad de días default para la expiracion de un ticket restricted
     * 
     * @param _exp Cantidad de días hasta expirar (1...9998) o 9999 -> Nunca expira
     */
    void machineCfg::setRestTktDefExpiration(const uint16_t _exp)
    {
        if ((_exp == 0) || (_exp > 9999)) throw std::invalid_argument("0 < _exp <= 9999");
        maq.extVal.resTktdefExp = _exp;
    }

    /**
     * @brief Obtiene la cantidad de días default hasta expirar de un ticket restricted.
     * 
     * @return uint16_t Cantidad de días hasta expirar (1...9998) o 9999 -> Nunca expira
     */
    uint16_t machineCfg::getRestTktDefExpiration() const noexcept
    {
        return maq.extVal.resTktdefExp;
    }

    /**
     * @brief Salva la configuracion de maquina en un buffer dado
     * 
     * @param _buffer Buffer donde almacenar
     * @return true Pudo almacenar
     * @return false No pudo almacenar
     */
    bool machineCfg::save(std::vector<uint8_t>& _buffer)
    {
        bool sal = false;

        try
        {
            uint8_t* pAux = reinterpret_cast<uint8_t*>(&maq);
            std::copy(pAux, pAux + sizeof(maq), std::back_inserter(_buffer));
            sal = true;
        }
        catch(...)
        {
        }

        return sal;        
    }

    /**
     * @brief Almacena la configuracion de SAS en un buffer dado
     * 
     * @param _buffer Buffer donde almacenar
     * @param _size Cantidad de bytes almacenados
     */
    void machineCfg::save(uint8_t* _buffer, uint16_t* _size)
    {
        memcpy(_buffer, &maq, sizeof(maq));
        *_size = static_cast<uint16_t>(sizeof(maq));
    }

    /**
     * @brief Carga la configuracion de maquina desde un buffer
     * 
     * @param _buffer Buffer que contiene la configuracion
     * @param _ix Posicion dentro del buffer donde empezar
     * @return uint32_t Nueva posicion posterior a la configuracion de maquina (0 si error)
     */
    uint32_t machineCfg::load(const std::vector<uint8_t>& _buffer, const uint32_t _ix)
    {
        uint32_t pos = 0;

        if ((_buffer.size() - _ix) >= sizeof(maq))
        {
            const cfgMaq* pMaq = reinterpret_cast<const cfgMaq*>(_buffer.data() + _ix);
            maq = *pMaq;
            pos = _ix + sizeof(maq);
        }

        return pos;
    }

    /**
     * @brief Calcula el CRC de la config de maquina
     * 
     * @param _seed Semilla a utilizar
     * @return uint16_t CRC
     */
    uint16_t machineCfg::crc(const uint16_t _seed) const
    {
        return sasCRC(reinterpret_cast<const uint8_t*>(&maq), sizeof(maq), _seed);
    }

    /**
     * @brief Indica si AFT esta habilitado o no
     * 
     * @return true Esta habilitado
     * @return false No esta habilitado
     */
    bool machineCfg::aftEnabled() const
    {
        return (maq.machineFeatures[1] & MFEAT2_AFT) ? true : false;
    }
    //---------------------------------------------------------------------------

    /**
     * @brief Habilita / deshabilita el juego
     *
     * @param _ena true -> Habilita, false -> Deshabilita
     */
    void gameObj::enable(const bool _ena)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (pGame->gameNbr == 0) throw std::logic_error("Game 0 can't be enabled/disabled");
        pGame->enabled = _ena ? 1 : 0;
    }

    /**
     * @brief Retorna el estado de habilitacion de un juego
     *
     * @return true -> Habilitado, false -> deshabilitado
     */
    bool gameObj::getEnabled() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return pGame->enabled;
    }

    /**
     * @brief Setea el ID del juego
     *
     * @param _id Cadena de caracteres del ID. 1 o 2 maximo
     */
    void gameObj::setID(const std::string& _id)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_id.empty() || (_id.length() > 2)) throw std::invalid_argument("_id must be 1 or 2 characters top");
        memset(pGame->gameID, 0, 2);
        memcpy(pGame->gameID, _id.c_str(), _id.length());
    }

    /**
     * @brief Retorna un string con el Game ID. Si no se seteo, la cadena devuelta esta vacia
     *
     * @return std::string Game ID de 0, 1 o 2 caracteres
     */
    std::string gameObj::getID() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        std::string sal;

        if (pGame->gameID[0]) sal.push_back(pGame->gameID[0]);
        if (pGame->gameID[1]) sal.push_back(pGame->gameID[1]);
        return sal;
    }

    /**
     * @brief Setea el ID Adicional del juego (entre 1 y 3 caracteres)
     * 
     * @param _id Cadena de caracteres del ID (Entre 1 y 3)
     */
    void gameObj::setAdditionalGameID(const std::string& _id)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_id.empty() || (_id.length() > 3)) throw std::invalid_argument("_id must be 1, 2 or 3 characters");
        memset(pGame->aditionalGameID, 0, 3);
        memcpy(pGame->aditionalGameID, _id.c_str(), _id.length());
    }

    /**
     * @brief Obtiene el ID de juego adicional. Si no se seteo uno la cadena devuelta esta vacia
     * 
     * @return std::string Cadena de entre 0 y 3 caracteres
     */
    std::string gameObj::getAdditionalGameID() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        std::string sal;

        if (pGame->aditionalGameID[0]) sal.push_back(pGame->aditionalGameID[0]);
        if (pGame->aditionalGameID[1]) sal.push_back(pGame->aditionalGameID[1]);
        if (pGame->aditionalGameID[2]) sal.push_back(pGame->aditionalGameID[2]);
        return sal;
    }

    /**
     * @brief Setea la denominacion del juego
     * 
     * @param _deno Valor de la denominacion (ver Codigos de denominacion SAS)
     */
    void gameObj::setDenomination(const uint8_t _deno)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_deno > DENO_5FC) throw std::invalid_argument("_deno out of range");
        pGame->denomination = _deno;
    }

    /**
     * @brief Obtiene la denominacion del juego
     * 
     * @return uint8_t Codigo SAS de denominacion
     */
    uint8_t gameObj::getDenomination() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return pGame->denomination;
    }

    /**
     * @brief Setea la apuesta maxima del juego
     * 
     * @param _bet Valor de la apuesta maxima para el juego (en creditos, 0..9999)
     */
    void gameObj::setMaxBet(const uint16_t _bet)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_bet > 9999) throw std::invalid_argument("_bet must be between 0 and 9999");
        pGame->maxBet = _bet;
    }

    /**
     * @brief Retorna el valor de la apuesta maxima del juego
     * 
     * @return uint16_t Valor en creditos (0..9999)
     */
    uint16_t gameObj::getMaxBet() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return pGame->maxBet;
    }

    /**
     * @brief Setea el grupo progresivo del juego
     * 
     * @param _grp Grupo progresivo
     */
    void gameObj::setProgressiveGrp(const uint8_t _grp)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        pGame->prgsvGroup = _grp;
    }

    /**
     * @brief Retorna el grupo progresivo del juego
     * 
     * @return uint8_t Grupo progresivo
     */
    uint8_t gameObj::getProgressiveGrp() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return pGame->prgsvGroup;
    }

    /**
     * @brief Establece el mapa de niveles de progresivo soportados
     * 
     * @param _lvls Mapa de niveles (bit 0 -> nivel 1, bit 1 -> nivel 2 .. bit 31 -> nivel 32)
     */
    void gameObj::setProgressiveLevels(const std::bitset<32>& _lvls)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        pGame->prgsvLevels = _lvls.to_ulong();
    }

    /**
     * @brief Obtiene el mapa de niveles progresivos de in juego
     * 
     * @return std::bitset<32> Mapa de niveles. (bit 0 -> nivel 1, bit 1 -> nivel 2 .. bit 31 -> nivel 32)
     */
    std::bitset<32> gameObj::getProgressiveLevels() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return std::bitset<32>(pGame->prgsvLevels);
    }

    /**
     * @brief Setea el ID de la tabla de pagos del juego
     * 
     * @param _id String del Id de la tabla de pagos (Entre 1 y 6 caracteres)
     */
    void gameObj::setPayTableID(const std::string& _id)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_id.empty() || (_id.length() > 6)) throw std::invalid_argument("_id must have between 1 and 6 characters");
        memset(pGame->payTableID, 0, 6);
        memcpy(pGame->payTableID, _id.c_str(), _id.length());
    }

    /**
     * @brief Retorna el ID de la tabla de pagos del juego
     * 
     * @return std::string String del Id de la tabla de pagos (Entre 1 y 6 caracteres, vacio si nunca se seteo)
     */
    std::string gameObj::getPayTableID() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        std::string sal;

        for (uint32_t i = 0; i < 6; ++i)
        {
            if (pGame->payTableID[i])
                sal.push_back(pGame->payTableID[i]);
            else
                break;
        }

        return sal;
    }

    /**
     * @brief Setea el nombre de la tabla de pagos del juego
     * 
     * @param _id String con el nombre de la tabla de pagos. (Entre 1 y 20 caracteres)
     */
    void gameObj::setPayTableName(const std::string& _id)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_id.empty() || (_id.length() > 20)) throw std::invalid_argument("_id must have between 1 and 20 characters");
        memset(pGame->payTableName, 0, 21);
        memcpy(pGame->payTableName, _id.c_str(), _id.length());
    }

    /**
     * @brief Retorna el nombre de la tabla de pagos del juego
     * 
     * @return std::string Nombre de la tabla de pagos (entre 1 y 20 caracteres, vacio si no se seteo)
     */
    std::string gameObj::getPayTableName() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        std::string sal;

        for (uint32_t i = 0; i < 20; ++i)
        {
            if (pGame->payTableName[i])
                sal.push_back(pGame->payTableName[i]);
            else
                break;
        }

        return sal;
    }

    /**
     * @brief Establece el porcentaje base de retorno del juego (Solo informativo)
     * 
     * @param _basePerc Porcentaje base expresado en multiplos de 100 (1,2 % -> 120, 97% -> 9700, etc..)
     */
    void gameObj::setBasePercent(const uint16_t _basePerc)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_basePerc > 10000) throw std::invalid_argument("The percentage can't be > 100%");
        char aux[5];
        sprintf(aux, "%u", _basePerc);
        memcpy(pGame->basePercent, aux, 4);
    }

    /**
     * @brief Obtiene el porcentaje base de un juego
     * 
     * @return uint16_t Valor del porcentaje base expresado en multiplos de 100 (1,2 % -> 120, 97% -> 9700, etc..)
     */
    uint16_t gameObj::getBasePercent() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        std::string aux;

        for (uint32_t i = 0; i < 4; ++i)
        {
            if (pGame->basePercent[i])
                aux.push_back(pGame->basePercent[i]);
            else
                break;
        }

        return atoi(aux.c_str());
    }

    /**
     * @brief Establece la cantidad de categorias de apuesta de un juego
     * 
     * @param _wc Cantidad de categorias (0 a 9999)
     */
    void gameObj::setWagerCategories(const uint16_t _wc)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_wc > 9999) throw std::invalid_argument("_wc must be <= 9999");
        pGame->wagerCats = _wc;
    }

    /**
     * @brief Retorna la cantidad de categorias de apuesta soportadas por un juego
     * 
     * @return uint16_t Cantidad de categorias de apuesta (0 a 9999)
     */
    uint16_t gameObj::getWagerCategories() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return pGame->wagerCats;
    }

    /**
     * @brief Establece el nombre de un juego
     * 
     * @param _name Cadena ASCII de hasta 20 caracteres (ASCII 32 a 126)
     */
    void gameObj::setName(const std::string& _name)
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        if (_name.empty() || _name.length() > 20) throw std::invalid_argument("_name must have between 1 and 20 characters");
        memset(pGame->name, 0, 21);
        memcpy(pGame->name, _name.c_str(), _name.length());
    }

    /**
     * @brief Retorna el nombre del juego
     * 
     * @return std::string Cadena ASCII de hasta 20 caracteres (ASCII 32 a 126)
     */
    std::string gameObj::getName() const
    {
        if (!pGame) throw std::logic_error("Invalid gameObj");
        return std::string(pGame->name);
    }
    //---------------------------------------------------------------------------

    /**
     * @brief Construct a new game Cfg::game Cfg object
     * 
     */
    gameCfg::gameCfg()
    {
        addGame();  // Agrego la maquina
    }
    
    /**
     * @brief Elimina todos los juegos para poder arrancar la config desde 0
     * 
     */
    void gameCfg::reset() 
    {
        games.clear();
        addGame();
    }

    /**
     * @brief Retorna la cantidad total de juegos + 1. (El juego 0 es la maquina como total)
     * 
     * @return uint32_t Cantidad total de juegos + 1
     */
    uint32_t gameCfg::getNumGames() const noexcept
    {
        return games.size();
    }

    /**
     * @brief Agrega un nuevo juego
     * 
     * @return cfgGame* puntero al nuevo juego
     */
    gameObj gameCfg::addGame()
    {
        if (games.size() == MAX_CFG_GAMES) throw std::logic_error("Can't add more games");

        try
        {
            cfgGame ng;
    
            memset(&ng, 0, sizeof(ng));
            ng.gameNbr = games.size();
            games.emplace_back(ng);
            return gameObj(std::addressof(games.back()));
        }
        catch (...)
        {
            return gameObj(nullptr);
        }
    }

    /**
     * @brief Retorna la configuracion de un juego en particular
     * 
     * @param _gn Numero de juego (0..24, 0 -> Maquina, 1-> Juego 1, ...)
     * @return gameObj Objeto de configuracion de juego
     */
    gameObj gameCfg::getGame(const uint32_t _gn)
    {
        if (_gn >= games.size()) throw std::invalid_argument("_gn out of range");
        return gameObj(std::addressof(games[_gn]));
    }

    /**
     * @brief Salva la configuracion de los juegos en un buffer. Se salva 4 bytes de cantidad de juegos
     * y a continuacion juego 0, juego 1, ...
     * 
     * @param _buffer Buffer donde salvar
     * @return true Pudo salvar
     * @return false No pudo salvar
     */
    bool gameCfg::save(std::vector<uint8_t>& _buffer)
    {
        bool sal = false;

        const uint32_t size = games.size();
        const uint8_t* pAux = reinterpret_cast<const uint8_t*>(&size);
        try
        {
            std::copy(pAux, pAux + sizeof(size), std::back_inserter(_buffer));
            for (const auto& game : games)
            {
                pAux = reinterpret_cast<const uint8_t*>(&game);
	            std::copy(pAux, pAux + sizeof(game), std::back_inserter(_buffer));
            }

            sal = true;
        }
        catch (...)
        {
        }

        return sal;
    }

    /**
     * @brief Almacena la configuracion de un juego en un buffer dado
     * 
     * @param _buffer Buffer donde almacenar
     * @param _size (In) Indice del juego, (out)Cantidad de bytes almacenados
     */
    void gameCfg::save(uint8_t* _buffer, uint16_t* _size)
    {
        cfgGame* pGame = &games[*_size];
        memcpy(_buffer, pGame, sizeof(cfgGame));
        *_size = static_cast<uint16_t>(sizeof(cfgGame));
    }

    /**
     * @brief Carga la configuracion de los juegos
     * 
     * @param _buffer Buffer donde se encuentran los datos
     * @param _ix Posicion del buffer donde comenzar
     * @return uint32_t Nueva posicion, luego de la configuracion de los juegos (0 si error)
     */
    uint32_t gameCfg::load(const std::vector<uint8_t>& _buffer, const uint32_t _ix)
    {
        uint32_t pos = 0;

        if ((_buffer.size() - _ix) >= sizeof(uint32_t))
        {
            const uint32_t* pSize = reinterpret_cast<const uint32_t*>(&_buffer[_ix]);
            if ((*pSize == 0) || (*pSize > MAX_CFG_GAMES)) return 0U;
            const uint32_t gSize = *pSize * sizeof(cfgGame);
            if ((_buffer.size() - _ix - *pSize) < gSize) return 0U;
            games.clear();
            try
            {
                games.resize(*pSize);
                memcpy(games.data(), &_buffer[_ix + sizeof(uint32_t)], gSize);
                pos = _ix + sizeof(uint32_t) + gSize;
            }
            catch (...)
            {
            }
        }

        return pos;
    }

    /**
     * @brief Calcula el CRC CCITT16 de los juegos
     * 
     * @param _seed Semilla a utilizar
     * @return uint16_t CRC
     */
    uint16_t gameCfg::crc(const uint16_t _seed) const
    {
        uint16_t crc = _seed;
        for (const auto& game : games)
        {
            crc = sasCRC(reinterpret_cast<const uint8_t*>(&game), sizeof(game), crc);
        }

        return crc;
    }

    //---------------------------------------------------------------------------
    static bool chequeoSeg(const uint32_t _pos, const uint32_t _size, const uint32_t _limit2)
    {
        return ((_pos + _size) <= _limit2);
    }

#define ERR_SIZE(str) {LOG_WARN("Cambio de tamaño detectado en la config: %s", str); return false;}

    /**
     * @brief Carga la config desde disco
     * 
     * @param _file Path al archivo de configuracion
     * @return true Cargo OK
     * @return false No pudo cargarla
     */
	bool engConfig::load(const char* _file)
    {
        assert(_file && "_file es NULL");

        struct stat st;

        // Compruebo el acceso y obtengo datos
        if (::stat(_file, &st) != 0)
        {
            LOG_WARN("Can't access configuration file: %s", _file);
            return false;
        }

        FILE* f = nullptr;

#ifdef __linux__

        f = fopen(_file, "rb");
        if (!f)
        {
            LOG_WARN("Can't open configuration file: %s", _file);
            return false;
        }

#else

        if (fopen_s(&f, _file, "rb"))
        {
            LOG_WARN("Can't open configuration file: %s", _file);
            return false;
        }

#endif

        bool sal = false;
        std::vector<uint8_t> buffer;
        uint32_t pos = 0;

        try
        {
            buffer.resize(st.st_size);
            {
                int res = fread(buffer.data(), sizeof(uint8_t), st.st_size, f);
                fclose(f);
                if (res != st.st_size)
                {
                    LOG_WARN("Error reading file: %s", _file);
                    return false;
                }
			}

            // Calculo el CRC
            {
            	const uint16_t crc = sasCRC(buffer.data() + 2, buffer.size() - 2, 0x55AA);
                if (chequeoSeg(0, sizeof(uint16_t), buffer.size()))
                {
                    const uint16_t cCrc = *reinterpret_cast<const uint16_t*>(buffer.data());
                    if (crc != cCrc)
                    {
                        LOG_WARN("Invalid config file CRC");
                        return false;
                    }

                    pos = sizeof(uint16_t);
                }
                else
                {
                    ERR_SIZE("CRC");
                }
            }

            // Recupero la config de maquina
            uint32_t res2 = maq.load(buffer, pos);
            if (!res2) ERR_SIZE("MAQ");
            pos = res2;

            // Recupero la config de SAS
            res2 = sas.load(buffer, pos);
            if (!res2) ERR_SIZE("SAS");
            pos = res2;

            // Recupero los juegos
            res2 = games.load(buffer, pos);
            if (!res2) ERR_SIZE("GAME");

            return true;
        }
        catch (const std::exception& _ex)
        {
            LOG_ERROR("Error reading config: %s -> %s", _file, _ex.what());
            return false;
        }
    }

    /**
     * @brief Almacena la configuracion en disco
     * 
     * @param _file Path al archivo
     * @return true Almaceno OK
     * @return false No pudo almacenar
     */
    bool engConfig::save(const char* _file)
    {
        std::vector<uint8_t> buffer;
        bool sal = false;
        FILE* f = nullptr;

#ifdef __linux__

        f = fopen(_file, "wb");
        if (!f)
        {
            LOG_WARN("Can't open configuration file: %s", _file);
            return false;
        }

#else

        if (fopen_s(&f, _file, "wb"))
        {
            LOG_WARN("Can't open configuration file: %s", _file);
            return false;
        }

#endif

        uint16_t crc = 0;
        const uint8_t* pAux = reinterpret_cast<const uint8_t*>(&crc);

        // Agrego el crc
        std::copy(pAux, pAux + 2, std::back_inserter(buffer));

        // Agrego la config de Maquina
        if (!maq.save(buffer))
        {
            LOG_ERROR("Can't save slot configuration");
            fclose(f);
            return false;
        }

        // Agrego la config de SAS
        if (!sas.save(buffer))
        {
            LOG_ERROR("Can't save SAS configuration");
            fclose(f);
            return false;
        }

        // Agrego la cantidad de juegos
        if (!games.save(buffer))
        {
            LOG_ERROR("Can't save games configuration");
            fclose(f);
            return false;
        }

        // Calculo el CRC
        crc = sasCRC(buffer.data() + 2, buffer.size() - 2, 0x55AA);
        pAux = reinterpret_cast<const uint8_t*>(&crc);
        buffer[0] = *pAux;
        buffer[1] = *(pAux + 1);

        // Almaceno
        if (fwrite(buffer.data(), sizeof(uint8_t), buffer.size(), f) != buffer.size())
        {
            LOG_ERROR("Can't write to config file");
        }
        else
        {
            sal = true;
        }

        fclose(f);

        return sal;
    }


    /**
     * @brief Verifica un CRC dado contra el calculado internamente
     *
     * @param _crc CRC dado
     * @return true Los CRC coinciden
     * @return false Los CRC no coinciden
     */
    bool engConfig::verifyCRC(const uint16_t _crc) const
    {
        uint16_t crc;

        crc = sas.crc(0);
        crc = maq.crc(crc);
        crc = games.crc(crc);
        LOG_DEBUG("Computed CRC -> %u - Received CRC -> %u", crc, _crc);
        return crc == _crc;
    }

    /**
     * @brief Obtiene el tipo de validacion utilizado por el sistema
     * 
     * @return uint8_t Tipo de validacion (SAS_VAL_SE: Secure Enhanced, SAS_VAL_SYS: System, SAS_VAL_STD: Std o None)
     */
    uint8_t engConfig::getValidationType() const
    {
        const uint8_t* pFeat = maq.getMachineFeatures();
        if (pFeat[0] & MFEAT1_VALSECENH)
            return SAS_VAL_SE;
        else if (pFeat[0] & MFEAT1_VALSYSTEM)
            return SAS_VAL_SYS;
        else
            return SAS_VAL_STD;
    }

    /**
     * @brief Envia la configuracion del equipo al SE
     * 
     * @return true si el SE acepto la config
     * @return false si no la acepto (o solo acepto parte)
     */
    bool engConfig::send(std::shared_ptr<EngineCom>& _ecom)
    {
        assert(_ecom && "_ecom no valido");

        // Enviamos la config
        if (!sendSASConfig(_ecom)) return false;
        if (!sendMaqConfig(_ecom)) return false;
        if (!sendGamesConfig(_ecom)) return false;

        return sendSaveConfig(_ecom);
    }

    /**
     * @brief Envia la configuracion SAS al SE
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool engConfig::sendSASConfig(std::shared_ptr<EngineCom>& _eCom)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_CFGSAS;
        sas.save(TXCMD(bo)->payload, &TXCMD(bo)->len);
        if (_eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK); 
        LOG_DEBUG("sendSASConfig->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia la configuracion general de maquina al SE
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool engConfig::sendMaqConfig(std::shared_ptr<EngineCom>& _eCom)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_CFGMAQ;
        maq.save(TXCMD(bo)->payload, &TXCMD(bo)->len);
        if (_eCom->notifyEngine(bo, bi)) (sal = RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendMaqConfig->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia la configuracion de los juegos al SE
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool engConfig::sendGamesConfig(std::shared_ptr<EngineCom>& _eCom)
    {
        bool sal = false;
        uint32_t i = 0;
        auto pGameCfg = getGame();
        const uint32_t totalGames = pGameCfg->getNumGames();

        // Envio la cantidad de juegos
        {
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

            TXCMD(bo)->cmd = CMD_SETNUMGAMES;
            TXCMD(bo)->len = 1;
            TXCMD(bo)->payload[0] = static_cast<uint8_t>(totalGames - 1);
            if (_eCom->notifyEngine(bo, bi))
            {
                if (RXCMD(bi)->payload[0] != CMD_ACK) return false;
            }
            else
            {
                return false;
            }
        }

        for (; i < totalGames; ++i)
        {
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

            TXCMD(bo)->cmd = CMD_CFGGAME;
            TXCMD(bo)->len = i;
            pGameCfg->save(TXCMD(bo)->payload, &TXCMD(bo)->len);
            if (!_eCom->notifyEngine(bo, bi)) break;
            if (RXCMD(bi)->payload[0] != CMD_ACK) break;
            LOG_DEBUG("sendGamesConfig->Game: %u", i);
        }

        sal = (i == totalGames);
        LOG_DEBUG("sendGamesConfig->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Envia el comando para salvar la configuracion.
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool engConfig::sendSaveConfig(std::shared_ptr<EngineCom>& _eCom)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_SAVECFG;
        TXCMD(bo)->len = 0;

        // Como la grabacion de Flash demora, vamos a alterar
        // el tiempo de espera
        if (_eCom->notifyEngine(bo, bi, 10000)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendSaveConfig->%s", sal ? "true" : "false");

        return sal;
    }
}

