#ifndef SM_DISCORD_EXT_DISCORDBOT_H
#define SM_DISCORD_EXT_DISCORDBOT_H

#include "extension.h"
#include "RequestThread.h"

#include <string/string.h>

class DiscordBot
{
public:
    explicit DiscordBot(const char *token);
    ~DiscordBot();

public:
    template <typename T>
    using REQUEST_FINISHED = void (*)(T obj, void *data);

private:
    template <typename T>
    void PerformRequest(const char *endpoint, RequestMethod method, const char *body, void *data, REQUEST_FINISHED<T> onRequestFinished);

private:
    pax::string _token;
};

#endif // !SM_DISCORD_EXT_DISCORDBOT_H
