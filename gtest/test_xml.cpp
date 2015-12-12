#include <iostream>
#include "MQPool/parse_xml.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char** argv) {
   ParseXmlObj myXml("../config/socket.xml");
   vector<map<string, string> > result_array = myXml.GetChildDataArray("Config.Server");
   for (auto key_value_map : result_array) {
     for (auto the_pair : key_value_map) {
       cout << the_pair.first << "--------- " << the_pair.second << endl;
     }
   }
  
   ParseXmlObj yourXml("../config/database.xml");
   map<string, string> key_value_map = yourXml.GetChildData("Config.MysqlConnection");
     for (auto the_pair : key_value_map) {
       cout << the_pair.first << "--------- " << the_pair.second << endl;
     }
   
  return 0;
}
