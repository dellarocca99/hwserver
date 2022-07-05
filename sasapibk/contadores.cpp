//---------------------------------------------------------------------------
#include "contadores.h"
#include "uart_test/Core/Src/gendefs.h"
#include <cassert>
#include <cstdio>
//---------------------------------------------------------------------------

#define CTRS_FILE   "simctrs.bin"

namespace EngineAPI
{
    /**
     * @brief Retorna la posicion del contador en el array
     * 
     * @param _ctrId Id SAS del contador
     * @return int32_t Entero entre 0 y 14, -1 si el contador no esta soportado
     */
    static int32_t getGameCtrIx(const uint8_t _ctrId)
    {
        int32_t sal = -1;

        switch (_ctrId)
        {
        case TOTAL_COIN_IN:		sal = 0; break;
        case TOTAL_COIN_OUT:	sal = 1; break;
        case TOTAL_JACKPOT:		sal = 2; break;
        case GAMES_PLAYED:		sal = 3; break;
        case GAMES_WON:			sal = 4; break;
        case GAMES_LOST:		sal = 5; break;
        case TOTAL_MPPTWIN:		sal = 6; break;
        case TOTAL_MPPRWIN:		sal = 7; break;
        case TOTAL_MPEXTBNS:	sal = 8; break;
        case TOTAL_APTWIN:		sal = 9; break;
        case TOTAL_APPWIN:		sal = 10; break;
        case TOTAL_APEXTBNS:	sal = 11; break;
        case TOTAL_WONCRED:		sal = 12; break;
        case WEIGHAVGTHPB:		sal = 13; break;
        case TOTAL_HP_CC:		sal = 14; break;
        }

        return sal;
    }

    counters::counters()
    {
        resetToZero();
    }

    /**
     * @brief Obtiene el Id SAS de un contador dada su posicion en el array
     * 
     * @param _pos Posicion en el array de contadores (0..14)
     * @return uint8_t Id SAS o 0xFF si no existe
     */
    uint8_t counters::getGameCounterIdByPos(const uint32_t _pos) noexcept
    {
        uint8_t sal = 0xFF;

        assert((_pos < 15) && "_pos invalida");

        switch (_pos)
        {
        case 0:		sal = TOTAL_COIN_IN; break;
        case 1:		sal = TOTAL_COIN_OUT; break;
        case 2:		sal = TOTAL_JACKPOT; break;
        case 3:		sal = GAMES_PLAYED; break;
        case 4:		sal = GAMES_WON; break;
        case 5:		sal = GAMES_LOST; break;
        case 6:		sal = TOTAL_MPPTWIN; break;
        case 7:		sal = TOTAL_MPPRWIN; break;
        case 8:		sal = TOTAL_MPEXTBNS; break;
        case 9:		sal = TOTAL_APTWIN; break;
        case 10:	sal = TOTAL_APPWIN; break;
        case 11:	sal = TOTAL_APEXTBNS; break;
        case 12:	sal = TOTAL_WONCRED; break;
        case 13:	sal = WEIGHAVGTHPB; break;
        case 14:	sal = TOTAL_HP_CC; break;
        }

        return sal;
    }

    /**
     * @brief Setea el contador de un juego, en un valor indicado
     * 
     * @param _ng Numero de juego (0->Maquina, 1..24 juego)
     * @param _ctr Id SAS del contador
     * @param _val Valor del contador
     */
    void counters::setCounter(
        const uint32_t _ng,
        const uint8_t _ctr,
        const uint64_t _val
        ) noexcept
    {
        assert((_ng < (gameCtrs.size() + 1)) && "Nro de juego fuera de rango");
        assert((_ctr < 0x80) && "Contador fuera de rango");

        if (_ng)
        {
            const int32_t ix = getGameCtrIx(_ctr);
            if (ix != -1)
            {
                gameCtrs[_ng][ix] = _val;
            }
        }
        else
        {
            maqCtrs[_ctr] = _val;
        }
    }

    /**
     * @brief Obtiene el valor de un contador, de un juego dado
     * 
     * @param _ng Numero de juego (0->Maquina, 1..24 juego)
     * @param _ctr Id SAS del contador
     * @return uint64_t Valor del contador
     */
    uint64_t counters::getCounter(const uint32_t _ng, const uint8_t _ctr) const noexcept
    {
        uint64_t sal = 0ULL;

        assert((_ng < (gameCtrs.size() + 1)) && "Nro de juego fuera de rango");
        assert((_ctr < 0x7F) && "Contador fuera de rango");

        if (_ng)
        {
            const int32_t ix = getGameCtrIx(_ctr);
            if (ix != -1)
            {
                sal = gameCtrs[_ng][ix];
            }
        }
        else
        {
            sal = maqCtrs[_ctr];
        }

        return sal;
    }

    /**
     * @brief Resetea a cero todos los contadores
     * 
     */
    void counters::resetToZero() noexcept
    {
        maqCtrs.fill(0ULL);
        for (auto& gctrs : gameCtrs)
        {
            gctrs.fill(0ULL);
        }
    }

    /**
     * @brief Carga los contadores desde disco
     * 
     * @return true Pudo cargar
     * @return false No pudo cargar
     */
    bool counters::load()
    {
        FILE* f = fopen(CTRS_FILE, "rb");

        if (f)
        {
            int res = fread(maqCtrs.data(), maqCtrs.size() * sizeof(uint64_t), 1, f);
            if (res != 1)
            {
                fclose(f);
                return false;
            }

            for (auto& game : gameCtrs)
            {
                res = fread(game.data(), game.size() * sizeof(uint64_t), 1, f);
                if (res != 1)
                {
                    fclose(f);
                    return false;
                }
            }

            fclose(f);
            return true;
        }
        else
        {
	        return false;
        }
    }

    /**
     * @brief Almacena los contadores en disco
     * 
     * @return true Pudo almacenar
     * @return false No pudo almacenar
     */
    bool counters::save()
    {
        FILE* f = fopen(CTRS_FILE, "wb");

        if (f)
        {
            int res = fwrite(maqCtrs.data(), maqCtrs.size() * sizeof(uint64_t), 1, f);
            if (res != 1)
            {
                fclose(f);
                return false;
            }

            for (const auto& game : gameCtrs)
            {
                res = fwrite(game.data(), game.size() * sizeof(uint64_t), 1, f);
                if (res != 1)
                {
                    fclose(f);
                    return false;
                }
            }

            fclose(f);
            return true;
        }
        else
        {
	        return false;
        }
    }
}

