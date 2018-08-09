#ifndef SM_DISCORD_EXT_REQUESTTHREAD_H
#define SM_DISCORD_EXT_REQUESTTHREAD_H

#include <string/string.h>
#include "extension.h"

enum class RequestMethod
{
    GET,
    POST
};

class RequestThread : public IThread
{
public:
    typedef void (*REQUEST_FINISHED)(long statuscode, const char *body, void *data);

    RequestThread(const char *token, const char *url, RequestMethod method, const char *body, void *data, REQUEST_FINISHED onRequestFinished);

public:
    void RunThread(IThreadHandle *pHandle) override;
    void OnTerminate(IThreadHandle *pHandle, bool cancel) override;

private:
    using string = pax::string;
    string _token;
    string _url;
    RequestMethod _method;
    string _body;
    void *_data;
    REQUEST_FINISHED _callback;
};


#endif // !SM_DISCORD_EXT_REQUESTTHREAD_H
