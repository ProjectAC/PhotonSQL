#include "../include/DBMS.h"

namespace Photon {

	std::vector<Row> Photon::DBMS::Select(const String &table, const Condition &c) {
		String indexname;
		std::vector<Row> result;
		if ()//”–ø…”√index
		{
			auto &item = IndexManager::getInstance().fetch(indexname, Attribute(), Attribute());
			for (auto &i : item) {
				auto temprow = RecordManager::getInstance().fetch(table, i.second);
				Table temptable = CatalogManager::getInstance().getTable(table);
				if (c.satisfy(temptable.getColumns(), temprow))
					result.push_back(temprow);
			}
		}
		else
		{
			auto &item = RecordManager::getInstance().traverse(table);
			for (auto &i : item) {
				Table temp = CatalogManager::getInstance().getTable(table);
				if (c.satisfy(temp.getColumns(), i.second))
					result.push_back(i.second);
			}
		}
		return result;
	}



	uint Photon::DBMS::Insert(const std::string & table, const std::vector<Row>& res)
	{	
		uint num = 0;
		uint id;
		for (auto &i : res) {
			id = RecordManager::getInstance().insert(table, i);
			auto temp = CatalogManager::getInstance().getTable(table).getIndicies();
			for (auto &j : temp) {
				IndexManager::getInstance().insert(j, i[id], id);
			}
			num++;
		}
		return num;
	}



	uint Photon::DBMS::Delete(const String &table, const Condition & c)
	{
		
		

	}

	bool Photon::DBMS::CreateTable(const std::string & table, const std::vector<Column> columns)
	{
		CatalogManager::getInstance().createTable(table, columns);
		return true;
	}

	bool DBMS::DropTable(const std::string & table)
	{
		auto &indicies = CatalogManager::getInstance().getTable(table).getIndicies();
		for (auto &i : indicies) {
			CatalogManager::getInstance().dropIndex(i);
		}		
		CatalogManager::getInstance().dropTable(table);
		return true;
	}

	bool DBMS::CreateIndex(const std::string & index, const std::string & table, const std::string & column)
	{
		CatalogManager::getInstance().createIndex(index, table, column);
		return true;
	}

	bool DBMS::DropIndex(const std::string & index)
	{
		CatalogManager::getInstance().dropIndex(index);
		return true;
	}




}