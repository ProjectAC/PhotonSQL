#include "include/CatalogManager.h"

using namespace std;
using namespace Photon;

int main()
{
    CatalogManager cm;

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

    return 0;
}