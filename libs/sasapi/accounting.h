#ifndef accountingH
#define accountingH

#include "enginecom.h"
#include "contadores.h"

#include <memory>

namespace EngineAPI
{
    /**
     * @brief class que representa el manejo de Accounting de SAS
     * 
     */
	class accounting
	{
	public:
		static accounting* init(std::shared_ptr<EngineCom>& _ecom);

        /**
         * @brief Obtiene un puntero al objeto de contadores (solo lectura)
         * 
         * @return const counters* Puntero a los contadores
         */
        const counters* getCounters() const noexcept
        {
            return &sasCtrs;
        }

		bool resetCtrsToZero(const bool _local = false);
		bool sendCounter(const uint32_t _gn, const uint8_t _ctrId, const uint64_t _val);
		void setCounter(const uint32_t _gn, const uint8_t _ctrId, const uint64_t _val);
        bool sendCounters(const uint32_t _gn, const std::vector<uint8_t>& _ctrs);

        // Pago Manual
        bool sendHandPay(const handPay_t& _hp);
        bool resetHandPay();

		// Legacy Pay
        bool sendLegacyPayOrMultJkpt(const lgyBonusPay_t& _lgcy);

        bool sendBill(const lastAccBill& _bill);

	private:
		std::shared_ptr<EngineCom> 	eCom;       //!< Objeto de comunicacion con el SE
		counters					sasCtrs;    //!< Objeto de Contadores
		static accounting*			pAcc;       //!< Singleton. Solo puede haber un accounting
	};
}

#endif // accountingH
