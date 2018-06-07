
#include "app.h"
#include "dataRetriever.h"
#include "priceDatabase.h"
#include "statistic.h"
#include "printer.h"

void app::run(int argc, const char** argv)
{
	//parse argument
	if (argc<2 || argc > 3)
	{
		throw std::runtime_error("Invalid input argument! Please see --help.\n");
	}
	if (argc == 2)
	{
		//--help TODO: use <boost/program_options.hpp>
		if(std::string(argv[1])=="--help")
		{
			*m_out << helpInfo();
			return;
		}
		//from file
		m_dataRetriever->retrieveFromFile(argv[1], m_priceDatabase);
	}
	else//from internet with start/end date
	{
		m_dataRetriever->retrieveFromWeb(boost::gregorian::from_simple_string(argv[1]), boost::gregorian::from_simple_string(argv[2]), m_priceDatabase);
	}

	auto data = m_priceDatabase->getAll();
	for (auto &i : data)
	{
		m_statisticsManager->accumulate(i);
	}

	m_printer->print(m_statisticsManager->print(), *m_out);
}

std::string app::helpInfo()
{
	auto ret = std::string("This application processes bpi price data and displays statistics.\n");
	ret += decltype(ret)("<Option 1: Input data from file>\n");
	ret += decltype(ret)("Provide a file path as argument (e.g. \"prices.json\"). The file needs to contain data in the format of https://api.coindesk.com/v1/bpi/historical/close.json?start=2018-01-01&end=2018-01-20\n");
	ret += decltype(ret)("<Option 2: Fetch data from web>\n");
	ret += decltype(ret)("Input two arguments representing start and end date(e.g. \"2018-01-04 2018-01-08\")"); 
	return ret;
}


appForBluzelle::appForBluzelle()
{
	m_dataRetriever.reset(new coinDeskDataRetriever);
	m_priceDatabase.reset(new priceDatabaseMap);
	m_statisticsManager.reset(new bluzelleStatisticsManager);
	m_printer.reset(new basicPrinter);
	m_out = decltype(m_out)(&std::cout, [](void*) {});
}
