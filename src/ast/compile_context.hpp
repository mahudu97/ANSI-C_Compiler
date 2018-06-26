#ifndef compile_context_hpp
#define compile_context_hpp

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

extern std::ofstream dstStream;

struct binding{
    uint reg_ID;        // register number //for global stores adrees
    std::string type;   // C type being stored
    uint stack_position;
    long int internal_value;
    bool is_global;
};

// use to identify tpye a primary expression is
typedef enum{
    S,   // string
    I,   // integer
    UI,  // unsigned integer
    LI,  // long integer
    UL,  // unsigned long
    C,   // character
    F,   // float
    D    // double
} LITERAL_TYPE;

struct CompileContext{



    //fp constants
    std::vector<std::string> fp_constant_dec;

    void print_fp_cd(){
        dstStream << std::fixed;
        for(uint i = 0; i < fp_constant_dec.size(); ++i)
            dstStream<<fp_constant_dec[i];

        fp_constant_dec.clear();
    }

    // fp regs
    bool fp_reg_free[20]; // caller-saved
    uint fp_assign_reg;
    uint get_fp_free_reg(){

        for(uint i=2u; i<=17u; i++){
            if( (fp_reg_free[i] == true) && (i!=12) && (i!=14)  && (i%2 == 0) && (extract_global || fp_not_in_err_stack(i)) ){ // register to be saved by calling function
                fp_reg_free[i] = false;                                    // short circuit

                        // EXPR_ASSIGNMENT 
                if(assigning && !assign_reg_set){ // need to set assign reg
                    fp_assign_reg = i;
                    assign_reg_set = true;
                }

                return i;
            }
        }

        // all reg occupied
        free_up_fp_reg();  // free up a reg from $8-$15
        return get_fp_free_reg(); // now get a free reg - guaranteed
    }

    uint fp_reg_counter;
    void free_up_fp_reg(){
        uint s_pos;
        bool found = false;
        std::string type;

        if(fp_reg_counter == assign_reg && assigning) fp_reg_counter += 2; // avoid freeing assign_reg

        // first save any varubale stored in fp_reg_counter
        // search through variables in current scope
        //std::cout << scope_index << std::endl;
        for(std::map<std::string, binding>::iterator it= scopes[scope_index].begin(); it !=scopes[scope_index].end(); ++it){
            if(it->second.reg_ID == fp_reg_counter && (it->first != expr_result)){ // replace this variable
                // save locally
                s_pos = it->second.stack_position;
                type = it->second.type;
                found = true;

                // update variable to not ina register
                it->second.reg_ID = 33;
            }
        }

        if(found){
            // store variable in reg back onto stack
            if(type == "float") dstStream<<"\tswc1\t$f"<<fp_reg_counter<<","<<s_pos*4<<"($fp)\n";

            else dstStream<<"\tsdc1\t$f"<<fp_reg_counter<<","<<s_pos*4<<"($fp)\n";
        }

        // now free it
        fp_reg_free[fp_reg_counter] = true; // free the reg
        if(++fp_reg_counter == 17u) fp_reg_counter = 2u; // loop back for next replacement

    }








    // normal regs
    bool reg_free[32];              // check if reg available
    uint get_free_reg(){

        for(uint i=0u; i<32u; i++){
            if(reg_free[i] == true && ( i >= 8u && i <= 14u ) && (extract_global || not_in_err_stack(i)) ){ // register to be saved by calling function
                reg_free[i] = false;                                    // short circuit

                        // EXPR_ASSIGNMENT 
                if(assigning && !assign_reg_set){ // need to set assign reg
                    assign_reg = i;
                    assign_reg_set = true;
                }

                return i;
            }
        }

        // all reg occupied
        free_up_reg();  // free up a reg from $8-$15
        return get_free_reg(); // now get a free reg - guaranteed
    }

    uint assign_reg; // this cannot be freed
    uint reg_counter;
    void free_up_reg(){
        uint s_pos;
        bool found = false;
        if(reg_counter == assign_reg && assigning) ++reg_counter; // avoid freeing assign_reg

        // first save any varubale stored in reg_counter
        // search through variables in current scope
        //std::cout << scope_index << std::endl;
        for(std::map<std::string, binding>::iterator it= scopes[scope_index].begin(); it !=scopes[scope_index].end(); ++it){
            if(it->second.reg_ID == reg_counter && (it->first != expr_result)){ // replace this variable
                // save locally
                s_pos = it->second.stack_position;
                found = true;

                // update variable to not ina register
                it->second.reg_ID = 33;
            } 
        }

        if(found){
            // store variable in reg back onto stack
            dstStream<<"\tsw\t$"<<reg_counter<<","<<s_pos*4<<"($fp)\n";
        }

        // now free it
        reg_free[reg_counter] = true; // free the reg
        if(++reg_counter == 14u) reg_counter = 8u; // loop back for next replacement

    }





