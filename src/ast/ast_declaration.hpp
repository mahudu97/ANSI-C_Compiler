#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"
#include "ast_statement.hpp"


class declaration_external : public Node {
    private:
        NodePtr arg; // could either be a function defition or a declaration - cannot tell

    public:    
        declaration_external(NodePtr _arg)
            : arg(_arg)
        {}

        std::string name = "declaration_external"; 

        virtual void PrettyPrint(std::ostream &dst) const override{
        
         /*
            if(next != null) next->print();
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }   
};

class definition_function : public Node{


    //DEFINITION_FUNCTION : 
    //                    SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND                  { $$ = new definition_function($1, $2, '$', $3); }
     

    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator;
        NodePtr  statement_compound;

    public:
        definition_function(NodePtr _arg1,NodePtr _arg2, NodePtr _arg3) // char used to differentiate
            :specifier_declaration(_arg1)
            ,declarator(_arg2)
            ,statement_compound(_arg3)
        {}
        
        definition_function(NodePtr _arg1,NodePtr _arg2) // char used to differentiate
            :specifier_declaration(_arg1)
            ,declarator(_arg2)
            ,statement_compound(NULL)
        {}

        std::string name = "definition_function"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            /*
                if(next != null) next->print();
                declaration->print();
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }
        
        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
           //         | SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND                  { $$ = new definition_function($1, $2, '$', $3); }
            if(statement_compound == NULL){  // prototype
                declarator->translate(dst,context);

                if(!context.function_dec){    //this checks if it is a global variable, which it is
                    context.globalVar.push_back(context.tmp_v);
                }

                dst<<"=0\n";        // initial 
 
                return;
            }
            
            
            context.function_dec = true;
            declarator->translate(dst,context); 
            context.indent++;
           
            std::vector<std::string>::iterator myVector;
            
            for(myVector = context.globalVar.begin() ; myVector != context.globalVar.end() ; myVector++){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                dst<<"global "<<*myVector<<std::endl;    
            }     
                
            statement_compound->translate(dst,context);
            context.indent--;
            context.function_dec = false;

        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(statement_compound == NULL){  // prototype
                return;
            }
                
            
            
            specifier_declaration->compile(dst,context);  //invoking the type
            context.function_type = context.tmp_v;
            
            
            
            context.parameter_num = 0;
            context.fp_parameter_num = 12;
            
            dst<<'\t'<<".text"<<'\n';
            declarator->compile(dst,context);
            
            context.fp_parameter_num = 12;
            context.parameter_num = 0;

            
            // presevre return address
            if(context.stack_size != 0){
                this->push_stack(dst,context); // shift arguments on stack
                this->push_stack(dst,context);

                context.pushing_parameters = false;


                dst<<"\tsw\t$31,"<<(context.stack_size)*4<<"($sp)\n"; // preseverve return addr
                context.save_stack_pos_31 = (context.stack_size);
                context.save_stack_pos_fp = (context.stack_size-1);
                dst<<"\tsw\t$fp,"<<(context.stack_size-1)*4<<"($sp)\n"; // stores value of fp intp sp+4
                dst<<"\taddu\t$fp,$sp,$0\n"; // moves value of sp into fp   
            }
            
            else{ // no arguments
                dst<<"\taddiu\t$sp,$sp,-8\n"; // pushed stack down
                context.stack_size++;
                context.stack_size++;
                dst<<"\tsw\t$31,"<<(context.stack_size)*4<<"($sp)\n"; // preseverve return addr
                context.save_stack_pos_31 = (context.stack_size);
                context.save_stack_pos_fp = (context.stack_size-1);
                dst<<"\tsw\t$fp,"<<(context.stack_size-1)*4<<"($sp)\n"; // stores value of fp intp sp+4
                dst<<"\taddu\t$fp,$sp,$0\n"; // moves value of sp into fp           
            }

            // now evaluate compound statement

            statement_compound->compile(dst, context);

            uint return_size = context.get_type_bytesize(context.functions[context.current_func]);
            // void function
            if(return_size == 0){
                dst<<"\taddu\t$sp,$fp,$0\n"; // restore sp
                dst<<"\tlw\t$31,"<<(context.save_stack_pos_31)*4<<"($sp)\n";
                dst<<"\tlw\t$fp,"<<(context.save_stack_pos_fp)*4<<"($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.stack_size*4<<"\n";
                dst<<"\tj\t$31\n\tnop\n"; // jump to return addr
            }

            dst<<'\t'<<".end "<<context.current_func<<"\n\n";
            // reset context
            context.function_end();

            context.print_fp_cd(); // floating constants
        
        }
};

class declaration : public Node{

    //DECLARATION : SPECIFIER_DECLARATION ';'                       { $$ = new declaration($1); }
    //            | SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'  { $$ = new declaration($1, $2); }

    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator_list_init;
        
    public:
        declaration(NodePtr _arg1)
            :specifier_declaration(_arg1)
            ,declarator_list_init(NULL)
        {}
        
        declaration(NodePtr _arg1,NodePtr _arg2) 
            :specifier_declaration(_arg1)
            ,declarator_list_init(_arg2)
        {}

        std::string name = "declaration"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) {next->print();
                // SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'
            }
            // SPECIFIER_DECLARATION ';'
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            //specifier_declaration->translate(dst,context);
           // dst<<" ";
            declarator_list_init->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(declarator_list_init != NULL){

                
                specifier_declaration->compile(dst,context); // assigns tmp_v with C type
                

                context.global_var_size = std::to_string(context.get_type_bytesize(context.tmp_v) );// default
                
                declarator_list_init->compile(dst,context); // Returns Identifier of variable to temp_v            

            }

        }
        
        
};

class specifier_declaration : public Node{
    private:
        NodePtr present;
        NodePtr next;
        
    public:
        specifier_declaration(NodePtr _arg1)
            :present(_arg1)
            ,next(NULL)
        {}
        specifier_declaration(NodePtr _arg1,NodePtr _arg2)
            :present(_arg1)
            ,next(_arg2)
        {}    
    public:
        std::string name = "specifier_declaration";   

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*        

            if(next != null) next->print();
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            present->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            present->compile(dst,context);
            
            if(context.tmp_v == "unsigned"){
                
                next->compile(dst,context);
                
                context.tmp_v = "unsigned " + context.tmp_v;
            }

            if(context.tmp_v == "signed"){
                
                next->compile(dst,context);
                
                context.tmp_v = "signed " + context.tmp_v;
            }

            if(context.tmp_v == "long"){
                
                next->compile(dst,context);
                
                context.tmp_v = "long " + context.tmp_v;
            }

            if(context.tmp_v == "short"){
                
                next->compile(dst,context);
                
                context.tmp_v = "short " + context.tmp_v;
            }
            
            
            
            
            
            
            
            
        
        
        }
};

class list_declaration : public Node{
    //LIST_DECLARATION : DECLARATION
    //               | LIST_DECLARATION DECLARATION    
    
    private:  
        NodePtr declaration;
        NodePtr list_declaration1;
        
    public:   
        list_declaration(NodePtr _arg1)
            :declaration(_arg1)
            ,list_declaration1(NULL)
        {}
            
        list_declaration(NodePtr _arg1,NodePtr _arg2)
            :declaration(_arg2)
            ,list_declaration1(_arg1)
        {}

        std::string name = "list_declaration";              
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            list_declaration1->translate(dst,context);
            declaration->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            
            list_declaration1->compile(dst,context);
            
            declaration->compile(dst,context);
            
            
            

        }
};

class declarator_init_list : public Node{
    //init_declarator_list
    //  : init_declarator
    //  | init_declarator_list ',' init_declarator
    //  ;

    private:
        NodePtr declarator_init1;
        NodePtr declarator_init;

    public:    
        declarator_init_list(NodePtr _arg1)
            :declarator_init1(_arg1)
            ,declarator_init(NULL)
        {}
        
        declarator_init_list(NodePtr _arg1,NodePtr _arg2)
            :declarator_init1(_arg2)
            ,declarator_init(_arg1)
        {}
    
    public:

        std::string name = "declarator_init_list"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(declarator_init != NULL){
                declarator_init->translate(dst,context);

            }
                declarator_init1->translate(dst,context);            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {

            std::string type = context.tmp_v; // save type

            declarator_init->compile(dst,context);
            
            context.tmp_v = type;

            declarator_init1->compile(dst,context);
            


        }
};

class declarator_init : public Node{

    //DECLARATOR_INIT : DECLARATOR                       
    //                | DECLARATOR ASSIGN INITIALIZER     

    private:
        NodePtr declarator;
        NodePtr initializer;
        char symbol;
        
    public:
    
        declarator_init(NodePtr _arg1) 
            :declarator(_arg1)
            ,initializer(NULL)
            ,symbol(' ')
        {}
    
        declarator_init(NodePtr _arg1,char name,NodePtr _arg2)
            :declarator(_arg1)
            ,initializer(_arg2)
            ,symbol(name)
        {}
        
        
    public:
        
        std::string name = "declarator_init";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }
        
        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            
            declarator->translate(dst,context);
            
            
            if(!context.function_dec){    //this checks if it is a global variable, which it is
                context.globalVar.push_back(context.tmp_v);
            }

            if(symbol == '='){
                dst<<"=";
                initializer->translate(dst,context);
                dst<<"\n";
            }
            else{
                dst<<"=0\n";        // initial 
            }
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            binding temp;

            bool local_var = (context.scope_index != 0 ) && !context.extern_global;

            
           
            temp.type = context.tmp_v; // set from declaration
            context.set_expr_result_type(); // use for assign cases on type

            temp.reg_ID = 33;    // forgot to initialise reg_ID into 33 -> not empty 

            
            if(local_var) this->push_stack(dst,context); //stack size is changed here.(incremented)
            
            else if(context.scope_index == 0) dst<<"\t.data\t"<<'\n'; //global variables
                  
            temp.stack_position = context.stack_size;

            if(local_var && (temp.type == "double")) this->push_stack(dst,context); // double takes up 2 words



            declarator->compile(dst,context);   //stores into tmp_v (variable name)

            temp.is_global = false;

            if(context.scope_index == 0){
            
                temp.is_global = true;
                dst<<"\t.size\t"<<context.tmp_v<<", "<<context.global_var_size<<'\n';
                dst<<context.tmp_v<<":\n";
            }
           

            if(context.extern_global){ // extern global declaration
                context.extern_globals[context.tmp_v] = true;
            }
            else{
                context.scopes[context.scope_index][context.tmp_v] = temp; 
            }

            if(local_var){

                if(temp.type == "double")
                {
                    if( initializer != NULL){

                        

                        context.assigning = true;
                        context.declaring = true;
                        initializer->compile(dst,context); //if its a constant it stores into expression_results
                        context.declaring = false;
                        context.assigning = false;

                        if( regex_match(context.expr_result, context.is_reg) ){
                            uint reg;
                            sscanf(context.expr_result.c_str(), "$%d", &reg );
                            dst<<"\tmov.d\t$f18,$f"<<reg<<'\n';
                        }

                        else{ // constant -> fp_constant_dec
                            std::string fpc = context.makeName("$dpc");

                            std::string tmp = "\t.data\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = fpc + ":\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = "\t.double\t" + context.expr_result + "\n\n";
                            context.fp_constant_dec.push_back(tmp);


                            dst<<"\tlui\t$15,%hi("<<fpc<<")\n";
                            dst<<"\tldc1\t$f18,%lo("<<fpc<<")($15)\n";
                        }
                        
                        dst<<"\tsdc1\t$f18,"<<temp.stack_position*4<<"($fp)"<<std::endl; //stores the value onto the correct position on the stack.
                    }

                }



                else if(temp.type == "float")
                {
                    if( initializer != NULL){

                        std::string fpc = context.makeName("$fpc");

                        context.assigning = true;
                        context.declaring = true;
                        initializer->compile(dst,context); //if its a constant it stores into expression_results
                        context.declaring = false;
                        context.assigning = false;

                        if( regex_match(context.expr_result, context.is_reg) ){
                            uint reg;
                            sscanf(context.expr_result.c_str(), "$%d", &reg );
                            dst<<"\tmov.s\t$f18,$f"<<reg<<'\n';
                        }

                        else{ // constant -> fp_constant_dec
                            std::string tmp = "\t.data\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = fpc + ":\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = "\t.float\t" + context.expr_result + "\n\n";
                            context.fp_constant_dec.push_back(tmp);


                            dst<<"\tlui\t$15,%hi("<<fpc<<")\n";
                            dst<<"\tlwc1\t$f18,%lo("<<fpc<<")($15)\n";
                        }
                        
                        dst<<"\tswc1\t$f18,"<<temp.stack_position*4<<"($fp)"<<std::endl; //stores the value onto the correct position on the stack.
                    }
                }



                else // int
                {
                    if( initializer != NULL){
                        context.assigning = true;
                        context.declaring = true;
                        initializer->compile(dst,context); //if its a constant it stores into expression_results
                        context.declaring = false;
                        dst<<"\tadd\t$15,$0,"<<context.expr_result<<'\n';
                        context.assigning = false;

                    }
                    else{
                        dst<<"\tmove\t$15,$0\n";
                    }
                    dst<<"\tsw\t$15,"<<temp.stack_position*4<<"($fp)"<<std::endl; //stores the value onto the correct position on the stack.
                }

                context.declarations++; // keep track of how many declaration were made in current scope
            }
            
            else if (context.scope_index == 0) {

                if( initializer != NULL){

                    if(temp.type == "double")
                    {}

                    if(temp.type == "float")
                    {}

                    else // int
                    {
                        context.assigning = true;
                        context.declaring = true;
                        initializer->compile(dst,context); //if its a constant it stores into expression_results
                        context.declaring = false;
                        context.assigning = false;                    
                        dst<<"\t.word\t"<<context.internal_expr_value<<'\n';
                    }   
                }
            }
            
            context.extern_global = false; //reset

        }
};

class initializer : public Node{
    //INITIALIZER : EXPR_ASSIGNMENT    
    //           | L_BRACKET LIST_INITIALIZER R_BRACKET
    //           | L_BRACKET LIST_INITIALIZER ',' R_BRACKET 
            
    private:
        NodePtr list_initializer;

    public: 
        initializer(NodePtr _arg1) 
            : list_initializer(_arg1)
        {}

        std::string name = "initializer";
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_initializer : public Node{
    //LIST_INITIALIZER : INITIALIZER
    //                | LIST_INITIALIZER ',' INITIALIZER
                    
    private:
        NodePtr initializer ;
        NodePtr rec;

    public:
        list_initializer(NodePtr _arg1)
            :initializer(_arg1)
            ,rec(NULL)
        {}
        
        list_initializer(NodePtr _arg1,NodePtr _arg2)
            :initializer(_arg1)
            ,rec(_arg2)
        {}

    public:
        std::string name = "list_initializer";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator_abstract : public Node{
    //DECLARATOR_ABSTRACT : POINTER
    //                    | DECLARATOR_DIRECT_ABSTRACT
    //                    | POINTER DECLARATOR_DIRECT_ABSTRACT

    private:    
        NodePtr pointer;
        NodePtr declarator_direct_abstract;
        
    public:
        declarator_abstract(NodePtr _arg1)
            :pointer(_arg1)
            ,declarator_direct_abstract(NULL)
        {}
        
        
        declarator_abstract(NodePtr _arg1,NodePtr _arg2)
            :pointer(_arg1)
            ,declarator_direct_abstract(_arg2)
        {}
        
    public:
        std::string name = "declarator_abstract";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator_direct_abstract : public Node{
    //                   DECLARATOR_DIRECT_ABSTRACT : L_BRACKET DECLARATOR_ABSTRACT R_BRACKET           1 pointer needed
    //                           | L_SQUARE R_SQUARE                                                         0 needed
    //                           | L_SQUARE EXPR_CONST R_SQUARE                                       1 pointer needed
    //                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE R_SQUARE                         1 pointer
    //                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE EXPR_CONST R_SQUARE              2 pointer
    //                           | L_BRACKET R_BRACKET                                                      0
    //                           | L_BRACKET LIST_PARAM_TYPE R_BRACKET                                      1 
    //                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET R_BRACKET                                   1
    //                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET LIST_PARAM_TYPE R_BRACKET                       2 pointer

    private:
        NodePtr now;
        NodePtr next;
        bool bracket;
        
    public:
    // LOOK AT THE DIFFERENTIATION LATER
    /*
            declarator_direct_abstract(NodePtr _arg1,char x)
                :now(_arg1)
                ,next(NULL)
            // ,bracket(true)
            {}
            */
            
            declarator_direct_abstract()
                :now(NULL)
                ,next(NULL)
            // ,bracket(false)
            {}
            
            declarator_direct_abstract(NodePtr _arg1)
                :now(_arg1)
                ,next(NULL)
            //  ,bracket(false)
            {}
    /*
        declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2,char x)
            :now(_arg1)
            ,next(_arg2)
         //   ,bracket(false)
        {}
        */
        /*
        declarator_direct_abstract(char x)
            :now(NULL)
            ,next(NULL)
          //  ,bracket(true)
        {}   
        */
        declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2)
            :now(_arg1)
            ,next(_arg2)
           // ,bracket(true)
        {} 

