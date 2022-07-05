#ifndef __GENDEFS_H__
#define __GENDEFS_H__

#include <stdint.h>

/*! \file */

// VERSION
#define SAS_VERSION				"602"

// Canales de SAS
#define MAX_CANALES_SAS			2

// Response
#define SAS_PKT					((int32_t)-1)
#define SAS_ACK					((int32_t)0)
#define SAS_NACK				((int32_t)1)
#define SAS_NONE				((int32_t)2)

// Validation
#define SAS_VAL_STD				((uint8_t)0)	//!< Validacion STD o None
#define SAS_VAL_SE				((uint8_t)1)	//!< Validacion Secure Enhanced
#define SAS_VAL_SYS				((uint8_t)2)	//!< Validacion System
#define SAS_VAL_NONE			((uint8_t)255)	//!< Sin validation

// SAS Polls
#define POLL_ENADISGAME			((uint8_t)0x09)
#define POLL_SENDM10TO15		((uint8_t)0x0F)
#define POLL_SENDTCCM			((uint8_t)0x10)
#define POLL_SENDTCIN			((uint8_t)0x11)
#define POLL_SENDTCOUT			((uint8_t)0x12)
#define POLL_SENDTDROP			((uint8_t)0x13)
#define POLL_SENDTJCKP			((uint8_t)0x14)
#define POLL_SENDGAMESPYD		((uint8_t)0x15)
#define POLL_SENDGAMESW			((uint8_t)0x16)
#define POLL_SENDGAMESLST		((uint8_t)0x17)
#define POLL_SENDGPLPUSDC		((uint8_t)0x18)
#define POLL_SENDM11TO15		((uint8_t)0x19)
#define POLL_SENDCURCRED		((uint8_t)0x1A)
#define POLL_HANDPAYINFO		((uint8_t)0x1B)
#define POLL_SENDMETERS			((uint8_t)0x1C)
#define POLL_SENDTOTBILL		((uint8_t)0x1E)
#define POLL_GAMEIDINFO			((uint8_t)0x1F)
#define POLL_ROMSIGNATURE		((uint8_t)0x21)
#define POLL_SENDTHPCC			((uint8_t)0x2D)
#define POLL_SENDSELMTRSGN		((uint8_t)0x2F)

#define POLL_SEN1DMETER			((uint8_t)0x31)
#define POLL_SEN2DMETER			((uint8_t)0x32)
#define POLL_SEN5DMETER			((uint8_t)0x33)
#define POLL_SEN10DMETER		((uint8_t)0x34)
#define POLL_SEN20DMETER		((uint8_t)0x35)
#define POLL_SEN50DMETER		((uint8_t)0x36)
#define POLL_SEN100DMETER		((uint8_t)0x37)
#define POLL_SEN500DMETER		((uint8_t)0x38)
#define POLL_SEN1KDMETER		((uint8_t)0x39)
#define POLL_SEN200DMETER		((uint8_t)0x3A)
#define POLL_SEN25DMETER		((uint8_t)0x3B)
#define POLL_SEN2KDMETER		((uint8_t)0x3C)
#define POLL_SENDCOTKTINF		((uint8_t)0x3D)
#define POLL_SEN2K5DMETER		((uint8_t)0x3E)
#define POLL_SEN5KDMETER		((uint8_t)0x3F)
#define POLL_SEN10KDMETER		((uint8_t)0x40)
#define POLL_SEND20KMETER		((uint8_t)0x41)
#define POLL_SEN25KDMETER		((uint8_t)0x42)
#define POLL_SEN50KDMETER		((uint8_t)0x43)
#define POLL_SEN100KDMETER		((uint8_t)0x44)
#define POLL_SEN250DMETER		((uint8_t)0x45)

#define POLL_SENDLACCBILL		((uint8_t)0x48)
#define POLL_SETSECENVALID		((uint8_t)0x4C)
#define POLL_SENDENVALIDINF		((uint8_t)0x4D)
#define POLL_HOPPERSTATUS		((uint8_t)0x4F)
#define POLL_NUMGAMES			((uint8_t)0x51)
#define POLL_SENDGNMETERS		((uint8_t)0x52)
#define POLL_SENDGAMENCFG		((uint8_t)0x53)
#define POLL_SASVERSERNBR		((uint8_t)0x54)
#define POLL_SENDGAMESEL		((uint8_t)0x55)
#define POLL_SENDENAGAMES		((uint8_t)0x56)
#define POLL_SENDPENCOINF		((uint8_t)0x57)
#define POLL_RECVVALIDNBR		((uint8_t)0x58)
#define POLL_SENDEXTMETERS		((uint8_t)0x6F)
#define POLL_SENDTKTVALIDDT		((uint8_t)0x70)
#define POLL_REDEEMTKT			((uint8_t)0x71)
#define POLL_AFTTRXFUNDS		((uint8_t)0x72)
#define POLL_AFTREGISTERGM		((uint8_t)0x73)
#define POLL_AFTLOCKSTRQST		((uint8_t)0x74)
#define POLL_SETCUSTAFTTKTDATA	((uint8_t)0x76)
#define POLL_EXTVALIDSTAT		((uint8_t)0x7B)
#define POLL_SETEXTTKTDATA		((uint8_t)0x7C)
#define POLL_SETTKTDATA			((uint8_t)0x7D)
#define POLL_SENDDATETIME		((uint8_t)0x7E)
#define POLL_RECVDATETIME		((uint8_t)0x7F)
#define POLL_SENDCARDINFO		((uint8_t)0x8E)
#define POLL_SENDREELSTOPINFO	((uint8_t)0x8F)
#define POLL_RESETHANDPAY		((uint8_t)0x94)
#define POLL_SENDFEATURES		((uint8_t)0xA0)
#define POLL_SENDEXTMETERSA		((uint8_t)0xAF)
#define POLL_MULTIDENOPREAM		((uint8_t)0xB0)
#define POLL_SENDTKNDENO		((uint8_t)0xB3)
#define POLL_SENDWCINFO			((uint8_t)0xB4)
#define POLL_SENDEXTGAMENINFO	((uint8_t)0xB5)

/**
 * @brief Codigos de Eventos SAS
 * 
 */
#define EX_NOACTIVITY			((uint8_t)0x00)	//!< No activity
#define EX_SLOT_DOOR_OPENED		((uint8_t)0x11)	//!< Slot door was opened
#define EX_SLOT_DOOR_CLOSED		((uint8_t)0x12)	//!< Slot door was closed
#define EX_DROP_DOOR_OPENED		((uint8_t)0x13)	//!< Drop door was opened
#define EX_DROP_DOOR_CLOSED		((uint8_t)0x14)	//!< Drop door was closed
#define EX_CARD_CAGE_OPENED		((uint8_t)0x15)	//!< Card cage was opened 
#define EX_CARD_CAGE_CLOSED		((uint8_t)0x16)	//!< Card cage was closed
#define EX_POWER_ON				((uint8_t)0x17)	//!< AC power was applied to gaming machine
#define EX_POWER_OFF			((uint8_t)0x18)	//!< AC power was lost from gaming machine
#define EX_CASHBOX_OPENED		((uint8_t)0x19)	//!< Cashbox door was opened
#define EX_CASHBOX_CLOSED		((uint8_t)0x1A)	//!< Cashbox door was closed
#define EX_CASHBOX_REMOVED		((uint8_t)0x1B)	//!< Cashbox was removed
#define EX_CASHBOX_INSTALLED	((uint8_t)0x1C)	//!< Cashbox was installed
#define EX_BELLYDOOR_OPENED		((uint8_t)0x1D)	//!< Belly door was opened
#define EX_BELLYDOOR_CLOSED		((uint8_t)0x1E)	//!< Belly door was closed
#define EX_GENERAL_TILT			((uint8_t)0x20)	//!< General tilt (Use this tilt when other exception tilt 
												//!< codes do not apply or when the tilt condition cannot be determined.)
