#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"

class statement_compound : public Node{
    //STATEMENT_COMPOUND : L_BRACE R_BRACE                                  { $$ = new statement_compound(); }
    //                   | L_BRACE LIST_STATEMENT R_BRACE                   { $$ = new statement_compound($2); }

    private:
        NodePtr list;

    public:
        statement_compound()
            :list(NULL)
        {}

        statement_compound(NodePtr _arg)
            :list(_arg)
        {}

        
        std::string name = "statement_compound";

        virtual std::string get_name() const override{
            return "statement_compound";
        }
            
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
         
            if(list == NULL){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                dst<<"pass\n";
            }
            
            else{
                
                list->translate(dst,context);
            }
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {

            context.scope_index++; // incrementing the scope index

            std::map<std::string,binding>bindings; //create a map for pushing it out at the end.
            
            // copy in params for a function
            if(context.parameter){
                std::map<std::string, binding>::iterator it;
                
                for(it = context.param_bindings.begin(); it != context.param_bindings.end(); ++it){
                    bindings[it->first] = it->second;
                }
                context.param_bindings.clear();
                context.parameter = false;
            }

            context.scopes.push_back(bindings);
            
            uint declaration_amount_save = context.declarations;
            context.declarations = 0;


            if(list != NULL){
                
                list->compile(dst,context);
            }
            
            // clear stack from this scope - get rid of variables etc
            // context.declarations has amount of declarations
                uint new_size = context.stack_size - context.declarations;
                for(uint i = new_size ; i > 0; --i){
                    dst<<"\tlw\t$15,"<<i*4<<"($fp)\n";
                    dst<<"\tsw\t$15,"<<(i+context.declarations)*4<<"($fp)\n";
                }
                dst<<"\taddiu\t$sp,$sp,"<<context.declarations*4<<"\n";
                dst<<"\tmove\t$fp,$sp\n";

                context.stack_size = new_size;
                context.declarations = declaration_amount_save;
        

            context.scope_index--; // decrementing the scope index so we can keep track of which scope we are in
            context.scopes.pop_back(); // pop out

            context.extern_globals.clear(); // clear out external global vars


        }
};

class list_statement : public Node{
    //LIST_STATEMENT : STATEMENT
    //               | LIST_STATEMENT STATEMENT         

    private:
        NodePtr current;
        NodePtr next;