        std::string name = "declarator_direct_abstract";
        
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_parameter : public Node{


    //DECLARATION_PARAMETER : SPECIFIER_DECLARATION DECLARATOR        
    //                      | SPECIFIER_DECLARATION DECLARATOR_ABSTRACT
    //                      | SPECIFIER_DECLARATION
    private:
        NodePtr current;
        NodePtr recur;

    public:
        declaration_parameter(NodePtr _arg1, NodePtr _arg2)
            :current(_arg1)
            ,recur(_arg2)
        {}
        
        declaration_parameter(NodePtr _arg1)
            :current(_arg1)
            ,recur(NULL)
        {}

        std::string name = "declaration_parameter";
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(recur != NULL) recur->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            current->compile(dst,context);
            
            
            binding tmp;
            tmp.type = context.tmp_v;
            tmp.reg_ID = 33;
            tmp.is_global = false;
            
            context.pushing_parameters = true;
            context.parameter = true;
            recur->compile(dst,context);        //stores variable name in tmp_v
            
            // add var to stack

            if(tmp.type == "double"){
                this->push_stack(dst,context);
                this->push_stack(dst,context);
                dst<<"\tsdc1\t$f"<<context.fp_parameter_num<<","<<(context.stack_size-1)*4<<"($sp)\n";

                tmp.stack_position = context.stack_size-1;
                context.fp_parameter_num += 2;  // for next parameter
            }
            else if(tmp.type == "float"){
                this->push_stack(dst,context);
                dst<<"\tswc1\t$f"<<context.fp_parameter_num<<","<<context.stack_size*4<<"($sp)\n";

                tmp.stack_position = context.stack_size;
                context.fp_parameter_num += 2;  // for next parameter

            }
            else{
                this->push_stack(dst,context);
                dst<<"\tsw\t$a"<<context.parameter_num<<","<<context.stack_size*4<<"($sp)\n";
                tmp.stack_position = context.stack_size;
                context.parameter_num++;  // for next parameter 
            }


            context.param_bindings[context.tmp_v] = tmp;                      
            
            
        }
};

