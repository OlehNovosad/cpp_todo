#include "../inc/todo.h"

#include <iostream>
#include <ostream>
#include <sqlite3.h>

ToDo::ToDo()
{
    int res = sqlite3_open("todo.db", &db_);
    if (res != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db_) << std::endl;
    }

    const char* sql =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "task TEXT NOT NULL, "
        "done INTEGER NOT NULL DEFAULT 0"
        ");";

    char* errMsg = nullptr;
    res = sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg);
    if (res != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

ToDo::~ToDo()
{
    if (db_)
    {
        sqlite3_close(db_);
    }
}

void ToDo::add(const std::string& task) const
{
    const char* sql =
        "INSERT INTO tasks (task, done) VALUES (?, 0);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }

    sqlite3_bind_text(stmt, 1, task.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error(sqlite3_errmsg(db_));
    }

    sqlite3_finalize(stmt);
}
