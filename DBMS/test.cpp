#include "include/CatalogManager.h"
#include "include/IndexManager.h"
#include "include/RecordManager.h"
#include "include/BpTree.h"
#include "include/Condition.h"
#include <iostream>

using namespace std;
using namespace Photon;

BufferManager bm;
IndexManager im;
CatalogManager cm;
RecordManager rm;

void testCatalog()
{
    /*
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
    0,
    false,
    false
    });

    cm.createTable("Person", columns);
    cm.saveCatalog("../Storage/test.json");
    */

    cm.loadCatalog("../Storage/test.json");

    Table t = cm.getTable("Person");

    cout << t.getIncrement() << endl;

    for (auto &c : t.getColumns())
    {
        cout << c.name.c_str() << " " << c.type << " " << c.length << " " << c.notNull << " " << c.unique << endl;
    }
}

void testBpTree()
{
    BpTree tree("../Storage/indicies/test.idx", AttributeType::STRING, 1200);
    for (int i = 1; i <= 12; i++)
    {
        //printf("=============================================\n");
        //printf("Insert #%d :\n", i);

        string s;
        s += 'a' + i - 1;
        tree.insert(s, i);
        //tree.debug();
    }

    tree.erase("d");
    tree.erase("f");
    tree.erase("j");

    for (auto x = BpIterator(&tree, Attribute("d")), y = BpIterator(&tree, Attribute("k")); x != y; ++x)
    {
        auto t = *x;
        printf("(%s, %u) ", get<string>(t.first).c_str(), t.second);
    }
    printf("\n");
}

void testBpTreeLoad()
{
    BpTree tree("../Storage/indicies/test.idx");
    tree.debug();

    for (auto x = BpIterator(&tree, Attribute("d")), y = BpIterator(&tree, Attribute("k")); x != y; ++x)
    {
        auto t = *x;
        printf("(%s, %u) ", get<string>(t.first).c_str(), t.second);
    }
    printf("\n");
}

void testIndexManager()
{
    auto res = im.fetch("test", Attribute("c"), Attribute());

    for (auto &r : res)
    {
        printf("(%s, %u) ", get<string>(r.first).c_str(), r.second);
    }
    printf("\n");
}

void testRecordManager()
{
    cm.loadCatalog("../Storage/test.json");
    
    /*
    Row row;
    row.push_back(Attribute("sol"));
    row.push_back(Attribute("SOL"));
    row.push_back(Attribute(Real(19.0)));
    
    rm.insert("Person", row);
    
    row = rm.fetch("Person", 0);
    cout << row[2].index() << endl;
    cout << get<string>(row[0]) << endl;
    cout << get<string>(row[1]) << endl;
    cout << get<Real>(row[2]) << endl;
    */

    for (int i = 0; i < 10; i++)
    {
        Row row;
        string s;
        s = 'a' + i;
        row.push_back(Attribute(s));
        s = 'A' + i;
        row.push_back(Attribute(s));
        row.push_back(Attribute(Real(20)));
        rm.insert("Person", row);
    }

    cout << cm.getTable("Person").getIncrement() << endl;
    auto &x = rm.traverse("Person");
    auto a = x.begin();
    auto b = x.end();

    for (auto &t : x)
    {
        cout << t.first << " ";
        cout << get<string>(t.second[0]) << " ";
        cout << get<string>(t.second[1]) << " ";
        cout << get<Real>(t.second[2]) << endl;
    }
}

void testSQL()
{
    cm.loadCatalog("../Storage/test.json");

    //SQL sql = "select * from Person where id = 'SOL' and age < 20;";
    SQL sql = "insert into Person values ('sol', 'sol', 19), ('xiang', 'xiang', 19);";
    cout << sql.getTable() << endl;
    auto &t = sql.getRows();
}

int main()
{
    //testBpTree();
    //testBpTreeLoad();
    //testCatalog();
    //testIndexManager();
    //testRecordManager();

    testSQL();

    system("pause");

    return 0;
}