    std::string expr_result_reg; // deal with recursion and passing registers
    bool err_top;
    bool err_bottom;
    bool err_overide;
    bool sizeof_type;


    std::vector<uint> err_stack;
    uint err_overide_reg;

    std::vector<uint> fp_err_stack;
    uint fp_err_overide_reg;


    bool not_in_err_stack(uint x){
        for(uint i = 0; i < err_stack.size(); ++i){
            if(err_stack[i] == x){
                return false; // found this value in the stack
            }
        }
        
        return true; // free to use this value - its not been preserved
    }

    bool fp_not_in_err_stack(uint x){
        for(uint i = 0; i < fp_err_stack.size(); ++i){
            if(fp_err_stack[i] == x){
                return false; // found this value in the stack
            }
        }
        
        return true; // free to use this value - its not been preserved
    }
    
    
    bool assigning;  // when true - do not free up assign reg
    bool assign_reg_set;









    bool update_variable(){ // return true when given a new reg - i.e. loaded from the stack // only for variables

        uint local = scopes[scope_index][expr_result].reg_ID; // store reg ID locally

        uint s_pos = scopes[scope_index][expr_result].stack_position;

        if(!declaring && (s_pos == 0) && (scope_index > 0) && no_match() ){   //var not in this scope
            binding tmp = out_of_scope(scope_index-1);
            local = tmp.reg_ID;
            s_pos = tmp.stack_position;
        }
            
        if((local == 33) && (s_pos != 0)){    // unassigned -
            if( (expr_primary_type == F) || (expr_primary_type == D) ){
                local = get_fp_free_reg();
            }
            else local = get_free_reg();

            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            scopes[scope_index][expr_result].stack_position = s_pos;

                       
            if( expr_primary_type == F ){
                dstStream<<"\tlwc1\t$f"<<local<<","<<s_pos*4<<"($sp)\n";
            }
            else if( expr_primary_type == D ){
                dstStream<<"\tldc1\t$f"<<local<<","<<s_pos*4<<"($sp)\n";
            }
            else{
                dstStream<<"\tlw\t$"<<local<<","<<s_pos*4<<"($sp)\n";
            }

            return true;
        }    

        return false;   // no update made
    }

    void force_update_variable(){ // return true when given a new reg - i.e. loaded from the stack // only for variables
        uint local;
        // float variable
        if( (expr_primary_type == F) || (expr_primary_type == D) ){
            local = get_fp_free_reg();
        }
        else local = get_free_reg();

        uint s_pos;
        if(!global_force_update){
            s_pos = scopes[scope_index][expr_result].stack_position;


            if(!declaring && (s_pos == 0) && (scope_index > 0) && no_match() ){   //var not in this scope
                s_pos = out_of_scope(scope_index-1).stack_position;
            }
        
            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            scopes[scope_index][expr_result].stack_position = s_pos;

            if( expr_primary_type == F ){
                if(!(s_pos==0)) dstStream<<"\tlwc1\t$f"<<local<<","<<s_pos*4<<"($sp)\n";
            }
            else if( expr_primary_type == D ){
                if(!(s_pos==0)) dstStream<<"\tldc1\t$f"<<local<<","<<s_pos*4<<"($sp)\n";
            }
            else{
                if(!(s_pos==0)) dstStream<<"\tlw\t$"<<local<<","<<s_pos*4<<"($sp)\n";
            }

        }

        else{
            scopes[0][expr_result].reg_ID = local;
        }

    }

    bool declaring;

    binding out_of_scope(uint index){
        binding res;
        //iterate through map from scope above
        if(scopes[index].count(expr_result) == 1){
            res = scopes[index][expr_result];
            return res;
        }

        if(index == 0) std::exit(EXIT_FAILURE); // safety for invalid C
        res = out_of_scope(index-1);
        return res;
    }

