#include "include/CatalogManager.h"
#include "include/BpTree.h"
#include <iostream>

using namespace std;
using namespace Photon;

void testCatalog()
{
    CatalogManager cm;

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
    BufferManager bm;
    BpTree tree("Storage/test.idx", AttributeType::STRING, 1200);
    tree.debug();
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
    tree.debug();
    tree.erase("f");
    tree.debug();
    tree.erase("j");

    for (auto x = BpIterator(&tree, Attribute("d")), y = BpIterator(&tree, Attribute("k")); x != y; ++x)
    {
        auto t = *x;
        printf("(%s, %u) ", get<string>(t.first).c_str(), t.second);
    }
    printf("\n");
}

int main()
{
    testBpTree();

    system("pause");

    return 0;
}