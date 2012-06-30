#include <string>

#include "document.h"

using std::string;

int main() {
  kamiah::Document doc;

  string str;
  doc.GetData(&str);

  return 0;
}