#define EX_COININ_TILT			((uint8_t)0x21)	//!< Coin in tilt
#define EX_COINOUT_TILT			((uint8_t)0x22)	//!< Coin out tilt
#define EX_HOPPER_EMPTY			((uint8_t)0x23)	//!< Hopper empty detected
#define EX_EXTRACOIN_PAID		((uint8_t)0x24)	//!< Extra coin paid
#define EX_DIVERTER_MALFUNC		((uint8_t)0x25)	//!< Diverter malfunction (controls coins to drop or hopper)
#define EX_CASHBOX_FULL			((uint8_t)0x27)	//!< Cashbox full detected
#define EX_BILL_JAM				((uint8_t)0x28)	//!< Bill jam
#define EX_BILLACCEPT_HWFAIL	((uint8_t)0x29)	//!< Bill acceptor hardware failure
#define EX_REVERSEBILL_DETECT	((uint8_t)0x2A)	//!< Reverse bill detected
#define EX_BILL_REJECTED		((uint8_t)0x2B)	//!< Bill rejected
#define EX_CNTRFAITBILL_DETECT	((uint8_t)0x2C)	//!< Counterfeit bill detected
#define EX_REVERSE_COININ		((uint8_t)0x2D)	//!< Reverse coin in detected
#define EX_CASHBOX_NEAR_FULL	((uint8_t)0x2E)	//!< Cashbox near full detected
#define EX_CMOSRAM_ERROR_DR		((uint8_t)0x31)	//!< CMOS RAM error (data recovered from EEPROM)
#define EX_CMOSRAM_ERROR_NDR	((uint8_t)0x32)	//!< CMOS RAM error (no data recovered from EEPROM)
#define EX_CMOSRAM_ERROR_DEV	((uint8_t)0x33)	//!< CMOS RAM error (bad device)
#define EX_EEPROM_ERROR_DATA	((uint8_t)0x34)	//!< EEPROM error (data error)
#define EX_EEPROM_ERROR_DEV		((uint8_t)0x35)	//!< EEPROM error (bad device)
#define EX_EPROM_ERROR_CHKDIF	((uint8_t)0x36)	//!< EPROM error (different checksum – version changed)
#define EX_EPROM_ERROR_CHK		((uint8_t)0x37)	//!< EPROM error (bad checksum compare)
#define EX_PEPROM_ERROR_CHKDIF	((uint8_t)0x38)	//!< Partitioned EPROM error (checksum – version changed)
#define EX_PEPROM_ERROR_CHK		((uint8_t)0x39)	//!< Partitioned EPROM error (bad checksum compare)
#define EX_MEMRESET_ERROR		((uint8_t)0x3A)	//!< Memory error reset (operator used self test switch)	
#define EX_LOW_BACKUP_BATTERY	((uint8_t)0x3B)	//!< Low backup battery detected
#define EX_OPERATOR_CHNG_OPT	((uint8_t)0x3C)	//!< Operator changed options (This is sent whenever the operator changes
												//!< configuration options. This includes, but is not limited to, denomination,
												//!< gaming machine address, or any option that affects the response to long polls
												//!< 1F, 53, 54, 56, A0, B2, B3, B4, or B5.)
#define EX_CASHOUTTKT_PRINTED	((uint8_t)0x3D)	//!< A cash out ticket has been printed
#define EX_HANDPAY_VALIDATED	((uint8_t)0x3E)	//!< A handpay has been validated
#define EX_VALIDATIONID_NOTCFG	((uint8_t)0x3F)	//!< Validation ID not configured
#define EX_REEL_TILT			((uint8_t)0x40)	//!< Reel Tilt (Which reel is not specified.)
#define EX_REEL1_TILT			((uint8_t)0x41)	//!< Reel 1 tilt
#define EX_REEL2_TILT			((uint8_t)0x42)	//!< Reel 2 tilt
#define EX_REEL3_TILT			((uint8_t)0x43)	//!< Reel 3 tilt
#define EX_REEL4_TILT			((uint8_t)0x44)	//!< Reel 4 tilt
#define EX_REEL5_TILT			((uint8_t)0x45)	//!< Reel 5 tilt
#define EX_REELMECH_DISCON		((uint8_t)0x46)	//!< Reel mechanism disconnected
#define EX_$1_ACCEPTED			((uint8_t)0x47)	//!< $1.00 bill accepted (non-RTE only)
#define EX_$5_ACCEPTED			((uint8_t)0x48)	//!< $5.00 bill accepted (non-RTE only)
#define EX_$10_ACCEPTED			((uint8_t)0x49)	//!< $10.00 bill accepted (non-RTE only)
#define EX_$20_ACCEPTED			((uint8_t)0x4A)	//!< $20.00 bill accepted (non-RTE only)
#define EX_$50_ACCEPTED			((uint8_t)0x4B)	//!< $50.00 bill accepted (non-RTE only)
#define EX_$100_ACCEPTED		((uint8_t)0x4C)	//!< $100.00 bill accepted (non-RTE only)
#define EX_$2_ACCEPTED			((uint8_t)0x4D)	//!< $2.00 bill accepted (non-RTE only)
#define EX_$500_ACCEPTED		((uint8_t)0x4E)	//!< $500.00 bill accepted (non-RTE only)
#define EX_BILL_ACCEPTED		((uint8_t)0x4F)	//!< Bill accepted (In non-RTE mode, use this exception for all bills without a
												//!< specific exception. In RTE mode, use for all bill denominations.)
