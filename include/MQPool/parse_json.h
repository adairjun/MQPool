#ifndef MQPOOL_INCLUDE_PARSE_JSON_H_
#define MQPOOL_INCLUDE_PARSE_JSON_H_

/* 以前一直都是把解析json文件的代码放在池的构造函数当中,这里把它独立出来
 *  使用boost的json解析库,也推荐使用rapidjson,
 * rapidjson是仿造rapidxml写出来的,不过没有用在boost库当中
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

class ParseJsonObj {
 public:
  explicit ParseJsonObj();
  explicit ParseJsonObj(string configPath);
  virtual ~ParseJsonObj();
  ParseJsonObj(const ParseJsonObj&) = delete;
  ParseJsonObj& operator=(const ParseJsonObj&) = delete;
  void Dump() const;

  string GetConfigPath() const;

  ptree* GetPtree() const;

  /*
   * GetChildData("root.child") will get "a" and "b"
   * { "root":
   *     { "child":
   *        { "a": "1",
   *          "b": "2" 
   *        }
   *     }
   *  }
   */
  map<string, string> GetChildData(const string& path);

  /*
   * GetChildDataArray("root.child") will get "a" and "b"
   * { "root":
   *     { "child":
   *        [
   *        { "a": "1",
   *          "b": "2" 
   *        },
   *        { "a": "1",
   *          "b": "2" 
   *        }
   *        ]
   *     }
   *  }
   */
  vector<map<string, string> > GetChildDataArray(const string& path);

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
  string configPath_; 
  //这里不用ptree对象而用指针的意义在于如果使用ptree对象的话，构造的时候就必须完全构造这个对象
  ptree* pt_;
};

#endif /* MQPOOL_INCLUDE_PARSE_JSON_H */
