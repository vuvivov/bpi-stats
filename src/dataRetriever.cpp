#include "dataRetriever.h"
#include "priceDatabase.h"
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <boost/algorithm/string/find.hpp>
#include <boost/tokenizer.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <regex>
#include <string>

bool dataRetriever::retrieveFromString(std::string _source, std::shared_ptr<priceDatabase> _target)
{
	m_rawData = _source;
	return parse(_target);
}

bool dataRetriever::retrieveFromFile(std::string _fileName, std::shared_ptr<priceDatabase> _target)
{
	std::ifstream t(_fileName);
	m_rawData = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return parse(_target);
}


namespace
{
	const std::regex url_regex{
		R"((?:https://)?)"      // Throw away http(s):// and www. if they're there
		R"((?:http://)?)"
		R"((?:www\.)?)"
		R"(([^/\.]+\.[^/]+))"  // Domain must be something.something
		R"((/.*)?)"            // Target, if present, must be /something
	};
}

bool dataRetriever::retrieveFromWeb(boost::gregorian::date _start, boost::gregorian::date _end, std::shared_ptr<priceDatabase> _target)
{
	auto url = getURL(_start, _end);

	//code referenced from: https://github.com/bluzelle/swarmDB/tree/devel/utils
	using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
	namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

	boost::asio::io_context ioc;
	tcp::resolver resolver{ ioc };
	tcp::socket socket{ ioc };

	std::smatch what;
	if (!std::regex_match(url, what, url_regex))
	{
		throw std::runtime_error("could not parse url " + url);
	}

	std::string host = what[1];
	std::string target = what[2];

	auto const results = resolver.resolve(host, "80");
	boost::asio::connect(socket, results.begin(), results.end());

	http::request<http::string_body> req{ http::verb::get, target, 11 };
	req.set(http::field::host, host);
	req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	http::write(socket, req);

	boost::beast::flat_buffer buffer;
	http::response<http::string_body> res;

	http::read(socket, buffer, res);

	socket.shutdown(tcp::socket::shutdown_both);
	//END code referenced from: https://github.com/bluzelle/swarmDB/tree/devel/utils

	m_rawData = res.body();
	return parse(_target);
}


bool coinDeskDataRetriever::parse(std::shared_ptr<priceDatabase> _target)const
{
	//look for prices
	auto iBegin = std::distance(m_rawData.begin(), boost::algorithm::find_nth(m_rawData, "\{", 1).begin()) + 1;
	auto iEnd = std::distance(m_rawData.begin(), boost::algorithm::find_nth(m_rawData, "\}", 0).begin()) - 1;
	auto prices = m_rawData.substr(iBegin, iEnd - iBegin + 1);

	//process string
	boost::tokenizer<boost::escaped_list_separator<char> > tk(prices, boost::escaped_list_separator<char>('\\', ',', '\"'));
	for (auto &i : tk)
	{
		auto iSeperate = i.find(':');
		_target->push(std::make_pair(boost::gregorian::from_string(i.substr(0, iSeperate)), std::stod(i.substr(iSeperate + 1))));
	}
	return true;
}

std::string coinDeskDataRetriever::getURL(boost::gregorian::date _start, boost::gregorian::date _end) const
{
	std::string ret = "https://api.coindesk.com/v1/bpi/historical/close.json?start=";
	ret += boost::gregorian::to_iso_extended_string(_start);
	ret += "&end=";
	ret += boost::gregorian::to_iso_extended_string(_end);
	return ret;
}
