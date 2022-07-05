//---------------------------------------------------------------------------

#ifndef contadoresH
#define contadoresH
//---------------------------------------------------------------------------
#include <cstdint>
#include <array>

#define NUM_CTRSMAQ     256
#define NUM_CTRSGAME    15
#define NUM_GAMES       24

namespace EngineAPI
{
    using gameCtrs_t = std::array<uint64_t, NUM_CTRSGAME>;

    /**
     * @brief Class que maneja los contadores en forma local
     * 
     */
    class counters
    {
    public:
        counters();
        bool load();
        bool save();
        void setCounter(const uint32_t _ng, const uint8_t _ctr, const uint64_t _val) noexcept;
        uint64_t getCounter(const uint32_t _ng, const uint8_t _ctr) const noexcept;
        void resetToZero() noexcept;

        static uint8_t getGameCounterIdByPos(const uint32_t _pos) noexcept;

    private:
        std::array<uint64_t, NUM_CTRSMAQ>	maqCtrs;   		//!< Contadores generales
        std::array<gameCtrs_t, NUM_GAMES> 	gameCtrs; 		//!< Contadores x juego
    };
}

#endif
