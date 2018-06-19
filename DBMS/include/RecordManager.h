#pragma once

#include <vector>
#include <utility>
#include <variant>
#include <bitset>
#include "Definitions.h"
#include "Catalog.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#include "Exception.h"

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
            std::pair<uint, const Row &> operator *() const;
            const RecordIterator & operator ++();

        private:

            const std::string &tableName;
            uint id;
        };

        class RecordResult
        {
        public:
            RecordIterator begin() const;
            RecordIterator end() const;
            RecordResult(const std::string &tableName, uint __begin, uint __end);
        
        private:
            
            std::string tableName;
            uint beginID, endID;
        };

        RecordResult traverse(const std::string &tableName);
        Row fetch(const std::string &tableName, uint id);
        void insert(const std::string &tableName, const Row &row);

        RecordManager();

    private:
        
        Row decode(const std::vector<Column> &columns, byte *p);
        void encode(byte *p, const std::vector<Column> &columns, const Row &r);
        
        void writeBit(byte *p, uint pos, bool value);
        bool readBit(byte *p, uint pos);
        
        static RecordManager *instance;
    };
}
