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

    IndexManager::~IndexManager()
    {
        for (auto &tree : indicies)
            delete tree.second;
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

        BpTree *tree = indicies[indexName] = new BpTree("../Storage/indicies/" + indexName + ".idx", table.getColumn(cid).type, table.getColumn(cid).length);

        auto &dataSet = rm.traverse(index.table);
        for (auto &record : dataSet)
            tree->insert(record.second[cid], record.first);
    }

    void IndexManager::drop(const std::string &indexName)
    {
        delete findIndex(indexName);
        indicies.erase(indexName);
        BufferManager::getInstance().drop("../Storage/indicies/" + indexName + ".idx");
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