#include <iostream>
#include "MQPool/parse_xml.h"

using namespace std;

int main(int argc, char** argv) {
  ParseXmlObj myXml("../config/socket.xml");
  myXml.Dump();

  myXml.GetChild("Connection");  

}
