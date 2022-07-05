#ifndef enginecomH
#define enginecomH

#include "engineapi.h"
#include <map>
#include <thread>

#define TXCMD(a)    reinterpret_cast<command_t*>(&a[2])
#define RXCMD(a)    reinterpret_cast<const command_t*>(a)

namespace EngineAPI
{
    /**
     * @brief class de comunicacion con el SAS Engine
     * 
     */
    class EngineCom
    {
    public:
        bool init(const configSE_t& _cfg);
        void deInit();
        bool isValid() const noexcept
        {
            return (pSerial != nullptr);
        }

        // UID del SE
        bool getSEID(uint8_t* _seid);

        // Comandos
        bool sendHello(uint8_t& _validCfg, uint16_t& _crc, bool& _nv);
        bool sendReady();
        bool sendReset();
        bool sendResetNVRam();
        bool sendFyH();
        bool sendQuery(pcCmd& _cmd);
        bool protocolDebugger(const bool _ena);

        // TESTING
        bool sendNVData(const uint8_t* _arr, const uint16_t _size, const uint16_t _addr);
        bool recvNVData(uint8_t* _arr, const uint16_t _size, const uint16_t _addr);

        bool notifyEngine(uint8_t* _out, uint8_t* _in, const uint32_t _to = 0UL);

    private:
        configSE_t  configSE;           //!< Configuracion del engine
        serialPort* pSerial{nullptr};   //!< Objeto del puerto serie
        std::mutex  mtxPort;            //!< Mutex de acceso al puerto

		bool initCOMPort() noexcept;
		void deinitCOMPort() noexcept;

		bool sendPkt(commData_t& _cd);
		bool recvPkt(commData_t& _cd);
        bool sendAndRecv(commData_t& _cd);
		void processRx(commData_t& _cd);
        bool rxFsm(commData_t& _cd, const uint8_t* _buff, const uint32_t _size);
        bool isValidCmd(commData_t& _cd) noexcept;
        bool makeFromCmd(commData_t& _cd);
    };
}

#endif // enginecomH
