#ifndef aftH
#define aftH

#include "enginecom.h"
#include "utils.h"

#include <memory>

namespace EngineAPI
{
	/**
	 * @brief Enum que representa los codigos de los campos, para los recibos custom
	 * 
	 */
	enum eCustRcptFields
	{
		eFldTrSrcDst 	= 0x00,
		eFldFyH			= 0x01,
		eFldPatronName	= 0x10,
		eFldPatronAccN	= 0x11,
		eFldAccBalance	= 0x13,
		eFldDebCrdNbr	= 0x41,
		eFldTrxFee		= 0x42,
		eFldTotalDebAm	= 0x43
	};

	/**
	 * @brief Estructura que representa un campo de recibo custom
	 * 
	 */
	struct custRcptData_t
	{
		eCustRcptFields field;
		std::string		value;
	};

	/**
	 * @brief Class que representa una transferencia AFT
	 * 
	 */
	class aftTransfer
	{
	public:
		/**
		 * @brief Establece la transferencia actual. 
		 * 
		 * @param _trx estructura aftTrxCmd_t recibida en el comando CMD_QUERY, sub comando CMD_AFTINITRX
		 */
		void setCurrent(const aftTrxCmd_t& _trx);

		/**
		 * @brief Obtiene el codigo de transferencia 
		 * 
		 * @return uint8_t (0->Full, 1->Partial)
		 */
		uint8_t getCode() const
		{
			return currTrxRx.code;
		}

		/**
		 * @brief Identifica si se solicito imprimir un recibo
		 * 
		 * @return true Se solicito impresion de recibo
		 * @return false No se solicito impresion de recibo
		 */
		bool printAsked() const
		{
			return (currTrxRx.flags & AFTTRXF_TRXRREQ) ? true : false;
		}

		/**
		 * @brief Establece el estado de la transferencia actual
		 * 
		 * @param _st Estado (ver AFT Transfer status)
		 */
		void setStatus(const uint8_t _st)
		{
			currTrxTx.status = _st;
		}

		/**
		 * @brief Establece el estado de impresion del recibo
		 * 
		 * @param _st Estado (ver AFT Transaction Receipt Status Codes)
		 */
		void setReceiptStatus(const uint8_t _st)
		{
			currTrxTx.rcpStatus = _st;
		}

		/**
		 * @brief Establezco el valor del monto casheable
		 * 
		 * @param _amnt Valor en centavos
		 */
		void setCasheableAmnt(const uint64_t _amnt)
		{
			currTrxTx.cashAmnt = _amnt;
		}

		/**
		 * @brief Establezco el valor non restricted
		 *
		 * @param _amnt Valor en centavos
		 */
		void setNonRestAmnt(const uint64_t _amnt)
		{
			currTrxTx.nonRestAmnt = _amnt;
		}

		/**
		 * @brief Establezco el valor restricted
		 *
		 * @param _amnt Valor en centavos
		 */
		void setRestAmnt(const uint64_t _amnt)
		{
			currTrxTx.restAmnt = _amnt;
		}

		/**
		 * @brief Retorna si el host solicito el control del Host Cashout Enable
		 *
		 * @return true Lo solicito
		 * @return false No lo solicito
		 */
		bool hostCashoutEnableCtrl() const
		{
			return (currTrxRx.flags & AFTTRXF_HCOENAC) ? true : false;
		}

		/**
		 * @brief Estado del host cashout enable solicitado por el host. Solo valido
		 * si hostCashoutEnableCtrl retorna true
		 * 
		 * @return true Habilitar
		 * @return false Des habilitar
		 */
		bool hostCashoutEnable() const
		{
			return (currTrxRx.flags & AFTTRXF_HCOENA) ? true : false;
		}

		/**
		 * @brief Obtiene el modo de cashout to host requerido por el host
		 * 
		 * @return uint8_t (0->soft, 1->hard)
		 */
		uint8_t hostCashoutModeReq() const
		{
			return (currTrxRx.flags & AFTTRXF_HCOMODE);
		}

		/**
		 * @brief Indica si la transferencia es a pedido del GM o no
		 * 
		 * @return true A pedido del GM
		 * @return false A pedido del Host
		 */
		bool cashoutFromGM() const
		{
			return (currTrxRx.flags & AFTTRXF_COGMREQ) ? true : false;
		}

		/**
		 * @brief Si esto es true, el equipo debe realizar otro lock al finalizar
		 * la transferencia actual. Esto es para permitir multiples transferencias
		 * sin la necesidad de enviar otras transacciones de lock.
		 * 
		 * @return true Lock After Transfer solicitado
		 * @return false Lock After Transfer no solicitado
		 */
		bool lockAfterTransfer() const
		{
			return (currTrxRx.flags & AFTTRXF_LKATREQ) ? true : false;
		}

