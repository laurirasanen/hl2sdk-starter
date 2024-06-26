#include "ServerPlugin.h"
#include "tier1.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ServerPlugin g_serverPlugin = ServerPlugin {};
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(
    ServerPlugin,
    IServerPluginCallbacks,
    INTERFACEVERSION_ISERVERPLUGINCALLBACKS,
    g_serverPlugin
);

bool ServerPlugin::Load(CreateInterfaceFn engineFactory, CreateInterfaceFn serverFactory)
{
    ConnectTier1Libraries(&engineFactory, 1);

    m_engineInterfaces = EngineInterfaces(engineFactory, serverFactory);
    if (!m_engineInterfaces.IsValid())
    {
        Msg("[ServerPlugin] Scientist: \"Do you suspect some alien subterfuge "
            "behind this failure?\"\n");
        Msg("[ServerPlugin] Failed to get interfaces from the engine\n");
        m_engineInterfaces.Print();
        return false;
    }

    MathLib_Init(2.2f, 2.2f, 0.0f, 2);
    ConVar_Register(0);

    Msg("[ServerPlugin] Scientist: \"Testing...testing... Everything seems to be in order.\"\n");
    return true;
}

void ServerPlugin::Unload()
{
    Msg("[ServerPlugin] Unload\n");

    if (m_gameEventsHooked)
    {
        m_engineInterfaces.gameEventManager->RemoveListener(this);
    }

    ConVar_Unregister();
    DisconnectTier1Libraries();
}

void ServerPlugin::Pause()
{
    Msg("[ServerPlugin] Pause\n");
}

void ServerPlugin::UnPause()
{
    Msg("[ServerPlugin] UnPause\n");
}

const char* ServerPlugin::GetPluginDescription()
{
    return "Server Plugin Example";
}

void ServerPlugin::LevelInit(const char* pMapName)
{
    Msg("[ServerPlugin] LevelInit: \"%s\"\n", pMapName);
    ListenToGameEvent("player_hurt");
}

void ServerPlugin::ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
}

void ServerPlugin::GameFrame(bool simulating)
{
}

void ServerPlugin::LevelShutdown()
{
    Msg("[ServerPlugin] LevelShutdown\n");
}

void ServerPlugin::ClientPutInServer(edict_t* pEntity, const char* playername)
{
    Msg("[ServerPlugin] ClientPutInServer: \"%s\"\n", playername);
}

void ServerPlugin::SetCommandClient(int index)
{
    m_iClientCommandIndex = index;
}

void ServerPlugin::ClientActive(edict_t* pEntity)
{
}

void ServerPlugin::ClientDisconnect(edict_t* pEntity)
{
}

void ServerPlugin::ClientSettingsChanged(edict_t* pEdict)
{
}

PLUGIN_RESULT ServerPlugin::ClientConnect(
    bool*       bAllowConnect,
    edict_t*    pEntity,
    const char* pszName,
    const char* pszAddress,
    char*       reject,
    int         maxrejectlen
)
{
    Msg("[ServerPlugin] ClientConnect: \"%s\" from \"%s\"\n", pszName, pszAddress);
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT ServerPlugin::ClientCommand(edict_t* pEntity, const CCommand& args)
{
    if (!pEntity || pEntity->IsFree())
    {
        return PLUGIN_CONTINUE;
    }

    Msg("[ServerPlugin] ClientCommand: \"%s\"\n", args.GetCommandString());
    return PLUGIN_CONTINUE;
}

PLUGIN_RESULT ServerPlugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
    Msg("[ServerPlugin] NetworkIDValidated: \"%s\" with ID \"%s\"\n", pszUserName, pszNetworkID);
    return PLUGIN_CONTINUE;
}

void ServerPlugin::OnQueryCvarValueFinished(
    QueryCvarCookie_t     iCookie,
    edict_t*              pPlayerEntity,
    EQueryCvarValueStatus eStatus,
    const char*           pCvarName,
    const char*           pCvarValue
)
{
}

void ServerPlugin::FireGameEvent(IGameEvent* event)
{
    Msg("[ServerPlugin] FireGameEvent: \"%s\"\n", event->GetName());
}

void ServerPlugin::ListenToGameEvent(const char* name)
{
    m_engineInterfaces.gameEventManager->AddListener(this, name, true);
    m_gameEventsHooked = true;
}

CON_COMMAND(example, "Example command")
{
    Msg("[ServerPlugin] Scientist: \"Do you know who ate all the donuts?\"\n");
}