    public:   
    /*
        list_statement(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}
        */
        list_statement(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "list_statement";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            /*

            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'^ ';
            }
            exp->PrettyPrint(dst);
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            current->translate(dst,context);
            next->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            current->compile(dst,context);
            next->compile(dst,context);
        }

};

class statement_expr : public Node{
    //STATEMENT_EXPR : ';'
    //               | EXPR ';'

    private:
        NodePtr current;

    public:
            //since first line its no non terminal matched, terminal symbol only so maybe empty set?
        
        statement_expr()
            :current(NULL)
        {}
        
        statement_expr(NodePtr _arg1)
            :current(_arg1)
        {}
 
        std::string name = "statement_expr";
   
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'^ ';
            }
            exp->PrettyPrint(dst);
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
        
            for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
            }     
            if(current != NULL){
                current->translate(dst,context);
            }
            dst<<std::endl;
            
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            current->compile(dst,context);
        }
};
        
class statement : public Node{
    //STATEMENT : STATEMENT_LABELED
    //          | STATEMENT_COMPOUND
    //          | STATEMENT_EXPR
    //          | STATEMENT_SELECTION
    //          | STATEMENT_ITERATION
    //          | STATEMENT_JUMP
    //          | LIST_DECLARATION

    private:
        NodePtr next_statement;
        
    public:
    
    /*
        statement(NodePtr _arg1)
            :next_statement(_arg1)
        {}
    */
        std::string name = "statement";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
           next_statement->PrettyPrint(dst);
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

class statement_labeled :public Node{
    //STATEMENT_LABELED : IDENTIFIER ':' STATEMENT
    //                  | CASE EXPR_CONST ':' STATEMENT
    //                  | DEFAULT ':' STATEMENT
                  
    private:
        NodePtr statement;
        std::string labels;
        NodePtr const_expr;

    public: 
        statement_labeled(std::string symbol,NodePtr _arg1)
            :statement(_arg1)
            ,labels(symbol)
            ,const_expr(NULL)
        {}
        
        statement_labeled(std::string symbol,NodePtr _arg1,NodePtr _arg2)
            :statement(_arg2)
            ,labels(symbol)
            ,const_expr(_arg1)
        {} 

        std::string name = "statement_labeled";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(const_expr != NULL){
                labels->PrettyPrint(dst);
                const_expr->PrettyPrint(dst);
                dst<<':';    
                statement->PrettyPrint(dst);
            }
            else{
                labels->PrettyPrint(dst);
                dst<<':';    
                statement->PrettyPrint(dst);     
            }   
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

            if(const_expr == NULL && labels != "default"){ // label
                dst<<"$"<<labels<<":\n";
                statement->compile(dst,context);            
            }
           
            else if(labels == "default"){
                // will have gone through all cases
                // now just compile default
                statement->compile(dst,context);
                // switch_label would be printed here
            }
            
            else{ // case
                std::string skip = context.makeName("skip");

                // eval const_expr
                const_expr->compile(dst,context);
                uint ce_reg = context.extract_expr_reg();

                // load switch expr
                context.expr_result = context.switch_end_label;
                uint sw_reg = context.extract_expr_reg();

                // compare to switch expr - bne to skip
                dst<<"\tbne\t$"<<ce_reg<<",$"<<sw_reg<<",$"<<skip<<"\n\tnop\n";

                    statement->compile(dst,context);

                dst<<"$"<<skip<<":\n";
            
            }
        }

};

class statement_jump : public Node{ 
    //STATEMENT_JUMP : GOTO IDENTIFIER ';'
    //               | CONTINUE ';'
    //               | BREAK ';'
    //               | RETURN ';'
    //               | RETURN EXPR ';'

    private:
        NodePtr expr;
        std::string symbol;
        std::string symbol_2;

    public:
        statement_jump(std::string name_1,std::string name_2)
            :symbol(name_1)
            ,symbol_2(name_2)
        {}
        
        statement_jump(std::string name_1)
            :symbol(name_1)
            ,symbol_2(" ")
        {}    
        
        statement_jump(std::string name_1,NodePtr _arg)
            : expr(_arg)
            , symbol(name_1)
            , symbol_2(" ")
            
        {}

        std::string name = "statement_jump";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
              
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"return ";
                
                if(expr != NULL){
                    
                    expr->translate(dst,context);
                }
                