#define EX_$200_ACCEPTED		((uint8_t)0x50)	//!< $200.00 bill accepted (non-RTE only)
#define EX_HANDPAY_PENDING		((uint8_t)0x51)	//!< Handpay is pending (Progressive, non-progressive or cancelled credits)
#define EX_HANDPAY_RESET		((uint8_t)0x52)	//!< Handpay was reset (Jackpot reset switch activated)
#define EX_NOPRGSVINFO_5S		((uint8_t)0x53)	//!< No progressive information has been received for 5 seconds
#define EX_PRGSV_WIN			((uint8_t)0x54)	//!< Progressive win (cashout device/credit paid)
#define EX_HANDPAY_CANCLD_USR	((uint8_t)0x55)	//!< Player has cancelled the handpay request
#define EX_SASPRGSVLVL_HIT		((uint8_t)0x56)	//!< SAS progressive level hit
#define EX_SYSTEMVAL_RQST		((uint8_t)0x57)	//!< System validation request
#define EX_PRNCOMM_ERROR		((uint8_t)0x60)	//!< Printer communication error
#define EX_PRN_PAPEROUT			((uint8_t)0x61)	//!< Printer paper out error
#define EX_CASHOUTBTN_PRESSED	((uint8_t)0x66)	//!< Cash out button pressed
#define EX_TICKET_INSERTED		((uint8_t)0x67)	//!< Ticket has been inserted
#define EX_TICKETTRX_COMPLETED	((uint8_t)0x68)	//!< Ticket transfer complete
#define EX_AFTTRX_COMPLETED		((uint8_t)0x69)	//!< AFT transfer complete
#define EX_AFTRQST_HOST_CASHOUT	((uint8_t)0x6A)	//!< AFT request for host cashout
#define EX_AFTRQST_HOSTCASHOUTW	((uint8_t)0x6B)	//!< AFT request for host to cash out win
#define EX_AFTRQST_REGISTER		((uint8_t)0x6C)	//!< AFT request to register
#define EX_REGISTR_ACK			((uint8_t)0x6D)	//!< AFT registration acknowledged
#define EX_REGISTR_CANCLD		((uint8_t)0x6E)	//!< AFT registration cancelled
#define EX_GAME_LOCKED			((uint8_t)0x6F)	//!< Game locked
#define EX_EXCEPTBUFF_OVERFLOW	((uint8_t)0x70)	//!< Exception buffer overflow
#define EX_CHNGLAMP_ON			((uint8_t)0x71)	//!< Change lamp on
#define EX_CHNGLAMP_OFF			((uint8_t)0x72)	//!< Change lamp off
#define EX_PRN_PAPERLOW			((uint8_t)0x74)	//!< Printer paper low
#define EX_PRN_POWEROFF			((uint8_t)0x75)	//!< Printer power off
#define EX_PRN_POWERON			((uint8_t)0x76)	//!< Printer power on
#define EX_PRN_REPLACERBN		((uint8_t)0x77)	//!< Replace printer ribbon
#define EX_PRN_CARRIAGE_JAM		((uint8_t)0x78)	//!< Printer carriage jammed
#define EX_COININ_LOCKOUT_MALF	((uint8_t)0x79)	//!< Coin in lockout malfunction (coin accepted while coin mech disabled)
#define EX_GMSOFTMETERS_TOZERO	((uint8_t)0x7A)	//!< Gaming machine soft (lifetime-to-date) meters reset to zero
#define EX_BILLVAL_TOTALS_RST	((uint8_t)0x7B)	//!< Bill validator (period) totals have been reset by an attendant/operator
#define EX_LGCYPAYORMJ_OCURRED	((uint8_t)0x7C)	//!< A legacy bonus pay awarded and/or a multiplied jackpot occurred
#define EX_GAME_STARTED			((uint8_t)0x7E)	//!< Game has started
#define EX_GAME_ENDED			((uint8_t)0x7F)	//!< Game has ended
#define EX_HOPPER_FULL			((uint8_t)0x80)	//!< Hopper full detected
#define EX_HOPPERLVL_LOW		((uint8_t)0x81)	//!< Hopper level low detected
#define EX_DMORATTENMNU_ENTER	((uint8_t)0x82)	//!< Display meters or attendant menu has been entered
#define EX_DMORATTENMNU_ENXIT	((uint8_t)0x83)	//!< Display meters or attendant menu has been exited
#define EX_SLFTSTOROPER_ENTER	((uint8_t)0x84)	//!< Self test or operator menu has been entered
#define EX_SLFTSTOROPER_EXIT	((uint8_t)0x85)	//!< Self test or operator menu has been exited
#define EX_GM_OUTOFSERVICE		((uint8_t)0x86)	//!< Gaming machine is out of service (by attendant)
#define EX_PLAYER_RQST_DRAW		((uint8_t)0x87)	//!< Player has requested draw cards (only send when in RTE mode)
#define EX_REELN_STOPPED		((uint8_t)0x88)	//!< Reel N has stopped (only send when in RTE mode)
#define EX_COINCRED_WAGERED		((uint8_t)0x89)	//!< Coin/credit wagered (only send when in RTE mode, and only send if the
												//!< configured max bet is 10 or less)
#define EX_GAMERECALLENTR		((uint8_t)0x8A)	//!< Game recall entry has been displayed
#define EX_CARDHELD_UNHLD		((uint8_t)0x8B)	//!< Card held/not held (only send when in RTE mode)
#define EX_GAME_SELECTED		((uint8_t)0x8C)	//!< Game selected
#define EX_COMPLIST_CHNG		((uint8_t)0x8E)	//!< Component list changed
#define EX_AUTH_COMPLETED		((uint8_t)0x8F)	//!< Authentication complete
#define EX_PWROFF_CARDCAGE		((uint8_t)0x98)	//!< Power off card cage access
#define EX_PWROFF_SLOTDOOR		((uint8_t)0x99)	//!< Power off slot door access
#define EX_PWROFF_CASHBOXDOOR	((uint8_t)0x9A)	//!< Power off cashbox door access
#define EX_PWROFF_DROPDOOR		((uint8_t)0x9B)	//!< Power off drop door access

#define EX_REALTIME				((uint8_t)0xFF)	//!< Event response to long poll

// ************** Codigos de Error ******************
#define ERR_OK		((int32_t)0)		// OK
#define ERR_NOMEM	((int32_t)-1)		// Heap insuficiente
#define ERR_INVENG	((int32_t)-2)		// Engine no valida
#define ERR_NOTALL	((int32_t)-3)		// Operacion no permitida
#define ERR_INVPARM	((int32_t)-4)		// Parametros invalidos
#define ERR_INVPOLL	((int32_t)-5)		// Poll no valido
#define ERR_CRC		((int32_t)-6)		// CRC no valido

/**
 * @brief Contadores
 * 
 */
