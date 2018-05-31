#pragma once

#include <vector>
#include "Definitions.h"
#include "Catalog.h"

namespace Photon
{
    class RecordManager
    {
    public:

        static RecordManager &getInstance();
        
        class RecordIterator
        {
        public:
            RecordIterator(const std::string &tableName, const std::vector<Column> &columns, uint id);

            bool operator != (const RecordIterator &i) const;
            const Row & operator *() const;
            const RecordIterator& operator ++();
        };

        class RecordResult
        {
        public:
            RecordIterator begin() const;
            RecordIterator end() const;
            RecordResult(const std::string &tableName, const std::vector<Column> &columns, uint __begin, uint __end);
        
        private:
            
        };

        RecordResult traverse(const std::string &tableName, const std::vector<Column> &columns);
        const Row & fetch(const std::string &tableName, const std::vector<Column> &columns, uint _begin);

    private:

        static RecordManager *instance;
    };
}