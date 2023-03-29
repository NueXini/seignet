#pragma once

#include "httplib.hpp"
#include "URI.hpp"
#include "MagicBox.h"

#include <iostream>
#include <regex>

class SchoolNet
{
public:
  // const std::string SchoolNetUrl = "http://enet.10000.gd.cn";
  const std::string GDDXUrl = "http://www.189.cn";
  const std::string UserAgent = "CCTP/WinSVR4/1057";
  const std::string AlgoID = "00000000-0000-0000-0000-000000000000";
  // const std::string ClientID = "00000000-0000-0000-0000-000000000000";
  const std::string Url = "http://121.8.177.212:7001";
  const std::string PortalNode = "http://125.88.59.131:10002";
  std::string Wlanuserip = "";
  std::string SchoolId = "1099";
  std::string SchoolDomain = "sise.cn";
  std::string SchoolArea = "020";
  std::string Checksum = "";
  std::string TicketUrl = "";
  std::string AuthUrl = "";

  SchoolNet(std::string Account, std::string Password);
  SchoolNet(std::string Account, std::string Password, std::string SchoolId, std::string SchoolDomain, std::string SchoolArea);

  void Go();
  std::string GetLocationUrl();

private:
  std::string Account;
  std::string Password;
  // Function
  std::string Indexcgi();
  std::string Ticketcgi();
};
