#include "db-connect.h"

void Data_List::list_free(std::list<Data_List> data) {
    std::list<Data_List>::iterator iter;
    for(iter = data.begin(); iter != data.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            free(iter->argv[i]);
            free(iter->column[i]);
        }
        free(iter->argv);
        free(iter->column);
    }
}

std::list<Data_List> DB_Connect::db_select(char* db_name, std::string table, std::list<std::string> column_list) {
    char * err_msg = 0;
    int rc = sqlite3_open(db_name, &db);

    std::list<Data_List> dl;

    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return dl;
    }

    std::string columns("");
    std::list<std::string>::iterator iter;
    for(iter = column_list.begin(); iter != column_list.end(); ++iter) {
        columns += *iter;
        columns += ", ";
    }

    columns.pop_back();
    columns.pop_back();

    char* sql = (char*)malloc(columns.size() + table.size() + 14);
    std::sprintf(sql, "SELECT %s FROM %s", columns.data(), table.data());
    rc = sqlite3_exec(db, sql, callback, &dl, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return dl;
    }
    sqlite3_close(db);
    return dl;
}

int DB_Connect::callback(void* NotUsed, int ac, char** av, char** c) {
    Data_List data;
    data.argc = ac;
    data.argv = (char**)malloc(ac);
    data.column = (char**)malloc(ac);
    for(int i = 0; i < ac; ++i) {
        data.argv[i] = (char*)malloc(sizeof(av[i]));
        data.column[i] = (char*)malloc(sizeof(c[i]));
        strcpy(data.argv[i], av[i]);
        strcpy(data.column[i], c[i]);
    }

    std::list<Data_List>* dl = (std::list<Data_List>*)NotUsed;
    dl->push_back(data);
    return 0;
}

int main(void)
{
    DB_Connect db_connect;

    std::list<Data_List> dl;
    std::list<std::string> columns;
    columns.push_back("*");
    dl = db_connect.db_select("test.db", "Cars", columns);

    std::list<Data_List>::iterator iter;
    for(iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);

    return 0;
}
