#pragma once

#include "Definitions.h"
#include "Catalog.h"

namespace Photon
{
    class IndexManager
    {
    public:

        static IndexManager &getInstance();

        class IndexIterator
        {
        public:
            IndexIterator(const std::string &indexName, Attribute id);

            bool operator != (const IndexIterator &i) const;
            const uint & operator *() const;
            const IndexIterator& operator ++();
        };

        class IndexResult
        {
        public:
            IndexIterator begin() const;
            IndexIterator end() const;
            IndexResult(const std::string &indexName, Attribute __begin, Attribute __end);

        private:

        };

        const IndexResult &fetch(const std::string &indexName, Attribute __begin, Attribute __end);

    private:

        static IndexManager *instance;
    };
}