		/**
		 * @brief Indica si se deben utilizar los datos del recibo aportados
		 * por el poll 76 o los propios
		 * 
		 * @return true Utilizar los datos del poll 76
		 * @return false Utilizar los propios
		 */
		bool useCustTktData() const
		{
			return (currTrxRx.flags & AFTTRXF_UCTKTDT) ? true : false;
		}

		/**
		 * @brief Indica si la transferencia debe ser aceptada solo de estar lockeado
		 * 
		 * @return true Aceptar solo si se esta lockeado
		 * @return false Aceptar aunque no se este lockeado
		 */
		bool acceptTrOnlyIfLocked() const
		{
			return (currTrxRx.flags & AFTTRXF_ATRXOFL) ? true : false;
		}

		/**
		 * @brief Establece quien tiene el control del Host Cashout Enable
		 * 
		 * @param _val true lo comanda el Host, false lo comanda el GM
		 */
		void setCashoutToHostControl(const bool _val)
		{
			SETORRESET(currTrxTx.flags, AFTTRXF_HCOENAC, _val);
		}

		/**
		 * @brief Establece el estado del Host Cashout
		 * 
		 * @param _val true -> Enabled, false -> Disabled
		 */
		void setCashoutToHost(const bool _val)
		{
			SETORRESET(currTrxTx.flags, AFTTRXF_HCOENA, _val);
		}

		/**
		 * @brief Establece el modo de host cashout
		 * 
		 * @param _val true -> Hard, false -> Soft
		 */
		void setHostCashoutMode(const bool _val)
		{
			SETORRESET(currTrxTx.flags, AFTTRXF_HCOMODE, _val);
		}

		/**
		 * @brief Obtiene el monto total de la transaccion
		 * 
		 * @return uint64_t Monto total en centavos
		 */
		uint64_t getTotalTrxAmnt() const
		{
			return currTrxRx.cashAmnt + currTrxRx.nonRestAmnt + currTrxRx.restAmnt;
		}

		/**
		 * @brief Obtiene el tipo de transferencia
		 * 
		 * @return uint8_t Tipo (ver AFT transfer types)
		 */
		uint8_t getTransferType() const
		{
			return currTrxRx.type;
		}

		/**
		 * @brief Obtiene el monto Casheable solicitado
		 * 
		 * @return uint64_t Monto en centavos
		 */
		uint64_t getReqCasheableAmnt() const
		{
			return currTrxRx.cashAmnt;
		}

		/**
		 * @brief Obtiene el monto Non Restricted solicitado
		 * 
		 * @return uint64_t Monto en centavos
		 */
		uint64_t getReqNonRestAmnt() const
		{
			return currTrxRx.nonRestAmnt;
		}

		/**
		 * @brief Obtiene el monto restricted solicitado
		 * 
		 * @return uint64_t Monto en centavos
		 */
		uint64_t getReqRestAmnt() const
		{
			return currTrxRx.restAmnt;
		}

		// Establece el Restricted Pool ID
		void setPoolID(const uint8_t _id)
		{
			currTrxTx.poolId = _id;
		}

		void getCustomReceiptData(std::vector<custRcptData_t>& _rcptData);

		/**
		 * @brief Obtiene el timeout de lock enviado por el host
		 * 
		 * @return uint32_t Timeout de Lock en milisegundos
		 */
		uint32_t getLockTimeout() const
		{
			if (lockAfterTransfer())
				return (static_cast<uint32_t>(currTrxRx.lockTimeout) * 10U);
			else
				return 0UL;
		}

		std::string getTransactionID() const;
		std::string getExpirationDate() const;

		aftTrx_t* getTxData()
		{
			return &currTrxTx;
		}

	private:
		aftTrxCmd_t	currTrxRx;
		aftTrx_t 	currTrxTx;
	};

	/**
	 * @brief Class que representa un pedido de Lock de AFT
	 * 
	 */
	class aftLock
	{
	public:
        aftLock();

		/**
		 * @brief Asigna el comando de request que llego desde el SE
		 * 
		 * @param _rq Comando de request
		 */
		void setRequest(const aftLockReq_t& _rq)
		{
			request = _rq;
		}

		/**
		 * @brief Obtiene el comando (LOCK o CANCEL)
		 * 
		 * @return uint8_t Comando (ver AFT Lock Codes)
		 */
		uint8_t getCode() const
		{
			return request.code;
		}

		/**
		 * @brief Obtiene la duracion del lock en milisegundos (Solo valido si getCode retorno AFTLCK_RQST)
		 * 
		 * @return uint32_t 
		 */
		uint32_t getTimeout() const
		{
			return (static_cast<uint32_t>(request.lockTimeout) * 10U);
		}

		/**
		 * @brief Indica si se solicito que las transferencias al GM esten disponibles
		 * 
		 * @return true 
		 * @return false 
		 */
		bool getTrToGMOK() const
		{
			return (request.transferCond & 0x01) ? true : false;
		}

		/**
		 * @brief Indica si se solicito que las transferencias desde el GM esten disponibles
		 * 
		 * @return true 
		 * @return false 
		 */
		bool getTrFromGMOK() const
		{
			return (request.transferCond & 0x02) ? true : false;
		}

