#include <iostream>
#include "MQPool/parse_xml.h"

using namespace std;

int main(int argc, char** argv) {
  ParseXmlObj myXml("../config/socket.xml");

  ParseXmlObj child = myXml.GetChild("Config.Server");  
  for (auto pos = child.Begin(); pos!= child.End(); ++pos) {
    ParseXmlObj nextChild = child.GetChild("Connection");
    for (auto nextpos = nextChild.Begin(); nextpos!= nextChild.End(); ++nextpos) {
      if (nextChild.GetChildKey(nextpos) == "IP")  cout << nextChild.GetChildData(nextpos) << endl;
      if (nextChild.GetChildKey(nextpos) == "Port")  cout << nextChild.GetChildData(nextpos) << endl;
      if (nextChild.GetChildKey(nextpos) == "Backlog")  cout << nextChild.GetChildData(nextpos) << endl;
    }
  }
  return 0;
}
