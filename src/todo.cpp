#include "../inc/todo.h"

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sqlite3.h>

void prepare_stmt(sqlite3* db_, const char* sql, sqlite3_stmt*& stmt)
{
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
}

void run_sql_cmd(sqlite3* db_, sqlite3_stmt* stmt)
{
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
}

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
    prepare_stmt(db_, sql, stmt);

    sqlite3_bind_text(stmt, 1, task.c_str(), -1, SQLITE_TRANSIENT);

    run_sql_cmd(db_, stmt);

    sqlite3_finalize(stmt);
}

void ToDo::remove(const int id) const
{
    const char* sql =
        "DELETE FROM tasks WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    prepare_stmt(db_, sql, stmt);

    sqlite3_bind_int(stmt, 1, id);

    run_sql_cmd(db_, stmt);

    sqlite3_finalize(stmt);
}

void ToDo::done(const int id) const
{
    const char* sql =
        "UPDATE tasks SET done = 1 WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    prepare_stmt(db_, sql, stmt);

    sqlite3_bind_int(stmt, 1, id);

    run_sql_cmd(db_, stmt);

    sqlite3_finalize(stmt);
}

void ToDo::list() const
{
    const char* sql =
        "SELECT id, task, done FROM tasks ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;
    prepare_stmt(db_, sql, stmt);

    std::cout << std::left
        << std::setw(6) << "ID"
        << std::setw(50) << "TASK"
        << "DONE\n";
    std::cout << std::string(5 + 50 + 5, '-') << std::endl;

    while (true)
    {
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* task = sqlite3_column_text(stmt, 1);
            int done = sqlite3_column_int(stmt, 2);

            std::cout << std::left
                << std::setw(6) << id
                << std::setw(50) << task
                << (done ? "yes" : "no")
                << "\n";
        }
        else if (rc == SQLITE_DONE)
        {
            break;
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error(sqlite3_errmsg(db_));
        }
    }

    sqlite3_finalize(stmt);
}