class declarator_direct : public Node{

    //          DECLARATOR_DIRECT : IDENTIFIER
    //                | L_BRACKET DECLARATOR R_BRACKET                              (declarator)
    //                | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE              declarator_direct [exprconst]
    //                | DECLARATOR_DIRECT L_SQUARE R_SQUARE                         declarator_direct []
    //                | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET       declarator_direct (list_param_type)
    //                | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET       declarator_direct (list_identifier)
    //                | DECLARATOR_DIRECT L_BRACKET R_BRACKET                       declarator_direct ()
    private:
        NodePtr  current;
        NodePtr  next;
        bool brackets;
        bool squares;
        std::string symbol;
        
    public:
        declarator_direct(std::string name)
            :current(NULL)
            ,next(NULL)
            ,brackets(false) //MEHEDI really wants false
            ,squares(false)
            ,symbol(name)
        {}
        
        declarator_direct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
            ,brackets(true)
            ,squares(false)
            ,symbol(" ")
        {}
        
        declarator_direct(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
            ,brackets(true) // fix this for the actual thing
            ,squares(false)
            ,symbol(" ")
        {}

        std::string name = "declarator_direct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(symbol != " "){
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }            
                dst<<symbol;
                context.tmp_v = symbol;
                
                if(symbol == "main"){
                    context.main_exists = true;
                }
            }          
            else{

                
                    dst<<"def ";

                    current->translate(dst,context);
                    dst<<"(";
                    if(next != NULL){
                        next->translate(dst,context);
                    }  
                    dst<<")"<<":"<<std::endl;  
                
            }
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(symbol != " "){

    
                if(!context.parameter && (context.scope_index == 0)){
                    dst<<'\t'<<".global "<<symbol<<'\n';
                    context.current_func = symbol;
                    
                }
                
                context.tmp_v = symbol;                    
                    
            }
            
            else if((next == NULL) && brackets){
                    current->compile(dst,context);
                    dst<<'\t'<<".ent "<<context.current_func<<'\n';
                    dst<<context.current_func<<":"<<'\n';

                    
                    context.functions[context.current_func] = context.function_type;

            }
            
            else if((next != NULL)  && brackets){
                    
                    current->compile(dst,context);
                    dst<<'\t'<<".ent "<<context.current_func<<'\n';
                    dst<<context.current_func<<":"<<'\n';
                    
                    context.functions[context.current_func] = context.function_type;
                    
                    next->compile(dst,context);
                    
                    
            
            
            }
            
            //HEY 
        }

};

