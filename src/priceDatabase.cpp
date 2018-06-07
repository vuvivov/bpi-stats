
#include "priceDatabase.h"

void priceDatabaseMap::push(pricePoint _in)
{
	m_data.try_emplace(_in.first,_in.second);
}

std::vector<pricePoint> priceDatabaseMap::getAll()
{
	return std::vector<pricePoint>(std::begin(m_data), std::end(m_data));
}
