#include "../include/DBMS.h"

namespace Photon {

	std::vector<Row> Photon::DBMS::Select(const String &table, const Condition &c)
    {
		String indexname;
		std::vector<Row> result;

        Table &temptable = cm.getTable(table);

		if ((indexname = c.getIndex()) != "")
		{
			auto &item = im.fetch(indexname, c.getLeft(), c.getRight());
			for (auto &i : item) {
				auto temprow = rm.fetch(table, i.second);
				if (c.satisfy(temptable.getColumns(), temprow))
					result.push_back(temprow);
			}
		}
		else
		{
			auto &item = rm.traverse(table);
			for (auto &i : item) {
				if (c.satisfy(temptable.getColumns(), i.second))
					result.push_back(i.second);
			}
		}
		return result;
	}

	uint Photon::DBMS::Insert(const std::string & table, const std::vector<Row>& res)
	{
		uint num = 0;
		uint id;
        auto &t = cm.getTable(table);
        auto &temp = t.getIndicies();
		for (auto &i : res)
        {
			num += ((id = rm.insert(table, i)) != -1);
			for (auto &j : temp)
				im.insert(j, i[t.hasColumn(cm.getIndex(j).column)], id);
		}
		return num;
	}

	uint Photon::DBMS::Delete(const String &table, const Condition & c)
	{
        String indexname;
        int res = 0;

        Table &temptable = cm.getTable(table);

        if ((indexname = c.getIndex()) != "")
        {
            auto &item = im.fetch(indexname, c.getLeft(), c.getRight());
            for (auto &i : item) {
                auto temprow = rm.fetch(table, i.second);
                if (c.satisfy(temptable.getColumns(), temprow))
                    res += rm.hide(table, i.second);
            }
        }
        else
        {
            auto &item = rm.traverse(table);
            for (auto &i : item) {
                if (c.satisfy(temptable.getColumns(), i.second))
                    res += rm.hide(table, i.first);
            }
        }
        return res;
	}

	bool Photon::DBMS::CreateTable(const std::string & table, const std::vector<Column> columns)
	{
		cm.createTable(table, columns);
		return true;
	}

	bool DBMS::DropTable(const std::string & table)
	{
		cm.dropTable(table);
        bm.drop("../Storage/records/" + table + ".rcd");
		return true;
	}

	bool DBMS::CreateIndex(const std::string & index, const std::string & table, const std::string & column)
	{
		cm.createIndex(index, table, column);
        im.build(index);
		return true;
	}

	bool DBMS::DropIndex(const std::string & index)
	{
		cm.dropIndex(index);
        im.drop(index);
		return true;
	}

}