		/**
		 * @brief Indica si se solicito que las transferencias al printer esten disponibles
		 * 
		 * @return true 
		 * @return false 
		 */
		bool getTrToPrnOK() const
		{
			return (request.transferCond & 0x04) ? true : false;
		}

		/**
		 * @brief Indica si se solicito que las transferencias de bonus al GM esten disponibles
		 * 
		 * @return true 
		 * @return false 
		 */
		bool getBonusAwToGMOK() const
		{
			return (request.transferCond & 0x08) ? true : false;
		}

		/**
		 * @brief Establece el status del lock
		 * 
		 * @param _st status (ver AFT Lock status)
		 */
		void setStatus(const uint8_t _st)
		{
			lockStat.status = _st;
		}

		/**
		 * @brief Establece las transferencias AFT soportadas
		 * 
		 * @param _val Mapa de bits (ver AFT Available Trx Bits)
		 */
		void setAvailTrx(const uint8_t _val)
		{
			lockStat.availTrx = _val;
		}

		/**
		 * @brief Establece las opciones de Cashout to host
		 * 
		 * @param _val Mapa de bits (ver AFT Cashout to host bits)
		 */
		void setHostCashoutSt(const uint8_t _val)
		{
			lockStat.hostSt = _val;
		}

		/**
		 * @brief Setea el estado de AFT
		 * 
		 * @param _val Mapa de bits (ver AFT AFT Status bits)
		 */
		void setAFTStatus(const uint8_t _val)
		{
			lockStat.aftSt = _val;
			if ((_val & AFTST_INHOUSEENA) || (_val & AFTST_BONUSTRENA) || (_val & AFTST_DEBITTRENA))
				lockStat.aftSt |= 0x80;
		}

		/**
		 * @brief Establece el monto casheable disponible
		 * 
		 * @param _val Monto en centavos
		 */
		void setCurrCashAmnt(const uint64_t _val)
		{
			lockStat.currCashAmnt = _val;
		}

		/**
		 * @brief Establece el monto restricted disponible
		 * 
		 * @param _val Monto en centavos
		 */
		void setCurrRestAmnt(const uint64_t _val)
		{
			lockStat.currResAmnt = _val;
		}

		/**
		 * @brief Establece el monto non restricted disponible
		 * 
		 * @param _val Monto en centavos
		 */
		void setCurrNonRestAmnt(const uint64_t _val)
		{
			lockStat.currNRAmnt = _val;
		}

		void setRestrictedExp(const uint8_t* _val);

		/**
		 * @brief Establece el limite de transferencia.
		 * 
		 * @param _val Valor en centavos
		 */
		void setGMTransferLimit(const uint64_t _val)
		{
			lockStat.gmTrxLimit = _val;
		}

		/**
		 * @brief Establece el pool ID para los montos restricted (Current restricted pool ID, if restricted non-zero)
		 * 
		 * @param _val Pool ID
		 */
		void setRestPoolID(const uint8_t _val)
		{
			lockStat.resPoolId = _val;
		}

		aftLockStat_t* getTxData()
		{
			return &lockStat;
		}

	private:
		aftLockStat_t 	lockStat;	//!< Paquete de Status de Lock
		aftLockReq_t	request;	//!< Request de lock
	};

	/**
	 * @brief Struct que representa un item de ticket AFT
	 * 
	 */
	struct aftTicketItem
	{
		uint8_t code;
		std::string value;
	};

	/**
	 * @brief Class que representa un Ticket de AFT
	 * 
	 */
	class aftTicketData
	{
	public:
		void setDataCmd(const aftTktData_t& _dataCmd);
		void getData(std::vector<aftTicketItem>& _items);

		/**
		 * @brief Indica si se enviaron datos o no
		 * 
		 * @return true Se recibieron datos
		 * @return false No se recibieron
		 */
		bool isSet() const
		{
			return (tktDataCmd.size != 0);
		}

	private:
		aftTktData_t tktDataCmd;
	};

	/**
	 * @brief Class para manejo de AFT
	 * 
	 */
	class AFT
	{
	public:
		aftTransfer 	transfer;		//!< Objeto de transferencias AFT
		aftLock			lock;			//!< Objeto de Lock request AFT
		aftTicketData	ticketData;		//!< Objeto ticket data AFT
		aftTicketData	custTicketData;	//!< Objeto ticket data AFT

	public:
		void init(std::shared_ptr<EngineCom>& _eCom);

		// Comandos
        bool getAFTRegInfo(aftRegData_t& _regInfo);
        bool initAFTRegistration();
        bool sendAFTRegOperatorACK();
        bool updateAFTLsData();
		bool endAFTTransaction();
		bool updateAFTTrStatus();
		bool requestHostCashout(const uint8_t _type);

	private:
		std::shared_ptr<EngineCom> 	eCom;	//!< Objeto de comunicacion con el SE
	};
}

#endif // aftH
