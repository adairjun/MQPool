#include "MQPool/parse_xml.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using std::cout;
using std::endl;

int main() {
  ParseXmlObj myXml("../config/testXml.xml");
//  vector<map<string, string> > result_array = myXml.GetChildDataArray("Config.Server.Connection");
//  for (auto key_value_map : result_array) {
//    for (auto the_pair : key_value_map) {
//      cout << the_pair.first << "---" << the_pair.second << endl;
//    } 
//  }
//
// result_array = myXml.GetChildDataArray("Config.Client.Connection");
//  for (auto key_value_map : result_array) {
//    for (auto the_pair : key_value_map) {
//      cout << the_pair.first << "---" << the_pair.second << endl;
//    } 
//  }
//
//  cout << "=========" << endl;
//  cout << myXml.GetChildData("hello.MysqlConnection.IP") << endl;
//  cout << "=========" << endl;
//
//  map<string, string> key_value_map = myXml.GetChildDataMap("hello.MysqlConnection");
//  for (auto the_pair : key_value_map) {
//    cout << the_pair.first << "---" << the_pair.second << endl;
//  }
//
//  cout << "+++++++++" << endl;
//  cout << myXml.GetAttr("errors.error", "id") << endl;
//  cout << "+++++++++" << endl;
//
////==================== put ====================
//
//  myXml.PutChildData("testPut", "testPut");
//  myXml.PutChildData("testPut2", "testPut2");
//
//  map<string, string> my_map;
//  my_map.insert(make_pair("IP","0.0.0.1"));
//  my_map.insert(make_pair("Port","1"));
//  my_map.insert(make_pair("max_con","1"));
//  myXml.PutChildDataMap("hello.TestConnection", my_map);
//
//  myXml.PutAttr("errors.error", "test", "ttttt");
//
//  myXml.SaveConfig();
//
//===========================================
   
  cout << myXml.GetAttrByAttr("errors", "id", "DB_ERROR_EXECUTE","value") << endl;

  vector<string> result = myXml.GetAttrArray("errors", "id");
  for (auto i : result) {
    cout << i << endl; 
  }
  return 0;
}
