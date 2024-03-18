#include "list"
#include "string"
#include "fstream"
#include "cstdio"

void defineHeader(std::string outputDir, std::string baseName, std::list<std::string> types);
void defineAst(std::string outputDir, std::string baseName, std::list<std::string> types);
void defineType(std::ofstream& out, std::string baseName, std::string className, std::string fieldList);
std::string trim(std::string toTrim);
std::list<std::string> split(std::string str_to_split, std::string splitPoint);



