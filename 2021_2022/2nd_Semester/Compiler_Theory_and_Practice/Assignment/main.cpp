#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "Helper/Helper.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

using std::cout;
using std::cerr;
using std::string;
using std::istreambuf_iterator;
using std::ifstream;
using std::unique_ptr;
using std::vector;

//To read the file which is specified by the filename
//Assign said file to s
void readFile(string &s);

unique_ptr<string> file;
unique_ptr<Lexer> lexer;
unique_ptr<Parser> parser;

vector<Token> tokens;

string filename = "";


int main(int argc, char *argv[]){
    if(argc > 1){
        filename = argv[1];
    }

    //Calling the Lexer
    file = unique_ptr<string>(new string());
    readFile(*file);

    cout << "Now Lexing:\n";
    lexer = unique_ptr<Lexer>(new Lexer(*file));
    lexer->lex();
    cout << "Lexer has finished Lexing\n\n";
    cout << "Obtained tokens from lexer\n\n";
    //This is used for debugging the lexer
    lexer->printTokens();
    lexer->removeComments();
    tokens = lexer->get_tokens();
    file.reset();
    lexer.reset();//Freeing pointer optimisations

    //Calling the Parser
    parser = unique_ptr<Parser>(new Parser(&tokens));
    cout << "\nNow Parsing\n";
    bool successParse = parser->parse();
    cout << (successParse? "\nSuccessful Parsing\n" : "\nError has occurred whilst Parsing") << "\n";
    if(! successParse){
        cout << "Unexpecton token " << tokens[parser->getTokenManagerIndex()].lexeme;
        cout << "at line " << tokens[parser->getTokenManagerIndex()].lineNumber << "\n";
        exit(EXIT_FAILURE);
    }

    ASTNode* tree = parser->getTree();

    //Perform XML Generation and Print it out to the user
    cout << "XML Representation:\n";
    XMLVisitor *xmlv = new XMLVisitor();
    //Generating the XML
    tree->accept(xmlv);
    xmlv->trimXMLNewLines();
    cout << xmlv->getXML();
    delete xmlv;

    //Performing Semantic Analysis pass
    SAVisitor *sav = new SAVisitor();
    cout << "\nNow running Semantic Analysis:\n";
    tree->accept(sav);
    cout << "\nSemantic Analysis was performed Successfully\n";
    delete sav;

    //Performing Interpreter Execution Pass
    IVisitor *iv = new IVisitor();
    cout << "\nNow running the Program:\n";
    tree->accept(iv);
    cout << "\nProgram has finished execution with the following return value: ";
    //An example showing that bool works
    iv->printValue(iv->getReturnValue());
    cout << "\n";
    delete iv;

    parser.reset();
    tokens = vector<Token>();

    return 0;
};

void readFile(string &s){
    ifstream ifs(filename);
    if(ifs){
        string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        s = str;
    }else{
        //if the file was not read successfully, the error is reported and exit
        cerr << "Error, was not able to read file\n";
        exit(EXIT_FAILURE);
    }
}
