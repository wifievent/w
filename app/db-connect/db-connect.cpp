#include "db-connect.h"

//  동적할당한 메모리 해제함수
void Data_List::list_free(std::list<Data_List> data) {
    for(std::list<Data_List>::iterator iter = data.begin(); iter != data.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            free(iter->argv[i]);
            free(iter->column[i]);
        }
        free(iter->argv);
        free(iter->column);
    }
}

//  db_select 함수
std::list<Data_List> DB_Connect::db_select(char* db_name, std::string table, std::list<std::string> column_list) {
    /*
    db_name: db파일 이름
    table: 테이블 명
    column_list: 출력할 컬럼 리스트
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name, &db);

    std::list<Data_List> dl;    //  select 결과 저장 list

    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return dl;
    }

    //  columns 문자열 만들기
    std::string columns("");
    for(std::list<std::string>::iterator iter = column_list.begin(); iter != column_list.end(); ++iter) {
        columns += *iter;
        columns += ", ";
    }

    //  끝의 ", " 제거
    columns.pop_back();
    columns.pop_back();

    //  sql문 만들기
    char* sql = (char*)malloc(columns.size() + table.size() + 14);
    std::sprintf(sql, "SELECT %s FROM %s", columns.data(), table.data());
    
    //  쿼리 날려서 결과 얻기
    rc = sqlite3_exec(db, sql, callback, &dl, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return dl;
    }

    //  자원 해제
    free(sql);
    sqlite3_close(db);
    return dl;
}

//  select에서 사용하는 callback함수
int DB_Connect::callback(void* dl, int ac, char** av, char** c) {
    //  결과를 저장할 변수
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
    //  결과를 저장
    std::list<Data_List>* data_list = (std::list<Data_List>*)dl;
    data_list->push_back(data);
    return 0;
}

//  db_insert 함수
int DB_Connect::db_insert(char* db_name, std::string table, std::list<std::string> values) {
    /*
    db_name: db파일 이름
    table: 테이블 명
    values: 저장할 value
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name, &db);

    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return -1;
    }

    //  values 문자열 만들기
    std::string v("");
    for(std::list<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter) {
        v += *iter;
        v += ", ";
    }

    //  끝의 ", " 제거
    v.pop_back();
    v.pop_back();

    //  sql문 만들기
    char* sql = (char*)malloc(22 + table.size() + v.size());
    sprintf(sql, "INSERT INTO %s VALUES(%s)", table.data(), v.data());
    
    //  쿼리 날리기
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    //  자원해제
    free(sql);
    sqlite3_close(db);
    return 0;
}

//  db_update 함수
int DB_Connect::db_update(char* db_name, std::string table, std::map<std::string, std::string> update_values, std::string condition) {
    /*
    db_name: db파일 이름
    table: 테이블 명
    update_values: 변경할 (column, value)쌍
    condition: where문에 올 조건식
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name, &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return -1;
    }

    //  set 문자열 만들기
    std::string set_value("");
    for(auto iter: update_values) {
        set_value += iter.first + " = " + iter.second + ", ";
    }

    //  끝의 ", " 제거
    set_value.pop_back();
    set_value.pop_back();

    //  sql문 만들기
    char* sql = (char*)malloc(20 + table.size() + set_value.size() + condition.size());
    sprintf(sql, "UPDATE %s SET %s WHERE %s", table.data(), set_value.data(), condition.data());
    
    //  쿼리 날리기
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to update data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    //  자원해제
    free(sql);
    sqlite3_close(db);
    return 0;
}

//  db_delete 함수
int DB_Connect::db_delete(char* db_name, std::string table, std::string condition) {
    /*
    db_name: db파일 이름
    table: 테이블 명
    condition: where문에 올 조건식
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name, &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return -1;
    }

    //  sql문 만들기
    char* sql = (char*)malloc(20 + table.size() + condition.size());
    sprintf(sql, "DELETE FROM %s WHERE %s", table.data(), condition.data());
    
    //  쿼리 날리기
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    //  자원해제
    free(sql);
    sqlite3_close(db);
    return 0;
}

int main(void)
{
    DB_Connect db_connect;

    //  insert test
    // std::list<std::string> values;

    // values.push_back(std::string("9"));
    // values.push_back(std::string("'TEST'"));
    // values.push_back(std::string("521430"));

    // db_connect.db_insert("test.db", "Cars", values);

    //  update test
    // std::map<std::string, std::string> update_set;
    // update_set.insert({"Name", "'Like'"});
    // update_set.insert({"Price", "75364"});
    // db_connect.db_update("test.db", "Cars", update_set, "Id = 9");

    //  delete test
    db_connect.db_delete("test.db", "Cars", "Id = 9");

    //  select test
    std::list<Data_List> dl;
    std::list<std::string> columns;
    columns.push_back("*");
    dl = db_connect.db_select("test.db", "Cars", columns);

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);

    return 0;
}
