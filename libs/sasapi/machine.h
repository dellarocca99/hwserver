#ifndef machineH
#define machineH

#include "enginecom.h"
#include <memory>

namespace EngineAPI
{
	/**
	 * @brief class que representa las funcionalidades SAS para el GM en general
	 * 
	 */
	class gameMachine
	{
	public:
		void init(std::shared_ptr<EngineCom>& _eCom);

	    // Eventos
		bool sendEvt(const uint8_t _evt);

        // Juegos
        bool sendSelectedGame(const uint32_t _gn);
        bool sendGameStart(const gameStart_t& _gs);
        bool sendGameEnd(const gameEnd_t& _ge);

        bool sendROMSignature(const uint16_t _romSig);

        bool sendReelStop(const uint8_t _rn, const uint8_t _fisStp);
        bool sendGameRecall(const uint16_t _gn, const uint16_t _recIx);
        bool sendCardHeld(const uint8_t _cn, const bool _held);
        bool sendCardInfo(const std::array<uint8_t, 5>& _cards, const sasHandType _ht);

	private:
		std::shared_ptr<EngineCom> eCom;
	};

	uint8_t makeCard(const sasCardSuits _suit, const sasCards _card) noexcept;
}

#endif // machineH
