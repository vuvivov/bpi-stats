
#include "statistic.h"
#include "priceDatabase.h"
#include "dataRetriever.h"


#define BOOST_TEST_MODULE test_bpistats
#include <boost/test/included/unit_test.hpp>
namespace utf = boost::unit_test;
namespace tt = boost::test_tools;


BOOST_AUTO_TEST_SUITE(test_statistic)

BOOST_AUTO_TEST_CASE(test_highLow, *utf::tolerance(0.00001))
{
	highLow temp;
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	auto result = temp.toStrings();
	BOOST_TEST(result.front().at(1) == "1.000000\n2011-01-01");
	BOOST_TEST(result.back().at(1) == "1.000000\n2011-01-01");
	for (auto i = 0; i < 3; i++)
	{
		temp.accumulate(std::make_pair(boost::gregorian::from_string("2012-01-01"), .5));
		result = temp.toStrings();
	}
	BOOST_TEST(result.front().at(1) == "1.000000\n2011-01-01");
	BOOST_TEST(result.back().at(1) == "0.500000\n2012-01-01\n2012-01-01\n2012-01-01");
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2012-01-02"), .5));
	result = temp.toStrings();
	BOOST_TEST(result.front().at(1) == "1.000000\n2011-01-01");
	BOOST_TEST(result.back().at(1) == "0.500000\n2012-01-01\n2012-01-01\n2012-01-01\n2012-01-02");
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2013-01-01"), .1));
	result = temp.toStrings();
	BOOST_TEST(result.front().at(1) == "1.000000\n2011-01-01");
	BOOST_TEST(result.back().at(1) == "0.100000\n2013-01-01");
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2014-01-01"), 1.1));
	result = temp.toStrings();
	BOOST_TEST(result.front().at(1) == "1.100000\n2014-01-01");
	BOOST_TEST(result.back().at(1) == "0.100000\n2013-01-01");
}


BOOST_AUTO_TEST_CASE(test_nPoints)
{
	nPoints temp;
	for (auto i = 1; i < 9; i++)
	{
		temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 111));
		BOOST_TEST(temp.toStrings().front().at(1) == std::to_string(i));
	}
}

BOOST_AUTO_TEST_CASE(test_averagePrice, *utf::tolerance(0.00001))
{
	averagePrice temp;
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	auto result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1);
	for (auto i = 0; i < 9; i++)
	{
		temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	}
	result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1);
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 10));
	result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1.8181818);
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), -20));
	result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 0);
}

BOOST_AUTO_TEST_CASE(test_medianPrice, *utf::tolerance(0.00001))
{
	medianPrice temp;
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	auto result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1);
	for (auto i = 2; i <= 9; i++)
	{
		temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
		result = temp.toStrings();
		BOOST_TEST(std::stod(result.front().at(1)) == 1);
	}

	medianPrice temp2;
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1);
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 2));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1.5);
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 3));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 2);
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 4));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 2.5);
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 5));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 3);
}


BOOST_AUTO_TEST_CASE(test_stdDevPrice, *utf::tolerance(0.00001))
{
	stdDevPrice temp;
	temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	auto result = temp.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 0);
	for (auto i = 2; i <= 9; i++)
	{
		temp.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
		result = temp.toStrings();
		BOOST_TEST(std::stod(result.front().at(1)) == 0);
	}

	stdDevPrice temp2; 
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 0);
	temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), -1));
	result = temp2.toStrings();
	BOOST_TEST(std::stod(result.front().at(1)) == 1);
	for (auto i = 0; i < 9; i++)
	{
		temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), 1));
		temp2.accumulate(std::make_pair(boost::gregorian::from_string("2011-01-01"), -1));
		result = temp2.toStrings();
		BOOST_TEST(std::stod(result.front().at(1)) == 1);
	}
}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(test_priceDatabase)

BOOST_AUTO_TEST_CASE(test_priceDatabaseMap_empty, *utf::tolerance(0.00001))
{
	priceDatabaseMap dataSet;
	BOOST_TEST(dataSet.getAll().empty());
}

BOOST_AUTO_TEST_CASE(test_priceDatabaseMap_pushAndGet, *utf::tolerance(0.00001))
{
	priceDatabaseMap dataSet;
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-05"), 1));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-02"), 3));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-03"), 2));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-03"), 22));//duplicate, ignore
	auto out = dataSet.getAll();
	BOOST_TEST(out.size()==3);
	auto it = out.begin();
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-02");
	BOOST_TEST(it->second == 3);
	it++;
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-03");
	BOOST_TEST(it->second == 2);
	it++;
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-05");
	BOOST_TEST(it->second == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_dataRetriever)
BOOST_AUTO_TEST_CASE(test_coinDeskDataRetriever, *utf::tolerance(0.00001))
{
	auto rawData = "{abcpi{\"2011-01-05\":1,\"2011-01-02\":3,\"2011-01-03\":2,\"2011-01-03\":111}sjdgi}we{sadf{sadf:dsaf}}";
	coinDeskDataRetriever temp;
	priceDatabaseMap dataSet;
	temp.retrieveFromString(rawData, std::make_shared<decltype(dataSet)>(dataSet));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-05"), 1));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-02"), 3));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-03"), 2));
	dataSet.push(std::make_pair(boost::gregorian::from_string("2011-01-03"), 22));//duplicate, ignore
	auto out = dataSet.getAll();
	BOOST_TEST(out.size() == 3);
	auto it = out.begin();
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-02");
	BOOST_TEST(it->second == 3);
	it++;
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-03");
	BOOST_TEST(it->second == 2);
	it++;
	BOOST_TEST(boost::gregorian::to_iso_extended_string(it->first) == "2011-01-05");
	BOOST_TEST(it->second == 1);
}

BOOST_AUTO_TEST_SUITE_END()
