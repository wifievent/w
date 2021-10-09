#include "db-connect.h"

//  동적할당한 메모리 해제함수
void Data_List::list_free(std::list<Data_List> data) {

}

//  DB_Connect 생성자
DB_Connect::DB_Connect() {
    //  JSON을 통해서 옵션화
    std::string db_name("netblock.db");

    //  db open
    rc = sqlite3_open(db_name.data(), &db);

    if(rc != SQLITE_OK) {
        GTRACE("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
}
DB_Connect::~DB_Connect() {
    sqlite3_close(db);
}

//  db_select 함수
std::list<Data_List> DB_Connect::select_query(std::string query) {
    /*
    query: SELECT 쿼리문

    return: Select 결과가 담긴 list<Data_List>
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수
    
    std::list<Data_List> dl;    //  select 결과 저장 list
    
    {
        std::lock_guard<std::mutex> lock(m);
        //  쿼리 날려서 결과 얻기
        rc = sqlite3_exec(db, query.data(), callback, &dl, &err_msg);
        
        if(rc != SQLITE_OK) {
            GTRACE("Failed to select data\n");
            GTRACE("SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
        }
    }

    return dl;
}

//  select에서 사용하는 callback함수
int DB_Connect::callback(void* dl, int ac, char** av, char** c) {
    //  결과를 저장할 변수
    Data_List data;
    data.argc = ac;
    for(int i = 0; i < ac; ++i) {
        data.argv.push_back(av[i]);
        data.column.push_back(c[i]);
    }
    //  결과를 저장
    std::list<Data_List>* data_list = (std::list<Data_List>*)dl;
    data_list->push_back(data);
    return 0;
}


//  send_query 함수
int DB_Connect::send_query(std::string query) {
    /*
    query: 쿼리문
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수
    int result = 0;

    {
        std::lock_guard<std::mutex> lock(m);
        //  쿼리 날리기
        rc = sqlite3_exec(db, query.data(), 0, 0, &err_msg);
        if(rc != SQLITE_OK) {
            GTRACE("Failed to send query\n");
            GTRACE("SQL error: %s\n", err_msg);

            sqlite3_free(err_msg);
            result = -1;
        }
    }
    
    return result;
}