#define TOTAL_COIN_IN			0x00	//!< Total coin in credits
#define TOTAL_COIN_OUT			0x01	//!< Total coin out credits
#define TOTAL_JACKPOT			0x02	//!< Total jackpot credits
#define TOTAL_HP_CC				0x03	//!< Total hand paid cancelled credits
#define TOTAL_CC				0x04	//!< Total cancelled credits
#define GAMES_PLAYED			0x05	//!< Games played
#define GAMES_WON				0x06	//!< Games won
#define GAMES_LOST				0x07	//!< Games lost
#define TOTAL_CRED_FCA			0x08	//!< Total credits from coin acceptor
#define TOTAL_CRED_PFH			0x09	//!< Total credits paid from hopper
#define TOTAL_CRED_FCD			0x0A	//!< Total credits from coins to drop
#define TOTAL_CRED_FBA			0x0B	//!< Total credits from bills accepted
#define CURRENT_CRED			0x0C	//!< Current credits
// Salteados 0x0D .. 0x14
#define TOTAL_TKIN				0x15	//!< Total ticket in, including cashable, nonrestricted and restricted tickets (credits)
#define TOTAL_TKOUT				0x16	//!< Total ticket out, including cashable, nonrestricted, restricted and debit tickets (credits)
#define TOTAL_ETTOGM			0x17	//!< Total electronic transfers to gaming machine, including cashable, nonrestricted, restricted and debit, whether transfer is to credit meter or to ticket (credits)
#define TOTAL_ETTOHOST			0x18	//!< Total electronic transfers to host, including cashable, nonrestricted, restricted and win amounts (credits)
#define TOTAL_RESAMPLY			0x19	//!< Total restricted amount played (credits)
#define TOTAL_NRAMTPLY			0x1A	//!< Total nonrestricted amount played (credits)
#define CURRENT_RESTR			0x1B	//!< Current restricted credits
#define TOTAL_MPPTWIN			0x1C	//!< Total machine paid paytable win, not including progressive or external bonus amounts (credits)
#define TOTAL_MPPRWIN			0x1D	//!< Total machine paid progressive win (credits)
#define TOTAL_MPEXTBNS			0x1E	//!< Total machine paid external bonus win (credits)
#define TOTAL_APTWIN			0x1F	//!< Total attendant paid paytable win, not including progressive or external bonus amounts (credits)
#define TOTAL_APPWIN			0x20	//!< Total attendant paid progressive win (credits)
#define TOTAL_APEXTBNS			0x21	//!< Total attendant paid external bonus win (credits)
#define TOTAL_WONCRED			0x22	//!< Total won credits (sum of total coin out and total jackpot)
#define TOTAL_HPCRED			0x23	//!< Total hand paid credits (sum of total hand paid cancelled credits and total jackpot)
#define TOTAL_DROP				0x24	//!< Total drop, including but not limited to coins to drop, bills to drop, tickets to drop, and electronic in (credits)
#define GAMES_SLPWRST			0x25	//!< Games since last power reset
#define GAMES_SSDCLOS			0x26	//!< Games since slot door closure
#define TOTAL_CFEXTCI			0x27	//!< Total credits from external coin acceptor
#define TOTAL_CASHTKIN			0x28	//!< Total cashable ticket in, including nonrestricted promotional tickets (credits)
#define TOTAL_RCATKINC			0x29	//!< Total regular cashable ticket in (credits)
#define TOTAL_RESPTKIN			0x2A	//!< Total restricted promotional ticket in (credits)
#define TOTAL_NRPRTKIN			0x2B	//!< Total nonrestricted promotional ticket in (credits)
#define TOTAL_CAHTKOUT			0x2C	//!< Total cashable ticket out, including debit tickets (credits)
#define TOTAL_RPTKOUT			0x2D	//!< Total restricted promotional ticket out (credits)
#define E_REGCASHTTGM			0x2E	//!< Electronic regular cashable transfers to gaming machine, not including external bonus awards (credits)
#define E_REPROMTTOGM			0x2F	//!< Electronic restricted promotional transfers to gaming machine, not including external bonus awards (credits)
#define E_NRPROMTTOGM			0x30	//!< Electronic nonrestricted promotional transfers to gaming machine, not including external bonus awards (credits)
#define E_DEBTTRTOGM			0x31	//!< Electronic debit transfers to gaming machine (credits)
#define E_REGCATRTOH			0x32	//!< Electronic regular cashable transfers to host (credits)
#define E_REPROMTTOH			0x33	//!< Electronic restricted promotional transfers to host (credits)
#define E_NRPTOMTTOH			0x34	//!< Electronic nonrestricted promotional transfers to host (credits)
#define TOTAL_RCATKINQ			0x35	//!< Total regular cashable ticket in (quantity)
#define TOTAL_REPTKINQ			0x36	//!< Total restricted promotional ticket in (quantity)
#define TOTAL_NRPTKINQ			0x37	//!< Total nonrestricted promotional ticket in (quantity)
#define TOTAL_CATKOUTQ			0x38	//!< Total cashable ticket out, including debit tickets (quantity)
#define TOTAL_RPTKOUTQ			0x39	//!< Total restricted promotional ticket out (quantity)
// 0x3A .. 0x3D Reserved
#define NBRBILLINSTKR			0x3E	//!< Number of bills currently in the stacker (Issue exception 7B when this meter is reset)
#define TOTAL_VOBINSTKR			0x3F	//!< Total value of bills currently in the stacker (credits) (Issue exception 7B when this meter is reset)
#define TOTAL_1DBILLS			0x40	//!< Total number of $1.00 bills accepted
#define TOTAL_2DBILLS			0x41	//!< Total number of $2.00 bills accepted
#define TOTAL_5DBILLS			0x42	//!< Total number of $5.00 bills accepted
#define TOTAL_10DBILLS			0x43	//!< Total number of $10.00 bills accepted
#define TOTAL_20DBILLS			0x44	//!< Total number of $20.00 bills accepted
#define TOTAL_25DBILLS			0x45	//!< Total number of $25.00 bills accepted
#define TOTAL_50DBILLS			0x46	//!< Total number of $50.00 bills accepted
#define TOTAL_100DBILLS			0x47	//!< Total number of $100.00 bills accepted
#define TOTAL_200DBILLS 		0x48 	//!< Total number of $200.00 bills accepted
#define TOTAL_250DBILLS			0x49 	//!< Total number of $250.00 bills accepted
#define TOTAL_500DBILLS			0x4A 	//!< Total number of $500.00 bills accepted
#define TOTAL_1KDBILLS			0x4B 	//!< Total number of $1,000.00 bills accepted
#define TOTAL_2KDBILLS			0x4C 	//!< Total number of $2,000.00 bills accepted
#define TOTAL_2K5DBILLS			0x4D 	//!< Total number of $2,500.00 bills accepted
#define TOTAL_5KDBILLS			0x4E 	//!< Total number of $5,000.00 bills accepted
#define TOTAL_10KDBILLS			0x4F 	//!< Total number of $10,000.00 bills accepted
#define TOTAL_20KDBILLS			0x50 	//!< Total number of $20,000.00 bills accepted
#define TOTAL_25KDBILLS			0x51 	//!< Total number of $25,000.00 bills accepted
#define TOTAL_50KDBILLS			0x52 	//!< Total number of $50,000.00 bills accepted
#define TOTAL_100KDBILLS		0x53 	//!< Total number of $100,000.00 bills accepted
#define TOTAL_200KDBILLS		0x54 	//!< Total number of $200,000.00 bills accepted
#define TOTAL_250KDBILLS		0x55 	//!< Total number of $250,000.00 bills accepted
#define TOTAL_500KDBILLS		0x56 	//!< Total number of $500,000.00 bills accepted
#define TOTAL_1MDBILLS			0x57 	//!< Total number of $1,000,000.00 bills accepted
#define TOTAL_CRFBILLSTD		0x58 	//!< Total credits from bills to drop
#define TOTAL_1DBILLSTD			0x59 	//!< Total number of $1.00 bills to drop
#define TOTAL_2DBILLSTD			0x5A 	//!< Total number of $2.00 bills to drop
#define TOTAL_5DBILLSTD			0x5B 	//!< Total number of $5.00 bills to drop
#define TOTAL_10DBILLSTD		0x5C 	//!< Total number of $10.00 bills to drop
#define TOTAL_20DBILLSTD		0x5D 	//!< Total number of $20.00 bills to drop
#define TOTAL_50DBILLSTD		0x5E 	//!< Total number of $50.00 bills to drop
#define TOTAL_100DBILLSTD		0x5F 	//!< Total number of $100.00 bills to drop
#define TOTAL_200DBILLSTD		0x60 	//!< Total number of $200.00 bills to drop
#define TOTAL_500DBILLSTD		0x61 	//!< Total number of $500.00 bills to drop
#define TOTAL_1KDBILLSTD		0x62 	//!< Total number of $1000.00 bills to drop 
#define TOTAL_CRFBDTH			0x63 	//!< Total credits from bills diverted to hopper
#define TOTAL_1DBILLSDTH		0x64 	//!< Total number of $1.00 bills diverted to hopper
#define TOTAL_2DBILLSDTH		0x65	//!< Total number of $2.00 bills diverted to hopper
#define TOTAL_5DBILLSDTH		0x66 	//!< Total number of $5.00 bills diverted to hopper
#define TOTAL_10DBILLSDTH		0x67 	//!< Total number of $10.00 bills diverted to hopper
#define TOTAL_20DBILLSDTH		0x68 	//!< Total number of $20.00 bills diverted to hopper
#define TOTAL_50DBILLSDTH		0x69 	//!< Total number of $50.00 bills diverted to hopper
#define TOTAL_100DBILLSDTH		0x6A 	//!< Total number of $100.00 bills diverted to hopper
#define TOTAL_200DBILLSDTH		0x6B 	//!< Total number of $200.00 bills diverted to hopper
#define TOTAL_500DBILLSDTH		0x6C 	//!< Total number of $500.00 bills diverted to hopper
#define TOTAL_1KDBILLSDTH		0x6D 	//!< Total number of $1000.00 bills diverted to hopper
#define TOTAL_CRFBDFH			0x6E 	//!< Total credits from bills dispensed from hopper
#define TOTAL_1DBILLSDFH		0x6F 	//!< Total number of $1.00 bills dispensed from hopper
#define TOTAL_2DBILLSDFH		0x70 	//!< Total number of $2.00 bills dispensed from hopper
#define TOTAL_5DBILLSDFH		0x71 	//!< Total number of $5.00 bills dispensed from hopper
#define TOTAL_10DBILLSDFH		0x72 	//!< Total number of $10.00 bills dispensed from hopper
#define TOTAL_20DBILLSDFH		0x73 	//!< Total number of $20.00 bills dispensed from hopper
#define TOTAL_50DBILLSDFH		0x74 	//!< Total number of $50.00 bills dispensed from hopper
#define TOTAL_100DBILLSDFH		0x75 	//!< Total number of $100.00 bills dispensed from hopper
#define TOTAL_200DBILLSDFH		0x76 	//!< Total number of $200.00 bills dispensed from hopper
#define TOTAL_500DBILLSDFH		0x77 	//!< Total number of $500.00 bills dispensed from hopper
#define TOTAL_1KDBILLSDFH		0x78 	//!< Total number of $1000.00 bills dispensed from hopper
// 0x79-0x7E Reserved for future use
#define WEIGHAVGTHPB			0x7F	//!< Weighted average theoretical payback percentage in hundredths of a percent
// 0x80-0x93 Acumulados de SAS
// 0094-009F Reserved for future use
// 0xA0-0xBD Acumulados de SAS
// 0xBE-0xFFFF Reserved for future use

