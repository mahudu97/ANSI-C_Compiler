#include <stdio.h>
#include <fstream>

#include "ast.hpp"

std::ofstream dstStream; // global to give access to struct

int main(int argc, char* argv[])
{

    if(argc != 5){
        fprintf(stderr, "incorrect argument amount\n");
        exit(1);
    }
    std::string reqO = argv[3];
    if(reqO!= "-o"){
        fprintf(stderr, "third argument is not '-o'\n");
        exit(1);
    }

    // Build AST (FE)
    // 2nd argument is source C file to compile
    
    freopen("tmp/stderr.txt", "w", stderr);
    
    const Node* ast = parseAST(argv[2]);

    fclose(stderr);

    // dest file
    dstStream.open(argv[4]);

    std::string mode = argv[1];
    
    // Print AST
    // bin/c_compiler -P [source-file.c] -o [dest-file.txt]
    if( mode == "-P" ){
        ast->PrettyPrint(dstStream);
    } 

    // Compile to Assembly
    // bin/c_compiler -S [source-file.c] -o [dest-file.s]
    if( mode == "-S" ){
        CompileContext CC;

        CC.stack_size = 0;
        CC.declarations = 0;
        std::map<std::string, binding> global;
        CC.scopes.push_back(global);
        // set regs to free
        for(int i=8; i<=23; i++){
            CC.reg_free[i] = true;
        }

        for(int i=0; i<=19; i++){
            CC.fp_reg_free[i] = true;
        }

        CC.UNARY_OP_MINUS_CHECK = false; // 
        
        CC.scope_index=0; //default global

        CC.err_bottom = false; //used to manage passing reg on recursion
        CC.err_top = false;
        
        // set regex's
        CC.reNum = ("[0-9]*");
        CC.reChar = ("L?['][.]+[']");
        CC.is_reg = ("\\$[0-9]+");
        CC.reFloat = ("-?([0-9]+|[0-9]*\\.[0-9]+([eE][-+]?[0-9]+)?)");

        CC.reg_counter = 8; // initialise reg to be replaced when no free reg's
        CC.fp_reg_counter = 2;
        CC.expr_result_reg = 34;
        CC.err_overide = false;

        CC.declaring = false; // distignuish whether to check through scopes above 

        CC.expr_primary_global_var = false;

        CC.sizeof_type = false;

        CC.extract_global = false;

        CC.assign_reg = 0;
        CC.assigning = false; // for expr_assing not to lose its reg
        CC.assign_reg_set = false;
        CC.parameter = false;

        CC.pushing_parameters = false;

        CC.calling_function = false;

        CC.global_force_update = false;

        CC.extern_global = false;

        CC.makeNameUnq = 0; // used for creating labels

        ast->compile(dstStream, CC);
    } 


    // Translate to Python
    // bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if( mode == "--translate" ){
        TranslateContext TC;
        TC.indent = 0;
        TC.function_dec = false;
        TC.main_exists = false;
        ast->translate(dstStream, TC);

        if(TC.main_exists){
            dstStream<<std::endl;
            dstStream<<"# Boilerplate"<<std::endl;
            dstStream<<"if __name__ == \"__main__\":"<<std::endl;
            dstStream<<"\timport sys"<<std::endl;
            dstStream<<"\tret=main()"<<std::endl;
            dstStream<<"\tsys.exit(ret)"<<std::endl;
        }
    }

    // close output file
    dstStream.close();

    return 0;
}
