#ifndef __ENGINE_DEFS_H__
#define __ENGINE_DEFS_H__

namespace EngineAPI
{
    enum class engineComState
    {
        comIdle,
        comSend,
        comWaitAnsw,
        comProcRx
    };

    enum class engineOLStatus
    {
        osOffline,
        osOnLineWithErr,
        osOnLineOK
    };

    enum class engFSMState
    {
        esIdle,
        esCmd,
    };

    enum class engProcState
    {
        epsInit,
        epsRun
    };
}

#endif // __ENGINE_DEFS_H__

