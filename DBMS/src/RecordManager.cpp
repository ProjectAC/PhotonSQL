#include "../include/RecordManager.h"


using namespace std;

namespace Photon
{
    ///////////// RecordManager /////////////
    
    const RecordManager::RecordResult &RecordManager::traverse(const std::string &tableName)
    {
        return RecordResult(tableName, 0, CatalogManager::getInstance().getTable(tableName).getIncrement());
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

    void RecordManager::insert(const std::string &tableName, const Row &row)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint id = table.getIncrement();
        uint rowSize = table.rowSize();
        uint rowsPerBlock = BufferManager::SIZE / rowSize;
        uint blockID = id / rowsPerBlock;
        uint innerID = id % rowsPerBlock;

        auto &buffer = BufferManager::getInstance();
        byte *p = buffer.get(tableName, blockID);
        encode(p + innerID, table.getColumns(), row);
    }

    void RecordManager::writeBit(byte *p, uint pos, bool value)
    {
        if (value)
            (*(p + pos / 8)) |= (1 << (pos & 7));
        else
            (*(p + pos / 8)) &= ~(1 << (pos & 7));
    }

    bool RecordManager::readBit(byte *p, uint pos)
    {
        return ((*(p + pos / 8)) >> (pos & 7)) & 1;
    }

    const Row & RecordManager::decode(const vector<Column> &columns, byte *p)
    {
        if (!readBit(p, 0))
            return Row();

        Row res;
        uint i = 1;
        byte *t = p;

        for (auto &c : columns)
        {
            if (!readBit(p, i))
                res.push_back( Attribute(monostate()) );
            else if (c.type == INTEGER)
                res.push_back( Attribute(*(Integer*)(t)) );
            else if (c.type == REAL)
                res.push_back( Attribute(*(Real*)(t)) );
            else if (c.type == STRING)
                res.push_back( Attribute(String(t)) );
            else
                throw UnknownTypeException();
            t += c.length;
            i++;
        }

        return res;
    }

    void RecordManager::encode(byte *p, const std::vector<Column> &columns, const Row &r)
    {
        uint n = (uint)columns.size();
        if (n != r.size())
            throw RowLengthMismatchException();

        uint bytes = n / 8 + 1;
        byte *t = p + bytes;
        
        writeBit(p, 0, true);

        for (uint i = 0; i < n; i++)
        {
            auto &c = columns[i];
            auto &a = r[i];

            if (r[i].index() == NIL)
                writeBit(p, i + 1, false);
            else
            {
                writeBit(p, i + 1, true);
                if (c.type == INTEGER)
                    memcpy(t, &a, sizeof(Integer));
                else if (c.type == REAL)
                    memcpy(t, &a, sizeof(Real));
                else if (c.type == STRING)
                    memcpy(t, &a, sizeof(char) * c.length);
                else
                    throw UnknownTypeException();
            }
            t += c.length;
        }
    }

    RecordManager *RecordManager::instance = nullptr;

    RecordManager::RecordManager()
    {
        if (instance == nullptr)
            instance = this;
    }

    RecordManager & RecordManager::getInstance()
    {
        return *instance;
    }

    ///////////// RecordIterator /////////////

    RecordManager::RecordIterator::RecordIterator(const std::string &tableName, uint id) :
        tableName(tableName),
        id(id)
    {
    }

    bool RecordManager::RecordIterator::operator != (const RecordManager::RecordIterator &i) const
    {
        return id != i.id || tableName != i.tableName;
    }

    std::pair<uint, const Row &> RecordManager::RecordIterator::operator *() const
    {
        return { id, RecordManager::getInstance().fetch(tableName, id) };
    }

    const RecordManager::RecordIterator & RecordManager::RecordIterator::operator ++()
    {
        for (id++; RecordManager::getInstance().fetch(tableName, id).size(); id++);
    }

    ///////////// RecordResult /////////////

    const RecordManager::RecordIterator & RecordManager::RecordResult::begin() const
    {
        return RecordIterator(tableName, beginID);
    }

    const RecordManager::RecordIterator & RecordManager::RecordResult::end() const
    {
        return RecordIterator(tableName, endID);
    }

    RecordManager::RecordResult::RecordResult(const std::string &tableName, uint __begin, uint __end) :
        tableName(tableName),
        beginID(__begin),
        endID(__end)
    {

    }
}
