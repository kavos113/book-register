#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include "Book.h"

class Parser
{
public:

    static Book Parse(const std::string& xml);

    static std::array<std::wstring, 2> GetTitle(const xercesc_3_3::DOMDocument *doc);
    static std::array<std::wstring, 2> GetAltTitle(const xercesc_3_3::DOMDocument *doc);
    static std::array<std::wstring, 2> GetSeries(const xercesc_3_3::DOMDocument *doc);
    static std::wstring GetCreators(const xercesc_3_3::DOMDocument *doc);
    static std::wstring GetPublisher(const xercesc_3_3::DOMDocument *doc);
    static std::wstring GetDate(const xercesc_3_3::DOMDocument *doc);
    static unsigned int GetPrice(const xercesc_3_3::DOMDocument *doc);
    static std::wstring GetPages(const xercesc_3_3::DOMDocument *doc);
    static double GetNdc(const xercesc_3_3::DOMDocument *doc);
    static std::wstring GetUrl(const xercesc_3_3::DOMDocument *doc);

    static void Initialize()
    {
        try
        {
            xercesc_3_3::XMLPlatformUtils::Initialize();
        }
        catch (const xercesc_3_3::XMLException& e)
        {
            std::cerr << "XMLPlatformUtils::Initialize() failed: " << xercesc_3_3::XMLString::transcode(e.getMessage())  << std::endl;
            return;
        }

        isInitialized = true;
    }

    static void Terminate()
    {
        if (isInitialized)
        {
            xercesc_3_3::XMLPlatformUtils::Terminate();
            isInitialized = false;
        }
    }

private:
    static bool isInitialized;
};



#endif //PARSER_H
