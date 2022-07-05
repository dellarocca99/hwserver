#ifndef serialH
#define serialH

#ifdef __BORLANDC__
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #define USE_WINDOWS
#elif defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #define USE_WINDOWS
#else
    #define USE_LINUX
#endif // __BORLANDC__

#include <string>
#include <cstdint>

#define WAIT_TIME			2	//2 ms
#define MAX_SERIAL_TX_BUFF	4096

/*! \file */

/**
 * @brief Resultado de la operaciones de send y recv
 * 
 */
enum eDevResult
{
	eDevError 	= -2,	//!< Error de puerto serial
	eDevWaiting = -1,	//!< Esperando la llegada / envio de datos
	eDevOk		= 0		//!< Datos recibidos / enviados
};

/**
 * @brief Class que representa al puerto serie
 * 
 */
class serialPort
{
private:
#ifdef WIN32
	HANDLE m_Port{INVALID_HANDLE_VALUE};
#else
	int32_t m_Port{-1};
#endif

public:
	bool open(const char* _devname, const char* _cfg);
	void close();
	void clear();

	//!Envia un paquete por el puerto serial, con control de resultado
	eDevResult send(const void* _buffer, const uint32_t _size);
	eDevResult recv(void* _buffer, const uint32_t _toRead, uint32_t& _readed);
};

#endif // serialH

