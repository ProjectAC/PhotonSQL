#include "../include/RecordManager.h"
#include "../include/BufferManager.h"
#include "../include/CatalogManager.h"
#include <utility>
#include <variant>

using namespace std;

namespace Photon
{
    ///////////// RecordManager /////////////
    
    RecordManager::RecordResult RecordManager::traverse(const std::string &tableName)
    {

    }

    const Row & RecordManager::fetch(const std::string &tableName, uint id)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint rowSize = table.rowSize();
        uint rowsPerBlock = BufferManager::SIZE / rowSize;
        uint blockID = id / rowsPerBlock;
        uint innerID = id % rowsPerBlock;

        auto &buffer = BufferManager::getInstance();
        byte *p = buffer.get(tableName, blockID);

        return decode(table.getColumns(), p + innerID);
    }

    void insert(byte *p, const std::string &tableName, const Row &row)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint rowSize = table.rowSize();
    }

    const Row & RecordManager::decode(const vector<Column> &columns, byte *p)
    {
        Row res;
        uint offset = 0;

        for (auto &c : columns)
        {
            if (c.type == INTEGER)
            {
                res.push_back( Attribute(*(Integer*)(p)) );
            }
            else if (c.type == REAL)
            {
                res.push_back( Attribute(*(Real*)(p)) );
            }
            else if (c.type == STRING)
            {
                res.push_back( Attribute(String(p)) );
            }
            p += c.length;
        }

        return res;
    }
}