/**
 * @brief Machine features
 * 
 */
#define MFEAT1_JACKPOTMULT      0x01	//!< Jackpot multiplier 			0 -> Disabled
#define MFEAT1_AFTBONUSAWARD    0x02	//!< AFT bonus awards				0 -> Disabled
#define MFEAT1_LGCYBONUSAWRD    0x04	//!< Legacy Bonus awards			0 -> Disabled
#define MFEAT1_TOURNAMENT       0x08	//!< Tournament					0 -> Disabled
#define MFEAT1_VALIDATEXT       0x10	//!< Validation extensions		0 -> Not Supported
#define MFEAT1_VALSYSTEM        0x20    //!< System Validation 			Valid Std son ambos en 0
#define MFEAT1_VALSECENH        0x40    //!< Validation Secure Enhanced	Valid Std son ambos en 0
#define MFEAT1_TKTREDEMPT       0x80	//!< Ticket redemption			0 -> Disabled

#define MFEAT2_METMODWCMWW      0x01    //!< Won credits metered when won
#define MFEAT2_METMODWCMWPP     0x02    //!< Won credits metered when played or paid
                                        //!< Ambos en 0 es Not specified
#define MFEAT2_TKTTDTCC         0x04    //!< Tickets to total drop and total cancelled credits
#define MFEAT2_EXTDMETERS       0x08	//!< Extended meters				0 -> Noy supported
#define MFEAT2_COMPAUTH         0x10	//!< Component Authentication		0 -> Not supported
#define MFEAT2_AFT              0x40	//!< Advanced Funds Transfer		0 -> Not supported
#define MFEAT2_MULTIDENOM       0x80	//!< Multi-denom extensions		0 -> Not supported

#define MFEAT3_MAXPOLLINGRATE   0x01    //!< Max Polling Rate 			1 -> 40ms
#define MFEAT3_MSASPRGSVWIN     0x02	//!< Multiple SAS progressive 
										//!< win reporting (long poll 87)	0 -> Not supported

/**
 * @brief Country codes
 * 
 */
#define COUNTRY_UNK		0 	//!< Unknown country code
#define COUNTRY_ARG		1 	//!< Argentina
#define COUNTRY_NW		2 	//!< Australia
#define COUNTRY_AUST	3 	//!< Austria
#define COUNTRY_BEL		4 	//!< Belgium
#define COUNTRY_BRA		5 	//!< Brazil
#define COUNTRY_BUL		6 	//!< Bulgaria
#define COUNTRY_CAN		7 	//!< Canada
#define COUNTRY_CMB		8 	//!< Columbia
#define COUNTRY_CHP		9 	//!< Cyprus
#define COUNTRY_CZ		10 	//!< Czechoslovakia
#define COUNTRY_DIN		11 	//!< Denmark
#define COUNTRY_FIN		12 	//!< Finland
#define COUNTRY_FR		13 	//!< France
#define COUNTRY_GER		14 	//!< Germany
#define COUNTRY_GRB		15 	//!< Great Britain
#define COUNTRY_GIB		16 	//!< Gibraltar
#define COUNTRY_GRK		17 	//!< Greece
#define COUNTRY_GUE		18 	//!< Guernsey
#define COUNTRY_HUN		19 	//!< Hungary
#define COUNTRY_IRE		20 	//!< Ireland
#define COUNTRY_ITALY	21 	//!< Italy
#define COUNTRY_JERSEY	22 	//!< Jersey
#define COUNTRY_LUX		23 	//!< Luxembourg
#define COUNTRY_MALTA	24 	//!< Malta
#define COUNTRY_MEX		25 	//!< Mexico
#define COUNTRY_MOR		26 	//!< Morocco
#define COUNTRY_NOR		27 	//!< Norway
#define COUNTRY_POL		28 	//!< Poland
#define COUNTRY_POR		29 	//!< Portugal
#define COUNTRY_RUM		30 	//!< Romania
#define COUNTRY_RUS		31 	//!< Russia
#define COUNTRY_ESP		32 	//!< Spain
#define COUNTRY_SA		33 	//!< South Africa
#define COUNTRY_SWE		34 	//!< Sweden
#define COUNTRY_SWI		35 	//!< Switzerland
#define COUNTRY_TK		36 	//!< Turkey
#define COUNTRY_USA		37 	//!< United States
#define COUNTRY_HOL		38 	//!< Holland
#define COUNTRY_EUR		39 	//!< Euro
//40-47 Reserved for future use

/**
 * @brief Bill denominations
 * 
 */
#define BILLD1		0 	//!< $1
#define BILLD2		1 	//!< $2
#define BILLD5		2 	//!< $5
#define BILLD10		3 	//!< $10
#define BILLD20		4 	//!< $20
#define BILLD25		5 	//!< $25
#define BILLD50		6 	//!< $50
#define BILLD100	7 	//!< $100
#define BILLD200	8 	//!< $200
#define BILLD250	9 	//!< $250
#define BILLD500	10 	//!< $500
#define BILLD1K		11 	//!< $1,000
#define BILLD2K		12 	//!< $2,000
#define BILLD2K5	13 	//!< $2,500
#define BILLD5K		14 	//!< $5,000
#define BILLD10K	15 	//!< $10,000
#define BILLD20K	16 	//!< $20,000
#define BILLD25K	17 	//!< $25,000
#define BILLD50K	18 	//!< $50,000
#define BILLD100K	19 	//!< $100,000
#define BILLD200K	20 	//!< $200,000
#define BILLD250K	21 	//!< $250,000
#define BILLD500K	22 	//!< $500,000
#define BILLD1M		23 	//!< $1,000,000
//24-31 Reserved for future use

