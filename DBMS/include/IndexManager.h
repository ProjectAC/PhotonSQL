#pragma once

#include <unordered_map>

#include "BpTree.h"
#include "Definitions.h"
#include "Catalog.h"

namespace Photon
{
    class IndexManager
    {
    public:

        static IndexManager &getInstance();
        IndexManager();

        class IndexResult
        {
        public:
            BpIterator begin() const;
            BpIterator end() const;
            IndexResult(BpTree *index, const Attribute &__begin, const Attribute & __end);

        private:

            BpIterator __begin, __end;
        };

        IndexResult fetch(const std::string &indexName, const Attribute &__begin, const Attribute &__end);
        void insert(const std::string &indexName, const Attribute &id, uint handle);
        void erase(const std::string &indexName, const Attribute &id);
        void build(const std::string &indexName);

        BpTree *findIndex(std::string name);

    private:
        
        static IndexManager *instance;
        std::unordered_map<std::string, BpTree*> indicies;
    };
}