#include "DiscordBot.h"

DiscordBot::DiscordBot(const char *token) : _token(token)
{

}

DiscordBot::~DiscordBot()
{

}

template <typename T>
void DiscordBot::PerformRequest(const char *endpoint, RequestMethod method, const char *body, void *data, REQUEST_FINISHED<T> onRequestFinished)
{
    struct PerformRequestUserp
    {
        void *data;
        DiscordBot::REQUEST_FINISHED<T> callback;
    };

    auto *userp = new PerformRequestUserp;
    userp->data = data;
    userp->callback = onRequestFinished;

    char url[256] = "https://discordapp.com/api/v6";
    strcat(url, endpoint);

    auto thread = new RequestThread(_token.c_str(), url, method, body, userp, [](long statuscode, const char *body, void *data) -> void {
        switch (statuscode)
        {
            case 200:
                auto *userp = reinterpret_cast<PerformRequestUserp *>(data);

                userp->callback(T(body), userp->data);

                break;

            default:
                smutils->LogError(myself, "Request returned %d.", statuscode);

                userp->callback({}, userp->data);

                break;
        }
    });

    threader->MakeThread(thread);
}