                dst<<"\n";

        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(expr != NULL) {
                                     
                expr->compile(dst, context); // prints out an immediate or a register value

                if( context.expr_primary_global_var ) { // global var
                    context.expr_primary_global_var = false;
                    uint gv_reg = context.get_free_reg();
                    dst<<"\tlui\t"<<"$"<<gv_reg<<",%hi("<<context.global_expr_result<<")\n";
                    dst<<"\tlw\t$"<<gv_reg<<",%lo("<<context.global_expr_result<<")($"<<gv_reg<<")\n";
                    dst<<"\tadd\t$2,$0,$"<<gv_reg<<std::endl;
                }
                
                else{
                    // not global
                    context.force_update_variable();

                    if( context.function_type == "double" )
                    {
                        if(regex_match(context.expr_result, context.reNum)){ // literal int
                            dst<<"\taddi\t$2,$0,"<<context.expr_result<<'\n';
                            dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            dst<<"\tcvt.d.w\t"<<"$f0,$f0\n";
                        }

                        else if(regex_match(context.expr_result, context.reFloat)){ // literal float
                            // declare constant
                            std::string fpc = context.makeName("$dpc");
                            std::string tmp = "\t.data\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = fpc + ":\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = "\t.double\t" + context.expr_result + "\n\n";
                            context.fp_constant_dec.push_back(tmp);


                            dst<<"\tlui\t$15,%hi("<<fpc<<")\n";
                            dst<<"\tldc1\t$f0,%lo("<<fpc<<")($15)\n";

                        }
                        
                        else if(regex_match(context.expr_result, context.reChar)){ // literal char
                            int temp_int;
                            std::stringstream convert(context.expr_result);
                            convert>>temp_int;
                            dst<<"\taddi\t$2,$0,"<<temp_int<<'\n';
                            dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            dst<<"\tcvt.d.w\t"<<"$f0,$f0\n";
                        }
                        
                        else if(regex_match(context.expr_result, context.is_reg)){ // register
                            uint reg_num;
                            sscanf(context.expr_result.c_str(), "$%d", &reg_num);

                            dst<<"\tmov.s\t$f0,$f"<<reg_num<<std::endl;
                        }  
                        
                        else{ // local var
                            std::string var_type = context.scopes[context.scope_index][context.expr_result].type;
                            uint var_reg = context.scopes[context.scope_index][context.expr_result].reg_ID;
                            uint var_pos = 4*context.scopes[context.scope_index][context.expr_result].stack_position;

                            if(var_type == "double"){
                                dst<<"\tldc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load double into f0
                            }
                            else if(var_type == "float"){
                                dst<<"\tlwc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load double into f0
                                dst<<"\tcvt.d.s\t"<<"$f0,$f0\n"; // 
                            }
                            else{
                                dst<<"\tadd\t$2,$0,$"<<var_reg<<std::endl;
                                dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                                dst<<"\tcvt.d.w\t"<<"$f0,$f0\n";
                            }                                                    
                            
                        }
                    }

                    if( context.function_type == "float" )
                    {
                        if(regex_match(context.expr_result, context.reNum)){ // literal int
                            dst<<"\taddi\t$2,$0,"<<context.expr_result<<'\n';
                            dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            dst<<"\tcvt.s.w\t"<<"$f0,$f0\n";
                        }

                        else if(regex_match(context.expr_result, context.reFloat)){ // literal float
                            // declare constant
                            std::string fpc = context.makeName("$dpc");
                            std::string tmp = "\t.data\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = fpc + ":\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = "\t.float\t" + context.expr_result + "\n\n";
                            context.fp_constant_dec.push_back(tmp);


                            dst<<"\tlui\t$15,%hi("<<fpc<<")\n";
                            dst<<"\tlwc1\t$f0,%lo("<<fpc<<")($15)\n";

                        }
                        
                        else if(regex_match(context.expr_result, context.reChar)){ // literal char
                            int temp_int;
                            std::stringstream convert(context.expr_result);
                            convert>>temp_int;
                            dst<<"\taddi\t$2,$0,"<<temp_int<<'\n';
                            dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            dst<<"\tcvt.s.w\t"<<"$f0,$f0\n";
                        }
                        
                        else if(regex_match(context.expr_result, context.is_reg)){ // register
                            uint reg_num;
                            sscanf(context.expr_result.c_str(), "$%d", &reg_num);

                            dst<<"\tmov.s\t$f0,$f"<<reg_num<<std::endl;
                        }  
                        
                        else{ // local var
                            std::string var_type = context.scopes[context.scope_index][context.expr_result].type;
                            uint var_reg = context.scopes[context.scope_index][context.expr_result].reg_ID;
                            uint var_pos = 4*context.scopes[context.scope_index][context.expr_result].stack_position;

                            if(var_type == "double"){
                                dst<<"\tldc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load double into f0
                                dst<<"\tcvt.s.d\t"<<"$f0,$f0\n";
                            }
                            else if(var_type == "float"){
                                dst<<"\tlwc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load float into f0
                            }
                            else{
                                dst<<"\tadd\t$2,$0,$"<<var_reg<<std::endl;
                                dst<<"\tmtc1\t"<<"$2,$f0\n"; //move from f0 to $2
                                dst<<"\tcvt.s.w\t"<<"$f0,$f0\n";
                            }                                                    
                            
                        }
                    }


                    if( (context.function_type == "int") || (context.function_type == "unsigned int") )
                    {
                        if(regex_match(context.expr_result, context.reNum)){ // literal
                            dst<<"\taddi\t$2,$0,"<<context.expr_result<<'\n';
                        }

                        else if(regex_match(context.expr_result, context.reFloat)){ // literal float
                            // declare constant
                            std::string fpc = context.makeName("$dpc");
                            std::string tmp = "\t.data\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = fpc + ":\n";
                            context.fp_constant_dec.push_back(tmp);
                            tmp = "\t.double\t" + context.expr_result + "\n\n";
                            context.fp_constant_dec.push_back(tmp);


                            dst<<"\tlui\t$15,%hi("<<fpc<<")\n";
                            dst<<"\tldc1\t$f0,%lo("<<fpc<<")($15)\n";

                            dst<<"\ttrunc.w.d\t"<<"$f0,$f0\n"; // truncate double to int (no rounding)
	                        dst<<"\tmfc1\t"<<"$2,$f0\n"; //move from f0 to $2
                        }
                        
                        else if(regex_match(context.expr_result, context.reChar)){ // literal char
                            int temp_int;
                            std::stringstream convert(context.expr_result);
                            convert>>temp_int;
                            dst<<"\taddi\t$2,$0,"<<temp_int<<'\n';
                        }
                        
                        else if(regex_match(context.expr_result, context.is_reg)){ // register
                            if(context.expr_primary_type == D){
                                uint reg;
                                sscanf(context.expr_result.c_str(), "$%d", &reg);

                                dst<<"\ttrunc.w.d\t"<<"$f0,$f"<<reg<<"\n"; // truncate double to int (no rounding)
	                            dst<<"\tmfc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            }

                            else if(context.expr_primary_type == F){
                                uint reg;
                                sscanf(context.expr_result.c_str(), "$%d", &reg);

                                dst<<"\ttrunc.w.s\t"<<"$f0,$f"<<reg<<"\n"; // truncate double to int (no rounding)
	                            dst<<"\tmfc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            }

                            else{
                                dst<<"\tadd\t$2,$0,"<<context.expr_result<<std::endl;
                            }
                            
                        }  
                        
                        else{ // local var
                            std::string var_type = context.scopes[context.scope_index][context.expr_result].type;
                            uint var_reg = context.scopes[context.scope_index][context.expr_result].reg_ID;
                            uint var_pos = 4*context.scopes[context.scope_index][context.expr_result].stack_position;

                            if(var_type == "double"){
                                dst<<"\tldc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load double into f0
                                dst<<"\ttrunc.w.d\t"<<"$f0,$f0\n"; // truncate double to int (no rounding)
	                            dst<<"\tmfc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            }
                            else if(var_type == "float"){
                                dst<<"\tlwc1\t"<<"$f0,"<<var_pos<<"($fp)\n"; // load double into f0
                                dst<<"\ttrunc.w.s\t"<<"$f0,$f0\n"; // truncate float to int (no rounding)
	                            dst<<"\tmfc1\t"<<"$2,$f0\n"; //move from f0 to $2
                            }
                            else{
                                dst<<"\tadd\t$2,$0,$"<<var_reg<<std::endl;
                            }                                                    
                            
                        }
                    }
                }

            }
            
            if(symbol == "return"){
                // push stack
                dst<<"\taddu\t$sp,$fp,$0\n"; // restore sp
                dst<<"\tlw\t$31,"<<(context.save_stack_pos_31)*4<<"($sp)\n";
                dst<<"\tlw\t$fp,"<<(context.save_stack_pos_fp)*4<<"($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.stack_size*4<<"\n";
                dst<<"\tj\t$31\n\tnop\n"; // jump to return addr
            }
            
            if(symbol == "goto"){
                
                dst<<"\tbeq\t$0,$0,$"<<symbol_2<<"\n\tnop\n";
            }

            if(symbol == "break"){
                
                dst<<"\tbeq\t$0,$0,$"<<context.break_label<<"\n\tnop\n";
            }

            if(symbol == "continue"){
                
                dst<<"\tbeq\t$0,$0,$"<<context.continue_label<<"\n\tnop\n";
            }
            
            
        }
};

class statement_iteration : public Node{
    //STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT
    //                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';' 
    //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT
    //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMENT

    private:
        NodePtr expr;
        NodePtr statement;
        NodePtr statement_expr;
        std::string symbol;
        std::string symbol_1;
        NodePtr statement_expr_rep;

    public:
        statement_iteration(std::string name,NodePtr _arg1,NodePtr _arg2)   //while loop 
            : expr(_arg1)
            , statement(_arg2)
            , statement_expr(NULL)
            , symbol(name)
            , symbol_1(" ")
            , statement_expr_rep(NULL)
        {}
        statement_iteration(std::string name,NodePtr _arg1,std::string name_1,NodePtr _arg2)  //do wnhile
            : expr(_arg2)
            , statement(_arg1)
            , statement_expr(NULL)
            , symbol(name)
            , symbol_1(name_1)
            , statement_expr_rep(NULL)
        {}
        statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2) //for 2 conditions
            :expr(NULL) 
            ,statement(_arg2)
            ,statement_expr(_arg)
            ,symbol(name)
            ,symbol_1(" ")    
            ,statement_expr_rep(_arg1)

        {} 
        statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2,NodePtr _arg3) //for loop  
            :expr(_arg2)
            ,statement(_arg3)
            ,statement_expr(_arg)
            ,symbol(name)
            ,symbol_1(" ")
            ,statement_expr_rep(_arg1)
        {}

