#include "MQPool/parse_json.h"
#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

using std::cout;
using std::endl;

int main() {
  ParseJsonObj myJson("../config/socket.json");
  ParseJsonObj child = myJson.GetChild("Config.Server");
  for (auto pos = child.Begin(); pos!= child.End(); ++pos) {
cout << pos->first << endl;
    for(auto v: child.GetChild("Connection")) {
cout << v << endl;
    }
  }




  /*
  boost::property_tree::ptree pt;
  const char* json_path = "../config/socket.json";
  boost::property_tree::read_json(json_path, pt);
  BOOST_AUTO(child, pt.get_child("Config.Server"));
  for (BOOST_AUTO(pos, child.begin()); pos!= child.end(); ++pos) {
   //这里应该遍历数组
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pos->second.get_child("")) {
      BOOST_AUTO(nextchild, v.second.get_child(""));
      for (BOOST_AUTO(nextpos, nextchild.begin()); nextpos!= nextchild.end(); ++nextpos) {
  	    //if (nextpos->first == "IP") serverHost_ = nextpos->second.data();
  	    //if (nextpos->first == "Port") serverPort_ = boost::lexical_cast<unsigned>(nextpos->second.data());
  	    //if (nextpos->first == "Backlog") serverBacklog_ = boost::lexical_cast<int>(nextpos->second.data());
        cout << nextpos->first << endl;
      } 
    }
  }
  */
  return 0;
}
