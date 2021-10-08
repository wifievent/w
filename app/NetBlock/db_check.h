#include "base/db-connect.h"

int dbCheck() {
    DB_Connect& db_connect = DB_Connect::getInstance();
    if(db_connect.rc != SQLITE_OK) {
        GTRACE("DB_open Error");
        return -1;
    }
    std::list<Data_List> dl;
    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'host'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE host (host_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, mac CHAR(17) NOT NULL, last_ip VARCHAR(15) NULL, name VARCHAR(30) NULL)");
    }
    Data_List::list_free(&dl);

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'time'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE time (time_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, start_time CHAR(4) NOT NULL, end_time CHAR(4) NOT NULL, day_of_the_week TINYINT NOT NULL)");
    }
    Data_List::list_free(&dl);

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'policy'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE policy (policy_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, host_id INTEGER NOT NULL, time_id INTEGER NOT NULL)");
    }
    Data_List::list_free(&dl);

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'block_host'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE VIEW block_host as SELECT mac, last_ip FROM host WHERE host_id = (SELECT host_id from policy where time_id = (select time_id from time where strftime(\"%H%M\", 'now', 'localtime') BETWEEN start_time AND end_time))");
    }
    Data_List::list_free(&dl);

    return 0;
}