        std::string name = "statement_iteration";
   
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {

            //STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT
            //                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';' 
            //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT
            //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMEN
            
            if(symbol == "while"){

                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"while (";
             
                expr->translate(dst,context);
             
                dst<<"):"<<std::endl;
             
                context.indent++;
             
                statement->translate(dst,context);
             
                context.indent--;
            }
            
             
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(symbol_1 == " " && statement_expr == NULL){              //while loop
                std::string bottom_label = context.makeName("btm");
                context.break_label = bottom_label;

                std::string top_label = context.makeName("top");
                context.continue_label = top_label;

                dst<<"$"<<top_label<<":\n";
                expr->compile(dst,context);        
        
                uint expr_reg = context.extract_expr_reg();
        
                dst<<"\tbeq\t"<<"$"<<expr_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";
          
                
                statement->compile(dst,context);

                expr_reg = context.extract_expr_reg();            
                dst<<"\tb\t"<<"$"<<top_label<<"\n\tnop\n";
                dst<<"$"<<bottom_label<<":\n";
            }
            
            
            
            if(symbol_1 != " " && statement_expr == NULL){              //do while loop
         
         
                std::string bottom_label = context.makeName("btm");
                context.break_label = bottom_label;

                std::string top_label = context.makeName("top");
                context.continue_label = top_label;       


                dst<<"$"<<top_label<<":\n";

                statement->compile(dst,context);                

                expr->compile(dst,context);        
                uint expr_reg = context.extract_expr_reg();  
            
                dst<<"\tbeq\t"<<"$"<<expr_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";
            
                dst<<"\tb\t"<<"$"<<top_label<<"\n\tnop\n";
            
                dst<<"$"<<bottom_label<<":\n";
            }
            
            
            
            else if(expr != NULL && statement_expr != NULL){      //for loop 
                uint tmp_condition_reg;    
                std::string top_label = context.makeName("top");
                context.continue_label = top_label;

                std::string bottom_label = context.makeName("bottom");
                context.break_label = bottom_label;


                statement_expr->compile(dst,context);           //generate the statement expression for declaration, eg int i
                dst<<"$"<<top_label<<":\n";

                statement_expr_rep->compile(dst,context);           //generate the condition statement eg i<10
                tmp_condition_reg = context.extract_expr_reg();     //extract the condition for the first time            
                
                dst<<"\tbeq\t"<<"$"<<tmp_condition_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";         //checking if conditions are met, if met exit;
 

                statement->compile(dst,context);                    //generate the statement body, actions taking place during the for loop

                expr->compile(dst,context);                             //operate on the variable  eg i++; only after first condition check thopugh
             
                dst<<"\tb\t"<<"$"<<top_label<<"\n\tnop\n";
              
                dst<<"$"<<bottom_label<<":\n"; 
            }

            else if(expr == NULL && statement_expr != NULL  ){      //for loop with 2 conditoons
               
                uint tmp_condition_reg;    
                std::string top_label = context.makeName("top");
                context.continue_label = top_label;

                std::string bottom_label = context.makeName("bottom");
                context.break_label = bottom_label;


                statement_expr->compile(dst,context);           //generate the statement expression for declaration, eg int i
                dst<<"$"<<top_label<<":\n";
               
                statement_expr_rep->compile(dst,context);           //generate the condition statement eg i<10
                tmp_condition_reg = context.extract_expr_reg();     //extract the condition for the first time            
                
                dst<<"\tbeq\t"<<"$"<<tmp_condition_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";         //checking if conditions are met, if met exit;
 

                statement->compile(dst,context);                    //generate the statement body, actions taking place during the for loop

                dst<<"\tb\t"<<"$"<<top_label<<"\n\tnop\n";
              
                dst<<"$"<<bottom_label<<":\n"; 
          
           }
            
            
        }
};

class statement_selection : public Node{
    //STATEMENT_SELECTION : IF L_BRACKET EXPR R_BRACKET STATEMENT
    //                    | IF L_BRACKET EXPR R_BRACKET STATEMENT ELSE STATEMENT
    //                    | SWITCH L_BRACKET EXPR R_BRACKET STATEMENT

