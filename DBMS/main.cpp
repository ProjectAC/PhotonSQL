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

    // ����Columns
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

    // �����������
    dbms.CreateTable("Person", columns);
    dbms.CreateIndex("PersonKey", "Person", "id");

    // ʹ��SQL��������
    sql = s = "insert into Person values ('sol', 'SOL', 19), ('xiang', 'Xiang', 18), ('root', 'Admin', 20);";
    cout << s << endl;
    auto &rows = sql.getRows();
    uint ins = dbms.Insert(sql.getTable(), rows);
    cout << ins << " row(s) inserted." << endl << endl;

    // ʹ��SQL��ѯ����
    sql = s = "select * from Person where id = 'sol' and age < 20;";
    cout << s << endl;
    cond = sql.getCondition();
    // ѡ��Ҫ�õ�����
    cond.setIndex("PersonKey", "sol", Attribute());
    res = dbms.Select("Person", cond);
    for (auto &row : res)
    {
        for (auto &t : row)
            cout << t << ", ";
        cout << endl;
    }
    cout << endl;

    // ��һ���ѯ�����ں�ɾ����Ա�
    sql = s = "select * from Person where id >= 'sol' and age <= 20;";
    cout << s << endl;
    cond = sql.getCondition();
    // ѡ��Ҫ�õ�����
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
    // û��ѡ��Ҫ�õ�����������û�п��õ�������ֻ����ѡ������ڲ�ѯ�����е��ж�Ӧ����������ֻ��ͨ��traverse�����������м�¼
    uint del = dbms.Delete("Person", cond);
    cout << del << " row(s) deleted." << endl << endl;

    // �ٴβ�ѯ�����Կ�����Ӧ�ļ�¼�Ѿ���ɾ��
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

    // ɾ���������ᱻ����ɾ��������ע�⵽�ļ���ʧ��
    dbms.DropTable("Person");
    try
    {
        // ��ͼ��һ�������ڵı���в�ѯ
        sql = s = "select * from Person where id = 'SOL' and age < 20;";
        cout << s << endl;
        cond = sql.getCondition();
        dbms.Select("Person", cond);
    }
    catch (...)
    {
        // ���Բ�׽�쳣
        printf("Failed\n");
    }
    cout << endl;

    system("pause");

    return 0;
}
