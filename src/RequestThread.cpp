#include "RequestThread.h"
#include "http/http.h"

RequestThread::RequestThread(const char *token, const char *url, RequestMethod method, const char *body, void *data,
                             RequestThread::REQUEST_FINISHED onRequestFinished) :
        _token(token),
        _url(url),
        _method(method),
        _body(body),
        _data(data),
        _callback(onRequestFinished)
{

}

void RequestThread::RunThread(IThreadHandle *pHandle)
{
    pax::HTTPSession session(_url);

    if (_method != RequestMethod::GET && _body.length())
        session.SetBody(_body);

    pax::HTTPResult res;

    switch (_method)
    {
        case RequestMethod::GET:
            res = session.Get();
            break;
        case RequestMethod::POST:
            res = session.Post();
            break;
    }

    _callback(res.statusCode, res.body.c_str(), _data);
}

void RequestThread::OnTerminate(IThreadHandle *pHandle, bool cancel)
{
    delete this;
}
