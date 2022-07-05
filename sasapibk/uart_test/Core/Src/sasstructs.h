/*
 * sasstructs.h
 *
 *  Created on: 12 oct. 2020
 *      Author: Juan
 */

#ifndef SRC_SASSTRUCTS_H_
#define SRC_SASSTRUCTS_H_

#include <stdint.h>
#include <stdbool.h>
#include "gendefs.h"

#define MAX_PAYLOAD		256
#define MAX_CFG_GAMES	25 // 0-> Machine + 24 juegos (1..24)

/*! \file */

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

/**
 * @brief Comando entre la PC y el SE
 * 
 */
struct command_t
{
	uint8_t 	cmd;					//!< Codigo del comando
	uint16_t	len;					//!< Cantidad de bytes en payload
	uint16_t	crc;					//!< Uso interno
	uint8_t		payload[0];				//!< Datos del comando
};

/**
 * @brief Estructura para transmision de contadores
 * 
 */
typedef struct
{
	uint8_t 	game;				//!< Juego al que pertenece el contador
	uint8_t 	code;				//!< Codigo SAS del contador
	uint64_t	value;				//!< Valor del contador
} txCounter;

/**
 * @brief Estructura con los datos del ticket
 */
typedef struct
{
	uint16_t 	hostID;				//!< Host identification number
	uint8_t		expiration;			//!< Number of days before ticket expires (00 = never expires)
	char 		location[40];		//!< Ubicacion
	char 		addr1[40];			//!< Address line 1
	char 		addr2[40];			//!< Address line 2
	char 		resTktTitle[16];	//!< Restricted ticket title
	char 		debTktTit[16];		//!< Debit ticket title
} tktData;

/**
 * @brief Estructura con la configuracion para SAS
 * 
 */
typedef struct
{
	uint8_t id;					//!< Id de SAS (1..127)
	uint8_t cantCanales;		//!< Cantidad de canales SAS habilitados 1 o 2
	uint8_t numDigxContador;	//!< Cantidad de digitos x contador 8 a 18
	uint8_t	legacyHP;			//!< 1 -> Support legacy handpay, 0 -> Not supported
} cfgSAS;

/**
 * @brief Extended Validation Status
 */
typedef struct
{
	uint16_t 	mask;				//!< Mascara de control (indica que se puede alterar y que no de stBitCtrl)
	uint16_t	stBitCtrl;			//!< Bits que indican estado
	uint16_t	cashTktRecExp;		//!< Dias de expiracion para tickets cashout y recibos
	uint16_t	resTktdefExp;		//!< Numero default de dias para que expire un ticket restricted
	uint32_t 	assetNbr;			//!< Asset Number ingresado por operador
} extValStatus_t;


/**
 * @brief Estructura con la configuracion general del equipo
 * 
 */
typedef struct
{
	char			serialNbr[41];			//!< Nro serie del equipo (0..40 bytes; finaliza en 0 binario)
	uint8_t			machineFeatures[3];		//!< Features del comando A0
	uint8_t			tokenDenom;				//!< Token denomination tabla C-4
	extValStatus_t	extVal;					//!< Info de Extended Validation
	tktData			ticketData;				//!< Config del operador de los datos del ticket
} cfgMaq;

/**
 * @brief Estructura con la configuracion de un juego
 * 
 */
typedef struct
{
	uint8_t		gameNbr;				//!< Nro de juego (1..MAX_CFG_GAMES)
	char		gameID[2];				//!< Game ID
	char		aditionalGameID[3];		//!< Aditional Game ID
	uint8_t		denomination;			//!< Denominacion
	uint16_t	maxBet;					//!< Apuesta maxima
	uint8_t		prgsvGroup;				//!< Grupo progresivo
	uint32_t	prgsvLevels;			//!< Mascara de niveles
	uint16_t	gameOptions;			//!< Opciones
	char		payTableID[6];			//!< ID de tabla de pagos
	char		basePercent[4];			//!< Theoretical base pay back percentage for maximum bet (Promedio de todos los juegos)
	char		name[21];				//!< Nombre
	char		payTableName[21];		//!< Nombre de la tabla de pagos
	uint16_t	wagerCats;				//!< Wager categories
	uint8_t		enabled;				//!< 1 si esta activo, 0 si no
} cfgGame;

/**
 * @brief Estructura con la configuracion de los billetes enviada por SAS
 * Bills, los elementos del array representan:
 * 0 -> $1, 1 -> $2, 2 -> $5 .. 8 -> $200 ... 16 -> $20.000
 * 
 */
