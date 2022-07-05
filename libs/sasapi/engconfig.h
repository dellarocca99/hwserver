//---------------------------------------------------------------------------

#ifndef engconfigH
#define engconfigH

#include "../uart_test/Core/Src/sasstructs.h"
#include "enginecom.h"
#include <vector>
#include <cstdint>
#include <bitset>
#include <string>
#include <memory>

/**
 * @brief Constantes de mascara para extended validation status
 * 
 */
// LSB
#define MIM_PRNASCASHOUT    ((uint16_t)0x0001)  //!< Use printer as cashout device
#define MIM_PRNASHPRECEP    ((uint16_t)0x0002)  //!< Use printer as handpay receipt device
#define MIM_VALHPANDHPR     ((uint16_t)0x0004)  //!< Validate handpays and handpay receipts
#define MIM_PRINTRESTKT     ((uint16_t)0x0008)  //!< Print restricted tickets
#define MIM_TKTFORRESAMT    ((uint16_t)0x0010)  //!< Tickets for foreign restricted amounts
#define MIM_TKTREDEMPT      ((uint16_t)0x0020)  //!< Ticket redemption

// MSB
#define MIM_SECENHCFG       ((uint16_t)0x8000)  //!< Secure enhanced validation configuration

namespace EngineAPI
{
    /**
     * @brief class base que representa la interfaz de persistencia
     * 
     */
    class iPersistent
    {
    public:
        virtual bool save(std::vector<uint8_t>& _buffer) = 0;
        virtual void save(uint8_t* _buffer, uint16_t* _size) = 0;
        virtual uint32_t load(const std::vector<uint8_t>& _buffer, const uint32_t _ix) = 0;
        virtual uint16_t crc(const uint16_t _seed) const = 0;
    };

    /**
     * @brief Class que maneja la configuracion de maquina
     * 
     */
    class machineCfg : public iPersistent
    {
    public:
        machineCfg() noexcept;

        // Getters Setters
        void setTokenDeno(const uint8_t _td);
        uint8_t getTokenDeno() const noexcept;
        void setSerialNumber(const std::string& _sn);
        std::string getSerialNumber() const noexcept;
        void setMachineFeatures(const uint8_t* _mf) noexcept;
        const uint8_t* getMachineFeatures() const noexcept;
        void setModifStatusMask(const uint16_t _msk, const bool _val);
        bool getModifStatusMask(const uint16_t _msk) const;
        void setStatusMask(const uint16_t _msk, const bool _val);
        void setFullStatusMask(const uint16_t _msk) noexcept;
        bool getStatusMask(const uint16_t _msk) const;
        void setAssetNumber(const uint32_t _an) noexcept;
        uint32_t getAssetNumber() const noexcept;
        void setCashTktAndRecExp(const uint16_t _exp);
        uint16_t getCashTktAndRecExp() const noexcept;
        void setRestTktDefExpiration(const uint16_t _exp);
        uint16_t getRestTktDefExpiration() const noexcept;
        bool aftEnabled() const;
        bool tktRedemptionEnabled() const;
        
        // Persistence
        bool save(std::vector<uint8_t>& _buffer) override;
        void save(uint8_t* _buffer, uint16_t* _size) override;
        uint32_t load(const std::vector<uint8_t>& _buffer, const uint32_t _ix) override;
        uint16_t crc(const uint16_t _seed) const override;

    private:
        cfgMaq	maq;    //!< Config de maquina
    };

    /**
     * @brief Class que maneja la configuracion de SAS
     * 
     */
    class sasCfg : public iPersistent
    {
    public:
        sasCfg() noexcept;

        // Getters Setters
        void setNumChannels(const uint8_t _nc);
        uint8_t getNumChannels() const noexcept;
        void setID(const uint8_t _id);
        uint8_t getID() const noexcept;
        void setLegacyHandPay(const bool _val) noexcept;
        bool getLegacyHandPay() const noexcept;
        void setMainChannel(const uint8_t _chnl);
        uint8_t getMainChannel() const noexcept;
        void setTITOChannel(const uint8_t _chnl);
        uint8_t getTITOChannel() const noexcept;
        void setAFTChannel(const uint8_t _chnl);
        uint8_t getAFTChannel() const noexcept;

        // Persistence
        bool save(std::vector<uint8_t>& _buffer) override;
        void save(uint8_t* _buffer, uint16_t* _size) override;
        uint32_t load(const std::vector<uint8_t>& _buffer, const uint32_t _ix) override;
        uint16_t crc(const uint16_t _seed) const override;

