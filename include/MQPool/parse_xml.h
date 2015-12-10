#ifndef MQPOOL_INCLUDE_PARSE_XML_H_
#define MQPOOL_INCLUDE_PARSE_XML_H_

/* 以前一直都是把解析xml文件的代码放在池的构造函数当中,这里把它独立出来
 *  使用boost的xml解析库,也推荐使用rapidxml,而且boost的xml解析库用的就是rapidxml
 */

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

using std::string;
using std::stringstream;
using std::map;
using std::make_pair;
using boost::property_tree::ptree;

class ParseXmlObj {
 public:
  explicit ParseXmlObj();
  explicit ParseXmlObj(string configPath);
  explicit ParseXmlObj(ptree* it);
  virtual ~ParseXmlObj();
  void Dump() const;

  string GetConfigPath() const;

  ptree* GetPtree() const;

  /*
   * get begin iterator and end iterator
   */
  ptree::iterator Begin() const;
  ptree::iterator End() const;

  /*
   * get child key by iterator
   */
  string GetChildKey(const ptree::iterator &it);

  /*
   * get child data by iterator
   */
  string GetChildData(const ptree::iterator &it);
  /*
   * get child by iterator
   */
  ParseXmlObj GetChild(const ptree::iterator &it);

  /*
   * get child by path
   * path is relative to current path.
   */
  ParseXmlObj GetChild(const string& path);

  /*
   * get attribute by path
   * path is relative to current path.
   */
  string GetAttr(string path, const string& attr);

  /*
   * get value by path
   * if path == "", return value of this ptree
   */
  string GetValue(const string& path);

  //===========================================================
  /*
   * push back a child
   */
  void PutChild(const string& key, const ParseXmlObj& child);

  /*
   * put arrribute
   */
  bool PutAttr(string path, const string& attribute, const string& attrvalue);

  /*
   * put value
   */
  bool PutValue(const string& path, const string& value);

  /*
   * add value
   */
  bool AddValue(const string& path, const string& value);

  /*
   * save config
   */
  void SaveConfig(const string& configPath);

 private:
  bool needDelete_;
  string configPath_; 
  //这里不用ptree对象而用指针的意义在于如果使用ptree对象的话，构造的时候就必须完全构造这个对象
  ptree* pt_;
};

#endif /* MQPOOL_INCLUDE_PARSE_XML_H */
