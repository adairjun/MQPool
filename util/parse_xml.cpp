#include "MQPool/parse_xml.h"
#include <boost/property_tree/xml_parser.hpp>


ParseXmlObj::ParseXmlObj() {
    configPath_ = "../config/messageQueue.xml";
    pt_ = new ptree;
    boost::property_tree::read_xml(configPath_, *pt_);
}

ParseXmlObj::ParseXmlObj(string configPath)
    : configPath_(configPath){
	pt_ = new ptree;
	boost::property_tree::read_xml(configPath_, *pt_);
}

ParseXmlObj::~ParseXmlObj() {
  delete pt_;
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

map<string, string> ParseXmlObj::GetChildData(const string& path) {
  map<string, string> key_value_map;
 
  auto child = pt_->get_child(path);
  for (auto pos = child.begin(); pos!= child.end(); ++pos) {
    key_value_map.insert(make_pair(pos->first, pos->second.data()));
  }
  return std::move(key_value_map);
}

vector<map<string, string> > ParseXmlObj::GetChildDataArray(const string& path) {
  vector<map<string, string> > result_array;
  map<string, string> key_value_map;
 
  auto child = pt_->get_child(path);
  for (auto pos = child.begin(); pos!= child.end(); ++pos) {
    auto nextchild = pos->second.get_child("");
    for (auto nextpos = nextchild.begin(); nextpos!= nextchild.end(); ++nextpos) {
      key_value_map.insert(make_pair(nextpos->first, nextpos->second.data()));
    }
    result_array.push_back(key_value_map);
    key_value_map.clear();
  }
  return std::move(result_array);
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
