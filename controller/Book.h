#pragma once
#include <string>
#include <vector>

struct Tag
{
    unsigned int id;
    std::wstring name;
};

struct Book
{
    unsigned int id;
    long long isbn;
    std::wstring title;
    std::wstring titleRuby;
    std::wstring altTitle;
    std::wstring altTitleRuby;
    std::wstring series;
    std::wstring seriesRuby;
    std::wstring creators;
    std::wstring publisher;
    std::wstring date;
    unsigned int price;
    std::wstring pages;
    double ndc;
    std::wstring location1;
    std::wstring location2;
    std::wstring url;
    std::vector<Tag> tags;
};
