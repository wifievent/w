#include "base/db-connect.h"
#include <fstream>
#include <vector>
#include <sstream>

std::vector<std::string> split(std::string input, char delimiter) {
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;

    while (getline(ss, temp, delimiter)) {
        temp.erase(temp.find_last_not_of("\r")+1);
        answer.push_back(temp);
    }

    return answer;

}

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

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'time'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE time (time_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, start_time CHAR(4) NOT NULL, end_time CHAR(4) NOT NULL, day_of_the_week TINYINT NOT NULL)");
    }

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'policy'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE policy (policy_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, host_id INTEGER NOT NULL, time_id INTEGER NOT NULL)");
    }

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'block_host'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE VIEW block_host as SELECT mac, last_ip, name FROM host WHERE host_id = (SELECT host_id from policy where time_id = (select time_id from time where strftime(\"%H%M\", 'now', 'localtime') BETWEEN start_time AND end_time AND strftime(\"%w\", 'now', 'localtime') = day_of_the_week))");
    }

    dl = db_connect.select_query("SELECT name FROM sqlite_master WHERE name = 'oui'");
    if(dl.size() == 0) {
        db_connect.send_query("CREATE TABLE oui (mac CHAR(20) NOT NULL PRIMARY KEY, organ VARCHAR(64) NOT NULL)");
        std::string resline;
        std::string mac_, oui_;
        std::vector<std::string> result;
        std::ifstream res("manuf_rep.txt");
        while (getline(res, resline)) {
            result = split(resline, '\t');
            std::string q = "INSERT INTO oui VALUES('" + result[0] + "', '" +  result[1] + "')";
            db_connect.send_query(q);
        }
    }
    Data_List::list_free(dl);
    return 0;
}
