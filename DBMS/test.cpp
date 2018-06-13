#include "include/CatalogManager.h"
#include <iostream>

using namespace std;
using namespace Photon;

int main()
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

    system("pause");

    return 0;
}