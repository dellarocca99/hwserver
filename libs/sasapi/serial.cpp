#include "serial.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>

#ifdef USE_LINUX
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <fcntl.h>
    #include <termios.h>
    #include <unistd.h>
    #include <errno.h>
#endif // USE_LINUX

#include "file_log.h"

/**
 * @brief Decodifica el string que contiene la config del puerto
 * 
 * @param _cfg String con la configuracion separada por comas "38400,N,8,1,N"
 * @param _iSpeed Velociadad del puerto
 * @param _iSize Bits x byte
 * @param _iStop Bits de stop
 * @param _cParity Paridad
 * @param _iFlow Control de flujo
 * @return true Si decodifico correctamente
 * @return false No decodifico correctamente
 */
static bool decode_attr(
	char* _cfg,
	int32_t& _iSpeed,
	int32_t& _iSize,
	int32_t& _iStop,
	char& _cParity,
	int32_t& _iFlow
)
{
	//Velocidad
	char* p = strtok(_cfg, ",");
	if (!p) return false;
	_iSpeed = atoi(p);

	//Paridad
	p = strtok(nullptr, ",");
	if (!p) return false;
	_cParity = p[0];

	//Cantidad de bits x byte
	p = strtok(nullptr, ",");
	if (!p) return false;
	_iSize = atoi(p);

	//Bits de stop
	p = strtok(nullptr, ",");
	if (!p) return false;
	_iStop = atoi(p);

	//Flow control
	p = strtok(nullptr, ",");
	if (!p) return false;
	_iFlow = p[0];

	return true;
}


#ifdef USE_WINDOWS

/**
 * @brief Abre un puerto serial con las opciones dadas
 * 
 * @param _devname Nombre del puerto serial (COM1 .. COMn)
 * @param _iSpeed Velocidad de operacion (hasta 115200)
 * @param _iSize Cantidad de bits x byte
 * @param _iStop Cantidad de bits de stop
 * @param _cParity Paridad (n -> no, e -> Even, o -> Odd, m -> Mark, s -> Space)
 * @param _iFlow Control de flujo (n -> none, x -> Xon/Xoff, h -> Hardware)
 * @return HANDLE Handle del puerto serial
 */
