
#include "statistic.h"

std::list<std::array<std::string,2>> statistic::toStrings()
{
	toStringsDetail();
	if (!m_count)
	{
		for (auto &&i : m_output)
		{
			i.at(1)="no data!";
		}
	}
	return m_output;
}


std::list<std::list<std::array<std::string,2>>> statisticsManager::print()
{
	decltype(print()) ret;
	for (auto &&stat : m_statistics)
	{
		ret.push_back(stat->toStrings());
	}
	return ret;
}


void statisticsManager::accumulate(const pricePoint _data)
{
	for (auto &&stat : m_statistics)
	{
		stat->accumulate(_data);
	}
}


bluzelleStatisticsManager::bluzelleStatisticsManager()
{
	m_statistics.push_back(std::make_unique<highLow>());
	m_statistics.push_back(std::make_unique<nPoints>());
	m_statistics.push_back(std::make_unique<averagePrice>());
	m_statistics.push_back(std::make_unique<medianPrice>());
	m_statistics.push_back(std::make_unique<stdDevPrice>());
}

void highLow::accumulateDetail(const pricePoint _data)
{
	if (_data.second == m_high)
	{
		m_highDates.push_back(_data.first);
	}
	else if (_data.second > m_high)
	{
		m_high = _data.second;
		m_highDates.clear();
		m_highDates.push_back(_data.first);
	}

	if (_data.second == m_low)
	{
		m_lowDates.push_back(_data.first);
	}
	else if (_data.second < m_low)
	{
		m_low = _data.second;
		m_lowDates.clear();
		m_lowDates.push_back(_data.first);
	}
}


void highLow::toStringsDetail()
{
	auto it = m_output.begin();
	it->at(1) = std::to_string(m_high);
	for (auto &i : m_highDates)
	{
		it->at(1) += '\n';
		it->at(1) += to_iso_extended_string(i);
	}
	it++;
	it->at(1) = std::to_string(m_low);
	for (auto &i : m_lowDates)
	{
		it->at(1) += '\n';
		it->at(1) += to_iso_extended_string(i);
	}
}

template<class T>
void simpleStat<T>::toStringsDetail()
{ 
	calculate(); 
	m_output = decltype(m_output){ {m_name, std::to_string(m_data)}};
};


void medianPrice::calculate()
{
	if (!m_count)
	{
		return;
	}
	std::vector<double> temp{ std::begin(m_prices), std::end(m_prices) };
	auto i1 = temp.size() / 2;
	auto i2=i1- (temp.size()+1) % 2;
	std::nth_element(temp.begin(), temp.begin() + i1+1, temp.end());
	std::nth_element(temp.begin(), temp.begin() + i2+1, temp.end());
	m_data = (temp.at(i1) + temp.at(i2)) / 2;
}
