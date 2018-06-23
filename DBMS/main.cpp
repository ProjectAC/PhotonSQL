#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>

#include "include/Condition.h"
#include "include/DBMS.h"

using namespace Photon;
using namespace std;

/*
string readSQL()
{
    int i;
    string res;
    string tmp;

    while (getline(cin, tmp))
    {
        res += tmp;
        for (i = tmp.length() - 1; i >= 0 && isspace(tmp[i]); i--);
        if (tmp[i] == ';')
            break;
    }
    return res;
}
*/

int main()
{
    // first
    DBMS dbms;
    SQL sql;
    Condition cond;
    vector<Row> res;

    vector<Column> columns;
    columns.push_back({
        "id",
        STRING,
        10,
        true,
        true
    });
    columns.push_back({
        "name",
        STRING,
        10,
        true,
        false
    });
    columns.push_back({
        "age",
        REAL,
        8,
        false,
        false
    });

    dbms.CreateTable("Person", columns);
    dbms.CreateIndex("PersonKey", "Person", "id");
    
    sql = "insert into Person values ('sol', 'SOL', 19), ('xiang', 'Xiang', 18), ('root', 'Admin', 20);";
    auto &rows = sql.getRows();
    uint ins = dbms.Insert(sql.getTable(), rows);
    cout << ins << " row(s) inserted." << endl;

    sql = "select * from Person where id = 'sol' and age < 20;";
    cond = sql.getCondition();
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }

    sql = "select * from Person where id >= 'sol' and age <= 20;";
    cond = sql.getCondition();
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }

    sql = "delete from Person where name = 'Xiang';";
    cond = sql.getCondition();
    uint del = dbms.Delete("Person", cond);
    cout << del << " row(s) deleted." << endl;
    
    sql = "select * from Person where id >= 'sol' and age <= 20;";
    cond = sql.getCondition();
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }

    dbms.DropTable("Person");
    try
    {
        SQL sql = "select * from Person where id = 'SOL' and age < 20;";
        Condition cond = sql.getCondition();
        dbms.Select("Person", cond);
    }
    catch (...)
    {
        printf("Failed\n");
    }

    system("pause");

    return 0;
}