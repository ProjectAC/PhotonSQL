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
            RecordIterator(const std::string &tableName, uint id);

            bool operator != (const RecordIterator &i) const;
            const Row & operator *() const;
            const RecordIterator& operator ++();
        };

        class RecordResult
        {
        public:
            RecordIterator begin() const;
            RecordIterator end() const;
            RecordResult(const std::string &tableName, uint __begin, uint __end);
        
        private:
            
        };

        RecordResult traverse(const std::string &tableName);
        const Row & fetch(const std::string &tableName, uint id);
        void insert(const std::string &tableName, const Row &row);

        RecordManager();

    private:
        
        const Row & decode(const std::vector<Column> &columns, byte *p);
        void encode(byte *p, const std::vector<Column> &columns, const Row &r);
        static RecordManager *instance;
    };
}