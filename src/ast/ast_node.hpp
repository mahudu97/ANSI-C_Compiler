#ifndef ast_node_hpp
#define ast_node_hpp

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <stdio.h>
#include <fstream>
#include <memory>

#include "compile_context.hpp"

// abstarct class for any node of the AST
class Node;

struct TranslateContext{
    int indent;
    std::vector<std::string> globalVar;
    bool function_dec;
    std::string tmp_v;
    bool main_exists;
};

typedef const Node* NodePtr;

class Node
{
public:
    std::string name;

    virtual std::string get_name() const{
        return name;
    }

    virtual ~Node()
    {}

    // print out AST
    virtual void PrettyPrint(std::ostream &dst) const =0;

    //! Tell and expression to print itself to the given stream
    virtual void translate(std::ostream &dst, TranslateContext &context) const =0;

    //! Evaluate the tree using the given mapping of variables to numbers
    virtual void compile(std::ostream &dst, CompileContext &context) const =0;


    
    void push_stack(std::ostream &dst, CompileContext &context) const{
        

        dst<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,-4"<<'\n';
        
        for(uint i = 1; i <= context.stack_size ; i++){ 
            dst<<"\tlw\t$15,"<<i*4+4<<"($sp)"<<'\n';                                        //permanently assign register 15 as temporary stack shifting storage
            dst<<"\tsw\t$15,"<<i*4<<"($sp)"<<'\n';
        }
        if(!context.pushing_parameters) dst<<"\taddu\t$fp,$sp,$0"<<std::endl;                       //outside of forloop
        context.stack_size++;
    }
    
};


#endif