/**
 * @brief Codigos de denominacion SAS
 * 
 */
#define DENO_NONE	((uint8_t)0)	//!< none
#define DENO_1C		((uint8_t)1)	//!< $0.01
#define	DENO_5C		((uint8_t)2)	//!< $0.05
#define	DENO_10C	((uint8_t)3)	//!< $0.10
#define	DENO_25C	((uint8_t)4)	//!< $0.25
#define DENO_50C	((uint8_t)5)	//!< $0.50
#define DENO_1D		((uint8_t)6)	//!< $1.00
#define DENO_5D		((uint8_t)7)	//!< $5.00
#define DENO_10D	((uint8_t)8)	//!< $10.00
#define DENO_20D	((uint8_t)9)	//!< $20.00
#define	DENO_100D	((uint8_t)10)	//!< $100.00
#define DENO_20C	((uint8_t)11)	//!< $0.20
#define DENO_2D		((uint8_t)12)	//!< $2.00
#define DENO_2D5	((uint8_t)13)	//!< $2.50
#define DENO_25D	((uint8_t)14)	//!< $25.00
#define DENO_50D	((uint8_t)15)	//!< $50.00
#define DENO_200D	((uint8_t)16)	//!< $200.00
#define DENO_250D	((uint8_t)17)	//!< $250.00
#define DENO_500D	((uint8_t)18)	//!< $500.00
#define DENO_1KD	((uint8_t)19)	//!< $1000.00
#define DENO_2KD	((uint8_t)20)	//!< $2000.00
#define DENO_2K5D	((uint8_t)21)	//!< $2500.00
#define DENO_5KD	((uint8_t)22)	//!< $5000.00
#define DENO_2C		((uint8_t)23)	//!< $0.02
#define DENO_3C		((uint8_t)24)	//!< $0.03
#define DENO_15C	((uint8_t)25)	//!< $0.15
#define DENO_40C	((uint8_t)26)	//!< $0.40
#define DENO_5HC	((uint8_t)27)	//!< $0.005
#define DENO_25QC	((uint8_t)28)	//!< $0.0025
#define DENO_2FC	((uint8_t)29)	//!< $0.002
#define DENO_1FC	((uint8_t)30)	//!< $0.001
#define DENO_5FC	((uint8_t)31)	//!< $0.0005

/**
 *	@brief Tipos de validacion para Registro
 */
#define TKTVAL_CASHOUT	((uint8_t)0x00) 	//!< Cashable ticket from cashout or win, no handpay lockup
#define TKTVAL_RESPROM 	((uint8_t)0x01) 	//!< Restricted promotional ticket from cashout
#define TKTVAL_AFTCASH	((uint8_t)0x02) 	//!< Cashable ticket from AFT transfer
#define TKTVAL_AFTREST	((uint8_t)0x03) 	//!< Restricted ticket from AFT transfer
#define TKTVAL_AFTDEBT	((uint8_t)0x04) 	//!< Debit ticket from AFT transfer
#define TKTVAL_CCHP		((uint8_t)0x10) 	//!< Cancelled credit handpay (receipt printed)
#define TKTVAL_JKPHP 	((uint8_t)0x20) 	//!< Jackpot handpay (receipt printed)
#define TKTVAL_CCHPN	((uint8_t)0x40) 	//!< Cancelled credit handpay (no receipt)
#define TKTVAL_JKPHPN	((uint8_t)0x60) 	//!< Jackpot handpay (no receipt)
#define TKTVAL_CASHTKRM	((uint8_t)0x80) 	//!< Cashable ticket redeemed
#define TKTVAL_RESPRORM	((uint8_t)0x81) 	//!< Restricted promotional ticket redeemed
#define TKTVAL_NRESPRM	((uint8_t)0x82) 	//!< Nonrestricted promotional ticket redeemed

/**
 * @brief Poll 71 transfer codes
 */
#define TX_CODE_VALIDCASH	((uint8_t)0x00) 	//!< Valid cashable ticket
#define TX_CODE_VALIDRPT	((uint8_t)0x01) 	//!< Valid restricted promotional ticket
#define TX_CODE_VALIDNRPT	((uint8_t)0x02) 	//!< Valid nonrestricted promotional ticket
#define TX_CODE_UNTOVALID	((uint8_t)0x80) 	//!< Unable to validate (no reason given / other)
#define TX_CODE_NVVALNBR	((uint8_t)0x81) 	//!< Not a valid validation number
#define TX_CODE_VNNOTINSYS	((uint8_t)0x82) 	//!< Validation number not in system
#define TX_CODE_TKTPENDING	((uint8_t)0x83) 	//!< Ticket marked pending in system
#define TX_CODE_TKTALREDEEM	((uint8_t)0x84) 	//!< Ticket already redeemed
#define TX_CODE_TKTEXPIRED	((uint8_t)0x85) 	//!< Ticket expired
#define TX_CODE_VALINFNAV	((uint8_t)0x86) 	//!< Validation information not available
#define TX_CODE_TKTANMSAM	((uint8_t)0x87) 	//!< Ticket amount does not match system amount
#define TX_CODE_TKTAEARL	((uint8_t)0x88) 	//!< Ticket amount exceeds auto redemption limit
#define TX_CODE_REQSTATUS	((uint8_t)0xFF) 	//!< Request for current ticket status

/**
 * @brief Machine status codes for poll 71
 */
// Binary codes 000xxxxx indicate ticket redemption successful
#define MCH_ST_TKTREDEEM	((uint8_t)0x00) //!< Cashable ticket redeemed
#define MCH_ST_RPTREDEEM	((uint8_t)0x01) //!< Restricted promotional ticket redeemed
#define MCH_ST_NRPTREDEEM	((uint8_t)0x02) //!< Nonrestricted promotional ticket redeemed

// Binary codes 001xxxxx indicate waiting for long poll 71
#define MCH_ST_WAITING		((uint8_t)0x20) //!< Waiting for long poll 71

// Binary codes 010xxxxx indicate ticket redemption pending
#define MCH_ST_TKTREDPEND	((uint8_t)0x40) //!< Ticket redemption pending (not complete)

// Binary codes 100xxxxx indicate ticket redemption failed
#define MCH_ST_TKTREJECTED	((uint8_t)0x80) //!< Ticket rejected by host, or unknown
#define MCH_ST_VALNBRNTMCH	((uint8_t)0x81) //!< Validation number does not match (response must include correct validation number)
#define MCH_ST_NVALTXFUNC	((uint8_t)0x82) //!< Not a valid transfer function
#define MCH_ST_NVALTXAMNT	((uint8_t)0x83) //!< Not a valid transfer amount (non-BCD)
#define MCH_ST_TXAMTEXMCL	((uint8_t)0x84) //!< Transfer amount exceeded the gaming machine credit limit
#define MCH_ST_TXAMNEMMDEN	((uint8_t)0x85) //!< Transfer amount not an even multiple of gaming machine denomination
#define MCH_ST_TXAMDNMTKAM	((uint8_t)0x86) //!< Transfer amount does not match ticket amount
#define MCH_ST_GMUNACCTX	((uint8_t)0x87) //!< Gaming machine unable to accept transfer at this time
#define MCH_ST_TKTREJDTTO	((uint8_t)0x88) //!< Ticket rejected due to timeout
#define MCH_ST_TKTREJDTLD	((uint8_t)0x89) //!< Ticket rejected due to comm link down
#define MCH_ST_TKTREDDIS	((uint8_t)0x8A) //!< Ticket redemption disabled
#define MCH_ST_TKTREJDTVFL	((uint8_t)0x8B) //!< Ticket rejected due to validator failure

