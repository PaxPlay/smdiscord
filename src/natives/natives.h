#ifndef SM_DISCORD_EXT_NATIVES_H
#define SM_DISCORD_EXT_NATIVES_H

#include "extension.h"

#define NATIVE(name) cell_t name(IPluginContext *pContext, const cell_t *params)

NATIVE(DiscordBot_DiscordBot);

const sp_nativeinfo_t Natives[] =
{
    {"DiscordBot.DiscordBot",       DiscordBot_DiscordBot},
    {nullptr,                       nullptr}
};


#endif // !SM_DISCORD_EXT_NATIVES_H
