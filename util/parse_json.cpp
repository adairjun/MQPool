#include "MQPool/parse_json.h"
#include <boost/property_tree/json_parser.hpp>


ParseJsonObj::ParseJsonObj() {
	needDelete_ = true;
    configPath_ = "../config/messageQueue.json";
    pt_ = new ptree;
    boost::property_tree::read_json(configPath_, *pt_);
}

ParseJsonObj::ParseJsonObj(string configPath)
    : configPath_(configPath){
	needDelete_ = true;
	pt_ = new ptree;
	boost::property_tree::read_json(configPath_, *pt_);
}

ParseJsonObj::ParseJsonObj(ptree* it)
    : pt_(it){
	// 多加一个成员needDelete的意义就在这里，当使用ptree指针来初始化ParseJsonObj对象的时候，就不能delete了
	// 否则会导致传给这个构造函数的指针成为野指针
	needDelete_ = false;
	configPath_ = "Constructe by ptree pointer";
}

ParseJsonObj::~ParseJsonObj() {
  if (needDelete_ && pt_ != NULL) {
    delete pt_;
  }
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

ptree::iterator ParseJsonObj::Begin() const {
  if (pt_ == NULL) {
	return ptree::iterator();
  }
  return pt_->begin();
}

ptree::iterator ParseJsonObj::End() const {
  if (pt_ == NULL) {
	return ptree::iterator();
  }
  return pt_->end();
}

string ParseJsonObj::GetChildKey(const ptree::iterator &it) {
  if(it == End()) {
    return string();
  }
  return it->first;
}

string ParseJsonObj::GetChildData(const ptree::iterator &it) {
  if(it == End()) {
    return string();
  }
  return it->second.data();
}

ParseJsonObj ParseJsonObj::GetChild(const ptree::iterator &it) {
  if(it == End()) {
    return ParseJsonObj(NULL);
  }
  return ParseJsonObj((&it->second));
}

ParseJsonObj ParseJsonObj::GetChild(const string& path) {
  if (pt_ == NULL) {
	return ParseJsonObj(NULL);
  }
  auto child = pt_->get_child_optional(path);
  if(child) {
    return ParseJsonObj(&(child.get()));
  }
  return ParseJsonObj(NULL);
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
