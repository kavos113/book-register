#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <curl/curl.h>

class Client
{
public:
    static void Fetch(const std::string& isbn)
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "curl_easy_init() failed." << std::endl;
            return;
        }

        std::string url = URL + isbn;
        std::string res;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(result) << std::endl;
            return;
        }

        std::cout << result << std::endl;

        curl_easy_cleanup(curl);
    }
private:


    static constexpr const char* URL = "https://ndlsearch.ndl.go.jp/api/sru?operation=searchRetrieve&version=1.2&recordSchema=dcndl&onlyBib=true&recordPacking=xml&query=isbn=";
};
