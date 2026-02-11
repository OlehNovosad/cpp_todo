#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

enum class TASK_CMD
{
	ADD = 0,
	LIST,
	REMOVE,
	DONE,
	UNKNOWN,
};

class ToDo
{
  private:
	sqlite3 *db_ = nullptr;

  public:
	explicit ToDo();
	~ToDo();

	void add(const std::string &task) const;
	void list() const;
	void remove(int id) const;
	void done(int id) const;
	std::string get_tasks_json() const;
};
