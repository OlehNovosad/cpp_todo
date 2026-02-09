#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

struct Task
{
    int id;
    bool done;
    std::string task;
};

class ToDo
{
private:
    sqlite3* db_ = nullptr;
    std::vector<Task> todo_vec;

public:
    explicit ToDo();
    ~ToDo();

    void add(const std::string& task) const;
    void list();
    void remove();
    void done();
};
