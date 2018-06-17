#include "include/DBMS.h"


using namespace std;

namespace Photon 
{
	std::vector<Row> Photon::DBMS::Select(const Condition & c)
	{
		return CatalogManager::getInstance().select(c);
	}


	uint Photon::DBMS::Insert(const std::string & table, const std::vector<Row>& res)
	{	
		Table* tobeinserted = &(CatalogManager::getInstance().getTable(table));
		tobeinserted->addRow(res);
		return 1;
	}

	uint Photon::DBMS::Delete(const Condition & c)
	{
		
	}

	bool Photon::DBMS::CreateTable(const std::string & table, const std::vector<Column> columns)
	{
		CatalogManager::getInstance().createTable(table, columns);
		return true;
	}


	bool Photon::DBMS::DropTable(const std::string & table)
	{
		CatalogManager::getInstance().dropTable(table);
		return true;
	}


	bool Photon::DBMS::CreateIndex(const std::string & index, const std::string & table, const std::string & column)
	{
		CatalogManager::getInstance().createIndex(index, table, column);
		return true;
	}

	bool Photon::DBMS::DropIndex(const std::string & index)
	{
		CatalogManager::getInstance().dropIndex(index);
		return true;
	}

}