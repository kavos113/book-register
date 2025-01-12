#include "Parser.h"

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <array>
#include <print>
#include <regex>
#include <sstream>

#include "../util.h"

bool Parser::isInitialized = false;

Book Parser::Parse(const std::string &xml)
{
    if (!isInitialized)
    {
        Initialize();
    }

    xercesc_3_3::XercesDOMParser parser;
    parser.setValidationScheme(xercesc_3_3::XercesDOMParser::Val_Always);

    xercesc_3_3::ErrorHandler* errHandler = new xercesc_3_3::HandlerBase();
    parser.setErrorHandler(errHandler);

    xercesc_3_3::MemBufInputSource source(reinterpret_cast<const XMLByte *>(xml.c_str()), xml.size(), "xml");

    try
    {
        parser.parse(source);

        xercesc_3_3::DOMDocument* doc = parser.getDocument();
        if (!doc)
        {
            return {};
        }

        auto title = GetTitle(doc);
        auto altTitle = GetAltTitle(doc);
        auto series = GetSeries(doc);
        auto creators = GetCreators(doc);
        auto publisher = GetPublisher(doc);
        auto date = GetDate(doc);
        auto price = GetPrice(doc);
        auto pages = GetPages(doc);
        auto ndc = GetNdc(doc);
        auto url = GetUrl(doc);

        return {
            0,
            9784764904057,
            title[0],
            title[1],
            altTitle[0],
            altTitle[1],
            series[0],
            series[1],
            creators,
            publisher,
            date,
            price,
            pages,
            ndc,
            L"",
            L"",
            url,
            {}
        };
    }
    catch (const xercesc_3_3::XMLException& e)
    {
        std::cerr << "XMLException: " << xercesc_3_3::XMLString::transcode(e.getMessage()) << std::endl;
    }
    catch (const xercesc_3_3::DOMException& e)
    {
        std::cerr << "DOMException: " << xercesc_3_3::XMLString::transcode(e.getMessage()) << std::endl;
    }
    catch (...)
    {
        std::cerr << "An error occurred." << std::endl;
    }

    delete errHandler;

    return {};
}

std::array<std::wstring, 2> Parser::GetTitle(const xercesc_3_3::DOMDocument *doc)
{
    std::wstring title;
    std::wstring transcription;

    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dc:title"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::ELEMENT_NODE)
    {
        child = child->getNextSibling();
    }

    if (!xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:Description")))
    {
        return {};
    }

    child = child->getFirstChild();
    while (child)
    {
        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:value")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                title = GetWString(titleValue);
            }
        }

        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("dcndl:transcription")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                transcription = GetWString(titleValue);
            }
        }
        child = child->getNextSibling();
    }

    return { title, transcription };
}

std::array<std::wstring, 2> Parser::GetAltTitle(const xercesc_3_3::DOMDocument *doc)
{
    std::wstring title;
    std::wstring transcription;

    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcndl:alternative"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::ELEMENT_NODE)
    {
        child = child->getNextSibling();
    }

    if (!xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:Description")))
    {
        return {};
    }

    child = child->getFirstChild();
    while (child)
    {
        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:value")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                title = GetWString(titleValue);
            }
        }

        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("dcndl:transcription")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                transcription = GetWString(titleValue);
            }
        }
        child = child->getNextSibling();
    }

    return { title, transcription };
}

std::array<std::wstring, 2> Parser::GetSeries(const xercesc_3_3::DOMDocument *doc)
{
    std::wstring title;
    std::wstring transcription;

    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcndl:seriesTitle"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::ELEMENT_NODE)
    {
        child = child->getNextSibling();
    }

    if (!xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:Description")))
    {
        return {};
    }

    child = child->getFirstChild();
    while (child)
    {
        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("rdf:value")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                title = GetWString(titleValue);
            }
        }

        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("dcndl:transcription")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* titleValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                transcription = GetWString(titleValue);
            }
        }
        child = child->getNextSibling();
    }

    return { title, transcription };
}

std::wstring Parser::GetCreators(const xercesc_3_3::DOMDocument *doc)
{
    std::wstringstream wss;

    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dc:creator"));
    if (!list)
    {
        return {};
    }

    for (XMLSize_t i = 0; i < list->getLength(); i++)
    {
        xercesc_3_3::DOMNode* node = list->item(i);
        if (!node)
        {
            continue;
        }

        xercesc_3_3::DOMNode* child = node->getFirstChild();
        if (!child)
        {
            continue;
        }

        while (child->getNodeType() != xercesc_3_3::DOMNode::TEXT_NODE)
        {
            child = child->getNextSibling();
        }

        char* creator = xercesc_3_3::XMLString::transcode(child->getNodeValue());
        if (i > 0)
        {
            wss << L", ";
        }
        std::wstring creatorStr = GetWString(creator);
        if (i != list->getLength() - 1) // remove null terminator
        {
            creatorStr.pop_back();
        }
        wss << creatorStr;
    }

    return wss.str();
}