    uint set_literal_reg(){ // give a register to literal value for temperorary usage later

        uint local = get_free_reg(); //  assign literal a register
        dstStream<<"\taddi\t$"<<local<<",$0,"<<expr_result<<'\n';
        return local;
        
    }
    

    uint extract_expr_reg(){

        if(expr_primary_global_var){ // expr primary
            expr_result = global_expr_result;
        }

        uint reg;
        //literal
        if(regex_match(expr_result, reNum)) reg = set_literal_reg();

        // check if reg
        else if(regex_match(expr_result, is_reg)) sscanf(expr_result.c_str(),"$%d", &reg);

        // constant for double
        else if( (expr_primary_type==D) && ((regex_match(expr_result, reNum))||(regex_match(expr_result, reFloat))) ){
        
            reg = get_fp_free_reg(); //  assign literal a register
            std::string fpc = makeName("$dpc");
                            
            std::string tmp = "\n\t.data\n";
            fp_constant_dec.push_back(tmp);
            tmp = fpc + ":\n";
            fp_constant_dec.push_back(tmp);
            tmp = "\t.double\t" + expr_result + "\n\n";
            fp_constant_dec.push_back(tmp);

            dstStream<<"\tlui\t$15,%hi("<<fpc<<")\n";
            dstStream<<"\tldc1\t$f"<<reg<<",%lo("<<fpc<<")($15)\n";
            
        }

        // constant for float
        else if( (expr_primary_type==F) && ((regex_match(expr_result, reNum))||(regex_match(expr_result, reFloat))) ){
            
            reg = get_fp_free_reg(); //  assign literal a register
            std::string fpc = makeName("$fpc");
                            
            std::string tmp = "\n\t.data\n";
            fp_constant_dec.push_back(tmp);
            tmp = fpc + ":\n";
            fp_constant_dec.push_back(tmp);
            tmp = "\t.float\t" + expr_result + "\n\n";
            fp_constant_dec.push_back(tmp);

            dstStream<<"\tlui\t$15,%hi("<<fpc<<")\n";
            dstStream<<"\tlwc1\t$f"<<reg<<",%lo("<<fpc<<")($15)\n";

        }

        // variable
        else{
            if(!expr_primary_global_var){  // not global
                uint reg_save = scopes[scope_index][expr_result].reg_ID;

                force_update_variable();  // force the variable a new reg

                reg = scopes[scope_index][expr_result].reg_ID;

                scopes[scope_index][expr_result].reg_ID = reg_save; //restore binding

            }

            else{   // global var
                extract_global = true;
                reg = get_free_reg();
                extract_global = false;
                
                // load address of global var from heap
                dstStream<<"\tlui\t"<<"$"<<reg<<",%hi("<<expr_result<<")\n";

                // load value form heap
                dstStream<<"\tlw\t$"<<reg<<",%lo("<<expr_result<<")($"<<reg<<")\n";
            }
        }

        return reg;
    }



    std::string am_i_bottom(){ // returns reg as num in string

        if(expr_primary_global_var){ // expr primary
            expr_result = global_expr_result;
        }
        // not conditional
        if(!err_overide){
            if(err_bottom == false){
                err_bottom = true; //

                uint err_stack_reg;

                if(regex_match(expr_result, reNum)){ // literal int
                    err_stack_reg = get_free_reg();
                    expr_result_reg = std::to_string( err_stack_reg ); // find a free reg - format [0-9]+
                    UNARY_UPDATE(); // check for unary opr
                    dstStream<<"\taddi\t"<<"$"<<expr_result_reg<<",$0,"<<expr_result<<'\n';  
                }
                else if(sizeof_type){ // size of type
                    err_stack_reg = get_free_reg();
                    expr_result_reg = std::to_string( err_stack_reg );
                }

                else if(calling_function){  // function call
                    if( ( expr_primary_type == F ) || ( expr_primary_type == D ) ){
                        err_stack_reg = get_fp_free_reg();
                    }
                    else{
                        err_stack_reg = get_free_reg();
                    }

                    expr_result_reg = std::to_string( err_stack_reg );
                }

                else
                {   // variable
                    if(!expr_primary_global_var){
                        uint reg_save = scopes[scope_index][expr_result].reg_ID;
                        force_update_variable();  // froce a new reg
                                                
                        err_stack_reg = scopes[scope_index][expr_result].reg_ID;
                        expr_result_reg = std::to_string(scopes[scope_index][expr_result].reg_ID);
                        
                        scopes[scope_index][expr_result].reg_ID = reg_save; //restore binding
                    }
                    else{ // global var load
                        uint reg_save = scopes[0][expr_result].reg_ID;

                        global_force_update = true;
                        force_update_variable();  // froce a new reg
                        global_force_update = false;
                            // load from heap
                        
                        err_stack_reg = scopes[0][expr_result].reg_ID;
                        
                        // load address of global var from heap
                        dstStream<<"\tlui\t"<<"$"<<err_stack_reg<<",%hi("<<expr_result<<")\n";

                        // load value form heap
                        dstStream<<"\tlw\t$"<<err_stack_reg<<",%lo("<<expr_result<<")($"<<err_stack_reg<<")\n";

                        scopes[0][expr_result].reg_ID = reg_save; //restore binding

                        expr_result_reg = std::to_string(err_stack_reg); // reg with value of global var
                    }
                }


                err_stack.push_back(err_stack_reg); // save reg to preserved stack
            }
        }
        
        else{
            expr_result_reg = std::to_string(err_overide_reg); // use set reg
        }

        return expr_result_reg;

    }

