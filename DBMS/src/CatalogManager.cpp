#include "../include/CatalogManager.h"
#include "../include/Exception.h"

using namespace std;

namespace Photon
{
    CatalogManager &CatalogManager::getInstance()
    {
        return *CatalogManager::instance;
    }

    CatalogManager::CatalogManager()
    {
        CatalogManager::instance = this;
    }

    Table &CatalogManager::getTable(const std::string &name)
    {
        auto t = tables.find(name);
        if (t == tables.end())
            throw ObjectNotFountException();
        return t->second;
    }

    Index &CatalogManager::getIndex(const std::string &index)
    {
        auto i = indicies.find(index);
        if (i == indicies.end())
            throw ObjectNotFountException();
        return i->second;
    }

    void CatalogManager::createTable(const std::string &tableName, const std::vector<Comlumn> columns)
    {
        tables.insert({tableName, Table(columns, vector(), 0)});
    }

    void CatalogManager::createIndex(const std::string &indexName, const std::string &tableName, const std::string columnName)
    {
        indicies.insert({tableName, Table(columns, vector(), 0)});
    }

    void CatalogManager::dropTable(const std::string &tableName)
    {
        tables.erase(tableName);
    }

    void CatalogManager::dropIndex(const std::string &indexName)
    {
        indicies.erase(indexName);
    }

    // TODO

    void CatalogManager::loadCatalog(const std::string &fileName)
    {

    }

    void CatalogManager::saveCatalog(const std::string &fileName)
    {

    }
}