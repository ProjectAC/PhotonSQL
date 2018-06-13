#include "../include/IndexManager.h"
#include "../include/RecordManager.h"
#include "../include/CatalogManager.h"

using namespace std;

namespace Photon
{
    IndexManager &IndexManager::getInstance()
    {
        return *IndexManager::instance;
    }

    IndexManager::IndexManager()
    {
        IndexManager::instance = this;
    }

    const IndexManager::IndexResult &IndexManager::fetch(const std::string &indexName, const Attribute &__begin, const Attribute &__end)
    {
        return { indexName, __begin, __end };
    }

    void IndexManager::insert(const std::string &indexName, const Attribute &id, uint handle)
    {

    }

    void IndexManager::remove(const std::string &indexName, const Attribute &id)
    {

    }

    void IndexManager::build(const std::string &indexName)
    {
        RecordManager &rm = RecordManager::getInstance();
        CatalogManager &cm = CatalogManager::getInstance();

        Index index = cm.getIndex(indexName);
        Table table = cm.getTable(index.table);
        uint cid = table.hasColumn(index.column);

        auto &dataSet = rm.traverse(index.table);
        for (auto &record : dataSet)
            insert(indexName, record.second[cid], record.first);
    }
}