// Binary codes 110xxxxx indicate incompatible poll
#define MCH_ST_NOTCMPCRC	((uint8_t)0xC0) //!< Not compatible with current redemption cycle (ignored)

// Binary codes 111xxxxx indicate no validation information available
#define MCH_ST_NOVALINFAV	((uint8_t)0xFF) //!< No validation information available


/**
 * @brief Cashout Type Code Values
 * 
 */
#define CSH_CASHTKT		((uint8_t)0x00)
#define CSH_RESTTKT		((uint8_t)0x01)
#define CSH_NOTWTNG		((uint8_t)0x80)

/**
 * @brief AFT registration codes
 */
#define AFT_RCINITIALIZE	((uint8_t)0x00)	//!< Initialize registration
#define AFT_RCREGISTERGM	((uint8_t)0x01)	//!< Register gaming machine
#define AFT_RCREQOPERACK	((uint8_t)0x40)	//!< Request operator acknowledgement
#define AFT_RCUNREGISTERGM	((uint8_t)0x80)	//!< Unregister gaming machine
#define AFT_RCREADCURRINF	((uint8_t)0xFF)	//!< Read current registration
/**
 * @brief AFT Registration status
 */
#define AFT_GMREGREADY		((uint8_t)0x00)	//!< Gaming machine registration ready
#define AFT_GMREGISTERED	((uint8_t)0x01)	//!< Gaming machine registered
#define AFT_GMREGPENDING	((uint8_t)0x40)	//!< Gaming machine registration pending
#define AFT_GMNOTREGIST		((uint8_t)0x80)	//!< Gaming machine not registered

/**
 *	@brief AFT Lock Codes
 */
#define AFTLCK_RQST			((uint8_t)0x00)	//!< Request Lock
#define AFTLCK_CANCEL		((uint8_t)0x80)	//!< Cancel Lock or pending lock
#define AFTLCK_ASK			((uint8_t)0xFF)	//!< Get current status

/**
 *	@brief AFT Lock status
 */
#define AFTLCK_ST_LOCKED	((uint8_t)0x00)	//!< Game locked
#define AFTLCK_ST_PENDLOCK	((uint8_t)0x40)	//!< Lock pending
#define AFTLCK_ST_NOTLCK	((uint8_t)0xFF)	//!< Not locked

/**
 * @brief AFT Available Trx Bits
 */
#define EAVT_TRXTOGMOK		((uint8_t)0x01)	//!< Transfer to gaming machine OK
#define EAVT_TRXFROMGMOK	((uint8_t)0x02)	//!< Transfer from gaming machine OK
#define EAVT_TRXTOPRNOK		((uint8_t)0x04)	//!< Transfer to printer OK
#define EAVT_WINAMPCTH		((uint8_t)0x08)	//!< Win amount pending cashout to host
#define EAVT_BONAWTOGMOK	((uint8_t)0x10)	//!< Bonus award to gaming machine OK

/**
 * @brief AFT Cashout to host bits
 */
#define EHCS_COTOHOSTCTRLH	((uint8_t)0x01)	//!< Cashout to host controllable by host
#define EHCS_COTOHOSTCTRLGM	((uint8_t)0x00)	//!< Cashout to host forced by gaming machine
#define EHCS_COTOHOSTSTENA	((uint8_t)0x02)	//!< Cashout to host currently enabled
#define EHCS_COTOHOSTSTDIS	((uint8_t)0x00)	//!< Cashout to host currently disabled
#define EHCS_COTOHOSTMODEHD	((uint8_t)0x04)	//!< Host cashout mode currently hard (only valid if cashout to host is enabled)
#define EHCS_COTOHOSTMODESF	((uint8_t)0x00)	//!< Host cashout mode currently soft

/**
 * @brief AFT AFT Status bits
 */
#define AFTST_PRNAVFORRCPT	((uint8_t)0x01)		//!< Printer available for receipts
#define AFTST_PARTRXHOALWD	((uint8_t)0x02)		//!< Partial transfers to host available
#define AFTST_CUSTTKTSUP	((uint8_t)0x04)		//!< Custom ticket data supported
#define AFTST_AFTREGISTERED	((uint8_t)0x08)		//!< AFT registered
#define AFTST_INHOUSEENA	((uint8_t)0x10)		//!< In house transfers enabled
#define AFTST_BONUSTRENA	((uint8_t)0x20)		//!< Bonus transfers enabled
#define AFTST_DEBITTRENA	((uint8_t)0x40)		//!< Debit transfers enabled

/**
 * @brief AFT transfer codes
 */
#define AFTTRXC_FULL		((uint8_t)0x00)	//!< Transfer FULL only
#define AFTTRXC_PARTIAL		((uint8_t)0x01)	//!< Partial transfers allowed
#define AFTTRXC_CANCEL		((uint8_t)0x80)	//!< Cancel trx request
#define AFTTRXC_INT1		((uint8_t)0xFE)	//!< Interrogation Request
#define AFTTRXC_INT2		((uint8_t)0xFF) //!< Interrogation Request (this closes transfer cycle)

/**
 * @brief AFT transfer types
 */
#define AFTTRX_IHHTGM	((uint8_t)0x00)		//!< Transfer in-house amount from host to gaming machine
#define AFTTRX_BCOWHTGM	((uint8_t)0x10) 	//!< Transfer bonus coin out win amount from host to gaming machine
#define AFTTRX_BJWHTGM	((uint8_t)0x11) 	//!< Transfer bonus jackpot win amount from host to gaming machine (force attendant pay lockup)
#define AFTTRX_IHHTTKT	((uint8_t)0x20) 	//!< Transfer in-house amount from host to ticket (only one amount type allowed per transfer)
#define AFTTRX_DHTGM	((uint8_t)0x40) 	//!< Transfer debit amount from host to gaming machine
#define AFTTRX_DHTTKT	((uint8_t)0x60) 	//!< Transfer debit amount from host to ticket
#define AFTTRX_IHGMTH	((uint8_t)0x80) 	//!< Transfer in-house amount from gaming machine to host
#define AFTTRX_IHWGMTH	((uint8_t)0x90) 	//!< Transfer win amount (in-house) from gaming machine to host

/**
 * @brief AFT transfer Flags
 */
#define AFTTRXF_HCOENAC	((uint8_t)0x01)	//!< 0 Host cashout enable control (1 = set enable to bit 1 state)
#define AFTTRXF_HCOENA	((uint8_t)0x02)	//!< 1 Host cashout enable (ignore if bit 0 = 0)
#define AFTTRXF_HCOMODE	((uint8_t)0x04)	//!< 2 Host cashout mode (0=soft, 1=hard) (ignore if bit 0 = 0)
#define AFTTRXF_COGMREQ	((uint8_t)0x08)	//!< 3 Cashout from gaming machine request
#define AFTTRXF_LKATREQ	((uint8_t)0x10)	//!< 4 Lock After Transfer request (See Section 8.9)
#define AFTTRXF_UCTKTDT	((uint8_t)0x20)	//!< 5 Use custom ticket data (from long poll 76)
#define AFTTRXF_ATRXOFL	((uint8_t)0x40)	//!< 6 Accept transfer only if locked
#define AFTTRXF_TRXRREQ	((uint8_t)0x80)	//!< 7 Transaction receipt request

/**
 * @ brief AFT Transfer status
 */
