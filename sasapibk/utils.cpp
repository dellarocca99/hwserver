//---------------------------------------------------------------------------

#pragma hdrstop

#include "utils.h"
#include <cstring>
#include <ctime>
#include <cassert>

#ifdef __linux__
	#include <unistd.h>
#else
	#include <windows.h>
#endif

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#define MAX_SAS_BCD		18	// 18 digitos

/**
 * @brief Convierte un numero a BCD. El numero BCD esta invertido por Requerimiento de SAS
 * 
 * @param _num Numero a convertir
 * @param _bcd Buffer que contendra el valor BCD
 * @param _inSize Cantidad de bytes a utilizar del buffer
 */
void numToBCD(const uint64_t _num, uint8_t* _bcd, const uint32_t _inSize)
{
	uint64_t aux = _num;
	uint8_t buff[MAX_SAS_BCD];
	int32_t ix = 0, pos;

	memset(_bcd, 0, _inSize);
	if (_num == 0)
		return;

    memset(buff, 0, sizeof(buff));
	while (aux)
	{
		uint64_t resto = aux % 10ULL;
		aux /= 10;
		buff[ix++] = resto;
	}

	pos = _inSize - 1;
	for (int i = 0; i < ix; ++i)
	{
		if (i % 2)
			_bcd[pos - i / 2] |= buff[i] << 4;
		else
			_bcd[pos - i / 2] = buff[i];
	}
}

/**
 * @brief Convierte un valor BCD a un numero decimal. (El BCD debe estar invertido)
 * 
 * @param _buffer Buffer con el Valor BCD
 * @param _size Cantidad de bytes ocupados
 * @return uint64_t Numero decimal
 */
uint64_t bcdToNum(const uint8_t* _buffer, const uint32_t _size)
{
	uint64_t sal = 0;
	uint64_t mul = 1;
	const int32_t realSize = _size * 2UL;
	uint8_t shift = 0;

	for (int32_t nibble = realSize - 1; nibble >= 0; --nibble)
	{
		const uint64_t aux = (_buffer[nibble / 2] >> shift) & 0x0F;
		sal += aux * mul;
		mul *= 10;
		if (shift)
			shift = 0;
		else
			shift = 4;
	}

	return sal;
}

/**
 * @brief Calcula el CRC CCITT16 de un buffer
 * 
 * @param _buffer Buffer de datos para calcular
 * @param _size Longitud en bytes del buffer
 * @param _crcval Semilla inicial (0 para SAS)
 * @return uint16_t Crc calculado
 */
uint16_t sasCRC(const uint8_t* _buffer, const int32_t _size, uint16_t _crcval)
{
	uint32_t c, q;

	for (int32_t ix = 0; ix < _size; ++ix)
	{
		c = _buffer[ix];
		q = (_crcval ^ c) & 017;
		_crcval = (_crcval >> 4) ^ (q * 010201);
		q = (_crcval ^ (c >> 4)) & 017;
		_crcval = (_crcval >> 4) ^ (q * 010201);
	}

	return (_crcval);
}

// BCD correcto

/**
 * @brief Convierte un numero a BCD. El orden del BCD es el correcto
 * 
 * @param _num Numero a convertir
 * @param _bcd Buffer que contendra el valor BCD
 * @param _inSize Cantidad de bytes a utilizar del buffer
 */
void numToBCDR(const uint64_t _num, uint8_t* _bcd, const uint32_t _inSize)
{
	uint64_t aux = _num;
	uint8_t buff[MAX_SAS_BCD];
	int32_t ix = 0;

	memset(_bcd, 0, _inSize);
	if (_num == 0) return;

    memset(buff, 0, sizeof(buff));
	while (aux)
	{
		uint64_t resto = aux % 10ULL;
		aux /= 10;
		buff[ix++] = resto;
	}

	for (uint32_t i = 0; i < _inSize; ++i)
    {
        _bcd[i] = (buff[2 * i + 1] << 4) | buff[2 * i];
    }
}

/**
 * @brief Convierte un valor BCD a un numero decimal. El BCD debe estar en el orden normal
 * 
 * @param _buffer Buffer con el Valor BCD
 * @param _size Cantidad de bytes ocupados
 * @return uint64_t Numero decimal
 */
uint64_t bcdToNumR(const uint8_t* _buffer, const uint32_t _size)
{
	uint64_t sal = 0;
	uint64_t mul = 1;
	const int32_t realSize = _size * 2UL;
	uint8_t shift = 0;

	for (int32_t nibble = 0; nibble < realSize; ++nibble)
	{
		const uint64_t aux = (_buffer[nibble / 2] >> shift) & 0x0F;
		sal += aux * mul;
		mul *= 10;
		if (shift)
			shift = 0;
		else
			shift = 4;
	}

	return sal;
}

/**
 * @brief Genera dos buffers BCD, uno con la fecha actual y otro con la hora actual.
 * El formato de la fecha es MMDDYYYY y el de la hora HHMMSS (24hs)
 * 
 * @param _d Buffer para la fecha (4 BCD)
 * @param _t Buffer para la hora (3 BCD)
 */
void bcdDateTime(uint8_t* _d, uint8_t* _t)
{
	assert(_d && "_d no puede ser NULL");
	assert(_t && "_t no puede ser NULL");

	time_t now = time(nullptr);
	tm lt;

#ifdef __linux__
	tm* pLt = localtime(&now);
	if (pLt) 
		memcpy(&lt, pLt, sizeof(tm));
	else
		return;
#elif defined _MSC_VER
    if (localtime_s(&lt, &now)) return;
#else
	if (localtime_s(&now, &lt) == nullptr) return;
#endif

	numToBCD(lt.tm_mon + 1, _d, 1);
	numToBCD(lt.tm_mday, &_d[1], 1);
	numToBCD(lt.tm_year + 1900, &_d[2], 2);
	numToBCD(lt.tm_hour, _t, 1);
	numToBCD(lt.tm_min, &_t[1], 1);
	numToBCD(lt.tm_sec, &_t[2], 1);
}

/**
 * @brief Pasa un buffer BCD a formato de cadena en hexadecimal
 * 
 * @param _bcd Buffer BCD
 * @param _size Cantidad de bytes a utilizar del buffer
 * @param _sVal Cadena hexa de salida
 */
void bcdToStr(const uint8_t* _bcd, const uint32_t _size, std::string& _sVal)
{
	assert(_bcd);

	_sVal.clear();
	for (int i = 0; i < 8; ++i)
	{
		char num[3];

		sprintf(num, "%02X", _bcd[i]);
		_sVal += num;
	}
}

/**
 * @brief Pone en pausa el thread actual por la cantidad de Ms especificada
 * 
 * @param _ms Cantidad de Ms a esperar
 */
void osWait(const uint32_t _ms)
{
#ifdef __linux__
	usleep(1000 * _ms);
#else
	Sleep(_ms);
#endif
}

/**
 * @brief Obtiene el tick actual del SO
 * 
 * @return uint32_t Cantidad de milisegundos desde el start del SO
 */
uint32_t getOSTick()
{
#ifdef __linux__
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	tictac = (now.tv_sec * 1000 + (now.tv_nsec / 1000000));
#else
    return ::GetTickCount();
#endif
}

/**
 * @brief Inicializa el logueo. Debe ser llamada antes que ningun metodo de la API!!!
 * 
 * @param _lvl Nivel de log
 * @param _path Path al archivo de log
 */
void initLogging(const LogLevel _lvl, const std::string _path)
{
    if (logger_initFileLogger(_path.c_str(), 1024 * 1024, 2)) logger_setLevel(_lvl);
}
