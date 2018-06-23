#include "../include/RecordManager.h"
#include "iostream"

using namespace std;

namespace Photon
{
    ///////////// RecordManager /////////////
    
    RecordManager::RecordResult RecordManager::traverse(const std::string &tableName)
    {
        return RecordResult(tableName, 0, CatalogManager::getInstance().getTable(tableName).getIncrement());
    }

    Row RecordManager::fetch(const std::string &tableName, uint id)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint rowSize = table.rowSize();
        uint rowsPerBlock = BufferManager::SIZE / rowSize;
        uint blockID = id / rowsPerBlock;
        uint innerID = id % rowsPerBlock;

        auto &buffer = BufferManager::getInstance();
        byte *p = buffer.get("../Storage/records/" + tableName + ".rcd", blockID);

        return decode(table.getColumns(), p + innerID * rowSize);
    }

    bool RecordManager::hide(const std::string &tableName, uint id)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint rowSize = table.rowSize();
        uint rowsPerBlock = BufferManager::SIZE / rowSize;
        uint blockID = id / rowsPerBlock;
        uint innerID = id % rowsPerBlock;

        auto &buffer = BufferManager::getInstance();
        byte *p = buffer.get("../Storage/records/" + tableName + ".rcd", blockID);

        if (readBit(p + innerID * rowSize, 0))
        {
            writeBit(p + innerID * rowSize, 0, false);
            return true;
        }
        else
            return false;
    }

    uint RecordManager::insert(const std::string &tableName, const Row &row)
    {
        Table &table = CatalogManager::getInstance().getTable(tableName);
        uint id = table.getIncrement(); table.count();
        uint rowSize = table.rowSize();
        uint rowsPerBlock = BufferManager::SIZE / rowSize;
        uint blockID = id / rowsPerBlock;
        uint innerID = id % rowsPerBlock;

        auto &buffer = BufferManager::getInstance();
        byte *p = buffer.get("../Storage/records/" + tableName + ".rcd", blockID);
        encode(p + innerID * rowSize, table.getColumns(), row);
        return id;
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

	Row RecordManager::decode(const vector<Column> &columns, byte *p)
	{
		if (!readBit(p, 0))
			return Row();

		Row res;
		uint i = 1;
		byte *t = p + columns.size() / 8 + 1;

		for (auto &c : columns)
		{
			if (!readBit(p, i)) {
				Attribute attr1;
				attr1 = monostate();
				res.push_back(attr1);
			}
			//res.push_back( Attribute(monostate()) );
			else if (c.type == INTEGER) {
				Attribute attr1;
				attr1 = *(Integer*)(t);
				res.push_back(attr1);
			}
			// res.push_back( Attribute(*(Integer*)(t)) );
			else if (c.type == REAL) {
				Attribute attr1;
				attr1 = *(Real*)(t);
				res.push_back(attr1);
			}
			//res.push_back( Attribute(*(Real*)(t)) );
			else if (c.type == STRING) {
				Attribute attr1;
				attr1 = String(t);
				res.push_back(attr1);
			}
			// res.push_back( Attribute(String(t)) );
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
                {
                    Integer tmp = get<Integer>(a);
                    memcpy(t, &tmp, sizeof(Integer));
                }
                else if (c.type == REAL)
                {
                    Real tmp = get<Real>(a);
                    memcpy(t, &tmp, sizeof(Real));
                }
                else if (c.type == STRING)
                {
                    memset(t, 0, sizeof(char) * c.length);
                    const string &tmp = get<String>(a);
                    memcpy(t, tmp.c_str(), sizeof(char) * tmp.length());
                }
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

    std::pair<uint, Row> RecordManager::RecordIterator::operator *() const
    {
        return { id, RecordManager::getInstance().fetch(tableName, id) };
    }

    RecordManager::RecordIterator & RecordManager::RecordIterator::operator ++()
    {
        CatalogManager &cm = CatalogManager::getInstance();
        RecordManager &rm = RecordManager::getInstance();
        uint size = cm.getTable(tableName).getIncrement();
          
        for (id++; id < size && !rm.fetch(tableName, id).size(); id++);
        return *this;
    }

    ///////////// RecordResult /////////////

    RecordManager::RecordIterator RecordManager::RecordResult::begin() const
    {
        return RecordIterator(tableName, beginID);
    }

    RecordManager::RecordIterator RecordManager::RecordResult::end() const
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
