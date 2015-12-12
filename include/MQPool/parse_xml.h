#ifndef MQPOOL_INCLUDE_PARSE_XML_H_
#define MQPOOL_INCLUDE_PARSE_XML_H_

/* 以前一直都是把解析xml文件的代码放在池的构造函数当中,这里把它独立出来
 *  使用boost的xml解析库,也推荐使用rapidxml,而且boost的xml解析库用的就是rapidxml
 */

#include <string>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>

using std::string;
using std::vector;
using std::map;
using std::make_pair;
using boost::property_tree::ptree;

class ParseXmlObj {
 public:
  explicit ParseXmlObj();
  explicit ParseXmlObj(string configPath);
  virtual ~ParseXmlObj();
  ParseXmlObj(const ParseXmlObj&) = delete;
  ParseXmlObj& operator=(const ParseXmlObj&) = delete;
  void Dump() const;

  string GetConfigPath() const;

  ptree* GetPtree() const;

  /*
   *
   */
  map<string, string> GetChildData(const string& path);

  /*
   * 这里就没有必要写GetChildData了,因为json有数组的概念但是xml没有 
   * 这里是拿到这种形式的数据:
   * <root>
   *   <child>
   *   </child>
   *   <child>
   *   </child>
   * </root>
   * 是将child当中的数据放入map里面,然后一个child是一个map,再把map放入vector当中
   * 和这种形式:
   * <root>
   *   <child>
   *   </child>
   * </root>
   * 是一样的,所以说在xml当中GetChildDataArray和GetChildData是一样的
   */
  vector<map<string, string> > GetChildDataArray(const string& path);

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
  string configPath_; 
  //这里不用ptree对象而用指针的意义在于如果使用ptree对象的话，构造的时候就必须完全构造这个对象
  ptree* pt_;
};

#endif /* MQPOOL_INCLUDE_PARSE_XML_H */
