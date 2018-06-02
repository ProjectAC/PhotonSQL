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
}