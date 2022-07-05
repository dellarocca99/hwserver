#ifndef __COUNTERSDTO_H__
#define __COUNTERSDTO_H__


#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class CounterValueDTO : public  oatpp::DTO {
    DTO_INIT(CounterValueDTO, DTO);

    DTO_FIELD(UInt32, gn);
    DTO_FIELD(Int64, value);
};

class CounterDTO : public  oatpp::DTO {
    DTO_INIT(CounterDTO, DTO);

    DTO_FIELD(UInt8, id);
    DTO_FIELD(oatpp::List<oatpp::Object<CounterValueDTO>>, values);
};

class CountersDTO : public  oatpp::DTO {
    DTO_INIT(CountersDTO, DTO);

    DTO_FIELD(List<Object<CounterDTO>>, meters);
};


#include OATPP_CODEGEN_END(DTO)


#endif // __COUNTERSDTO_H__