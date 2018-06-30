#include <iostream>
#include <string>
#include <ctype.h>
#include <cstdio>

#include "include/Condition.h"
#include "include/DBMS.h"

using namespace Photon;
using namespace std;

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

int main()
{
    DBMS dbms;
    SQL sql;
    string s;
    Condition cond;
    vector<Row> res;

    // 建立Columns
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

    // 创建表和索引
    dbms.CreateTable("Person", columns);
    dbms.CreateIndex("PersonKey", "Person", "id");

    // 使用SQL插入数据
    sql = s = "insert into Person values ('sol', 'SOL', 19), ('xiang', 'Xiang', 18), ('root', 'Admin', 20);";
    cout << s << endl;
    auto &rows = sql.getRows();
    uint ins = dbms.Insert(sql.getTable(), rows);
    cout << ins << " row(s) inserted." << endl << endl;

    // 使用SQL查询数据
    sql = s = "select * from Person where id = 'sol' and age < 20;";
    cout << s << endl;
    cond = sql.getCondition();
    // 选择要用的索引
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }
    cout << endl;

    // 另一组查询，用于和删除后对比
    sql = s = "select * from Person where id >= 'sol' and age <= 20;";
    cout << s << endl;
    cond = sql.getCondition();
    // 选择要用的索引
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }
    cout << endl;

    sql = s = "delete from Person where name = 'Xiang';";
    cout << s << endl;
    cond = sql.getCondition();
    // 没有选择要用的索引；由于没有可用的索引（只可以选择出现在查询条件中的列对应的索引），只能通过traverse方法遍历所有记录
    uint del = dbms.Delete("Person", cond);
    cout << del << " row(s) deleted." << endl << endl;

    // 再次查询，可以看到对应的记录已经被删除
    sql = s = "select * from Person where id >= 'sol' and age <= 20;";
    cout << s << endl;
    cond = sql.getCondition();
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }
    cout << endl;

    // 删除表，索引会被级联删除，可以注意到文件消失了
    dbms.DropTable("Person");
    try
    {
        // 试图对一个不存在的表进行查询
        sql = s = "select * from Person where id = 'SOL' and age < 20;";
        cout << s << endl;
        cond = sql.getCondition();
        dbms.Select("Person", cond);
    }
    catch (...)
    {
        // 可以捕捉异常
        printf("Failed\n");
    }
    cout << endl;

    system("pause");

    return 0;
}