    bool am_i_top(){
        if(err_top == false){
            err_top = true; // a node is top
            return true;
        }
        return false;

    }

    void i_am_top(std::string r){
        if(err_top == true){
            err_top = false;
            err_bottom = false;
            if(! (err_stack.size() == 0)){
                err_stack.pop_back();
            }
            if(! (fp_err_stack.size() == 0)){
                fp_err_stack.pop_back();
            }

            /*
            if(expr_primary_type == D){
                uint new_r = = get_free_reg(); 
                dstStream<<"\tcvt.w.d\t"<<"$f"<<r<<",$f"<<r<<"\n";
                dstStream<<"\tmfc1\t"<<"$"<<new_r<<",$f"<<r<<"\n";
            }
            else if(expr_primary_type == F){
                uint new_r = = get_free_reg(); 
                dstStream<<"\tcvt.s.d\t"<<"$f"<<r<<",$f"<<r<<"\n";
                dstStream<<"\tmfc1\t"<<"$"<<new_r<<",$f"<<r<<"\n";
            }
            */
            expr_result = "$"+r;
        }
    }










    std::string break_label;
    std::string continue_label;
    std::string switch_end_label;

    long int internal_expr_value;
    long int internal_temp_value;





    // used to check if variable is a literal - set in int main
    std::regex reNum;
    std::regex reChar;
    std::regex is_reg;
    std::regex reFloat;

    bool no_match(){
        if(
            !regex_match(expr_result, reNum)
            &&
            !regex_match(expr_result, reChar)
            &&
            !regex_match(expr_result, is_reg)
            &&
            !regex_match(expr_result, reFloat)
        ){
            return true;        // is a variable
        }
        return false; // not a variable
    }






    uint stack_size;                // dealing with stack pointer

    uint declarations;      // keep track of amount of declarations inside a scope

    std::vector<std::map<std::string, binding> > scopes; // vector of bindings for each scope
    // map - variable name to register number
    uint scope_index;       // use to access orrect map in vector


    std::string current_func;
    std::string tmp_v;              // used to transfer variable name across nodes
    std::string global_var_size;


    std::string expr_result;
    std::string global_expr_result;

    bool global_force_update;


    bool expr_primary_global_var; // for assigning a glboal var a new valuie
    LITERAL_TYPE expr_primary_type;
    std::string expr_cast_type;
    bool type_cast;

    void set_expr_result_type(){        // used in declare
            std::string type = scopes[scope_index][expr_result].type;
            if(type == "int"){
                expr_primary_type = I;
            }
            else if(type == "unsigned int"){
                expr_primary_type = UI;
            }
            else if(type == "char"){
                expr_primary_type = C;
            }
            else if(type == "float"){
                expr_primary_type = F;
            }
            else if(type == "double"){
                expr_primary_type = D;
            }
    }






    int makeNameUnq;

    std::string makeName(std::string base){
        return base+std::to_string(makeNameUnq++);
    }
    const char* makeNameChar(const char* base){ // use to create new file
        std::string res = base+std::to_string(makeNameUnq++);
        const char* ret = res.c_str() + '.' + 's';
        return ret;
    }




    bool UNARY_OP_MINUS_CHECK;
    void UNARY_OP_MINUS_UPDATE(){
        expr_result = "-" + expr_result;
        UNARY_OP_MINUS_CHECK = false;
    }

