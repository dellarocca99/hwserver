#include "aft.h"
#include "utils.h"

#include <cassert>
#include <algorithm>
#include <iterator>
#include <ctime>

namespace EngineAPI
{
	void AFT::init(std::shared_ptr<EngineCom>& _eCom)
	{
		assert(_eCom && "_eCom no valido");

		eCom = _eCom;
	}

    /**
     * @brief Obtiene la información de registro de AFT
     * La información de registro AFT solo esta disponible si el GM fue registrado previamente
     * 
     * @param _regInfo Informacion de registro
     * @return true Si se pudo enviar el comando
     * @return false Si no se pudo enviar
     */
    bool AFT::getAFTRegInfo(aftRegData_t& _regInfo)
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_GETAFTREGIN;
        TXCMD(bo)->len = 0;
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
            if (sal) memcpy(&_regInfo, &RXCMD(bi)->payload[1], sizeof(_regInfo));
        }

        LOG_DEBUG("protocolDebugger()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Inicia el proceso de registro AFT desde la PC.
     * El registro AFT solo es necesario en caso de requerirse transferencias DEBIT
     * 
     * @return true Si se pudo enviar el comando
     * @return false Si no se pudo enviar
     */
    bool AFT::initAFTRegistration()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_INITAFTREG;
        TXCMD(bo)->len = 0;
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("initAFTRegistration()->%s", sal ? "true" : "false");
        return sal;
    }


    /**
     * @brief Envia el ACK del operador para una registracion AFT
     * A veces (no es obligatorio en el standard por lo que el host puede no solicitarla)
     * el host solicita la confirmación de registro AFT; de ser así, este método debe ser utilizado
     * para responder luego que el operador confirme (mediante llave u opción de menu).
     * 
     * @return true Si se pudo enviar el comando
     * @return false Si no se pudo enviar
     */
    bool AFT::sendAFTRegOperatorACK()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_OPACKAFTREG;
        TXCMD(bo)->len = 0;
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("sendAFTRegOperatorACK()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Modifica los datos del Lock Status de AFT
     * Este método se utiliza para informar al HOST SAS los parámetros requeridos por el mecanismo
     * de lock AFT
     * 
     * @param _ls Informacion de lock AFT
     * @param _st Estado de lock AFT (ver AFT Lock status)
     * @return true Si se pudo enviar el comando
     * @return false Si no se pudo enviar
     */
    bool AFT::updateAFTLsData()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        auto pLs = lock.getTxData();

        TXCMD(bo)->cmd = CMD_UPDAFTDATA;
        TXCMD(bo)->len = sizeof(aftLockStat_t);
        memcpy(TXCMD(bo)->payload, pLs, sizeof(aftLockStat_t));
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("updateAFTData()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Da por finalizada una transferencia AFT
     * Esté método se utiliza para dar por finalizada una transferencia AFT
     * tanto por exito como por rechazo.
     * 
     * @return true Pudo enviar el comando
     * @return false No pudo enviar el comando
     */
    bool AFT::endAFTTransaction()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        auto pTxData = transfer.getTxData();

        // Informo la FyH de finalizacion
        bcdDateTime(pTxData->date,  pTxData->time);

        // Si la transferencia fallo, los montos van en 0
        if (pTxData->status & 0x80)
        {
            pTxData->cashAmnt = 0;
            pTxData->restAmnt = 0;
            pTxData->nonRestAmnt = 0;
        }

        TXCMD(bo)->cmd = CMD_ENDAFTTR;
        TXCMD(bo)->len = sizeof(aftTrx_t);
        memcpy(TXCMD(bo)->payload, pTxData, sizeof(aftTrx_t));
        if (eCom->notifyEngine(bo, bi, 2000))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("endAFTTransaction()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Actualiza el estado y valores de una transferencia AFT
     * Este método se utiliza para actualizar los valores solicitados en la transferencia.
     * 
     * @return true Pudo enviar el comando
     * @return false No pudo enviar el comando
     */
    bool AFT::updateAFTTrStatus()
    {
        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        auto pTxData = transfer.getTxData();

        // Me aseguro que FyH vayan en 0
        memset(pTxData->date, 0, sizeof(pTxData->date));
        memset(pTxData->time, 0, sizeof(pTxData->time));

        TXCMD(bo)->cmd = CMD_UAFTTRSTAT;
        TXCMD(bo)->len = sizeof(aftTrx_t);
        memcpy(TXCMD(bo)->payload, pTxData, sizeof(aftTrx_t));
        if (eCom->notifyEngine(bo, bi, 2000))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("updateAFTTrStatus()->%s", sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Solicita al HOST que inicie una transferencia desde la GM -> Host
     * 
     * @param _type Tipo de transferencia (AFT_RQSTHOSTCO -> Normal, AFT_RQSTHOSTCOW -> Win)
     * @return true Se pudo hacer la solicitud
     * @return false No se pudo hacer la solicitud
     */
    bool AFT::requestHostCashout(const uint8_t _type)
    {
        assert(((_type == AFT_RQSTHOSTCO) || (_type == AFT_RQSTHOSTCOW)) && "_type incorrecto");

        bool sal = false;
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

        TXCMD(bo)->cmd = CMD_RQSTHOSTCO;
        TXCMD(bo)->len = 1;
        TXCMD(bo)->payload[0] = _type;
        if (eCom->notifyEngine(bo, bi))
        {
            sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }

        LOG_DEBUG("requestHostCashout(%u)->%s", _type, sal ? "true" : "false");
        return sal;
    }

    /**
     * @brief Establece los datos de la transferencia en curso
     * 
     * @param _trx Comando de transferencia recibido
     */
    void aftTransfer::setCurrent(const aftTrxCmd_t& _trx)
    {
        currTrxRx = _trx;
        memcpy(&currTrxTx, &_trx, sizeof(currTrxTx));
    }

    /**
     * @brief Obtiene un array con los campos del recibo
     * 
     * @param _rcptData Array de campos 
     */
    void aftTransfer::getCustomReceiptData(std::vector<custRcptData_t>& _rcptData)
    {
        _rcptData.clear();
        int8_t size = currTrxRx.rcptDataLen;
        uint8_t pos = 0;
        uint8_t dataSize;

        while (size > 0)
        {
            custRcptData_t data;
            char aux[100];

            data.field = static_cast<eCustRcptFields>(currTrxRx.receiptData[pos++]);
            dataSize = currTrxRx.receiptData[pos++];
            memcpy(aux, &currTrxRx.receiptData[pos], dataSize);
            aux[dataSize] = '\0';
            data.value = aux;
            _rcptData.emplace_back(data);
            pos += dataSize;
            size -= dataSize;
        }
    }

    /**
     * @brief Obtiene el ID de la transaccion
     * 
     * @return std::string ID
     */
    std::string aftTransfer::getTransactionID() const
    {
        std::string sal;

        for (uint32_t i = 0; i < currTrxRx.trxIdLen; ++i)
            sal += static_cast<char>(currTrxRx.trxID[i]);
        return sal;
    }
    
    /**
     * @brief Obtiene la fecha de expiracion del ticket
     * 
     * @return std::string representando la fecha (YYYY-MM-DD)
     */
    std::string aftTransfer::getExpirationDate() const
    {
        char expDate[11];

        // Determinamos el tipo de formato
        if ((currTrxRx.expiration[0] == 0) && (currTrxRx.expiration[1] == 0))
        {
            // 0000NNNN days
            uint32_t days = (currTrxRx.expiration[2] >> 4) * 1000;
            days += (currTrxRx.expiration[2] & 0x0F) * 100;
            days += (currTrxRx.expiration[3] >> 4) * 10;
            days += (currTrxRx.expiration[3] & 0x0F);
            time_t now = time(nullptr) + (days * 86400);
#if defined(_MSC_VER)
            struct tm lt;

            localtime_s(&lt, &now);
            sprintf(
                expDate,
                "%04d-%02d-%02d",
                lt.tm_year + 1900,
                lt.tm_mon + 1,
                lt.tm_mday
            );
#else
            struct tm* lt = localtime(&now);
            sprintf(
                expDate,
                "%04d-%02d-%02d",
                lt->tm_year + 1900,
                lt->tm_mon + 1,
                lt->tm_mday
            );
#endif
        }
        else
        {
            // MMDDYYYY
            const uint32_t mon = (currTrxRx.expiration[0] >> 4) * 10 + (currTrxRx.expiration[0] & 0x0F);
            const uint32_t day = (currTrxRx.expiration[1] >> 4) * 10 + (currTrxRx.expiration[1] & 0x0F);
            const uint32_t year = (currTrxRx.expiration[2] >> 4) * 1000 +
                                (currTrxRx.expiration[2] & 0x0F) * 100 +
                                (currTrxRx.expiration[3] >> 4) * 10 +
                                (currTrxRx.expiration[3] & 0x0F);
            sprintf(expDate, "%04d-%02d-%02d", year, mon, day);
        }

        return std::string(expDate);
    }

    /**
     * @brief Ctor. Inicializa el Lock Status a no locked
     *
     */
    aftLock::aftLock()
    {
        memset(&lockStat, 0, sizeof(lockStat));
        lockStat.status = AFTLCK_ST_NOTLCK;
    }

    /**
     * @brief Establece la expiracion del monto restricted
     * 
     * @param _val Expiracion en BCD (date in MMDDYYYY format or 0000NNNN days format, if restricted amount non-zero)
     */
    void aftLock::setRestrictedExp(const uint8_t* _val)
    {
        memcpy(lockStat.restExp, _val, 4);
    }

    /**
     * @brief Asigna los datos del ticket AFT recibidos desde el SE (CMD_AFTTKTDATA). Ojo no pisar
     * todos los datos con los items recibidos aqui. Solo los codigos discriminados
     * 
     * @param _dataCmd Datos del comando CMD_AFTTKTDATA
     */
    void aftTicketData::setDataCmd(const aftTktData_t& _dataCmd)
    {
        tktDataCmd = _dataCmd;
    }

    /**
     * @brief Obtiene la lista de items enviados en el comando CMD_AFTTKTDATA
     * 
     * @param _items Vector con los items
     */
    void aftTicketData::getData(std::vector<aftTicketItem>& _items)
    {
        int32_t size = tktDataCmd.size;
        uint32_t pos = 0;

        _items.clear();
        if (size == 0) return;

        while (size > 0)
        {
            aftTicketItem item;
            int32_t itemSize;

            item.code = tktDataCmd.data[pos++];
            itemSize = tktDataCmd.data[pos++];
            std::copy(
            	&tktDataCmd.data[pos],
                &tktDataCmd.data[pos + itemSize],
                std::back_inserter(item.value)
            );
            _items.emplace_back(item);
            pos += itemSize;
            size -= itemSize + 2;
        }
    }

}
