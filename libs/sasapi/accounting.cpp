#include "accounting.h"
#include "file_log.h"
#include <cassert>

namespace EngineAPI
{
	accounting* accounting::pAcc = nullptr;

	/**
	 * @brief Inicializa el objeto de accounting (si ya no lo estaba)
	 * 
	 * @param _eCom Objeto de comunicacion con el SE
	 * @return accounting* Puntero al objeto de accounting
	 */
	accounting* accounting::init(std::shared_ptr<EngineCom>& _eCom)
	{
		assert(_eCom && "_eCom no valido");
		if (pAcc) return pAcc;

		try
		{
			pAcc = new accounting;
			pAcc->eCom = _eCom;
		}
		catch(const std::exception& e)
		{
			LOG_ERROR("Error creating new accounting: %s", e.what());
		}
		
		return pAcc;
	}

	/**
     * @brief Envia el comando de reset a cero de los contadores al SE.
     * Si la GM ofrece el borrado de contadores, esta es la apia utilizar
     * ya que fuerza al SE a enviar el evento correspondiente
     * 
     * @param _local true si el borrado es local, false (default) si se solicita
     * borrado al engine.
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::resetCtrsToZero(const bool _local)
    {
        bool sal = false;

        if (!_local)
        {
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

            TXCMD(bo)->cmd = CMD_RESETCTRS;
            TXCMD(bo)->len = 0;
            if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        }
        else
        {
            sal = true;
        }

        if (sal) sasCtrs.resetToZero();
        LOG_DEBUG("resetCtrsToZero->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia un pago manual al SE.
     * Al momento de entrar en pago manual, la GM debe llamar este método.
     *
     * @param _hp Datos del hand pay
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::sendHandPay(const handPay_t& _hp)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_HANDPAY;
        TXCMD(bo)->len = sizeof(_hp);
        memcpy(TXCMD(bo)->payload, &_hp, sizeof(_hp));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendHandPay->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Resetea el Hand Pay
     * Este método se invoca cuando el operador resetea el pago manual.
     * 
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::resetHandPay()
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_RHNDPAYPC;
        TXCMD(bo)->len = 0;
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("resetHandPay->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia un contador al SE
     * Con este método, se envía el valor de un contador en particular, para un juego en particular.
     * El juego 0, representa los contadores generales de la GM.
     * 
     * @param _gn Nro de juego. 0 -> Contador de maquina
     * @param _ctrId Id de SAS del contador
     * @param _val Valor del contador
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::sendCounter(const uint32_t _gn, const uint8_t _ctrId, const uint64_t _val)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        txCounter* ctr = reinterpret_cast<txCounter*>(TXCMD(bo)->payload);
        bool sal = false;

        TXCMD(bo)->cmd = CMD_COUNTER;
        TXCMD(bo)->len = sizeof(txCounter);
        ctr->game = static_cast<uint8_t>(_gn);
        ctr->code = _ctrId;
        ctr->value = _val;
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        if (sal) sasCtrs.setCounter(_gn, _ctrId, _val);
        LOG_DEBUG("sendCounter(%u, %u, %llu)->%s", _gn, _ctrId, _val, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Setea el valor de un contador localmente. 
     * Esto se utiliza en conjunto con el comando sendCounters para enviar 
     * varios (hasta 20) contadores simultaneamente
     * 
     * @param _gn Nro de juego. 0 -> Contador de maquina
     * @param _ctrId Id de SAS del contador
     * @param _val Valor del contador
     */
    void accounting::setCounter(const uint32_t _gn, const uint8_t _ctrId, const uint64_t _val)
    {
        sasCtrs.setCounter(_gn, _ctrId, _val);
    }

    /**
     * @brief Envia un conjunto (hasta 20) de contadores al SE
     * 
     * @param _gn Nro de juego. 0 -> Contador de maquina
     * @param _ctrs Vector de Ids de SAS de contadores a enviar
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::sendCounters(const uint32_t _gn, const std::vector<uint8_t>& _ctrs)
    {
        assert(!_ctrs.empty() && "La lista de contadores no puede estar vacia");

        uint32_t ctrsLeft = _ctrs.size();
        uint32_t ctr = 0;

        do
        {
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
            uint32_t nc = (ctrsLeft > 20) ? 20 : ctrsLeft;

            TXCMD(bo)->cmd = CMD_COUNTERS;
            TXCMD(bo)->len = nc * sizeof(txCounter);
            txCounter* pCtr = reinterpret_cast<txCounter*>(TXCMD(bo)->payload);

            for (uint32_t i = 0; i < nc; ++i)
            {
                pCtr[i].game = _gn;
                pCtr[i].code = _ctrs[ctr];
                pCtr[i].value = sasCtrs.getCounter(_gn, _ctrs[ctr]);
                ++ctr;
            }

            if (eCom->notifyEngine(bo, bi) && (RXCMD(bi)->payload[0] == CMD_ACK))
            {
                ctrsLeft -= nc;
            }
            else
            {
                LOG_DEBUG("sendCounters(%u,...)->false", _gn);
                return false;
            }
        } while (ctrsLeft);

        LOG_DEBUG("sendCounters(%u,...)->true", _gn);
        return true;
    }

    /**
     * @brief Envia los datos de un bonus legacy o jackpot multiplied al SE.
     * 
     * @param _lgcy Datos del pago
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::sendLegacyPayOrMultJkpt(const lgyBonusPay_t& _lgcy)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_LGCYPMJKPT;
        TXCMD(bo)->len = sizeof(_lgcy);
        memcpy(TXCMD(bo)->payload, &_lgcy, sizeof(_lgcy));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendLegacyPayOrMultJkpt()->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia Los datos sobre el ultimo billete aceptado al SE
     * Este metodo se llama cuando la GM acepta un billete desde el bill acceptor.
     * 
     * @param _bill Datos del ultimo billete
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool accounting::sendBill(const lastAccBill& _bill)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_SETLBA;
        TXCMD(bo)->len = sizeof(_bill);
        memcpy(TXCMD(bo)->payload, &_bill, sizeof(_bill));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendBill()->%s", sal ? "true" : "false");

        return sal;
    }
}