typedef struct
{
	uint8_t bills[24];			//!< 1 -> Bill Habilitado, 0 -> Deshabilitado
	uint8_t billAccptMode;		//!< 0 -> Disable bill acceptor after each accepted bill
								//!< 1 -> Keep bill acceptor enabled after each accepted bill
} cfgBills_t;

/**
 * @brief Estructura con los datos de un pago manual
 * 
 */
typedef struct
{
	uint8_t 	prgsvGroup;		//!< Grupo progresivo
	uint8_t		prgsvLevel;		//!< Nivel progresivo
	uint64_t 	amount;			//!< Monto del pago
	uint16_t 	parcialPay;		//!< Pago parcial hecho anterior
	uint8_t 	resetId;		//!< Metodo de reset disponible
}  handPay_t;

/**
 * @brief Estructura con el ultimo billete aceptado
 * 
 */
typedef struct
{
	uint8_t countryCode;	//!< Codigo SAS de Pais
	uint8_t denoCode;		//!< Codigo de denominacion SAS del billete
	uint32_t billMeter;		//!< Valor del contador de billetes correspondiente
} lastAccBill;

/**
 * @brief Estructura con los datos de un bonus legacy o multiplied jackpot
 * 
 */
typedef struct
{
	uint8_t multiplier;		//!< Multiplicador
	uint32_t multWin;		//!< WIn multiplicado
	uint8_t taxStatus;		//!< Estado de impuestos
	uint32_t bonus;			//!< Valor del bonus
} lgyBonusPay_t;

/**
 * @brief Estructura con los datos del inicio de una partida
 * 
 */
typedef struct
{
	uint16_t credWag;		//!< Creditos apostados
	uint32_t totCoinIn;		//!< Total Coin In
	uint8_t wagType;		//!< Tipo de apuesta
	uint8_t prgsvGrp;		//!< Grupo progresivo
} gameStart_t;

/**
 * @brief Estructura con los datos del fin de una partida
 * 
 */
typedef struct
{
	uint32_t win;	//!< Ganancia de la partida
} gameEnd_t;

/**
 * @brief Estructura con los datos de parada de un rodillo
 * 
 */
typedef	struct
{
	uint8_t reelNbr;	//!< Nro de rodillo
	uint8_t physStp;	//!< Pos fisica de parada
} reelNStop_t;

/**
 * @brief Estructura con el estado de parada de los rodillos (los primeros 9)
 * 
 */
typedef struct
{
	reelNStop_t stops[9];	//!< Datos de los rodillos
} reelStopInfo_t;

/**
 * @brief Estructura con los datos de una mano de cartas
 * 
 */
typedef struct
{
	uint8_t handType;	//!< Tipo SAS de mano
	uint8_t hand[5];	//!< Datos de las primeras 5 cartas
} cardInfo_t;

/**
 * @brief Estructura con los datos para habilitar / deshabilitar un juego,
 * En multigame y Multi Deno
 * 
 */
typedef struct
{
	uint8_t gameNbr;	//!< Nro de juego
	uint8_t enable;		//!< 1 -> Enable, 0 -> Disable
	uint8_t denom;		//!< Para que denominacion se deshabilita / habilita
} enaDisGame_t;

/**
 * @brief Estructura con la semilla para el calculo del ROM Signature
 * 
 */
typedef struct
{
	uint16_t seed;	//!< Semilla 
} romSig_t;

/**
 * @brief Estructura con el metodo de reset para los jackpot hand payed
 * 
 */
typedef struct
{
	uint8_t method;			//!< Metodo SAS (0 -> Standard HP, 1 -> Reset to credit meter)
} jackpotHPResetMethod_t;

/**
 * @brief Estructura con la bandera de hab / deshab del auto re-bet
 * 
 */
typedef struct
{
	uint8_t enable;		//!< 0 -> Disable, 1 -> Enable
} autoReBet_t;

/**
 * @brief Estructura con el flag de habilitacion / deshabilitacion del modo RTE
 * 
 */
typedef struct
{
	uint8_t enabled;	//!< 0 -> Disable, 1 -> Enable
} RTE_t;

/**
 * @brief Estructura con los datos de la entrada del historico de juegos
 * 
 */
typedef struct
{
	uint16_t gameNbr;	//!< Nro de juego de este historico
	uint16_t recIdx;	//!< Indice de la partida
} gameRecEntry_t;

/**
 * @brief Estructura con la carta fijada / liberada
 * 
 */
typedef struct
{
	uint8_t card;	//!< Carta que se fijo / libero. Bits 0 .. 4 cartas de izq a der, or 0x80 -> Fijada
} cardHeld_t;