    void UNARY_UPDATE(){
        if(UNARY_OP_MINUS_CHECK) UNARY_OP_MINUS_UPDATE();
    }





    // global checks

    bool extract_global;

    std::map<std::string,bool> extern_globals; // clear at end of a scope
    bool extern_global;

    bool check_global(uint index, std::string var){
        // returns false if find var names not in global scope
        //searchthrough map of current scope
        uint found = scopes[index].count(var);

        uint Extern =  extern_globals.count(var);

            // external global
            if(Extern){
                return true;
            }

            // not global
            if((index != 0) && found ){
                return false;
            }


            // global
            else if((index == 0) && found ){
                return true;
            }
            

        if(index == 0){
            return false; // function name
        }

        return check_global(index-1, var);
    }






    // function calling
    void function_end(){
        stack_size = 0;
        
    }


    void save_regs(bool reg_array[] , bool fp_reg_array[] ){

        for(uint i=0; i<32; ++i){
            reg_array[i] = reg_free[i];
            reg_free[i] = true;
            fp_reg_array[i] = fp_reg_free[i];
            fp_reg_free[i] = true;
        }

            


        dstStream<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,-96"<<'\n';
        
        for(uint i = 1; i <= stack_size ; i++){ 
            dstStream<<"\tlw\t$15,"<<i*4+96<<"($sp)"<<'\n'; 
            dstStream<<"\tsw\t$15,"<<i*4<<"($sp)"<<'\n';
        }                    

        for(uint i = 0; i < 8; i++){
            //store to stack
            dstStream<<"\tsw\t$"<<i+8<<","<<(i+stack_size+1)*4<<"($sp)"<<'\n';
        }

        for(uint i = 0; i < 16; i++){
            //store to stack
            if( (i+2)%2 == 0) dstStream<<"\tsdc1\t$f"<<i+2<<","<<(i+stack_size+9)*4<<"($sp)"<<'\n';
        }


        dstStream<<"\taddu\t$fp,$sp,$0"<<std::endl; 
        stack_size += 24;
    }

    void restore_regs(bool reg_array[], bool fp_reg_array[] ){

        for(uint i=0; i<32; ++i){
            reg_free[i] = reg_array[i];
            fp_reg_free[i] = fp_reg_array[i];
        }
        
        for(uint i = 0; i < 16; i++){
            //store to stack
            if( (17-i)%2 == 0) dstStream<<"\tldc1\t$f"<<17-i<<","<<(stack_size-i)*4<<"($sp)"<<'\n';
        }

        for(uint i = 0; i < 8; i++){
            //load from stack
            dstStream<<"\tlw\t$"<<15-i<<","<<(stack_size-i-16)*4<<"($sp)"<<'\n';
        }

        // shift up
        for(uint i = 1; i <= stack_size-24 ; i++){ 
            dstStream<<"\tlw\t$15,"<<i*4<<"($sp)"<<'\n'; 
            dstStream<<"\tsw\t$15,"<<i*4+96<<"($sp)"<<'\n';
        }                   

        dstStream<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,96"<<'\n';
        stack_size -= 24;
        dstStream<<"\taddu\t$fp,$sp,$0"<<std::endl;  
    }


    bool pushing_parameters;
    
    

    int func_arg_reg_count, fp_func_arg_reg_count;
    std::string function_type;
    std::map<std::string,std::string>functions;

    bool calling_function;

    bool is_function(std::string name){
        if(functions.count(name) == 0){
            return false; // not a funciton
        }
        return true;
    }
    
    uint get_type_bytesize(std::string type){
    
        if(type == "char" || type == "unsigned char" || type == "signed char"){
            return 1;
        }
        
        
        else if(type == "short" || type == "signed short" || type == "unsigned short"){
            return 2;
        }
        
        else if(type == "long long int" || type == "long long" || type == "signed long long" || type == "unsigned long long" || 
                         type == "signed long long int" || type == "unsigned long long int" || type == "long float"|| type == "double"){
        
            return 8;
        
        }

        else if(type == "void"){
            return 0;
        }

        else{
            return 4;
        }
        
    }    
    
    bool parameter;
    
    uint parameter_num, fp_parameter_num;
    
    std::map<std::string, binding> param_bindings;    
    
    uint save_stack_pos_31;
    uint save_stack_pos_fp;


};


#endif