    private:
        NodePtr expr;
        NodePtr statement;
        NodePtr statement_rep;
        std::string symbol_1;
        std::string symbol_2;      
        
    public:
        statement_selection(std::string name,NodePtr _arg,NodePtr _arg1)
            :expr(_arg)
            ,statement(_arg1)
            ,statement_rep(NULL)
            ,symbol_1(name)
            ,symbol_2(" ")
        {}
        
        statement_selection(std::string name,NodePtr _arg,NodePtr _arg1,std::string name1 , NodePtr _arg2)
            :expr(_arg)
            ,statement(_arg1)
            ,statement_rep(_arg2)
            ,symbol_1(name)
            ,symbol_2(name1)
        {}

        std::string name = "statement_selection";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);         
  
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(symbol_1 == "if"){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                dst<<"if(";
                expr->translate(dst,context);
                dst<<")"<<":"<<std::endl;
                
                context.indent++;
                statement->translate(dst,context);
                context.indent--;
            }
            if(symbol_2 == "else"){
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"else:"<<std::endl;
                context.indent++;
                statement_rep->translate(dst,context);
                context.indent--;
            }           
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(symbol_1 == "if" && symbol_2 == " "){
            
                expr->compile(dst,context); // eval expr
            
                uint expr_reg = context.extract_expr_reg();
                std::string bottom_label = context.makeName("if_label");
                
                dst<<"\tbeq\t"<<"$"<<expr_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";
      
                statement->compile(dst,context);

                dst<<"$"<<bottom_label<<":\n"; 
            }
            
