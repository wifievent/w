#include <cstdio>
#include <string>
#include <cstring>
#include <list>

#include <sqlite3.h>

struct Data_List {
    int argc;
    char** argv;
    char** column;

    static void list_free(std::list<Data_List> data);
};

struct DB_Connect {
    sqlite3* db;
    char* db_name;

    std::list<Data_List> db_select(char* db_name, std::string table, std::list<std::string> column_list);
    static int callback(void* NotUsed, int ac, char** av, char** c);
    int db_insert(char* db_name, std::string table, std::list<std::string> values);
    
};
