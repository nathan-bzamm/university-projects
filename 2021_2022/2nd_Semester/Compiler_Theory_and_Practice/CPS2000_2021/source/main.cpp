#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "SemanticAnalysisVisitor.h"

int main(){
     //system("dir");
    std::ifstream in("test.txt", std::ios::in | std::ios::binary);
    std::string contents;
    if (in){
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
//      std::cout<< contents <<std::endl;
     }
    else{
         std::cout<<"ERROR READING FILE"<<std::endl;
     }
//    Lexer* lex = new Lexer("var x:float = 36\n");

    Lexer* lex = new Lexer(contents);

//    Lexer* lex = new Lexer("var x:int = 36 + 5\n");
//    Lexer* lex = new Lexer("var x:int = functionCallBro(3)\n");
//    Lexer* lex = new Lexer("var x:bool = (3 == 3) < 4\n");
    std::cout<<"Finished Lexical analysis. Starting Parsing phase."<<std::endl;
    Parser* parsy = new Parser(lex);
    parsy->parse();
    std::cout<<"FINISH"<<std::endl; //use to place temporary breakpoint


//    auto testSemVis = new SemanticAnalysisVisitor();
//    testSemVis->testTable();
//    lex->parseString();
    return 0;
}