/*
 * enginesvrdefs.h
 *
 *  Created on: Oct 6, 2020
 *      Author: Juan
 */

#include <stdint.h>
#include "sasstructs.h"

#ifndef SRC_ENGINESVRDEFS_H_
#define SRC_ENGINESVRDEFS_H_

/*! \file */

/**
 * @brief Constantes que representan los comandos con el SAS Engine
 * 
 */
//#define STX             ((uint8_t)0x02)		//!< Start Tx Packet
//#define ETX             ((uint8_t)0x03)		//!< End Tx Packet

#define CMD_ACK         ((uint8_t)0x00)		//!< ACK de un comando

#define CMD_ROMSIG		((uint8_t)0x01)		//!< Calcular el ROM Signature
#define CMD_LOCKPLAY	((uint8_t)0x02)		//!< Bloquear la maquina
#define CMD_ENABLEPLAY	((uint8_t)0x03)		//!< Desbloquear la maquina
#define CMD_ENABILLACCP	((uint8_t)0x04)		//!< Habilitar el billetero
#define CMD_DISBILLACCP	((uint8_t)0x05)		//!< Deshabilitar el billetero
#define CMD_CFGBILLS	((uint8_t)0x06)		//!< Configurar billetes
#define CMD_ENAJKPHPRST	((uint8_t)0x07)		//!< Metodo habilitado de reset de jackpot hand pay
#define CMD_ENAAUTORBET	((uint8_t)0x08)		//!< Habilitar auto re-bet
#define CMD_RSTHANDPAY	((uint8_t)0x09)		//!< Se reseteo el Hand Pay
#define CMD_ENADISGAME	((uint8_t)0x0A)		//!< Enable / Disable game
#define CMD_ENADISRTE	((uint8_t)0x0B)		//!< Enable / Disable real time events
#define CMD_EXTVALST	((uint8_t)0x0C)		//!< Extended Validation Status
#define CMD_TKTDATA		((uint8_t)0x0D)		//!< Set ticket data
#define CMD_AFTREGFIN	((uint8_t)0x0E)		//!< Fin de registro AFT
#define CMD_AFTREGCNLD	((uint8_t)0x0F)		//!< Cancelada la registracion AFT
#define CMD_AFTOPERACK	((uint8_t)0x10)		//!< Solicitud de ACK del operador en registracion AFT
#define CMD_AFTLOCKREQ	((uint8_t)0x11)		//!< Solicitud de lockeo AFT
#define CMD_AFTINITRX	((uint8_t)0x12)		//!< Solicitud de Trx AFT
#define CMD_AFTTKTDATA	((uint8_t)0x13)		//!< Set AFT ticket data
#define CMD_AFTCTKTDATA	((uint8_t)0x14)		//!< Set AFT custom ticket data

