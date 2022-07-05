//---------------------------------------------------------------------------

#ifndef utilsH
#define utilsH

#include <cstdint>
#include <string>
#include "file_log.h"

#define SETORRESET(m, x, v) {if (v) m |= x; else m &= ~x;}

void numToBCD(const uint64_t _num, uint8_t* _bcd, const uint32_t _inSize);
uint64_t bcdToNum(const uint8_t* _buffer, const uint32_t _size);
uint16_t sasCRC(const uint8_t* _buffer, const int32_t _size, uint16_t _crcval);
void numToBCDR(const uint64_t _num, uint8_t* _bcd, const uint32_t _inSize);
uint64_t bcdToNumR(const uint8_t* _buffer, const uint32_t _size);
void bcdDateTime(uint8_t* _d, uint8_t* _t);
void bcdToStr(const uint8_t* _bcd, const uint32_t _size, std::string& _sVal);
void osWait(const uint32_t _ms);
uint32_t getOSTick();
void initLogging(const LogLevel _lvl, const std::string _path);

//---------------------------------------------------------------------------
#endif