/**
 * @brief Estructura con el nro de juego seleccionado
 * 
 */
typedef struct
{
	uint16_t gameNbr;	//!< Numero de juego
} gameSel_t;

/**
 * @brief Estructura de transmision de eventos RTE
 * 
 */
typedef struct
{
	uint8_t evt;						//!< Codigo de Evento

	union
	{
		lastAccBill		billAcc;		//!< Datod del evento EX_BILL_ACCEPTED
		lgyBonusPay_t 	lgyBonusPay;	//!< Datod del evento EX_LGCYPAYORMJ_OCURRED
		gameStart_t		gameStart;		//!< Datod del evento EX_GAME_STARTED
		gameEnd_t		gameEnd;		//!< Datod del evento EX_GAME_ENDED
		reelNStop_t		reelNStp;		//!< Datod del evento EX_REELN_STOPPED
		gameRecEntry_t	gameRecEntry;	//!< Datod del evento EX_GAMERECALLENTR
		cardHeld_t		cardHeld;		//!< Datod del evento EX_CARDHELD_UNHLD
		gameSel_t		gameSel;		//!< Datod del evento EX_GAME_SELECTED
	} data;
} rtEvent;

/**
 * @brief Estructura que representa un registro de validacion (TITO)
 */
typedef struct
{
	uint8_t		type;				//!< Tipo de validacion
	uint8_t		fecha[4];			//!< MMDDYYYY BCD
	uint8_t		hora[3];			//!< HHMMSS 24hs BCD
	uint8_t		validationNbr[8];	//!< Validation Number BCD
	uint8_t 	amount[5];			//!< Monto del ticket en centavos BCD
	uint16_t	number;				//!< Ticket number 0001 - 9999 (Roll over 9999 -> 0000) FFFF
									//!< validation sin ticket
	uint8_t		valSysID;			//!< 00 -> Secure enhanced, 01-99 -> System ID
	uint8_t		expiration[4];		//!< MMDDYYYY o 00000001-00009998 -> Cant de dias hasta expirar
	 	 	 	 	 	 	 	 	//!< 00009999 -> Nunca expira o 00000000 -> No se imprimio o no se
	 	 	 	 	 	 	 	 	//!< soportan extensiones de validacion
	uint16_t	poolID;				//!< 0000 -> Not restricted o desconocido
} valRecord;

/**
 * @brief Estructura que representa los datos de validacion en modo Secure Enhanced
 */
typedef struct
{
	uint32_t id;		//!< Id de validacion
	uint32_t seqNbr;	//!< Nro de secuencia
} secureEnhValid;

/**
 * @brief Estructura que representa los datos de validacion en modo System
 */
typedef struct
{
	uint8_t 	id;			//!< Id de validacion
	uint64_t 	number;		//!< Validation number
	uint8_t		coType;		//!< Tipo de cashout
	uint64_t	amnt;		//!< Monto en centavos
	bool		ready;		//!< true si el host respondio
} systemValid;

/**
 * @brief Estructura con los datos del ticket in
 */
typedef struct
{
    uint8_t 	status;         //!< Ticket status (0x00 -> Escrow, 0xFF -> None in escrow)
    uint64_t    amount;         //!< Monto en centavos
    uint8_t     validData[9];	//!< Validation data
    bool		sent;			//!< true si se envio
} ticket_t;

/**
 * @brief Estructura con los datos de redencion de un ticket
 */
typedef struct
{
	uint8_t 	txCode;			//!< Codigo de operacion
	uint64_t	txAmount;		//!< Monto del ticket
	uint8_t		validData[9];	//!< Validation Data
	uint8_t		restExp[4];		//!< Restricted expiration
	uint16_t	poolId;			//!< Restricted pool ID
	bool		valid;			//!< true si tiene datos
} tktRedeem_t;

/**
 * @brief Estructura de datos para TicketRedeemed (PC -> SE)
 */
typedef struct
{
	uint8_t		machineSt;		//!< Machine status
	uint64_t	txAmount;		//!< Monto en centavos de la Tx
    uint8_t     validData[9];	//!< Validation data
} tktRedeemSt_t;


/**
 * @brief Estructura de datos de registracion AFT
 */
typedef struct
{
	uint8_t		status;			//!< AFT registration status
	uint32_t	posId;			//!< POS Id (si es 0, no se pueden hacer Debit transfers)
	uint8_t		regKey[20];		//!< AFT Registration Key
} aftRegData_t;

/**
 * @brief Estructura de datos del request de lock AFT
 */
