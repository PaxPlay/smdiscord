#include "http.h"

#include <assert.h>
#include <cstdio>
#include <cstring>

#include <curl/curl.h>

#include "extension.h"

struct ReadData
{
    pax::string *str;
    size_t pos;
};

static size_t writefunc(char *ptr, size_t size, size_t nmemb, void *userp)
{
    size_t realsize =  size * nmemb;
    auto *str = reinterpret_cast<pax::string *>(userp);

    str->append(ptr, realsize);
    return realsize;
}

static size_t readfunc(char *buffer, size_t size, size_t nitems, void *userp)
{
    auto *data = reinterpret_cast<ReadData *>(userp);
    if (data->pos >= data->str->length())
        return 0;

    size_t realsize = size * nitems;
    if (data->str->length() < data->pos + realsize)
    {
        realsize = data->str->length() - data->pos;
    }

    memcpy(buffer, data->str->c_str() + data->pos, realsize);

    data->pos += realsize;

    return realsize;
}

namespace pax
{
    void HTTPInit()
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    void HTTPCleanup()
    {
        curl_global_cleanup();
    }

    HTTPSession::HTTPSession(string url) : _curl(curl_easy_init()), _headers(nullptr), _url(url)
    {
        assert(_curl);

        curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_result.body);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writefunc);

        curl_easy_setopt(_curl, CURLOPT_HEADERDATA, &_headerString);
        curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, writefunc);
    }

    HTTPSession::~HTTPSession()
    {
        curl_easy_cleanup(_curl);
        if (_headers)
            curl_slist_free_all(_headers);
    }

    /**
     * Set whether the host should be verified. Set to true by default.
     * @param verify        Whether the host should be verified.
     */
    void HTTPSession::VerifyHost(bool verify)
    {
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, static_cast<long>(verify));
    }

    void HTTPSession::SetBody(string body)
    {
        _body = body;
    }

    HTTPResult HTTPSession::Get()
    {
        curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "GET");

        Perform(false);

        return _result;
    }

    HTTPResult HTTPSession::Post()
    {
        curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "POST");

        Perform(true);

        return _result;
    }

    void HTTPSession::Perform(bool upload)
    {
        ReadData data;
        if (_body.length() && upload)
        {
            data.str = &_body;
            data.pos = 0;
            curl_easy_setopt(_curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(_curl, CURLOPT_READDATA, &data);
            curl_easy_setopt(_curl, CURLOPT_READFUNCTION, readfunc);
            curl_easy_setopt(_curl, CURLOPT_INFILESIZE, (curl_off_t)_body.length());

        }


        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);

        CURLcode err = curl_easy_perform(_curl);
        if (err != CURLE_OK)
            smutils->LogMessage(myself, "Couldn't perform request: %s", curl_easy_strerror(err));

        curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &_result.statusCode);
    }

    void HTTPSession::ParseHeaders()
    {

    }
}
