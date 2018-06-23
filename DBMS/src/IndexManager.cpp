#include "../include/IndexManager.h"
#include "../include/RecordManager.h"
#include "../include/CatalogManager.h"

using namespace std;

namespace Photon
{
    ///////////// IndexManager /////////////

    IndexManager *IndexManager::instance = nullptr;

    IndexManager &IndexManager::getInstance()
    {
        return *IndexManager::instance;
    }

    IndexManager::IndexManager()
    {
        if (instance == nullptr)
            instance = this;
    }

    IndexManager::IndexResult IndexManager::fetch(const std::string &indexName, const Attribute &__begin, const Attribute &__end)
    {
        return { findIndex(indexName), __begin, __end };
    }

    void IndexManager::insert(const std::string &indexName, const Attribute &id, uint handle)
    {
        findIndex(indexName)->insert(id, handle);
    }

    void IndexManager::erase(const std::string &indexName, const Attribute &id)
    {
        findIndex(indexName)->erase(id);
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

    BpTree *IndexManager::findIndex(std::string name)
    {
        if (indicies.find(name) == indicies.end())
            indicies[name] = new BpTree("../Storage/indicies/" + name + ".idx");
        return indicies[name];
    }

    ///////////// IndexResult /////////////

    IndexManager::IndexResult::IndexResult(BpTree *tree, const Attribute &__begin, const Attribute & __end) :
        __begin(__begin.index() ? BpIterator(tree, __begin) : BpIterator(tree, true )),
        __end  (__end  .index() ? BpIterator(tree, __end  ) : BpIterator(tree, false))
    {
    }

    BpIterator IndexManager::IndexResult::begin() const
    {
        return __begin;
    }

    BpIterator IndexManager::IndexResult::end() const
    {
        return __end;
    }
}