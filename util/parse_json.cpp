#include "MQPool/parse_json.h"
#include <boost/property_tree/json_parser.hpp>


ParseJsonObj::ParseJsonObj() {
    configPath_ = "../config/messageQueue.json";
    pt_ = new ptree;
    boost::property_tree::read_json(configPath_, *pt_);
}

ParseJsonObj::ParseJsonObj(string configPath)
    : configPath_(configPath){
	pt_ = new ptree;
	boost::property_tree::read_json(configPath_, *pt_);
}

ParseJsonObj::~ParseJsonObj() {
  delete pt_;
}

void ParseJsonObj::Dump() const {
  printf("\n=====ParseJsonObj Dump START ========== \n");
  printf("configPath__=%s ", configPath_.c_str());
  printf("pt_=%p ", pt_);
  printf("\n===ParseJsonObj DUMP END ============\n");
}

string ParseJsonObj::GetConfigPath() const {
  return configPath_;
}

ptree* ParseJsonObj::GetPtree() const {
  return pt_;
}

map<string, string> ParseJsonObj::GetChildData(const string& path) {
  map<string, string> key_value_map;
  
  auto child = pt_->get_child(path);
  for (auto pos = child.begin(); pos!= child.end(); ++pos) {
    key_value_map.insert(make_pair(pos->first, pos->second.data()));
  }
  return std::move(key_value_map);
}

vector<map<string, string> > ParseJsonObj::GetChildDataArray(const string& path) {
  vector<map<string, string> > result_array;
  map<string, string> key_value_map;

  auto child = pt_->get_child(path);
    for (ptree::value_type &v : child.get_child("")) {
      auto nextchild = v.second.get_child("");
      for (auto pos = nextchild.begin(); pos!= nextchild.end(); ++pos) {
        key_value_map.insert(make_pair(pos->first, pos->second.data()));        
      }
      result_array.push_back(key_value_map);
      key_value_map.clear();
    }
  return std::move(result_array);
}

void ParseJsonObj::PutChild(const string& key, const ParseJsonObj& child) {
  if(pt_ != NULL) {
    pt_->push_back(make_pair(key, *(child.GetPtree())));
  }
}

void ParseJsonObj::SaveConfig(const string& configPath) {
  if(pt_ != NULL) {
    boost::property_tree::write_json(configPath, *pt_);
  }
}
