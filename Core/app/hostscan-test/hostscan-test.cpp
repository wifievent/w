#include "db_check.h"
#include "core.h"
int main()
{
    dbCheck();
    Core core;
    core.start();
    std::cout << "press any key to close" <<std::endl;
    std::string s;
    std::getline(std::cin,s);
    core.stop();
    return 0;
}
