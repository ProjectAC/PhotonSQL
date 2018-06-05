#include "../include/IndexManager.h"

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

    }

    void IndexManager::insert(const std::string &indexName, const Attribute &id, uint handle)
    {

    }

    void IndexManager::remove(const std::string &indexName, const Attribute &id)
    {

    }

    void IndexManager::build(const std::string &indexName)
    {

    }
}