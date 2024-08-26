#include <stdio.h>
#include "vip_nofalldamage.h"
#include <sstream>
#include "schemasystem/schemasystem.h"

NoFallDamageModule g_NoFallDamageModule;

IUtilsApi* g_pUtils = nullptr;
IVIPApi* g_pVIPCore = nullptr;
IVEngineServer2* engine = nullptr;
CGameEntitySystem* g_pGameEntitySystem = nullptr;
CEntitySystem* g_pEntitySystem = nullptr;
IPlayersApi* g_pPlayers = nullptr;

bool g_bNoFallDamage[64]; 

PLUGIN_EXPOSE(NoFallDamageModule, g_NoFallDamageModule);

bool NoFallDamageModule::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
{
    PLUGIN_SAVEVARS();
    GET_V_IFACE_ANY(GetEngineFactory, g_pSchemaSystem, ISchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, SOURCE2ENGINETOSERVER_INTERFACE_VERSION);
    GET_V_IFACE_CURRENT(GetFileSystemFactory, g_pFullFileSystem, IFileSystem, FILESYSTEM_INTERFACE_VERSION);

    g_SMAPI->AddListener(this, this);
    return true;
}

void OnStartupServer()
{
    g_pGameEntitySystem = g_pUtils->GetCGameEntitySystem();
    g_pEntitySystem = g_pUtils->GetCEntitySystem();
}

bool NoFallDamageModule::Unload(char* error, size_t maxlen)
{
    delete g_pVIPCore;
    delete g_pUtils;
    delete g_pPlayers;
    return true;
}

CGameEntitySystem* GameEntitySystem()
{
    return g_pUtils->GetCGameEntitySystem();
}

bool OnTakeDamage(int iVictimSlot, CTakeDamageInfo& pDamageInfo)
{
    int iSlot = iVictimSlot;

    if (g_bNoFallDamage[iSlot] && (pDamageInfo.m_bitsDamageType & DMG_FALL)) {
        return false;  
    }

    return true; 
}


void OnClientAuthorized(int iSlot, uint64 iSteamID64)
{
    g_pUtils->NextFrame([iSlot]() {
        g_bNoFallDamage[iSlot] = g_pVIPCore->VIP_GetClientFeatureBool(iSlot, "nofalldamage");
        });
}

bool OnToggle(int iSlot, const char* szFeature, VIP_ToggleState eOldStatus, VIP_ToggleState& eNewStatus)
{
    if (strcmp(szFeature, "nofalldamage") == 0) {
        g_bNoFallDamage[iSlot] = (eNewStatus == ENABLED);
    }
    return false;
}

void NoFallDamageModule::AllPluginsLoaded()
{
    char error[64];
    int ret;

    g_pUtils = (IUtilsApi*)g_SMAPI->MetaFactory(Utils_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_SMAPI->Format(error, sizeof(error), "Missing Utils system plugin");
        ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pPlayers = (IPlayersApi*)g_SMAPI->MetaFactory(PLAYERS_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_SMAPI->Format(error, sizeof(error), "Missing Players system plugin");
        ConColorMsg(Color(255, 0, 0, 255), "[%s] %s\n", GetLogTag(), error);
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pVIPCore = (IVIPApi*)g_SMAPI->MetaFactory(VIP_INTERFACE, &ret, NULL);
    if (ret == META_IFACE_FAILED)
    {
        g_pUtils->ErrorLog("[%s] Missing VIP system plugin", GetLogTag());
        std::string sBuffer = "meta unload " + std::to_string(g_PLID);
        engine->ServerCommand(sBuffer.c_str());
        return;
    }

    g_pVIPCore->VIP_RegisterFeature("nofalldamage", VIP_BOOL, TOGGLABLE, nullptr, OnToggle);
    g_pUtils->StartupServer(g_PLID, OnStartupServer);
    g_pPlayers->HookOnClientAuthorized(g_PLID, OnClientAuthorized);
    g_pUtils->HookOnTakeDamagePre(g_PLID, OnTakeDamage);
}

const char* NoFallDamageModule::GetLicense()
{
    return "Public";
}

const char* NoFallDamageModule::GetVersion()
{
    return "1.0";
}

const char* NoFallDamageModule::GetDate()
{
    return __DATE__;
}

const char* NoFallDamageModule::GetLogTag()
{
    return "[VIP-NoFallDamage]";
}

const char* NoFallDamageModule::GetAuthor()
{
    return "ABKAM";
}

const char* NoFallDamageModule::GetDescription()
{
    return "VIP Module to disable fall damage for VIP players";
}

const char* NoFallDamageModule::GetName()
{
    return "[VIP] No Fall Damage";
}

const char* NoFallDamageModule::GetURL()
{
    return "https://discord.com/invite/g798xERK5Y";
}
 