#include "SchoolNet.h"

SchoolNet::SchoolNet(std::string Account, std::string Password)
{
	this->Account = Account;
	this->Password = Password;
}

SchoolNet::SchoolNet(std::string Account, std::string Password, std::string SchoolId, std::string SchoolDomain, std::string SchoolArea)
{
	this->Account = Account;
	this->Password = Password;
	if (SchoolId != "")
	{
		this->SchoolId = SchoolId;
	}
	if (SchoolDomain != "")
	{
		this->SchoolDomain = SchoolDomain;
	}
	if (SchoolArea != "")
	{
		this->SchoolArea = SchoolArea;
	}
}

std::string SchoolNet::GetLocationUrl()
{
	httplib::Client cli(GDDXUrl);
	auto res = cli.Get("/", { {"User-Agent", UserAgent} });
	if (res->has_header("Location"))
	{
		return res->get_header_value("Location");
		// http://125.88.59.131:10001/?wlanuserip=172.17.153.124&wlanacip=183.3.151.148
	}
	return std::string();
}

void SchoolNet::Go()
{
	Indexcgi();
	auto res = Ticketcgi();
	std::cout << res << std::endl;
}

std::string SchoolNet::Indexcgi()
{
	std::string GDDXUrl = GetLocationUrl();
	if (GDDXUrl != "")
	{
		auto uri = uri::parse_uri(GDDXUrl);
		httplib::Client cli(Url);
		// http://121.8.177.212:7001/index.cgi?wlanuserip=172.17.153.124&wlanacip=183.3.151.148&portal_node=http://125.88.59.131:10002
		httplib::Headers headers = {
			{"User-Agent", UserAgent},
			{"CDC-SchoolId", SchoolId},
		};
		if (auto res = cli.Get("/index.cgi?" + uri.fragment + "&portal_node=" + PortalNode, headers))
		{
			std::smatch regex_results;
			if (std::regex_match(res->body, regex_results, std::regex(".*<ticket-url><!\\[CDATA\\[(.*)\\]\\]></ticket-url>.*<auth-url><!\\[CDATA\\[(.*)\\]\\]></auth-url>.*")))
			{
				TicketUrl = regex_results[1].str(); // http://121.8.177.212:7001/ticket.cgi?wlanuserip=172.17.153.124&wlanacip=183.3.151.148&portal_node=http://125.88.59.131:10002
				AuthUrl = regex_results[2].str();   // http://121.8.177.212:7001/auth.cgi
			}
			else
			{
				return std::string();
			}
		}
		else
		{
			std::cout << res.error() << std::endl;
		}
	}
	return std::string();
}

std::string SchoolNet::Ticketcgi()
{
	auto uri = uri::parse_uri(TicketUrl);
	httplib::Client cli("http://"+uri.authority.authority);
	std::string clientid = MagicBox::generate_uuid();
	Wlanuserip = uri.query.at("wlanuserip");
	httplib::Headers headers = {
		{"User-Agent", UserAgent},
		{"Algo-ID", AlgoID},
		{"Client-ID", clientid},
		{"CDC-Checksum", MagicBox::generate_checksum(UserAgent,clientid,Wlanuserip)},
		{"CDC-SchoolId", SchoolId},
		{"CDC-Domain", SchoolDomain},
		{"CDC-Area", SchoolArea},
	};
	httplib::Params params = {"key","text"};
	auto res = cli.Post("/ticket.cgi?" + uri.fragment, headers,params);
	return res->body;
}