static HANDLE set_port_options(
	const char* _devname,
	int32_t _iSpeed,
	int32_t _iSize,
	int32_t _iStop,
	char _cParity,
	int32_t _iFlow
)
{
	DCB dcbSerial = { 0 };
	COMMTIMEOUTS timeouts = { 0 };
	HANDLE Port;

	//Abrimos el puerto
	Port = CreateFileA(
		_devname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (Port == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR(
        	"Error opening serial port. GetLastError->%u",
            GetLastError()
        );
		return INVALID_HANDLE_VALUE;
	}

	// Seteamos el tama�o de los buffers de Tx y Rx
	if (!SetupComm(Port, 4096, 4096))
		return INVALID_HANDLE_VALUE;

	//Seteamos los par�metros
	dcbSerial.DCBlength = sizeof(DCB);
	GetCommState(Port, &dcbSerial);
	dcbSerial.BaudRate = _iSpeed;
	dcbSerial.ByteSize = BYTE(_iSize);

	if (_iStop == 1)
		dcbSerial.StopBits = ONESTOPBIT;
	else if (_iStop == 2)
		dcbSerial.StopBits = TWOSTOPBITS;

	dcbSerial.fBinary = TRUE; //Windows no soporta Seriales no binarios

	switch (_cParity)
	{
	case 'n': dcbSerial.Parity = NOPARITY; break;
	case 'e': dcbSerial.Parity = EVENPARITY; break;
	case 'o': dcbSerial.Parity = ODDPARITY; break;
	case 'm': dcbSerial.Parity = MARKPARITY; break;
	case 's': dcbSerial.Parity = SPACEPARITY;
	}

	if (dcbSerial.Parity != NOPARITY)
		dcbSerial.fParity = TRUE;

	switch (_iFlow )
	{
	case 'x':
		dcbSerial.fOutX = 1;
		dcbSerial.fInX = 1;
		dcbSerial.fDtrControl = DTR_CONTROL_ENABLE;
		dcbSerial.fRtsControl = RTS_CONTROL_ENABLE;
		break;

	case 'h':
		dcbSerial.fDtrControl = DTR_CONTROL_ENABLE;
		dcbSerial.fRtsControl = RTS_CONTROL_ENABLE;
		break;
	}

	if (!SetCommState(Port, &dcbSerial))
	{
		LOG_ERROR("Error in SetCommState. Err: %u", GetLastError());
		return INVALID_HANDLE_VALUE;
	}

	//Timeouts para hacerlo no bloqueante
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(Port, &timeouts))
	{
        LOG_ERROR("Error in SetCommTimeouts. Err: %u", GetLastError());
		return INVALID_HANDLE_VALUE;
	}

	PurgeComm(Port, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	ClearCommError(Port, 0, 0);
	return Port;
}
#else

/**
 * @brief Abre un puerto serial con las opciones dadas
 * 
 * @param _devname Nombre del puerto serial (/dev/ttyS0 ...)
 * @param _iSpeed Velocidad de operacion (hasta 115200)
 * @param _iSize Cantidad de bits x byte
 * @param _iStop Cantidad de bits de stop
 * @param _cParity Paridad (n -> no, e -> Even, o -> Odd)
 * @param _iFlow Control de flujo (n -> none, x -> Xon/Xoff, h -> Hardware)
 * @return int32_t Handle del puerto serial
 */
static int32_t set_port_options(
	const char* _devname,
	int32_t _iSpeed,
	int32_t _iSize,
	int32_t _iStop,
	char _cParity,
	int32_t _iFlow
)
{
	int32_t port;
	speed_t baudrate;
	struct termios options;

	port = open(_devname, O_RDWR | O_NONBLOCK | O_NOCTTY);
	if (port == -1)
    {
        LOG_ERROR("Error in open. Errno: %d", errno);
		return -1;
    }

	// Tomamos la config actual del puerto
	if (tcgetattr(port, &options) == -1)
    {
        LOG_ERROR("Error in tcgetattr. Errno: %d", errno);
	    return -1;
    }

	// set up raw mode / no echo / binary
	options.c_cflag |= (tcflag_t)  (CLOCAL | CREAD);
	options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
									   ISIG | IEXTEN); //|ECHOPRT

	options.c_oflag &= (tcflag_t) ~(OPOST);
	options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK );

	//Seteamos la velocidad
	switch (_iSpeed)
	{
	case 1200:		baudrate = B1200; break;
	case 2400:		baudrate = B2400; break;
	case 4800:		baudrate = B4800; break;
	case 9600:		baudrate = B9600; break;
	case 19200:		baudrate = B19200; break;
	case 38400:		baudrate = B38400; break;
	case 57600:		baudrate = B57600; break;
	case 115200:
	default:		baudrate = B115200; break;
	}

	cfsetispeed(&options, baudrate);
	cfsetospeed(&options, baudrate);

	//Databits
	options.c_cflag &= (tcflag_t) ~CSIZE;

	switch (_iSize)
	{
	case 5:
		options.c_cflag |= CS5;
		break;

	case 6:
		options.c_cflag |= CS6;
		break;

	case 7:
		options.c_cflag |= CS7;
		break;

	case 8:
	default:
		options.c_cflag |= CS8;
		break;
	}

	//Bits de stop
	switch (_iStop)
	{
	case 2:
		options.c_cflag |= (CSTOPB);
		break;

	case 1:
	default:
		options.c_cflag &= (tcflag_t) ~(CSTOPB);
		break;
	}

	//Paridad
	options.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP);
	switch (_cParity)
	{
	case 'e':
	case 'E':
		options.c_cflag &= (tcflag_t) ~(PARODD);
		options.c_cflag |=  (PARENB);
		break;

	case 'o':
	case 'O':
		options.c_cflag |=  (PARENB | PARODD);
		break;

	case 'n':
	case 'N':
	default:
		options.c_cflag &= (tcflag_t) ~(PARENB | PARODD);
		break;
	}

	// Flow
	options.c_iflag &= (tcflag_t) ~(IXON | IXOFF);
	options.c_cflag &= (unsigned long) ~(CRTSCTS);


	// No bloqueante
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0x00;

	//Flush del contenido
	tcflush(port, TCOFLUSH);
	tcflush(port, TCIFLUSH);

	//Seteamos los atriutos
	if (tcsetattr(port, TCSANOW, &options))
	{
        LOG_ERROR("Error in tcsetattr. Errno: %d", errno);
		return -1;
	}

	return port;
}

#endif // USE_WINDOWS

