#ifndef CURL_HTTP_LIBRARY_H
#define CURL_HTTP_LIBRARY_H

#include <string/string.h>

typedef void CURL;
struct curl_slist;

namespace pax
{
    void HTTPInit();
    void HTTPCleanup();

    typedef long HTTPStatusCode;

    struct HTTPResult
    {
        HTTPStatusCode statusCode;
        string body;
    };

    class HTTPSession
    {
    public:
        explicit HTTPSession(string url);
        explicit HTTPSession(const char *url) : HTTPSession(string(url)) {}
        HTTPSession(const HTTPSession &other) = delete;
        HTTPSession(const HTTPSession &&other) = delete;

        ~HTTPSession();

        void VerifyHost(bool verify);
        void SetBody(string body);
        void SetBody(const char *body)
        {
            SetBody(string(body));
        }

        HTTPResult Get();
        HTTPResult Post();
    private:
        void Perform(bool upload);
        void ParseHeaders();

    private:
        CURL *_curl;
        curl_slist *_headers;

        string _url;
        string _body;

        string _headerString;
        HTTPResult _result;
    };
}

#endif