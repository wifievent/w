#pragma once
#include <cstdio>
#include <string>
#include <cstring>
#include <list>
#include <iostream>

#include "sqlite/sqlite3.h"
#include "base/gtrace.h"

struct Data_List {
    int argc;
    char** argv;
    char** column;

    static void list_free(std::list<Data_List> data);
};

struct DB_Connect {
    sqlite3* db;
    std::string db_name;

    DB_Connect(std::string db_name);

    std::list<Data_List> select_query(std::string query);
    int send_query(std::string query);
protected:
    static int callback(void* dl, int ac, char** av, char** c);
};
