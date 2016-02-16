#include "MQueue/parse_xml.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
  ParseXmlObj myXml("../config/testXml.xml");
  vector<map<string, string> > result_array = myXml.GetChildDataArray("Config.Server.Connection");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }

 result_array = myXml.GetChildDataArray("Config.Client.Connection");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }

  cout << "=========" << endl;
  cout << myXml.GetChildData("hello.MysqlConnection.IP") << endl;
  cout << "=========" << endl;

  map<string, string> key_value_map = myXml.GetChildDataMap("hello.MysqlConnection");
  for (auto the_pair : key_value_map) {
    cout << the_pair.first << "---" << the_pair.second << endl;
  }

  cout << "+++++++++" << endl;
  cout << myXml.GetAttr("errors.error", "id") << endl;
  cout << "+++++++++" << endl;

  vector<string> attr_vec = myXml.GetAttrArray("errors", "id");
  for (auto i : attr_vec) {
    cout << i << endl;
  }

  cout << myXml.GetAttrByAttr("errors", "id", "DB_ERROR_EXECUTE", "value") << endl;

//==================== put ====================

  myXml.PutChildData("testPut", "testPut");
  myXml.PutChildData("testPut2", "testPut2");

  map<string, string> my_map;
  my_map.insert(make_pair("IP","0.0.0.1"));
  my_map.insert(make_pair("Port","1"));
  my_map.insert(make_pair("max_con","1"));
  myXml.PutChildDataMap("hello.TestConnection", my_map);

  myXml.PutAttr("errors.error", "test", "ttttt");

  cout << myXml.GetAttrByAttr("errors", "id", "DB_ERROR_EXECUTE","value") << endl;

  map<string, string> other_map;
  other_map.insert(make_pair("id", "iiiii"));
  other_map.insert(make_pair("value", "000"));
  other_map.insert(make_pair("prompt", "ppppp"));
  other_map.insert(make_pair("msg", "mmmm"));
  myXml.AddLineByAttr("errors.error", other_map);


  myXml.SaveConfig();
  return 0;
}
