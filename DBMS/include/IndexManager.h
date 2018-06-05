#pragma once

// TODO: Change std::map to B+Tree
#include <map>

#include "Definitions.h"
#include "Catalog.h"

namespace Photon
{
    class IndexManager
    {
    public:

        static IndexManager &getInstance();
        IndexManager();

        class IndexIterator
        {
        public:
            IndexIterator(const std::string &indexName, const Attribute &id);

            bool operator != (const IndexIterator &i) const;
            const uint & operator *() const;
            const IndexIterator& operator ++();
        };

        class IndexResult
        {
        public:
            const IndexIterator &begin() const;
            const IndexIterator &end() const;
            IndexResult(const std::string &indexName, const Attribute &__begin, const Attribute & __end);

        private:

        };

        const IndexResult &fetch(const std::string &indexName, const Attribute &__begin, const Attribute &__end);
        void insert(const std::string &indexName, const Attribute &id, uint handle);
        void remove(const std::string &indexName, const Attribute &id);
        void build(const std::string &indexName);

    private:
        
        static IndexManager *instance;
    };
}