// Binary codes 000xxxxx indicate transfer successful
#define AFTTRXST_FULLTRXOK	((uint8_t)0x00) 	//!< Full transfer successful
#define AFTTRXST_PARTTRXOK	((uint8_t)0x01) 	//!< Partial transfer successful

//Binary codes 010xxxxx indicate transfer pending
#define AFTTRXST_TRXPEND	((uint8_t)0x40) 	//!< Transfer pending (not complete)

// Binary codes 100xxxxx indicate transfer failed
#define AFTTRXST_TRXCBH		((uint8_t)0x80) 	//!< Transfer cancelled by host
#define AFTTRXST_DUPTRXID	((uint8_t)0x81) 	//!< Transaction ID not unique (same as last successful transfer logged in history)
#define AFTTRXST_INVTRXFUNC	((uint8_t)0x82) 	//!< Not a valid transfer function (unsupported type, amount, index, etc.)
#define AFTTRXST_NVTAMOREXP	((uint8_t)0x83) 	//!< Not a valid transfer amount or expiration (non-BCD, etc.)
#define AFTTRXST_TAMEXGMLIM	((uint8_t)0x84) 	//!< Transfer amount exceeds the gaming machine transfer limit
#define AFTTRXST_TAMNMGMDEN	((uint8_t)0x85) 	//!< Transfer amount not an even multiple of gaming machine denomination
#define AFTTRXST_GMUNPPTRXH	((uint8_t)0x86) 	//!< Gaming machine unable to perform partial transfers to the host
#define AFTTRXST_GMUPTRXNOW	((uint8_t)0x87) 	//!< Gaming machine unable to perform transfers at this time (door open, tilt, disabled, cashout in progress, etc.)
#define AFTTRXST_GMNOTREG	((uint8_t)0x88) 	//!< Gaming machine not registered (required for debit transfers)
#define AFTTRXST_REGKEYNM	((uint8_t)0x89) 	//!< Registration key does not match
#define AFTTRXST_NOPOSID	((uint8_t)0x8A) 	//!< No POS ID (required for debit transfers)
#define AFTTRXST_NWCAVFORCO	((uint8_t)0x8B) 	//!< No won credits available for cashout
#define AFTTRXST_NOGMDENOS	((uint8_t)0x8C) 	//!< No gaming machine denomination set (unable to perform cents to credits conversion)
#define AFTTRXST_EXNVFTTTKT	((uint8_t)0x8D) 	//!< Expiration not valid for transfer to ticket (already expired)
#define AFTTRXST_TRXTTKTNA	((uint8_t)0x8E) 	//!< Transfer to ticket device not available
#define AFTTRXST_UTATERADP	((uint8_t)0x8F) 	//!< Unable to accept transfer due to existing restricted amounts from different pool
#define AFTTRXST_UNTPTRXREC	((uint8_t)0x90) 	//!< Unable to print transaction receipt (receipt device not currently available)
#define AFTTRXST_IDTPTRREC	((uint8_t)0x91)		//!< Insufficient data to print transaction receipt (required fields missing)
#define AFTTRXST_TRNAFSTRT	((uint8_t)0x92)		//!< Transaction receipt not allowed for specified transfer type
#define AFTTRXST_AN0ODNM	((uint8_t)0x93) 	//!< Asset number zero or does not match
#define AFTTRXST_GMNOTLOCK	((uint8_t)0x94) 	//!< Gaming machine not locked (transfer specified lock required)
#define AFTTRXST_TRXIDINV	((uint8_t)0x95) 	//!< Transaction ID not valid
#define AFTTRXST_UNEXPERROR	((uint8_t)0x9F) 	//!< Unexpected error

// Binary codes 110xxxxx indicate incompatible or unsupported poll
#define AFTTRXST_NCWCTRXINP	((uint8_t)0xC0) 	//!< Not compatible with current transfer in progress
#define AFTTRXST_UNTRXCODE	((uint8_t)0xC1) 	//!< Unsupported transfer code

// Binary codes 111xxxxx indicate no transfer information available
#define AFTTRXST_NOTRXINAV	((uint8_t)0xFF) 	//!< No transfer information available

/**
 * @brief AFT Transaction Receipt Status Codes
 */
#define AFTRCPT_PRINTED	((uint8_t)0x00) 	//!< Receipt printed
#define AFTRCPT_PINPROG	((uint8_t)0x20)	 	//!< Receipt printing in progress (not complete)
#define AFTRCPT_PENDING	((uint8_t)0x40) 	//!< Receipt pending (not complete)
#define AFTRCPT_NOTPRIN	((uint8_t)0xFF) 	//!< No receipt requested or receipt not printed

// Tipos de host cashout para CMD_RQSTHOSTCO
#define AFT_RQSTCONONE	((uint8_t)0x00)
#define AFT_RQSTHOSTCO	((uint8_t)0x01)
#define AFT_RQSTHOSTCOW ((uint8_t)0x02)

/**
 * @brief AFT Transfer receipt fields
 */
#define AFTTRRF_LOCATION	((uint8_t)0x00) 	//!< Location Variable ASCII text (22 max)
#define AFTTRRF_ADDR1		((uint8_t)0x01) 	//!< Address 1 Variable ASCII text (22 max)
#define AFTTRRF_ADDR2		((uint8_t)0x02) 	//!< Address 2 Variable ASCII text (22 max)
#define AFTTRRF_INHLINE1	((uint8_t)0x10)		//!< In-house line 1 Variable ASCII text (22 max)
#define AFTTRRF_INHLINE2	((uint8_t)0x11) 	//!< In-house line 2 Variable ASCII text (22 max)
#define AFTTRRF_INHLINE3	((uint8_t)0x12) 	//!< In-house line 3 Variable ASCII text (22 max)
#define AFTTRRF_INHLINE4	((uint8_t)0x13) 	//!< In-house line 4 Variable ASCII text (22 max)
#define AFTTRRF_DEBLINE1	((uint8_t)0x20) 	//!< Debit line 1 Variable ASCII text (22 max)
#define AFTTRRF_DEBLINE2	((uint8_t)0x21) 	//!< Debit line 2 Variable ASCII text (22 max)
#define AFTTRRF_DEBLINE3	((uint8_t)0x22) 	//!< Debit line 3 Variable ASCII text (22 max)
#define AFTTRRF_DEBLINE4	((uint8_t)0x23) 	//!< Debit line 4 Variable ASCII text (22 max)

/**
 * @brief Custom AFT ticket data elements
 */
#define AFTCUST_LOCATION	((uint8_t)0x00) 	//!< Custom AFT location Variable ASCII text (40 max)
#define AFTCUST_ADDR1		((uint8_t)0x01) 	//!< Custom AFT address 1 Variable ASCII text (40 max)
#define AFTCUST_ADDR2		((uint8_t)0x02) 	//!< Custom AFT address 2 Variable ASCII text (40 max)
#define AFTCUST_GRSELECT	((uint8_t)0x03) 	//!< Custom AFT graphics selector Variable ASCII text (3 max)
#define AFTCUST_TKTTITLE	((uint8_t)0x10) 	//!< Custom AFT ticket title Variable ASCII text (16 max)

#if defined(__cplusplus)
extern "C"
{
#endif

	// ******************* Estructura SAS ***********************
	// Puntero a funcion que implementa un long poll
	typedef int32_t(*poll_ptr)(const uint8_t*, const uint32_t, uint16_t*, uint32_t*);

#if defined(__cplusplus)
}
#endif

#endif //__GENDEFS_H__