std::wstring Parser::GetPublisher(const xercesc_3_3::DOMDocument *doc)
{
    std::wstring publisher;

    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcterms:publisher"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::ELEMENT_NODE)
    {
        child = child->getNextSibling();
    }

    if (!xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("foaf:Agent")))
    {
        return {};
    }

    child = child->getFirstChild();
    while (child)
    {
        if (child->getNodeType() == xercesc_3_3::DOMNode::ELEMENT_NODE
            && xercesc_3_3::XMLString::equals(child->getNodeName(), xercesc_3_3::XMLString::transcode("foaf:name")))
        {
            xercesc_3_3::DOMNode* grandChild = child->getFirstChild();
            if (grandChild && grandChild->getNodeType() == xercesc_3_3::DOMNode::TEXT_NODE)
            {
                char* publisherValue = xercesc_3_3::XMLString::transcode(grandChild->getNodeValue());
                publisher = GetWString(publisherValue);
                break;
            }
        }
        child = child->getNextSibling();
    }

    return publisher;
}

std::wstring Parser::GetDate(const xercesc_3_3::DOMDocument *doc)
{
    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcterms:date"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::TEXT_NODE)
    {
        child = child->getNextSibling();
    }

    std::wstring date = GetWString(xercesc_3_3::XMLString::transcode(child->getNodeValue()));
    return date;
}

unsigned int Parser::GetPrice(const xercesc_3_3::DOMDocument *doc)
{
    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcndl:price"));
    if (!list)
    {
        return 0;
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return 0;
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return 0;
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::TEXT_NODE)
    {
        child = child->getNextSibling();
    }

    std::string price = xercesc_3_3::XMLString::transcode(child->getNodeValue());
    std::smatch match;
    std::regex_search(price, match, std::regex(R"(\d+)"));
    return std::stoi(match[0].str());
}

std::wstring Parser::GetPages(const xercesc_3_3::DOMDocument *doc)
{
    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcterms:extent"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNode* child = node->getFirstChild();
    if (!child)
    {
        return {};
    }

    while (child->getNodeType() != xercesc_3_3::DOMNode::TEXT_NODE)
    {
        child = child->getNextSibling();
    }

    return GetWString(xercesc_3_3::XMLString::transcode(child->getNodeValue()));
}

double Parser::GetNdc(const xercesc_3_3::DOMDocument *doc)
{
    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcterms:subject"));
    if (!list)
    {
        return 0.0;
    }

    for (XMLSize_t i = 0; i < list->getLength(); i++)
    {
        xercesc_3_3::DOMNamedNodeMap* attributes = list->item(i)->getAttributes();
        if (!attributes || attributes->getLength() == 0)
        {
            continue;
        }

        for (int j = 0; j < attributes->getLength(); ++j)
        {
            xercesc_3_3::DOMNode* attribute = attributes->item(j);

            std::string name = xercesc_3_3::XMLString::transcode(attribute->getNodeName());
            std::string value = xercesc_3_3::XMLString::transcode(attribute->getNodeValue());

            if (name != "rdf:resource")
            {
                continue;
            }

            if (value.find("http://id.ndl.go.jp/class/ndc9/") != std::string::npos)
            {
                std::smatch match;
                std::regex_search(value, match, std::regex(R"(\d+\.\d+)"));
                return std::stod(match[0].str());
            }

        }
    }

    return 0.0;
}

std::wstring Parser::GetUrl(const xercesc_3_3::DOMDocument *doc)
{
    xercesc_3_3::DOMNodeList* list = doc->getElementsByTagName(xercesc_3_3::XMLString::transcode("dcndl:BibAdminResource"));
    if (!list)
    {
        return {};
    }

    xercesc_3_3::DOMNode* node = list->item(0);
    if (!node)
    {
        return {};
    }

    xercesc_3_3::DOMNamedNodeMap* attributes = node->getAttributes();
    if (!attributes)
    {
        return {};
    }

    for (int i = 0; i < attributes->getLength(); ++i)
    {
        xercesc_3_3::DOMNode* attribute = attributes->item(i);

        std::string name = xercesc_3_3::XMLString::transcode(attribute->getNodeName());
        std::string value = xercesc_3_3::XMLString::transcode(attribute->getNodeValue());

        if (name != "rdf:about")
        {
            continue;
        }

        std::wstring url = GetWString(value.c_str());
        return url;
    }

    return {};
}
