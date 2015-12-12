#include "MQPool/parse_json.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
  ParseJsonObj myJson("../config/socket.json");
  vector<map<string, string> > result_array = myJson.GetChildDataArray("Config.Server");
  for (auto key_value_map : result_array) {
    for (auto the_pair : key_value_map) {
      cout << the_pair.first << "---" << the_pair.second << endl;
    } 
  }

  ParseJsonObj yourJson("../config/database.json");
  map<string, string> key_value_map = yourJson.GetChildData("Config.MysqlConnection");
  for (auto the_pair : key_value_map) {
    cout << the_pair.first << "---" << the_pair.second << endl;
  }

  return 0;
}
