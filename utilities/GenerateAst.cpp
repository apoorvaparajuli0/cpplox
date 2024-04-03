#include "GenerateAst.hpp"

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "%s\n", "Usage: generate_ast <output directory>");
        exit(64);
    }

    std::list<std::string> exprTypes = {
    //CHALLENGE 10.2: Add Support for Lambda Expressions
    //   "Lambda   : std::vector<Token> params, std::vector<stmt_ptr> body",
      "Assign   : Token name, expr_ptr value",
      "Binary   : expr_ptr left, Token operator_, expr_ptr right",
      "Call     : expr_ptr callee, Token paren, std::vector<expr_ptr> arguments", 
      "Grouping : expr_ptr expression",
      "Literal  : Object value",
      "Logical  : expr_ptr left, Token operator_, expr_ptr right",
      "Unary    : Token operator_, expr_ptr right",
      "Variable : Token name"
    };

    //CHALLENGE 8.1: Add Support for Expression Evaluation in REPL
    //CHALLENGE 9.3: Add Support for Break Statements
    std::list<std::string> stmtTypes = {
    //"Break      : Object value",
      "Block      : std::vector<stmt_ptr> statements",
      "Expression : expr_ptr expression"/*, bool pure"*/,
      "Function   : Token name, std::vector<Token> params, std::vector<stmt_ptr> body",
      "If         : expr_ptr condition, stmt_ptr thenBranch, stmt_ptr elseBranch",
      "Print      : expr_ptr expression",
      "Return     : Token keyword, expr_ptr value",
      "Var        : Token name, expr_ptr initializer",
      "While      : expr_ptr condition, stmt_ptr body"
    };

    std::string outputDir = argv[1];
    defineHeader(outputDir, "Expr", "Stmt", exprTypes, stmtTypes);
    defineAst(outputDir, "Expr", "Stmt", exprTypes, stmtTypes);
}

void defineAst(std::string outputDir, std::string exprName, std::string stmtName, std::list<std::string> exprTypes, std::list<std::string> stmtTypes) {
    std::string expr_path = outputDir + "/src/" + exprName + ".cpp";    
    std::string stmt_path = outputDir + "/src/" + stmtName + ".cpp";

    std::ofstream out;
    out.open(expr_path, std::ios::out);
    out << "#include \"../headers/Expr.hpp\"\n\n";

    for(std::string type : exprTypes) {
        std::string className = trim(split(type, ": ").front());
        std::string fields = trim(split(type, ": ").back());
        defineType(out, exprName, className, fields);
    }

    out.close();

    out.open(stmt_path, std::ios::out);
    out << "#include \"../headers/Stmt.hpp\"\n\n";

    for(std::string type : stmtTypes) {
        std::string className = trim(split(type, ": ").front());
        std::string fields = trim(split(type, ": ").back());
        defineType(out, stmtName, className, fields);
    }

    out.close();
}

