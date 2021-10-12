#include "oui.h"

QString oui_db(std::string mac_, DB_Connect& db_connect){
    QString result;

    std::list<Data_List> dl;
    dl = db_connect.select_query("SELECT organ FROM oui WHERE substr(mac, 1, 8) = substr('" + mac_ + "', 1, 8)");

    for(std::list<Data_List>::iterator iter = dl.begin(); iter != dl.end(); ++iter) {
        result = QString::fromStdString(iter->argv[0]);
    }

    return result;
}
