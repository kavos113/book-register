#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

#include "Book.h"

class Database
{
public:
    static void Connect();
    static void Disconnect();

    static void Insert(const Book& book);
    static std::vector<Book> SelectAll();
private:
    static sqlite3* db;

    static int MultiSelectCallback(void* data, int num_col, char** col, char** col_name);

    static constexpr char CREATE_TABLE[] = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            isbn INTEGER NOT NULL,
            title TEXT NOT NULL,
            title_ruby TEXT NOT NULL,
            alt_title TEXT NOT NULL,
            alt_title_ruby TEXT NOT NULL,
            series TEXT NOT NULL,
            series_ruby TEXT NOT NULL,
            creators TEXT NOT NULL,
            publisher TEXT NOT NULL,
            date TEXT NOT NULL,
            price INTEGER NOT NULL,
            pages TEXT NOT NULL,
            ndc REAL NOT NULL,
            location1 TEXT NOT NULL,
            location2 TEXT NOT NULL,
            url TEXT NOT NULL
        );
    )";

    static constexpr char INSERT[] = R"(
        INSERT INTO books (
            isbn,
            title,
            title_ruby,
            alt_title,
            alt_title_ruby,
            series,
            series_ruby,
            creators,
            publisher,
            date,
            price,
            pages,
            ndc,
            location1,
            location2,
            url
        ) VALUES (? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,? ,?);
    )";

    static constexpr char SELECT_ALL[] = "SELECT * FROM books;";
};



#endif //DATABASE_H
