#ifndef titoH
#define titoH

#include "enginecom.h"
#include <memory>
#include <array>

namespace EngineAPI
{
    typedef std::array<uint8_t, 8> valNumber_t;

    /**
     * @brief class que representa la funcionalidad de TITO
     * 
     */
	class TITO
	{
	public:
		void init(std::shared_ptr<EngineCom>& _eCom);

        bool sendCashoutTktInfoStd(const uint32_t _validNbr, const uint64_t _monto);
        bool sendValidationRecord(
            const uint8_t _vt, 
            const uint64_t _amt, 
            const uint16_t _tktNbr,
            const valNumber_t& _valNbr,
            const std::vector<uint8_t>& _exp
            );
        bool getValidationID(secureEnhValid& _secEn);
        bool getValidationID(const uint8_t _coType, const uint32_t _amnt, systemValid& _sv);

        bool redeemTicket(const std::vector<uint8_t>& _vd, const uint32_t _amnt);
        bool getRedemptionStatus(ticket_t& _tktRedeem, bool& _valid);
        bool ticketRedeemed(const ticket_t& _trs);

	private:
		std::shared_ptr<EngineCom> eCom;
	};

	uint32_t calcValidationStd(const uint64_t _monto);
    void calcValidationSE(const secureEnhValid& _secEn, uint8_t* _valNbr);
    void strToBCD(const std::string& _str, std::vector<uint8_t>& _bcd);
}

#endif // titoH
