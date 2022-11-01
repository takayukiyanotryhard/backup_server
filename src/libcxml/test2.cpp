#include <iostream>
#include <string>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#define BUF_SIZE 0x10000
#define SFILE "/System/Library/CoreServices/SystemVersion.plist"

int main()
{
  // オンメモリのXMLデータ
    FILE *fp = fopen(SFILE, "r");
    char buf[BUF_SIZE];
    fread(buf, BUF_SIZE, 1, fp);
    fclose(fp);
  xml_document<> *doc = new xml_document<>();
  try {
    doc->parse<0>(buf);
  }
  catch(parse_error& err) {
    cout << err.what() << " " << err.where<char*>();
    return 1;
  }

  for (xml_node<> *node  = doc->first_node(); node;node = node->next_sibling()) {
    cout << "ノード名: " << node->name() << "\n";
    for (xml_node<> *child  = node->first_node(); child;child = child->next_sibling()) {
      cout << "ノード名: " << child->name() << "\n";
      cout << "テキスト: " << child->value() << "\n";
        for (xml_node<> *child2  = child->first_node(); child2;child2 = child2->next_sibling()) {
          cout << "ノード名: " << child2->name() << "\n";
          cout << "テキスト: " << child2->value() << "\n";
      }
    }
  }
    delete doc;
}