/**
 * @brief Abre el dispositivo serial indicado, con la config dada. El nombre del dispositivo
 * dependera del SO (en Windows sera un COMx y en linux un /dev/ttySx).
 * El string de configuracion es del tipo Velocidad,Paridad,Bits x Byte,Bits de Stop,Control de flujo 
 * + Velocidad: 		1200, 2400, .., 9600, .., 38400, .., 115200
 * + Paridad:			N -> None, E -> Par, O -> Impar
 * + Bits x Byte:		7 u 8
 * + Bits de stop:		1 o 2
 * + Control de flujo:	N -> None, X -> Xon/Xoff, H -> Hardware
 * 
 * Ej. "38400,N,8,1,N"
 * 
 * @param _devname Dispositivo serial
 * @param _cfg String de configuracion
 * @return true Pudo abrir el puerto
 * @return false No pudo abrir el puerto
 */
bool serialPort::open(const char* _devname, const char* _cfg)
{
	int32_t speed;
	int32_t size;
	int32_t stop;
	char parity;
	int32_t flow;

	if (!_devname) return false;

	assert(_cfg && "El parametro _cfg vino NULL");

	//Decodificamos el string
    char cfgAux[30];

    strcpy(cfgAux, _cfg);
	if (!decode_attr(cfgAux, speed, size, stop, parity, flow))
		return false;

	m_Port = set_port_options(_devname, speed, size, stop, parity, flow);

#ifdef USE_WINDOWS

	if (m_Port == INVALID_HANDLE_VALUE)
		return false;

#else

	if (m_Port == -1)
		return false;

#endif // USE_WINDOWS

	return true;
}

/**
 * @brief Limpia el contenido de los buffers de Tx y Rx de dispositivos
 *
 */
void serialPort::clear()
{
#ifdef USE_WINDOWS

    assert((m_Port != INVALID_HANDLE_VALUE) && "Puerto no abierto");
	PurgeComm(m_Port, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	ClearCommError(m_Port, nullptr, nullptr);

#else

    assert((m_Port != -1) && "Puerto no abierto");
	tcflush( m_Port, TCIOFLUSH );

#endif // USE_WINDOWS
}

/**
 * @brief Cierra el dispositivo serial
 * 
 */
void serialPort::close()
{
	//Devolvemos los recursos
#ifdef USE_WINDOWS

	if (m_Port != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_Port);
        m_Port = INVALID_HANDLE_VALUE;
    }

#else

	if (m_Port != -1)
    {
		::close(m_Port);
		m_Port = -1;
    }

#endif // USE_WINDOWS
}

/**
 * @brief Envia datos por el dispositivo serial
 * 
 * @param _buffer Buffer de datos
 * @param _size Cantidad de bytes en el buffer
 * @return eDevResult Resultado de la operacion
 */
eDevResult serialPort::send(const void* _buffer, const uint32_t _size)
{
	assert(_buffer && "_buffer es NULL");

#ifdef USE_WINDOWS

	DWORD Written;

    assert((m_Port != INVALID_HANDLE_VALUE) && "Puerto no abierto");
	if (!WriteFile(m_Port, _buffer, _size, &Written, nullptr))
    {
        LOG_ERROR("Error in WriteFile. Err: %u", GetLastError());
		return eDevError;
    }


#else

    assert((m_Port != -1) && "Puerto no abierto");

	if (write(m_Port, _buffer, _size) == -1)
    {
        LOG_ERROR("Error in write. Errno: %d", errno);
		return eDevError;
    }

#endif // USE_WINDOWS

	return eDevOk;
}

/**
 * @brief Recibe (en forma no bloqueante), datos desde el dispositivo
 * 
 * @param _buffer Buffer donde poner los bytes recibidos
 * @param _toRead Cantidad maxima de bytes a recibir
 * @param _readed Cantidad real recibida 
 * @return eDevResult Resultado de la operacion
 */
eDevResult serialPort::recv(void* _buffer, const uint32_t _toRead, uint32_t& _readed)
{
	assert(_buffer && "_buffer es NULL");

#ifdef USE_WINDOWS

	DWORD Readed;

    assert((m_Port != INVALID_HANDLE_VALUE) && "Puerto no abierto");
	if (!ReadFile(m_Port, _buffer, _toRead, &Readed, nullptr))
    {
        LOG_ERROR("Error in ReadFile. Err: %u", GetLastError());
		return eDevError;
    }

	if (Readed == 0)
	{
		return eDevWaiting;
	}
	else
	{
		_readed = Readed;
		return eDevOk;
	}

#else
	int32_t Res;

    assert((m_Port != -1) && "Puerto no abierto");
	Res = read(m_Port, _buffer, _toRead);
	switch (Res)
	{
	case -1:
		_readed = 0;
		return eDevError;
		break;

	case 0:
		_readed = 0;
		return eDevWaiting;
		break;

	default:
		_readed = Res;
		return eDevOk;
		break;
	}

#endif // USE_WINDOWS
}

