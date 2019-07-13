// This program finds unused guids.
// Author: brotalnia
//

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <array>
#include <set>
#include <sstream>

#include "Database\Database.h"

Database GameDb;

std::string MakeConnectionString()
{
    std::string mysql_host;
    std::string mysql_port;
    std::string mysql_user;
    std::string mysql_pass;
    std::string mysql_db;

    printf("Host: ");
    getline(std::cin, mysql_host);
    if (mysql_host.empty())
        mysql_host = "127.0.0.1";

    printf("Port: ");
    getline(std::cin, mysql_port);
    if (mysql_port.empty())
        mysql_port = "3306";

    printf("User: ");
    getline(std::cin, mysql_user);
    if (mysql_user.empty())
        mysql_user = "root";

    printf("Password: ");
    getline(std::cin, mysql_pass);
    if (mysql_pass.empty())
        mysql_pass = "root";

    printf("Database: ");
    getline(std::cin, mysql_db);
    if (mysql_db.empty())
        mysql_db = "mangos";

    return mysql_host + ";" + mysql_port + ";" + mysql_user + ";" + mysql_pass + ";" + mysql_db;
}

int main()
{
    printf("\nEnter your database connection info.\n");
    std::string const connection_string = MakeConnectionString();

    printf("\nConnecting to database.\n");
    if (!GameDb.Initialize(connection_string.c_str()))
    {
        printf("\nError: Cannot connect to database!\n");
        getchar();
        return 1;
    }

    std::string table_name;
    printf("Table: ");
    getline(std::cin, table_name);
    if (table_name.empty())
        table_name = "creature";

    std::string field_name;
    printf("Field: ");
    getline(std::cin, field_name);
    if (field_name.empty())
        field_name = "guid";

    std::vector<uint32> vGuids;

    printf("Loading spawn database.\n");
    if (std::shared_ptr<QueryResult> result = GameDb.Query("SELECT DISTINCT `%s` FROM `%s` ORDER BY `%s`", field_name.c_str(), table_name.c_str(), field_name.c_str()))
    {
        do
        {
            DbField* pFields = result->fetchCurrentRow();

            uint32 guid = pFields[0].getUInt32();
            vGuids.push_back(guid);
        } while (result->NextRow());
    }
    printf("Loaded %u guids.\n", vGuids.size());

    uint32 min;
    printf("Above: ");
    scanf("%d", &min);
    
    uint32 last = 0;
    for (auto i : vGuids)
    {
        if ((i > min) && (i > last + 1))
        {
            printf("Closest free guid: %u", last + 1);
            break;
        }
        last = i;
    }

    fflush(stdin);
    getchar();
    
    GameDb.Uninitialise();
    return 0;
}

