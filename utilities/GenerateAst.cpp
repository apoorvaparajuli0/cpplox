#include "GenerateAst.hpp"

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "%s\n", "Usage: generate_ast <output directory>");
        exit(64);
    }

    std::list<std::string> types = {
      "Binary   : Expr left, Token operator_, Expr right",
      "Grouping : Expr expression",
      "Literal  : Object value",
      "Unary    : Token operator_, Expr right"
    };

    std::string outputDir = argv[1];
    defineHeader(outputDir, "Expr", types);
    defineAst(outputDir, "Expr", types);
}

void defineAst(std::string outputDir, std::string baseName, std::list<std::string> types) {
    std::string path = outputDir + "/" + baseName + ".cpp";
    std::ofstream out;
    out.open(path, std::ios::out);
    out << "#include \"Expr.hpp\"\n\n";

    for(std::string type : types) {
        std::string className = trim(split(type, ":").front());
        std::string fields = trim(split(type, ":").back());
        defineType(out, baseName, className, fields);
    }

    out.close();
}

void defineHeader(std::string outputDir, std::string baseName, std::list<std::string> types) {
    std::string path = outputDir + "/" + baseName + ".hpp";
    std::ofstream out;
    out.open(path, std::ios::out);
    out << "#include \"list\"\n";
    out << "#include \"any\"\n";
    out << "#include \"Token.hpp\"\n\n";

    out << "class " + baseName + " {\n";
    out << "  public:\n";

    for(std::string type : types) {
        std::string className = trim(split(type, ":").front());
        out << "    class " + className + ";\n";
    }

    defineVisitor(out,  baseName, types);

    out << "\n";
    out << "    virtual std::any accept(Expr::Visitor visitor);\n";

    out << "};\n";
    out.close();
}

void defineVisitor(std::ofstream& out, std::string baseName, std::list<std::string> types) {
    out << "\n";
    out << "    class Visitor {\n";
    out << "      public:\n";

    for(std::string type : types) {
        std::string typeName = trim(split(type, ":").front());
        out << "        virtual std::any visit" + typeName + baseName + "(Expr::" + typeName + " " + toLowerCase(baseName) + ");\n";
    }

    out << "    };\n";
}

void defineType(std::ofstream& out, std::string baseName, std::string className, std::string fieldList) {
    out << "class " + baseName + "::" + className + " : " + baseName + " {\n";

    out << "    " + className + "(" + fieldList + ") {\n";

    std::list<std::string> fields = split(fieldList, ", ");

    for(std::string field : fields) {
        std::string name = split(field, " ").back();
        out << "      this->" + name + " = " + name + ";\n";
    }

    out << "    };\n\n";

    out << "    std::any accept(Visitor visitor) override {\n";
    out << "      return visitor.visit" + className + baseName + "(*this);\n";
    out << "    }\n";

    for(std::string field : fields) {
        out << "    " + field + ";\n";
    }

    out << "};\n";
}

std::string toLowerCase(std::string toLower) {
    std::string ret;

    for(char c : toLower) {
        if(c >= 'A' && c <= 'Z') {
            ret.push_back(c + 32);
        } else {
            ret.push_back(c);
        }
    }

    return ret;
}

//trims preceding and subsequent whitespace from a string
std::string trim(std::string toTrim) {

    int i = 0;
    int j = toTrim.length() - 1;
    while(toTrim.at(i) == 32) {
        i++;
    }
    while(toTrim.at(j) == 32) {
        j--;
    }

    return std::string{toTrim.substr(i, j + 1)};
}

//splits a string n times depending on the number of occurrences of "splitPoint" in
//str_to_split
std::list<std::string> split(std::string str_to_split, std::string splitPoint) {
    
    if(str_to_split.find(splitPoint) == std::string::npos) return std::list{str_to_split};

    int pos = str_to_split.find(splitPoint);
    int start = 0;
    std::list<std::string> ret_list;

    while(str_to_split.find(splitPoint) != std::string::npos) {
        std::string to_push = str_to_split.substr(start, pos);

        str_to_split = str_to_split.substr(pos + splitPoint.length(), str_to_split.length());
        pos = str_to_split.find(splitPoint);

        ret_list.push_back(to_push);

        if(str_to_split.find(splitPoint) == std::string::npos) { 
            ret_list.push_back(str_to_split); 
            break; 
        }
    }

    return ret_list;
}