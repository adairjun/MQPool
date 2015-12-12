#include "MQPool/parse_json.h"
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
  ParseJsonObj myJson("../config/socket.json");
  vector<map<string, string> > result_array = myJson.GetChildDataArray("Config.Server.Connection");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }
//
//  ParseJsonObj yourJson("../config/database.json");
//  map<string, string> key_value_map = yourJson.GetChildData("Config.MysqlConnection");
//  for (auto the_pair : key_value_map) {
//    cout << the_pair.first << "---" << the_pair.second << endl;
//  }
  //这里我使用boost来解析xml和json配置文件,也可以使用rapidxml或者rapidjson
  // 从配置文件socket.xml当中读入mysql的ip, 用户, 密码, 数据库名称,	
  boost::property_tree::ptree pt;	
  const char* json_path = "../config/socket.json";
  boost::property_tree::read_json(json_path, pt);
  BOOST_AUTO(child, pt.get_child("Config.Server.Connection"));
   //这里应该遍历数组
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, child.get_child("")) {
cout << v.first << endl;
      BOOST_AUTO(nextchild, v.second.get_child(""));
      for (BOOST_AUTO(pos, nextchild.begin()); pos!= nextchild.end(); ++pos) {
cout << pos->first << "--------" << pos->second.data() << endl;
      } 
    }
  return 0;
}