void defineHeader(std::string outputDir, std::string exprName, std::string stmtName, std::list<std::string> exprTypes, std::list<std::string> stmtTypes) {
    std::string path = outputDir + "/headers/" + "Visitor" + ".hpp";
    std::ofstream out;
    out.open(path, std::ios::out);
    out << "#ifndef VISITOR_HPP\n#define VISITOR_HPP\n\n";
    out << "#include \"any\"\n";
    out << "#include \"memory\"\n";
    out << "#include \"../headers/Token.hpp\"\n\n";

    out << "//Expression Types\n";
    for(std::string type : exprTypes) {
        std::string className = trim(split(type, ": ").front());
        out << "class " + className + ";\n";
    }

    out << "\n\n";

    out << "//Statement Types\n";
    for(std::string type : stmtTypes) {
        std::string className = trim(split(type, ": ").front());
        out << "class " + className + ";\n";
    }

    out << "\n";

    defineVisitor(out,  exprName, exprTypes);
    defineVisitor(out,  stmtName, stmtTypes);

    out << "\n";
    out << "class " + exprName + " {\n";

    out << "  public:\n";
    out << "    virtual Object accept(ExprVisitor<Object>& visitor) const = 0;\n";
    out << "    virtual ~Expr() = default;\n";

    out << "};\n\n";
    out << "class " + stmtName + " {\n";

    out << "  public:\n";
    out << "    virtual void accept(StmtVisitor<void>& visitor) const = 0;\n";
    out << "    virtual ~Stmt() = default;\n";

    out << "};\n\n";

    out << "using expr_ptr = std::shared_ptr<Expr>;\n";
    out << "using stmt_ptr = std::shared_ptr<Stmt>;\n\n";

    out << "#endif\n";

    out.close();

    path = outputDir + "/headers/" + exprName + ".hpp";
    out.open(path, std::ios::out);

    out << "#ifndef " + toUpperCase(exprName) + "_HPP\n";
    out << "#define " + toUpperCase(exprName) + "_HPP\n\n";
    out << "#include \"../headers/Visitor.hpp\"\n";
    out << "#include \"vector\"\n\n";

    for(std::string type : exprTypes) {
        std::string exprType = trim(split(type, ": ").front());
        out << "class " + exprType + " : public " + exprName + " {\n";

        std::string fieldList = trim(split(type, ": ").back());
        std::list<std::string> fields = split(fieldList, ", ");

        out << "  public:\n";
        for(std::string field : fields) {
            out << "    " + field + ";\n";
        }

        out << "\n";
        out << "  " + exprType + "(" + fieldList + ");\n";
        out << "  Object accept(ExprVisitor<Object>& visitor) const override;\n";

        out << "};\n";
    }

    out << "\n#endif\n";

    out.close();

    path = outputDir + "/headers/" + stmtName + ".hpp";
    out.open(path, std::ios::out);

    out << "#ifndef " + toUpperCase(stmtName) + "_HPP\n";
    out << "#define " + toUpperCase(stmtName) + "_HPP\n\n";
    out << "#include \"../headers/Visitor.hpp\"\n";
    out << "#include \"vector\"\n\n";

    for(std::string type : stmtTypes) {
        std::string stmtType = trim(split(type, ": ").front());
        out << "class " + stmtType + " : public " + stmtName + " {\n";

        std::string fieldList = trim(split(type, ": ").back());
        std::list<std::string> fields = split(fieldList, ", ");

        out << "  public:\n";
        for(std::string field : fields) {
            out << "    " + field + ";\n";
        }

        out << "\n";
        out << "  " + stmtType + "(" + fieldList + ");\n";
        out << "  void accept(StmtVisitor<void>& visitor) const override;\n";

        out << "};\n";
    }

    out << "\n#endif\n";
    out.close();
}

void defineVisitor(std::ofstream& out, std::string baseName, std::list<std::string> types) {
    out << "\n";
    out << "template<class R>\n";
    out << "class " + baseName + "Visitor {\n";
    out << "  public:\n";

    for(std::string type : types) {
        std::string typeName = trim(split(type, ": ").front());
        out << "    virtual R visit" + typeName + baseName + "(const " + typeName + "& " + toLowerCase(baseName) + ") = 0;\n";
    }
    out << "    virtual ~" + baseName + "Visitor() = default;\n";

    out << "};\n";
}

void defineType(std::ofstream& out, std::string baseName, std::string className, std::string fieldList) {

    out << className + "::" + className + "(" + fieldList + ") : \n";

    std::list<std::string> fields = split(fieldList, ", ");
    for(std::string field : fields) {
        std::string name = split(field, " ").back();
        if(field != fields.back()) {
            out << name + "{std::move(" + name + ")}, ";
        } else {
            out << name + "{std::move(" + name + ")} ";
        }
    }

    out << "{}";
    out << "\n";

    if(baseName == "Expr") {
        out << "Object " + className + "::accept(" + baseName + "Visitor<Object>& visitor) const {\n";
    } else {
        out << "void " + className + "::accept(" + baseName + "Visitor<void>& visitor) const {\n";
    }

    out << "  return visitor.visit" + className + baseName + "(*this);\n";
    out << "}\n\n";
}

std::string toUpperCase(std::string toUpper) {
    std::string ret;

    for(char c : toUpper) {
        if(c >= 'a' && c <= 'z') {
            ret.push_back(c - 32);
        } else {
            ret.push_back(c);
        }
    }

    return ret;
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