    private:
        cfgSAS sas;    //!< Config de SAS
    };

    /**
     * @brief class que representa la configuracion SAS de un juego
     * 
     */
    class gameObj
    {
    public:
        gameObj() : pGame(nullptr)
        {

        }

        explicit gameObj(cfgGame* _game) : pGame(_game)
        {
        }

        gameObj(const gameObj& _go) : pGame(_go.pGame)
        {
        }

        gameObj(gameObj&& _go) : pGame(_go.pGame)
        {
            _go.pGame = nullptr;
        }

        gameObj& operator =(const gameObj& _go)
        {
            if (this != &_go) pGame = _go.pGame ;
            return *this;
        }

        gameObj& operator =(gameObj&& _go)
        {
            if (this != & _go)
            {
                pGame = _go.pGame;
                _go.pGame = nullptr;
            }

            return *this;
        }

        void enable(const bool _ena);
        bool getEnabled() const;
        void setID(const std::string& _id);
        std::string getID() const;
        void setAdditionalGameID(const std::string& _id);
        std::string getAdditionalGameID() const;
        void setDenomination(const uint8_t _deno);
        uint8_t getDenomination() const;
        void setMaxBet(const uint16_t _bet);
        uint16_t getMaxBet() const;
        void setProgressiveGrp(const uint8_t _grp);
        uint8_t getProgressiveGrp() const;
        void setProgressiveLevels(const std::bitset<32>& _lvls);
        std::bitset<32> getProgressiveLevels() const;
        void setPayTableID(const std::string& _id);
        std::string getPayTableID() const;
        void setPayTableName(const std::string& _id);
        std::string getPayTableName() const;
        void setBasePercent(const uint16_t _basePerc);
        uint16_t getBasePercent() const;
        void setWagerCategories(const uint16_t _wc);
        uint16_t getWagerCategories() const;
        void setName(const std::string& _name);
        std::string getName() const;

    private:
        cfgGame* pGame{nullptr};
    };

    /**
     * @brief class container para las configuraciones de los juegos
     * 
     */
    class gameCfg : public iPersistent
    {
    public:
        gameCfg();
        void reset();

        gameObj addGame();
        gameObj getGame(const uint32_t _gn);

        // Getters Setters
        uint32_t getNumGames() const noexcept;

        // Persistence
        bool save(std::vector<uint8_t>& _buffer) override;
        void save(uint8_t* _buffer, uint16_t* _size) override;
        uint32_t load(const std::vector<uint8_t>& _buffer, const uint32_t _ix) override;
        uint16_t crc(const uint16_t _seed) const override;

    private:
        std::vector<cfgGame> games;
    };

    /**
     * @brief Class que maneja localmente la configuracion del SAS Engine
     * 
     */
    class engConfig
    {
    public:
        static engConfig* init();

        bool load(const char* _file);
        bool save(const char* _file);
        bool verifyCRC(const uint16_t _crc) const;

        /**
         * @brief Retorna un puntero a la config de SAS (solo lectura)
         * 
         * @return sasCfg* Puntero a la config SAS
         */
        sasCfg* getSAS() noexcept
        {
            return &sas;
        }

        /**
         * @brief Retorna un puntero a la config de Maquina (solo lectura)
         * 
         * @return machineCfg* Puntero a la config de maquina
         */
        machineCfg* getMaq() noexcept
        {
            return &maq;
        }

        /**
         * @brief Obtiene un puntero a la config de los juegos
         * 
         * @return gameCfg* Puntero a la config del juego
         */
        gameCfg* getGame()
        {
            return &games;
        }

        uint8_t getValidationType() const;
        bool send(std::shared_ptr<EngineCom>& _ecom);

    private:
        machineCfg          maq;    //!< Config de maquina
        sasCfg          	sas;    //!< Config de SAS
        gameCfg             games;	//!< Config de los juegos
        static engConfig*   cfg;    //!< Singleton

        bool sendSASConfig(std::shared_ptr<EngineCom>& _ecom);
        bool sendMaqConfig(std::shared_ptr<EngineCom>& _ecom);
        bool sendGamesConfig(std::shared_ptr<EngineCom>& _ecom);
        bool sendSaveConfig(std::shared_ptr<EngineCom>& _ecom);
    };
}

//---------------------------------------------------------------------------
#endif
