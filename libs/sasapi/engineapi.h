#ifndef engineapiH
#define engineapiH

#include <cstdint>
#include <vector>
#include <array>
#ifndef __linux__
    #include <windows.h>
#endif

#include <mutex>

#include "serial.h"
#include "enginedefs.h"

#include "../uart_test/Core/Src/enginesvrdefs.h"
#include "../uart_test/Core/Src/gendefs.h"

#define MAX_TXRX     1024

namespace EngineAPI
{
    /**
     * @brief Palos de las cartas
     * 
     */
    enum sasCardSuits
    {
        spades,         //!< Picas
        clubs,          //!< Treboles
        hearts,         //!< Corazones
        diamonds,       //!< Diamantes
        jokerSuit,      //!< Comodin
        otherSuit       //!< Otros
    };

    /**
     * @brief Cartas
     * 
     */
    enum sasCards
    {
        two,            //!< Dos
        three,          //!< Tres
        four,           //!< Cuatro
        five,           //!< Cinco
        six,            //!< Seis
        seven,          //!< Siete
        eight,          //!< Ocho
        nine,           //!< Nueve
        ten,            //!< Diez
        jack,           //!< Jack
        queen,          //!< Reina
        king,           //!< Rey
        ace,            //!< As
        jokerCard,      //!< Comodin
        otherCard       //!< Otra
    };

    /**
     * @brief Tipo de mano
     * 
     */
    enum sasHandType
    {
        dealt,          //!< Mano comun
        final           //!< Mano final
    };

    /**
     * @brief Union para tomar los unit16_t de a 1 byte
     * 
     */
    union rev16_t
	{
		uint8_t 	a[2];
		uint16_t 	val{0};
	};

    /**
     * @brief Estructura interna de comunicaciones
     * 
     */
    struct commData_t
    {
        rev16_t     size;                           //!< Tamaño del paquete arribado
        uint32_t 	lastPktSent{0},                 //!< Tick del ultimo paquete enviado
        			errCount{0},                    //!< Contador de errores
                    txSize{0},                      //!< Cantidad de bytes a transmitir
                    rxSize{0};                      //!< Cantidad de bytes recibidos (FSM)
        uint8_t* 	txBuff{nullptr};                //!< Buffer de Tx
        uint8_t* 	rxBuff{nullptr};                //!< Buffer de Tx
        engFSMState decoState{engFSMState::esIdle}; //!< Estado de la FSM de recepcion
    };

    /**
     * @brief Estructura de configuracion de la API
     * 
     */
    struct configSE_t
    {
#ifdef __linux__
        std::string device;                         //!< Device serial
#else
        uint32_t	portNbr{0U};       		    	//!< Nro de puerto Serie
#endif
        char        portCfg[20]{0};                 //!< Config del puerto serie
        uint8_t     SEID[3 * sizeof(uint32_t)];     //!< ID del SE
        uint32_t    rxTimeOut{0};                   //!< Timeout de recepcion de paquetes
    };
}

#endif // engineapiH