class struct_declarator : public Node{
    //STRUCT_DECLARATOR : DECLARATOR
    //                | ':' EXPR_CONST
    //                | DECLARATOR ':' EXPR_CONST

    private:
        NodePtr current;
        NodePtr next;

    public:
        struct_declarator(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}

        struct_declarator(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "struct_declarator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        } 
};

class list_struct_declarator : public Node{
    //LIST_STRUCT_DECLARATOR : STRUCT_DECLARATOR
    //                       | LIST_STRUCT_DECLARATOR ',' STRUCT_DECLARATOR

    private:
        NodePtr current;
        NodePtr next;

    public:
        list_struct_declarator(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}

        list_struct_declarator(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}
        
        
        std::string name = "list_struct_declarator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */ 
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_struct : public Node{
    //DECLARATION_STRUCT : LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR ';'

    private:
        NodePtr current;
        NodePtr next;
        
    public:
        declaration_struct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "declaration_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */ 
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_list_struct : public Node{
    //DECLARATION_LIST_STRUCT : DECLARATION_STRUCT
    //                        | DECLARATION_LIST_STRUCT DECLARATION_STRUCT

    private:
        NodePtr current;
        NodePtr rec;

    public:
        declaration_list_struct(NodePtr _arg1)
            :current(_arg1)
            ,rec(NULL)
        {}
        
        declaration_list_struct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,rec(_arg2)
        {}

        std::string name = "declaration_list_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */  
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator : public Node{
    //DECLARATOR : POINTER DECLARATOR_DIRECT
    //           | DECLARATOR_DIRECT  
    private:
        NodePtr ptr;
        NodePtr dd;
    public:
        declarator(NodePtr _arg1, NodePtr _arg2)
            : ptr(_arg1)
            , dd(_arg2)
        {}

    public:

        std::string name = "declarator";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            ptr->PrettyPrint(dst);
            dd->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;

            exit(1);
        }
};


#endif