typedef struct
{
	uint8_t 	code;			//!< Lock Code
	uint8_t 	transferCond;	//!< Mascara de transfer conditions (condiciones que debe cumplir la GM
								//!< para entrar en estado LOCKED)
	uint16_t	lockTimeout;	//!< Lock expiration time in hundredths of a second (0 - 9999)
} aftLockReq_t;

/**
 * @brief Estructura con los datos del estado AFT
 *
 */
typedef struct
{
	uint8_t status;				//!< Lock Status
	uint8_t availTrx;			//!< Available Trx
	uint8_t hostSt;				//!< Host Cashout Status
	uint8_t aftSt;				//!< AFT Status

	uint64_t	currCashAmnt;	//!< Current cashable amount on gaming machine, in cents
	uint64_t	currResAmnt;	//!< Current restricted amount on gaming machine, in cents
	uint64_t	currNRAmnt;		//!< Current nonrestricted amount on gaming machine, in cents
	uint64_t	gmTrxLimit;		//!< Maximum amount that may currently be transferred to the credit meter, in cents
	uint8_t		restExp[4];		//!< Current restricted expiration date in MMDDYYYY
								//!< format or 0000NNNN days format, if restricted amount non-zero
	uint16_t	resPoolId;		//!< Current restricted pool ID, if restricted non-zero
} aftLockStat_t;

/**
 * @brief Estructura que representa una transaccion AFT
 */
typedef struct
{
	uint8_t		code;				//!< AFT transaction Code 0 -> FULL, 1 -> Partial
	uint8_t		status;				//!< AFT Transfer Status
	uint8_t		rcpStatus;			//!< Receipt status
	uint8_t		type;				//!< AFT transfer type
	uint64_t	cashAmnt;			//!< Cashable amount, in cents
	uint64_t	restAmnt;			//!< Restricted amount, in cents
	uint64_t	nonRestAmnt;		//!< Current nonrestricted amount, in cents
	uint8_t		flags;				//!< AFT transfer flags
	uint8_t		trxIdLen;			//!< Transaction ID Lenght
	uint8_t		trxID[20];			//!< Transaction ID
	uint8_t		date[4];			//!< Trx Date BCD
	uint8_t		time[3];			//!< Trx Time BCD
	uint8_t 	expiration[4];		//!< Expiration date in MMDDYYYY format or 0000NNNN days format
	uint16_t	poolId;				//!< Restricted pool ID
	uint64_t	cumCashAmntMt;		//!< Cummulative casheable amount meter, in cents
	uint64_t	cumResAmntMt;		//!< Cummulative restricted amount meter, in cents
	uint64_t	cumNonResAmntMt;	//!< Cummulative non restricted amount meter, in cents
} aftTrx_t;

/**
 * @brief Estructura que representa un request de transferencia AFT
 */
typedef struct
{
	uint8_t		code;				//!< AFT transaction Code 0 -> FULL, 1 -> Partial
	uint8_t		status;				//!< AFT Transfer Status
	uint8_t		rcpStatus;			//!< Receipt status
	uint8_t		type;				//!< AFT transfer type
	uint64_t	cashAmnt;			//!< Cashable amount, in cents
	uint64_t	restAmnt;			//!< Restricted amount, in cents
	uint64_t	nonRestAmnt;		//!< Current nonrestricted amount, in cents
	uint8_t		flags;				//!< AFT transfer flags
	uint8_t		trxIdLen;			//!< Transaction ID Lenght
	uint8_t		trxID[20];			//!< Transaction ID
	uint8_t		date[4];			//!< Trx Date BCD
	uint8_t		time[3];			//!< Trx Time BCD
	uint8_t 	expiration[4];		//!< Expiration date in MMDDYYYY format or 0000NNNN days format
	uint16_t	poolId;				//!< Restricted pool ID
	uint64_t	cumCashAmntMt;		//!< Cummulative casheable amount meter, in cents
	uint64_t	cumResAmntMt;		//!< Cummulative restricted amount meter, in cents
	uint64_t	cumNonResAmntMt;	//!< Cummulative non restricted amount meter, in cents
	uint8_t		rcptDataLen;		//!< Receipt Data Lenght
	uint8_t		receiptData[165];	//!< Receipt data
	uint16_t	lockTimeout;		//!< Lock Timeout
} aftTrxCmd_t;


/**
 * @brief Estructura que representa los items variables de un ticket AFT
 */
typedef struct
{
	uint8_t size;
	uint8_t data[255];
} aftTktData_t;

#pragma pack()

#ifdef __cplusplus
}
#endif


#endif /* SRC_SASSTRUCTS_H_ */
