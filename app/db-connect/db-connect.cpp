#include "db-connect.h"

//  DB_Connect 생성자
DB_Connect::DB_Connect(std::string db_name) {
    this->db_name = db_name;
}

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
std::list<Data_List> DB_Connect::select_query(std::string query) {
    /*
    query: SELECT 쿼리문

    return: Select 결과가 담긴 list<Data_List>
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name.data(), &db);

    std::list<Data_List> dl;    //  select 결과 저장 list

    if(rc != SQLITE_OK) {
        GTRACE("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return dl;
    }
    
    //  쿼리 날려서 결과 얻기
    rc = sqlite3_exec(db, query.data(), callback, &dl, &err_msg);
    if(rc != SQLITE_OK) {
        GTRACE("Failed to select data\n");
        GTRACE("SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return dl;
    }

    //  자원 해제
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


//  send_query 함수
int DB_Connect::send_query(std::string query) {
    /*
    query: 쿼리문
    */
    char* err_msg = 0;    //  에러 메시지 저장 변수

    //  db open
    int rc = sqlite3_open(db_name.data(), &db);
    if(rc != SQLITE_OK) {
        GTRACE("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return -1;
    }
    
    //  쿼리 날리기
    rc = sqlite3_exec(db, query.data(), 0, 0, &err_msg);
    if(rc != SQLITE_OK) {
        GTRACE("Failed to send query\n");
        GTRACE("SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    //  자원해제
    sqlite3_close(db);
    return 0;
}

int main(void)
{
    DB_Connect db_connect("test.db");

    //  insert test
    db_connect.send_query("INSERT INTO Cars VALUES(9, 'TEST', 52143)");

    //  select test
    std::list<Data_List> dl;
    dl = db_connect.select_query("SELECT * FROM Cars");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);
    
    printf("--------------------------------------------------\n");

    //  update test
    db_connect.send_query("UPDATE Cars SET Name = 'PPL' WHERE Id = 9");

    dl = db_connect.select_query("SELECT * FROM Cars");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);

    printf("------------------------------------------------\n");

    //  delete test
    db_connect.send_query("DELETE FROM Cars WHERE Id = 9");

    dl = db_connect.select_query("SELECT * FROM Cars");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);
    
    printf("------------------------------------------------\n");

    db_connect.send_query("DROP TABLE TEST");
    db_connect.send_query("CREATE TABLE TEST (ID int NOT NULL, MAC CHAR(17) NOT NULL)");

    // std::list<std::string> value;
    db_connect.send_query("INSERT INTO TEST VALUES(1, 'DD:AA')");
    db_connect.send_query("INSERT INTO TEST VALUES(2, 'DD:21')");

    dl = db_connect.select_query("SELECT * FROM TEST");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        for(int i = 0; i < iter->argc; ++i) {
            printf("columns: %s, value: %s \n", iter->column[i], iter->argv[i]);
        }
    }

    Data_List::list_free(dl);
    return 0;
}
