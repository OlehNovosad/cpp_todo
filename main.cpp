#include "inc/todo.h"
#include <iostream>
#include <string>
#include <numeric>

void print_usage(const char* prog)
{
    std::cerr << "Usage:\n"
        << "  " << prog << " add <task>\n"
        << "  " << prog << " done <task_id>\n"
        << "  " << prog << " delete <task_id>\n";
}

int check_cmd(std::string const& cmd)
{
    if (std::strcmp(cmd.c_str(), "add") == 0)
    {
        return static_cast<int>(TASK_CMD::ADD);
    }
    if (std::strcmp(cmd.c_str(), "remove") == 0)
    {
        return static_cast<int>(TASK_CMD::REMOVE);
    }
    if (std::strcmp(cmd.c_str(), "done") == 0)
    {
        return static_cast<int>(TASK_CMD::DONE);
    }
    if (std::strcmp(cmd.c_str(), "list") == 0)
    {
        return static_cast<int>(TASK_CMD::LIST);
    }

    return static_cast<int>(TASK_CMD::UNKNOWN);
}

int main(const int argc, char** argv)
{
    const ToDo todo;

    switch (const int cmd = check_cmd(argv[1]))
    {
    case static_cast<int>(TASK_CMD::ADD):
        {
            std::vector<std::string> task;
            task.assign(argv + 2, argv + argc);
            const std::string combined = std::accumulate(task.begin(), task.end(), std::string(""),
                                                         [](const std::string& a, const std::string& b)
                                                         {
                                                             return a + (a.empty() ? "" : " ") + b;
                                                         });
            todo.add(combined);
        }
        break;
    case static_cast<int>(TASK_CMD::REMOVE):
        todo.remove(static_cast<int>(std::strtol(argv[2], nullptr, 10)));
        break;
    case static_cast<int>(TASK_CMD::DONE):
        todo.done(static_cast<int>(std::strtol(argv[2], nullptr, 10)));
        break;
    case static_cast<int>(TASK_CMD::LIST):
        todo.list();
        break;
    default:
        std::cerr << "Unknown command: " << cmd << std::endl;
    }
}
