#include "machine.h"
#include "engconfig.h"
#include "utils.h"

#include <cassert>
#include <cstring>

namespace EngineAPI
{
    /**
     * @brief Inicializa el objeto 
     * 
     * @param _eCom objeto de comunicacion con el SE
     */
	void gameMachine::init(std::shared_ptr<EngineCom>& _eCom)
	{
		assert(_eCom && "_eCom no es Valido");
		eCom = _eCom;
	}

    /**
     * @brief Envia un evento al SASEngine
     * 
     * @param _evt Codigo SAS del evento
     * @return true Si se envio
     * @return false Si no se envio
     */
	bool gameMachine::sendEvt(const uint8_t _evt)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_EVENT;
        TXCMD(bo)->len = 1;
        TXCMD(bo)->payload[0] = _evt;
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendEvt(%u)->%s", _evt, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia la SE el reporque que se ha seleccionado un juego 
     * 
     * @param _gn Nro de juego. 0 -> Ninguno, 1 -> Juego 1 ...
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendSelectedGame(const uint32_t _gn)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        gameSel_t* gs = reinterpret_cast<gameSel_t*>(TXCMD(bo)->payload);
        bool sal = false;

        if (_gn > engConfig::init()->getGame()->getNumGames())
        {
            LOG_DEBUG("sendSelectedGame(%u)->false: Juego fuera de rango", _gn);
            return false;
        }

        gs->gameNbr = _gn;
        TXCMD(bo)->cmd = CMD_SENDSELGAME;
        TXCMD(bo)->len = sizeof(*gs);
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendSelectedGame(%u)->%s", _gn, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia el resultado del calculo del ROM Signature al SE
     * 
     * @param _romSig Resultado del CRCCCITT16 de la ROM
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendROMSignature(const uint16_t _romSig)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_SETROMSIG;
        TXCMD(bo)->len = sizeof(_romSig);
        memcpy(TXCMD(bo)->payload, &_romSig, sizeof(_romSig));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendROMSignature()->%s", sal ? "true" : "false");

        return sal;
    }


    /**
     * @brief Envia los datos de inicio de partida al SE. Solo lo hace en modo RTE
     * 
     * @param _gs Datos de inicio de partida
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendGameStart(const gameStart_t& _gs)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_STARTGAME;
        TXCMD(bo)->len = sizeof(_gs);
        memcpy(TXCMD(bo)->payload, &_gs, sizeof(_gs));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendGameStart()->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia los datos de fin de partida al SE. Solo lo hace en modo RTE
     * 
     * @param _ge Datos de fin de partida
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendGameEnd(const gameEnd_t& _ge)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        bool sal = false;

        TXCMD(bo)->cmd = CMD_ENDGAME;
        TXCMD(bo)->len = sizeof(_ge);
        memcpy(TXCMD(bo)->payload, &_ge, sizeof(_ge));
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendGameEnd()->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia los datos de parada de rodillo al SE. Solo lo hace en modo RTE
     * Reel Order |1| |2| |3| ... |9|
     * 
     * @param _rn Nro de rodillo
     * @param _fisStp Posicion fisica de stop
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendReelStop(const uint8_t _rn, const uint8_t _fisStp)
    {
        // Los reels mayores a 9 se ignoran y el reel 0 es error
        assert((_rn > 0) && "El reel debe ser > 0");

        if (_rn < 10)
        {
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
            reelNStop_t* rs = reinterpret_cast<reelNStop_t*>(TXCMD(bo)->payload);
            bool sal = false;

            TXCMD(bo)->cmd = CMD_SENDRSTOP;
            TXCMD(bo)->len = sizeof(*rs);
            rs->reelNbr = _rn;
            rs->physStp = _fisStp;
            if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
            LOG_DEBUG("sendReelStop(%u, %u)->%s", _rn, _fisStp, sal ? "true" : "false");

            return sal;
        }
        else
        {
            LOG_DEBUG("sendReelStop(%u, %u)->(Not sent)", _rn, _fisStp);
            return true;
        }
    }

    /**
     * @brief Envia los datos del browsing del historico de partidas al SE. Solo lo hace en modo RTE
     * 
     * @param _gn Nro de juego
     * @param _recIx Indice de la partida visitada
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendGameRecall(const uint16_t _gn, const uint16_t _recIx)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        gameRecEntry_t* gre = reinterpret_cast<gameRecEntry_t*>(TXCMD(bo)->payload);
        bool sal = false;

        TXCMD(bo)->cmd = CMD_GAMERECETRY;
        TXCMD(bo)->len = sizeof(*gre);
        gre->gameNbr = _gn;
        gre->recIdx = _recIx;
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendGameRecall(%u, %u)->%s", _gn, _recIx, sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Envia los datos de las cartas que se fijan / liberan al SE. Solo lo hace en modo RTE
     * Card held, card order  |0| |1| |2| |3| |4|
     * 
     * @param _cn Nro de carta
     * @param _held true si se fijo, false si se libero
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendCardHeld(const uint8_t _cn, const bool _held)
    {
        if (_cn < 5)
        {
            bool sal = false;
            const uint8_t ch = _cn - 1;
            uint8_t bo[MAX_TXRX], bi[MAX_TXRX];

            TXCMD(bo)->cmd = CMD_CARDHELD;
            TXCMD(bo)->len = 1;
            TXCMD(bo)->payload[0] = ch | ((_held) ? 0x80 : 0x00);
            if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
            LOG_DEBUG("sendCardHeld(%u, %s)->%s", _cn, _held ? "true" : "false", sal ? "true" : "false");

            return sal;
        }
        else
        {
            LOG_DEBUG("sendCardHeld(%u, %s)->(Not sent)", _cn, _held ? "true" : "false");
            return true;
        }
    }

    /**
     * @brief Envia la informacion de la mano actual al SE.
     * Solo se reportan hasta las primeras 5 cartas de la mano (para manos mayores a 5)
     * el orden es de izq a der
     * 
     * @param _cards Array de cartas empaquetadas con makeCard
     * @param _ht Tipo de mano
     * @return true Si el comando se ejecuto
     * @return false Si no se ejecuto
     */
    bool gameMachine::sendCardInfo(const std::array<uint8_t, 5>& _cards, const sasHandType _ht)
    {
        uint8_t bo[MAX_TXRX], bi[MAX_TXRX];
        cardInfo_t* ci = reinterpret_cast<cardInfo_t*>(TXCMD(bo)->payload);
        bool sal = false;

        ci->handType = _ht;
        memcpy(ci->hand, _cards.data(), _cards.size());
        TXCMD(bo)->cmd = CMD_SENDCRDINF;
        TXCMD(bo)->len = sizeof(*ci);
        if (eCom->notifyEngine(bo, bi)) sal = (RXCMD(bi)->payload[0] == CMD_ACK);
        LOG_DEBUG("sendCardInfo()->%s", sal ? "true" : "false");

        return sal;
    }

    /**
     * @brief Empaqueta la info de una carta. Nibble alto -> Palo, Nibble bajo -> carta
     * 
     * @param _suit Palo de la carta
     * @param _card Carta
     * @return uint8_t Carta empaquetada
     */
    uint8_t makeCard(const sasCardSuits _suit, const sasCards _card) noexcept
    {
        return static_cast<uint8_t>(_card | (_suit << 4));
    }

}