#define CMD_UID			((uint8_t)0x40)		//!< Obtiene el UID del micro
#define CMD_COUNTER	    ((uint8_t)0x41)		//!< Enviar un contador
#define CMD_EVENT		((uint8_t)0x42)		//!< Envia un evento
#define CMD_CFGGAME     ((uint8_t)0x43)		//!< Enviar config del juego N
#define CMD_HANDPAY     ((uint8_t)0x44)		//!< Notifica un hand pay
#define CMD_CFGMAQ      ((uint8_t)0x45)		//!< Enviar config de la maquina
#define CMD_QUERY   	((uint8_t)0x46)		//!< Consulta generica
#define CMD_HELLO		((uint8_t)0x47)		//!< Comando de inicio
#define CMD_CFGSAS		((uint8_t)0x48)		//!< Cfg de SAS
#define CMD_SETROMSIG	((uint8_t)0x49)		//!< Setear el ROM Signature
#define CMD_SETHOPPER	((uint8_t)0x4A)		//!< Setea los datos del hopper
#define CMD_SETLBA		((uint8_t)0x4B)		//!< Setea el ultimo billete aceptado
#define CMD_FYH			((uint8_t)0x4C)		//!< Setea la fecha y hora del RTC
#define CMD_SAVECFG		((uint8_t)0x4D)		//!< Salva la config
#define CMD_COUNTERS	((uint8_t)0x4E)		//!< Envia varios contadores juntos
#define CMD_RESETCTRS	((uint8_t)0x4F)		//!< Reset a cero de contadores
#define CMD_SENDSELGAME	((uint8_t)0x50)		//!< Envia el juego seleccionado
#define CMD_STARTGAME	((uint8_t)0x51)		//!< Inicio de partida
#define CMD_ENDGAME		((uint8_t)0x52)		//!< Fin de juego
#define CMD_SENDRSTOP	((uint8_t)0x53)		//!< Reel stop info
#define CMD_GAMERECETRY	((uint8_t)0x54)		//!< Game recall entry
#define CMD_CARDHELD	((uint8_t)0x55)		//!< Card Held
#define CMD_LGCYPMJKPT	((uint8_t)0x56)		//!< Legacy pay or multiplied jackpot
#define CMD_SENDCRDINF	((uint8_t)0x57)		//!< Card info
#define CMD_GETVALINF	((uint8_t)0x58)		//!< Get Sec Enh Validation Info
#define CMD_SENDTKTSTD	((uint8_t)0x59)		//!< Send Std val ticket info
#define CMD_SENDVALRCD	((uint8_t)0x5A)		//!< Send validation record
#define CMD_SETNUMGAMES	((uint8_t)0x5B)		//!< Setea la cantidad de juegos
#define CMD_GETVALINFSY	((uint8_t)0x5C)		//!< Pide los datos de validacion System
#define CMD_GETVALINFST	((uint8_t)0x5D)		//!< Verifica que los datos de validation pedidos ya esten
#define CMD_READY		((uint8_t)0x5E)		//!< Se notifica al SE que debe iniciar comunicacion SAS
#define CMD_RESET		((uint8_t)0x5F)		//!< Warm reset del sistema
#define CMD_NVRESET		((uint8_t)0x60)		//!< Reset de nvram
#define CMD_TESTING     ((uint8_t)0x61)     //!< Comando de testing
#define CMD_RHNDPAYPC	((uint8_t)0x62)		//!< Se reseteo el Hand Pay desde PC
#define CMD_REDEEMTKT   ((uint8_t)0x63)     //!< Pedido de redencion de ticket
#define CMD_GETREDEEMST	((uint8_t)0x64)     //!< Obtener el estado de redencion del ticket
#define CMD_TKTREDEEMED	((uint8_t)0x65)		//!< Fin de redencion de ticket
#define CMD_PROTODEBUG	((uint8_t)0x66)		//!< Enable / disable protocol Debugger
#define CMD_GETAFTREGIN	((uint8_t)0x67)		//!< Obtiene la info de registro de AFT
#define CMD_INITAFTREG	((uint8_t)0x68)		//!< Inicia el proceso de registracion AFT
#define CMD_OPACKAFTREG	((uint8_t)0x69)		//!< ACK del operador a la registracion AFT
#define CMD_UPDAFTDATA	((uint8_t)0x6A)		//!< Update AFT Lock Status
#define CMD_UAFTTRSTAT	((uint8_t)0x6B)		//!< Update AFT Transaction Status
#define CMD_ENDAFTTR	((uint8_t)0x6C)		//!< Finish AFT Transaction
#define CMD_RQSTHOSTCO	((uint8_t)0x6D)		//!< Request host cashout

#define CMD_WAIT		((uint8_t)0xFE)		//!< Comando en proceso
#define CMD_NACK        ((uint8_t)0xFF)		//!< NACK de un comando

// Sub comandos para el comando de testing
#define SCMD_TST_NVW    ((uint8_t) 0x01)
#define SCMD_TST_NVR    ((uint8_t) 0x02)


#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(1)

/**
 * @brief Estructura de respuesta al comando Query
 * 
 */
typedef struct
{
	uint8_t cmd;						//!< ID del comando enviado a la PC
	uint8_t deletedEvts;				//!< Eventos a quitar de la NV
	uint8_t onLine[MAX_CANALES_SAS];	//!< 1 -> SAS OnLine, 0-> SAS offline
	uint8_t puedeImprimir;				//!< 0 -> No imprimir tickets, 1 -> Se puede imprimir

	/**
	 * @brief Datos respectivos y dependientes del comando
	 * 
	 */
	union
	{
		enaDisGame_t 			enaDisGame;				//!< Datos de CMD_ENADISGAME
		romSig_t				romSig;					//!< Datos de CMD_ROMSIG
		jackpotHPResetMethod_t	jackpotHPResetMethod;	//!< Datos de CMD_ENAJKPHPRST
		autoReBet_t				autoReBet;				//!< Datos de CMD_ENAAUTORBET
		cfgBills_t				cfgBills;				//!< Datos de CMD_CFGBILLS
		RTE_t					RTE;					//!< Datos de CMD_ENADISRTE
		extValStatus_t			extValStatus;			//!< Datos de CMD_EXTVALST
		tktData					ticketData;				//!< Datos de CMD_TKTDATA
		aftRegData_t			aftRegData;				//!< Datos de CMD_AFTREGFIN
		aftLockReq_t			aftLockReq;				//!< Datos de CMD_AFTLOCKREQ
		aftTrxCmd_t				aftTrx;					//!< Datos de CMD_AFTINITRX
		aftTktData_t			aftTktData;				//!< Datos de CMD_AFTTKTDATA
	} data;
} pcCmd;
#pragma pack()

#ifdef __cplusplus
}
#endif

#endif // SRC_ENGINESVRDEFS_H_

