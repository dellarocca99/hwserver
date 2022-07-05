#ifndef __CONFIGURATIONDTO_H__
#define __CONFIGURATIONDTO_H__


#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)


class ExtValDTO : public oatpp::DTO {
    DTO_INIT(ExtValDTO, DTO);

    DTO_FIELD(Boolean, prnascsh);
    DTO_FIELD(Boolean, prnashp);
    DTO_FIELD(Boolean, valhphpr);
    DTO_FIELD(Boolean, prnrestkt);
    DTO_FIELD(Boolean, tkfframnt);
    DTO_FIELD(Boolean, tktred);
    DTO_FIELD(Boolean, secencfg);
    DTO_FIELD(UInt32, cashTktRecExp);
    DTO_FIELD(UInt32, resTktdefExp);
    DTO_FIELD(UInt32, assetNbr);
};

class TicketDTO : public  oatpp::DTO {
    DTO_INIT(TicketDTO, DTO);

    DTO_FIELD(UInt32, hostID);
    DTO_FIELD(UInt32, expiration);
    DTO_FIELD(String, location);
    DTO_FIELD(String, addr1);
    DTO_FIELD(String, addr2);
    DTO_FIELD(String, resTktTitle);
    DTO_FIELD(String, debTktTit);
};

class GameDTO : public  oatpp::DTO {
    DTO_INIT(GameDTO, DTO);

    DTO_FIELD(UInt32, gameNbr);
    DTO_FIELD(String, gameID);
    DTO_FIELD(UInt32, denomination);
    DTO_FIELD(String, aditionalGameID);
    DTO_FIELD(UInt32, maxBet);
    DTO_FIELD(String, payTableID);
    DTO_FIELD(String, basePercent);
    DTO_FIELD(String, name);
    DTO_FIELD(String, payTableName);
};

class TitoDTO : public  oatpp::DTO {
    DTO_INIT(TitoDTO, DTO);

    DTO_FIELD(UInt32, type);
    DTO_FIELD(Boolean, none);
    DTO_FIELD(Boolean, secenh);
    DTO_FIELD(Boolean, system);
    DTO_FIELD(Boolean, tktRed);
    DTO_FIELD(Boolean, valext);
    DTO_FIELD(Boolean, tkttotdtcc);
};

class AftDTO : public  oatpp::DTO {
    DTO_INIT(AftDTO, DTO);

    DTO_FIELD(Boolean, enabled);
    DTO_FIELD(Boolean, bonus);
    DTO_FIELD(UInt32, hapctrl);
    DTO_FIELD(Boolean, cotohost);
    DTO_FIELD(UInt32, cotohostm);
    DTO_FIELD(Boolean, prnasrcpt);
    DTO_FIELD(Boolean, partrxtoh);
    DTO_FIELD(Boolean, custtktd);
    DTO_FIELD(Boolean, bonustrx);
    DTO_FIELD(Boolean, debittrx);
};

class LimitsDTO : public  oatpp::DTO {
    DTO_INIT(LimitsDTO, DTO);
    DTO_FIELD(UInt32, maxcashtrx);
};


class GameMachineDTO : public  oatpp::DTO {
    DTO_INIT(GameMachineDTO, DTO);

    DTO_FIELD(String, serialnbr);
    DTO_FIELD(Boolean, jkptmult);
    DTO_FIELD(Boolean, lgcybonus);
    DTO_FIELD(Boolean, tournmt);
    DTO_FIELD(UInt32, woncredmet);
    DTO_FIELD(Boolean, cmpauth);
    DTO_FIELD(Boolean, multdeno);
    DTO_FIELD(Boolean, rtesup);
    DTO_FIELD(Boolean, multsasprgsv);
    DTO_FIELD(Boolean, extmeters);
    DTO_FIELD(UInt32, batimeout);
    DTO_FIELD(UInt32, denom);

    DTO_FIELD(Object<ExtValDTO>, extval);
    DTO_FIELD(Object<TicketDTO>, ticketData);
    DTO_FIELD(List<Object<GameDTO>>, games);
};

class SASDTO : public  oatpp::DTO {
    DTO_INIT(SASDTO, DTO);

    DTO_FIELD(UInt8, id); 
    DTO_FIELD(UInt8, nc);
    DTO_FIELD(UInt8, lgcyhp);
};

class ConfigDTO : public  oatpp::DTO {
    DTO_INIT(ConfigDTO, DTO);

    DTO_FIELD(UInt32, seport);
    DTO_FIELD(UInt32, dbgport);
    DTO_FIELD(Object<SASDTO>, sas);
    DTO_FIELD(Object<GameMachineDTO>, gm);
    DTO_FIELD(Object<TitoDTO>, tito);
    DTO_FIELD(Object<AftDTO>, aft);
    DTO_FIELD(Object<LimitsDTO>, limits);
};


#include OATPP_CODEGEN_END(DTO)

#endif // __CONFIGURATIONDTO_H__