            if(symbol_1 == "if" && symbol_2 == "else"){\
                
                expr->compile(dst,context); // eval expr
            
                uint expr_reg = context.extract_expr_reg();
                
                std::string bottom_label = context.makeName("else_label");
                std::string exit_label = context.makeName("exit_label");

                
                dst<<"\tbeq\t"<<"$"<<expr_reg<<",$0,$"<<bottom_label<<"\n\tnop\n";
                
                statement->compile(dst,context);
                
                dst<<"\tbne\t$"<<expr_reg<<",$0,$"<<exit_label<<"\n\tnop\n";
                
                dst<<"$"<<bottom_label<<":\n"; 
                
                statement_rep->compile(dst,context);
                 
                dst<<"$"<<exit_label<<":\n"; 
            
            }
            
            if(symbol_1 == "switch"){
                std::string switch_label = context.makeName("0switch");

                std::string SWE_save = context.switch_end_label;
                context.switch_end_label = switch_label;
                context.break_label = switch_label;

                expr->compile(dst,context); // eval expr
                uint expr_reg = context.extract_expr_reg();

                // internal variable
                this->push_stack(dst,context);
                binding switch_expr;
                switch_expr.reg_ID = expr_reg;
                switch_expr.stack_position = context.stack_size;
                context.scopes[context.scope_index][switch_label] = switch_expr;
                dst<<"\tsw\t$"<<expr_reg<<","<<switch_expr.stack_position*4<<"($fp)\n";

                //compile cases
                statement->compile(dst,context);

                context.switch_end_label = SWE_save;


                //print switch end
                dst<<"$"<<switch_label<<":\n";
            }
            
     }
};


#endif
