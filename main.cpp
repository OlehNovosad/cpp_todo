#include "inc/todo.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>

#include <fstream>
#include <sstream>

#include "third_party/httplib.h"

void print_usage(const char *prog)
{
	std::cerr << "Usage:\n"
			  << "  " << prog << " add <task>\n"
			  << "  " << prog << " done <task_id>\n"
			  << "  " << prog << " delete <task_id>\n"
			  << "  " << prog << " server [port]\n";
}

int check_cmd(std::string const &cmd)
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
		if (std::strcmp(cmd.c_str(), "server") == 0)
		{
			return 100;
		}
	}

	return static_cast<int>(TASK_CMD::UNKNOWN);
}

int main(const int argc, char **argv)
{
	ToDo todo;
	httplib::Server svr;

	// Serve HTML
	svr.Get("/", [](const httplib::Request &, httplib::Response &res)
			{
				std::ifstream file("web/index.html");
				if (file.is_open())
				{
					std::stringstream buffer;
					buffer << file.rdbuf();
					res.set_content(buffer.str(), "text/html");
				}
				else
				{
					res.set_content("<html><body>Error: index.html not found</body></html>", "text/html");
				} });

	// API endpoint to get all tasks as JSON
	svr.Get("/api/tasks", [&todo](const httplib::Request &, httplib::Response &res)
			{
				std::string json = todo.get_tasks_json();
				res.set_content(json, "application/json"); });

	// API endpoint to add a task
	svr.Post("/api/tasks", [&todo](const httplib::Request &req, httplib::Response &res)
			 {
				if (req.has_param("task"))
				{
					std::string task = req.get_param_value("task");
					todo.add(task);
					res.set_content(R"({"status":"ok"})", "application/json");
				}
				else
				{
					res.set_content(R"({"status":"error","message":"Missing task parameter"})", "application/json");
				} });

	// API endpoint to mark task as done
	svr.Put("/api/tasks/:id/done", [&todo](const httplib::Request &req, httplib::Response &res)
			{
				std::string id_str = req.path_params.at("id");
				int id = static_cast<int>(std::strtol(id_str.c_str(), nullptr, 10));
				todo.done(id);
				res.set_content(R"({"status":"ok"})", "application/json"); });

	// API endpoint to delete task
	svr.Delete("/api/tasks/:id", [&todo](const httplib::Request &req, httplib::Response &res)
			   {
				   std::string id_str = req.path_params.at("id");
				   int id = static_cast<int>(std::strtol(id_str.c_str(), nullptr, 10));
				   todo.remove(id);
				   res.set_content(R"({"status":"ok"})", "application/json"); });

#if 0
	switch (const int cmd = check_cmd(argv[1]))
	{
	case static_cast<int>(TASK_CMD::ADD):
	{
		std::vector<std::string> task;
		task.assign(argv + 2, argv + argc);
		const std::string combined = std::accumulate(task.begin(), task.end(), std::string(""),
													 [](const std::string &a, const std::string &b)
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
#endif

	std::cout
		<< "Server running on http://localhost:8080\n";
	svr.listen("localhost", 8080);

	return 0;
}
