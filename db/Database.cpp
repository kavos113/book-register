#include "Database.h"

#include <iostream>

#include "../util.h"

sqlite3* Database::db = nullptr;

void Database::Connect()
{
    int res = sqlite3_open("book.db", &db);
    if (res != SQLITE_OK)
    {
        std::cerr << "sqlite3_open() failed: " << sqlite3_errmsg(db) << std::endl;
        exit(-1);
    }

    char* err = nullptr;

    res = sqlite3_exec(db, CREATE_TABLE, nullptr, nullptr, &err);
    if (res != SQLITE_OK)
    {
        std::cerr << "sqlite3_exec() failed: " << err << std::endl;
        sqlite3_close(db);
        sqlite3_free(err);
        exit(-1);
    }
}

void Database::Disconnect()
{
    sqlite3_close(db);
}

void Database::Insert(const Book &book)
{
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db, INSERT, -1, &stmt, nullptr);
    if (res != SQLITE_OK)
    {
        std::cerr << "sqlite3_prepare_v2() failed: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_int64(stmt, 1, book.isbn);
    sqlite3_bind_text(stmt, 2, GetCString(book.title).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, GetCString(book.titleRuby).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, GetCString(book.altTitle).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, GetCString(book.altTitleRuby).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, GetCString(book.series).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, GetCString(book.seriesRuby).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, GetCString(book.creators).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, GetCString(book.publisher).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10, GetCString(book.date).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 11, book.price);
    sqlite3_bind_text(stmt, 12, GetCString(book.pages).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 13, book.ndc);
    sqlite3_bind_text(stmt, 14, GetCString(book.location1).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 15, GetCString(book.location2).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 16, GetCString(book.url).c_str(), -1, SQLITE_TRANSIENT);

    res = sqlite3_step(stmt);
    if (res != SQLITE_DONE)
    {
        std::cerr << "sqlite3_step() failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

std::vector<Book> Database::SelectAll()
{
    std::vector<Book> books;
    char* err = nullptr;
    int res = sqlite3_exec(db, SELECT_ALL, MultiSelectCallback, &books, &err);
    if (res != SQLITE_OK)
    {
        std::cerr << "sqlite3_exec() failed: " << err << std::endl;
        sqlite3_free(err);
    }

    return books;
}

int Database::MultiSelectCallback(void *data, int num_col, char **col, char **col_name)
{
    std::vector<Book> *books = static_cast<std::vector<Book>*>(data);
    Book book = {};

    for (int i = 0; i < num_col; i++)
    {
        if (strcmp(col_name[i], "id") == 0) {
            book.id = atoi(col[i]);
        } else if (strcmp(col_name[i], "isbn") == 0) {
            book.isbn = atoll(col[i]);
        } else if (strcmp(col_name[i], "title") == 0) {
            book.title = GetWString(col[i]);
        } else if (strcmp(col_name[i], "title_ruby") == 0) {
            book.titleRuby = GetWString(col[i]);
        } else if (strcmp(col_name[i], "alt_title") == 0) {
            book.altTitle = GetWString(col[i]);
        } else if (strcmp(col_name[i], "alt_title_ruby") == 0) {
            book.altTitleRuby = GetWString(col[i]);
        } else if (strcmp(col_name[i], "series") == 0) {
            book.series = GetWString(col[i]);
        } else if (strcmp(col_name[i], "series_ruby") == 0) {
            book.seriesRuby = GetWString(col[i]);
        } else if (strcmp(col_name[i], "creators") == 0) {
            book.creators = GetWString(col[i]);
        } else if (strcmp(col_name[i], "publisher") == 0) {
            book.publisher = GetWString(col[i]);
        } else if (strcmp(col_name[i], "date") == 0) {
            book.date = GetWString(col[i]);
        } else if (strcmp(col_name[i], "price") == 0) {
            book.price = atoi(col[i]);
        } else if (strcmp(col_name[i], "pages") == 0) {
            book.pages = GetWString(col[i]);
        } else if (strcmp(col_name[i], "ndc") == 0) {
            book.ndc = atof(col[i]);
        } else if (strcmp(col_name[i], "location1") == 0) {
            book.location1 = GetWString(col[i]);
        } else if (strcmp(col_name[i], "location2") == 0) {
            book.location2 = GetWString(col[i]);
        } else if (strcmp(col_name[i], "url") == 0) {
            book.url = GetWString(col[i]);
        }
    }

    books->push_back(book);

    return 0;
}
