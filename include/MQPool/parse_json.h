#ifndef MQPOOL_INCLUDE_PARSE_JSON_H_
#define MQPOOL_INCLUDE_PARSE_JSON_H_

/* 以前一直都是把解析json文件的代码放在池的构造函数当中,这里把它独立出来
 *  使用boost的json解析库,也推荐使用rapidjson,
 * rapidjson是仿造rapidxml写出来的,不过没有用在boost库当中
 */

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>

using std::string;
using std::stringstream;
using std::map;
using std::make_pair;
using boost::property_tree::ptree;

class ParseJsonObj {
 public:
  explicit ParseJsonObj();
  explicit ParseJsonObj(string configPath);
  explicit ParseJsonObj(ptree* it);
  virtual ~ParseJsonObj();
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
  ParseJsonObj GetChild(const ptree::iterator &it);

  /*
   * get child by path
   * path is relative to current path.
   */
  ParseJsonObj GetChild(const string& path);

  //===========================================================
  /*
   * push back a child
   */
  void PutChild(const string& key, const ParseJsonObj& child);

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

#endif /* MQPOOL_INCLUDE_PARSE_JSON_H */
