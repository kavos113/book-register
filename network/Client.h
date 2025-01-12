#pragma once
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <curl/curl.h>

class Client
{
public:
    static std::string Fetch(const std::string& isbn)
    {
        auto now = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
        if (diff.count() < 2000)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000 - diff.count()));
        }

        CURL* curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "curl_easy_init() failed." << std::endl;
            return {};
        }

        std::string url = URL + isbn;
        std::string res;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnRecieve);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result) << std::endl;
            return {};
        }

        curl_easy_cleanup(curl);

        return res;
    }
private:
    static size_t OnRecieve(char* ptr, size_t size, size_t nmemb, std::string* stream)
    {
        size_t realsize = size * nmemb;
        stream->append(ptr, realsize);
        return realsize;
    }

    static std::chrono::time_point<std::chrono::system_clock> last;
    static constexpr const char* URL = "https://ndlsearch.ndl.go.jp/api/sru?operation=searchRetrieve&version=1.2&recordSchema=dcndl&onlyBib=true&recordPacking=xml&query=isbn=";
};
