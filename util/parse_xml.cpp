#include "MQPool/parse_xml.h"
#include <boost/property_tree/xml_parser.hpp>


ParseXmlObj::ParseXmlObj() {
	needDelete_ = true;
    configPath_ = "../config/messageQueue.xml";
    pt_ = new ptree;
    boost::property_tree::read_xml(configPath_, *pt_);
}

ParseXmlObj::ParseXmlObj(string configPath)
    : configPath_(configPath){
	needDelete_ = true;
	pt_ = new ptree;
	boost::property_tree::read_xml(configPath_, *pt_);
}

ParseXmlObj::ParseXmlObj(ptree* it)
    : pt_(it){
	// 多加一个成员needDelete的意义就在这里，当使用ptree指针来初始化ParseXmlObj对象的时候，就不能delete了
	// 否则会导致传给这个构造函数的指针成为野指针
	needDelete_ = false;
	configPath_ = "Constructe by ptree pointer";
}

ParseXmlObj::~ParseXmlObj() {
  if (needDelete_ && pt_ != NULL) {
    delete pt_;
  }
}

void ParseXmlObj::Dump() const {
  printf("\n=====ParseXmlObj Dump START ========== \n");
  printf("configPath__=%s ", configPath_.c_str());
  printf("pt_=%p ", pt_);
  printf("\n===ParseXMlObj DUMP END ============\n");
}

string ParseXmlObj::GetConfigPath() const {
	return configPath_;
}

ptree* ParseXmlObj::GetPtree() const {
	return pt_;
}

ptree::iterator ParseXmlObj::Begin() const {
  if (pt_ == NULL) {
	  return ptree::iterator();
  }
  return pt_->begin();
}

ptree::iterator ParseXmlObj::End() const {
  if (pt_ == NULL) {
	  return ptree::iterator();
  }
  return pt_->end();
}

string ParseXmlObj::GetChildKey(const ptree::iterator &it) {
  if(it == End()) {
    return string();
  }
  return it->first;
}

string ParseXmlObj::GetChildData(const ptree::iterator &it) {
  if(it == End()) {
    return string();
  }
  return it->second.data();
}

ParseXmlObj ParseXmlObj::GetChild(const ptree::iterator &it) {
  if(it == End()) {
    return ParseXmlObj(NULL);
  }
  return ParseXmlObj((&it->second));
}

ParseXmlObj ParseXmlObj::GetChild(const string& path) {
  if (pt_ == NULL) {
	return ParseXmlObj(NULL);
  }
  auto child = pt_->get_child_optional(path);
  if(child) {
    return ParseXmlObj(&(child.get()));
  }
  return ParseXmlObj(NULL);
}

string ParseXmlObj::GetAttr(string path, const string& attr) {
  if(pt_ == NULL || attr == "")
    return "";
  if(path != ""){
    path += "."; //add the domain descriptor
  }
  path += "<xmlattr>.";
  path += attr;
  return pt_->get<string>(path, "");
}

string ParseXmlObj::GetValue(const string& path) {
  if(pt_ == NULL){
	return "";
  }
  return pt_->get<string>(path, "");
}

void ParseXmlObj::PutChild(const string& key, const ParseXmlObj& child) {
  if(pt_ != NULL) {
    pt_->push_back(make_pair(key, *(child.GetPtree())));
  }
}

bool ParseXmlObj::PutAttr(string path, const string& attribute, const string& attrvalue) {
  if(pt_ == NULL || attribute == "") {
	return false;
  }
  try{
	if(path != "") {
	  path += "."; //add the domain descriptor
	}
	path += "<xmlattr>.";
	path += attribute;
	pt_->put<string>(path, attrvalue);
	return true;
  } catch(...) {
	//        cerr << "PutAttr error" << endl;
	return false;
  }
}

bool ParseXmlObj::PutValue(const string& path, const string& value) {
  if(pt_ == NULL) {
	        return false;
  }
  try{
    pt_->put<string>(path, value);
	return true;
  } catch(...) {
	//        cerr << "PutValue error" << endl;
  return false;
  }
}

bool ParseXmlObj::AddValue(const string& path, const string& value) {
  if(pt_ == NULL || path.empty()) {
    return false;
  }
  try{
    pt_->add<string>(path, value);
    return true;
    } catch(...) {
//        cerr << "AddValue error" << endl;
    return false;
  }
}

void ParseXmlObj::SaveConfig(const string& configPath) {
  if(pt_ != NULL) {
    boost::property_tree::write_xml(configPath, *pt_);
  }
}
