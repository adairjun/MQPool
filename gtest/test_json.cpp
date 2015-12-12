#include "MQPool/parse_json.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
  ParseJsonObj myJson("../config/testJson.json");
  vector<map<string, string> > result_array = myJson.GetChildDataArray("Config.Server.Connection");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }

 result_array = myJson.GetChildDataArray("Config.Client.Connection");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }

  cout << "=========" << endl;
  cout << myJson.GetChildData("hello.MysqlConnection.IP") << endl;
  cout << "=========" << endl;

  map<string, string> key_value_map = myJson.GetChildDataMap("hello.MysqlConnection");
  for (auto the_pair : key_value_map) {
    cout << the_pair.first << "---" << the_pair.second << endl;
  }

//==================== put ====================

  myJson.PutChildData("testPut", "testPut");
  myJson.PutChildData("testPut2", "testPut2");

  map<string, string> my_map;
  my_map.insert(make_pair("IP","0.0.0.1"));
  my_map.insert(make_pair("Port","1"));
  my_map.insert(make_pair("max_con","1"));
  myJson.PutChildDataMap("hello.TestConnection", my_map);

  map<string, string> the_map;
  the_map.insert(make_pair("IP", "0.0.0.1"));
  the_map.insert(make_pair("Port", "0"));
  the_map.insert(make_pair("Backlog", "1"));
  map<string, string> hello_map;
  hello_map.insert(make_pair("IP", "0.0.0.1"));
  hello_map.insert(make_pair("Port", "0"));
  hello_map.insert(make_pair("Backlog", "1"));
  vector<map<string, string> > the_vector;
  the_vector.push_back(the_map); 
  the_vector.push_back(hello_map); 
  myJson.PutChildDataArray("Config.Server.Nee", the_vector);


  myJson.SaveConfig();
  return 0;
}
