// Grammar developed with the guide of the draft C89 spec
// X3J11 Technical Committee on the C Programming Language. The C89 Draft. Available from: https://port70.net/~nsz/c/c89/c89-draft.html [Accessed 2nd February 2018].

%code requires{
  #include "ast.hpp"

  #include <cassert>

  extern FILE* yyin;
  extern const Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Node* node;
  std::string* string;
  int intValue;
  unsigned int uintValue;
  long int longintValue;
  unsigned long longuintValue;
  char characterValue;
  double floatValue;
}

           

%token IDENTIFIER STRING_LITERAL SIZEOF 
 
%token INT_C UNSIGNED_C LONG_C UNSIGNED_LONG_C
%token CHARACTER_C FLOAT_C

// Keywords
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE  BREAK RETURN
%token TYPEDEF EXTERN STATIC AUTO REGISTER STRUCT UNION ENUM ELIPSIS

%token R_BRACKET L_BRACKET R_SQUARE L_SQUARE R_BRACE L_BRACE


// Operators
%token OP_PLUS OP_MINUS OP_MUL OP_DIV OP_MOD OP_INC OP_DEC
%token OP_L OP_G OP_LE OP_GE OP_EQ OP_NE
%token OP_LNOT OP_LAND OP_LOR
%token OP_BAND OP_BOR OP_BXOR OP_B_ONESC OP_BRIGHT OP_BLEFT
%token OP_PTR 


// Assignments
%token ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token RIGHT_ASSIGN LEFT_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN


%type <node> EXPR EXPR_PRIMARY EXPR_POSTFIX EXPR_UNARY OPR_UNARY EXPR_CAST EXPR_MUL EXPR_ADD 
             EXPR_SHIFT EXPR_RELATIONAL EXPR_EQUALITY EXPR_AND EXPR_XOR EXPR_INCLUSIVE_OR 
             EXPR_LOGIC_AND EXPR_LOGIC_OR EXPR_CONDITIONAL EXPR_ASSIGNMENT OPR_ASSIGNMENT EXPR_CONST DECLARATION SPECIFIER_DECLARATION
             DECLARATOR_INIT_LIST DECLARATOR_INIT SPECIFIER_STORE_CLASS SPECIFIER_TYPE SPECIFIER_UNION_OR_STRUCT UNION_OR_STRUCT
             DECLARATION_LIST_STRUCT DECLARATION_STRUCT LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR STRUCT_DECLARATOR SPECIFIER_ENUM
             LIST_ENUMERATOR ENUMERATOR QUALIFIER_TYPE DECLARATOR DECLARATOR_DIRECT POINTER LIST_QUALIFIER_TYPE LIST_PARAM_TYPE
             LIST_PARAMETER DECLARATION_PARAMETER LIST_IDENTIFIER TYPE_NAME DECLARATOR_ABSTRACT 
             DECLARATOR_DIRECT_ABSTRACT INITIALIZER LIST_INITIALIZER ARG_EXPR_LIST
             STATEMENT STATEMENT_LABELED STATEMENT_COMPOUND LIST_DECLARATION LIST_STATEMENT STATEMENT_EXPR STATEMENT_SELECTION
             STATEMENT_ITERATION STATEMENT_JUMP
             PROGRAM DECLARATION_EXTERNAL DEFINITION_FUNCTION
                             
                                       // for grammar production rules
%type <string> IDENTIFIER STRING_LITERAL
%type <intValue> INT_C
%type <uintValue> UNSIGNED_C
%type <longintValue> LONG_C
%type <longuintValue> UNSIGNED_LONG_C
%type <characterValue> CHARACTER_C
%type <floatValue> FLOAT_C


%start ROOT

%%

ROOT : PROGRAM { g_root = $1; }


EXPR_PRIMARY  :IDENTIFIER                 { $$ = new expr_primary(*$1); }
              |INT_C                      { $$ = new expr_primary($1);  }
              |UNSIGNED_C                 { $$ = new expr_primary($1);  }
              |LONG_C                     { $$ = new expr_primary($1);  }
              |UNSIGNED_LONG_C            { $$ = new expr_primary($1);  }
              |CHARACTER_C                { $$ = new expr_primary($1);  }
              |FLOAT_C                    { $$ = new expr_primary($1);  }
              |STRING_LITERAL             { $$ = new expr_primary(*$1); }
              |L_BRACKET EXPR R_BRACKET   { $$ = new expr_primary($2);  }
            

EXPR_POSTFIX : EXPR_PRIMARY                                     { $$ = $1;                             }
             | EXPR_POSTFIX L_SQUARE EXPR R_SQUARE              { $$ = new expr_postfix($1, $3);       }
             | EXPR_POSTFIX L_BRACKET R_BRACKET                 { $$ = new expr_postfix($1, true);     }
             | EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET   { $$ = new expr_postfix($1, true, $3); }
             | EXPR_POSTFIX '.' IDENTIFIER                      { $$ = new expr_postfix($1, ".", *$3); }
             | EXPR_POSTFIX OP_PTR IDENTIFIER                   { $$ = new expr_postfix($1, "->", *$3);}
             | EXPR_POSTFIX OP_INC                              { $$ = new expr_postfix("++", $1);     }
             | EXPR_POSTFIX OP_DEC                              { $$ = new expr_postfix("--", $1);     }


ARG_EXPR_LIST : EXPR_ASSIGNMENT                       { $$ = new arg_expr_list($1);     }
              | ARG_EXPR_LIST ',' EXPR_ASSIGNMENT     { $$ = new arg_expr_list($1, $3); }

           
EXPR_UNARY : EXPR_POSTFIX                           { $$ = $1;                           } 
           | OP_INC EXPR_UNARY                      { $$ = new expr_unary("++", $2);     } 
           | OP_DEC EXPR_UNARY                      { $$ = new expr_unary("--", $2);     } 
           | OPR_UNARY EXPR_CAST                    { $$ = new expr_unary($1, $2);       } 
           | SIZEOF EXPR_UNARY                      { $$ = new expr_unary("sizeof", $2); }  
           | SIZEOF L_BRACKET TYPE_NAME R_BRACKET   { $$ = new expr_unary("sizeof", $3, true); }          


OPR_UNARY : OP_BAND     { $$ = new opr_unary("&"); }
          | OP_MUL      { $$ = new opr_unary("*"); }
          | OP_PLUS     { $$ = new opr_unary("+"); }
          | OP_MINUS    { $$ = new opr_unary("-"); }
          | OP_B_ONESC  { $$ = new opr_unary("~"); }
          | OP_LNOT     { $$ = new opr_unary("!"); }


EXPR_CAST : EXPR_UNARY                                { $$ = $1;                    }
          | L_BRACKET TYPE_NAME R_BRACKET EXPR_CAST   { $$ = new expr_cast($2, $4); }
        
                
EXPR_MUL : EXPR_CAST                  { $$ = $1;                        }
         | EXPR_MUL OP_MUL EXPR_CAST  { $$ = new expr_mul($1, "*", $3); }
         | EXPR_MUL OP_DIV EXPR_CAST  { $$ = new expr_mul($1, "/", $3); }
         | EXPR_MUL OP_MOD EXPR_CAST  { $$ = new expr_mul($1, "%", $3); }
         
                
EXPR_ADD : EXPR_MUL                   { $$ = $1;                        }
         | EXPR_ADD OP_PLUS EXPR_MUL  { $$ = new expr_add($1, "+", $3); }
         | EXPR_ADD OP_MINUS EXPR_MUL { $$ = new expr_add($1, "-", $3); }
            
             
EXPR_SHIFT : EXPR_ADD                       { $$ = $1;                           }
           | EXPR_SHIFT OP_BLEFT EXPR_ADD   { $$ = new expr_shift($1, "<<", $3); }
           | EXPR_SHIFT OP_BRIGHT EXPR_ADD  { $$ = new expr_shift($1, ">>", $3); }
        
                
EXPR_RELATIONAL : EXPR_SHIFT                        { $$ = $1;                                }
                | EXPR_RELATIONAL OP_L EXPR_SHIFT   { $$ = new expr_relational($1, "<", $3);  }
                | EXPR_RELATIONAL OP_G EXPR_SHIFT   { $$ = new expr_relational($1, ">", $3);  }
                | EXPR_RELATIONAL OP_LE EXPR_SHIFT  { $$ = new expr_relational($1, "<=", $3); }
                | EXPR_RELATIONAL OP_GE EXPR_SHIFT  { $$ = new expr_relational($1, ">=", $3); }
     
               
EXPR_EQUALITY : EXPR_RELATIONAL                     { $$ = $1;                              }
              | EXPR_EQUALITY OP_EQ EXPR_RELATIONAL { $$ = new expr_equality($1, "==", $3); }
              | EXPR_EQUALITY OP_NE EXPR_RELATIONAL { $$ = new expr_equality($1, "!=", $3); }


EXPR_AND : EXPR_EQUALITY                    { $$ = $1;                   }
         | EXPR_AND OP_BAND EXPR_EQUALITY   { $$ = new expr_and($1, $3); }
         

EXPR_XOR : EXPR_AND                   { $$ = $1;                   }
         | EXPR_XOR OP_BXOR EXPR_AND  { $$ = new expr_xor($1, $3); }
         

EXPR_INCLUSIVE_OR : EXPR_XOR                            { $$ = $1;                            }
                  | EXPR_INCLUSIVE_OR OP_BOR EXPR_XOR   { $$ = new expr_inclusive_or($1, $3); }
                  
                
EXPR_LOGIC_AND : EXPR_INCLUSIVE_OR                          { $$ = $1;                         }
               | EXPR_LOGIC_AND OP_LAND EXPR_INCLUSIVE_OR   { $$ = new expr_logic_and($1, $3); }
             
                          
EXPR_LOGIC_OR : EXPR_LOGIC_AND                        { $$ = $1;                         }
              | EXPR_LOGIC_OR OP_LOR EXPR_LOGIC_AND   { $$ = new expr_logic_or($1, $3); }
              
          
EXPR_CONDITIONAL : EXPR_LOGIC_OR                                { $$ = $1;                               }
                 | EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL  { $$ = new expr_conditional($1, $3, $5); }
         
       
EXPR_ASSIGNMENT : EXPR_CONDITIONAL                            { $$ = $1;                              }
                | EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT   { $$ = new expr_assignment($1, $2, $3); }

           
OPR_ASSIGNMENT : ASSIGN           { $$ = new opr_assignment("=");   }
               | MUL_ASSIGN       { $$ = new opr_assignment("*=");  }
               | DIV_ASSIGN       { $$ = new opr_assignment("/=");  }
               | MOD_ASSIGN       { $$ = new opr_assignment("%=");  }
               | ADD_ASSIGN       { $$ = new opr_assignment("+=");  }
               | SUB_ASSIGN       { $$ = new opr_assignment("-=");  }
               | LEFT_ASSIGN      { $$ = new opr_assignment("<<="); }
               | RIGHT_ASSIGN     { $$ = new opr_assignment(">>="); }
               | AND_ASSIGN       { $$ = new opr_assignment("&=");  }
               | XOR_ASSIGN       { $$ = new opr_assignment("^=");  }
               | OR_ASSIGN        { $$ = new opr_assignment("|=");  }


EXPR : EXPR_ASSIGNMENT            { $$ = $1;               }
     | EXPR ',' EXPR_ASSIGNMENT   { $$ = new expr($1, $3); }
     

EXPR_CONST : EXPR_CONDITIONAL   { $$ = $1; }


DECLARATION : SPECIFIER_DECLARATION ';'                       { $$ = new declaration($1); }
            | SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'  { $$ = new declaration($1, $2); }
            

SPECIFIER_DECLARATION : SPECIFIER_STORE_CLASS                         { $$ = $1; }
                       | SPECIFIER_STORE_CLASS SPECIFIER_DECLARATION  { $$ = new specifier_declaration($1, $2); }
                       | SPECIFIER_TYPE                               { $$ = $1; }
                       | SPECIFIER_TYPE SPECIFIER_DECLARATION         { $$ = new specifier_declaration($1, $2); }
                       | QUALIFIER_TYPE                               { $$ = $1; }
                       | QUALIFIER_TYPE SPECIFIER_DECLARATION         { $$ = new specifier_declaration($1, $2); }
 
 
DECLARATOR_INIT_LIST : DECLARATOR_INIT                              { $$ = $1;                              }
                     | DECLARATOR_INIT_LIST ',' DECLARATOR_INIT     { $$ = new declarator_init_list($1, $3);}


DECLARATOR_INIT : DECLARATOR                        { $$ = new declarator_init($1);          }
                | DECLARATOR ASSIGN INITIALIZER     { $$ = new declarator_init( $1,'=' , $3);}

              
SPECIFIER_STORE_CLASS : TYPEDEF   { $$ = new specifier_store_class("typedef");  }
                      | EXTERN    { $$ = new specifier_store_class("extern");   }
                      | STATIC    { $$ = new specifier_store_class("static");   }
                      | AUTO      { $$ = new specifier_store_class("auto");     }
                      | REGISTER  { $$ = new specifier_store_class("register"); }
              
                    
SPECIFIER_TYPE : VOID                         { $$ = new specifier_type("void");     }
               | CHAR                         { $$ = new specifier_type("char");     }
               | SHORT                        { $$ = new specifier_type("short");    }
               | INT                          { $$ = new specifier_type("int");      }
               | LONG                         { $$ = new specifier_type("long");     }
               | FLOAT                        { $$ = new specifier_type("float");    }
               | DOUBLE                       { $$ = new specifier_type("double");   }
               | SIGNED                       { $$ = new specifier_type("signed");   }
               | UNSIGNED                     { $$ = new specifier_type("unsigned"); }
               | SPECIFIER_UNION_OR_STRUCT    { $$ = $1;                             }
               | SPECIFIER_ENUM               { $$ = $1;                             }
               
             
SPECIFIER_UNION_OR_STRUCT : UNION_OR_STRUCT IDENTIFIER L_BRACE DECLARATION_LIST_STRUCT R_BRACE  { $$ = new specifier_union_or_struct($1, *$2, $4); }
                          | UNION_OR_STRUCT L_BRACE DECLARATION_LIST_STRUCT R_BRACE             { $$ = new specifier_union_or_struct($1, $3);      }
                          | UNION_OR_STRUCT IDENTIFIER                                          { $$ = new specifier_union_or_struct($1, *$2);    }
                          
                                                 
UNION_OR_STRUCT : STRUCT    { $$ = new union_or_struct("struct"); }
                | UNION     { $$ = new union_or_struct("union" ); }
                
               
DECLARATION_LIST_STRUCT : DECLARATION_STRUCT                                { $$ = $1;}
                        | DECLARATION_LIST_STRUCT DECLARATION_STRUCT        { $$ = new declaration_list_struct($1, $2);}
                        

DECLARATION_STRUCT : LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR ';'   { $$ = new declaration_struct ($1, $2); }


LIST_SPEC_QUAL : SPECIFIER_TYPE LIST_SPEC_QUAL    { $$ = new list_spec_qual($1, $2); }
               | SPECIFIER_TYPE                   { $$ = $1;                         }
               | QUALIFIER_TYPE LIST_SPEC_QUAL    { $$ = new list_spec_qual($1, $2); }
               | QUALIFIER_TYPE                   { $$ = $1;                         }
               
                            
LIST_STRUCT_DECLARATOR : STRUCT_DECLARATOR                                  { $$ = $1;}
                       | LIST_STRUCT_DECLARATOR ',' STRUCT_DECLARATOR       { $$ = new list_struct_declarator($1, $3);}
                      

STRUCT_DECLARATOR : DECLARATOR                 {$$ = $1;}
                  | ':' EXPR_CONST             {$$ = new struct_declarator($2);}
                  | DECLARATOR ':' EXPR_CONST  {$$ = new struct_declarator($1, $3);}
  

SPECIFIER_ENUM : ENUM L_BRACE LIST_ENUMERATOR R_BRACE               { $$ = new specifier_enum($3);      }
               | ENUM IDENTIFIER L_BRACE LIST_ENUMERATOR R_BRACE    { $$ = new specifier_enum(*$2, $4); }
               | ENUM IDENTIFIER                                    { $$ = new specifier_enum(*$2);     }
               

LIST_ENUMERATOR : ENUMERATOR                        { $$ = $1;                          }
                | LIST_ENUMERATOR ',' ENUMERATOR    { $$ = new list_enumerator($1, $3); }
                
            
ENUMERATOR : IDENTIFIER                      { $$ = new enumerator(*$1);     }
           | IDENTIFIER ASSIGN EXPR_CONST    { $$ = new enumerator(*$1, $3); }
           

QUALIFIER_TYPE : CONST      { $$ = new qualifier_type("const"); }
               | VOLATILE   { $$ = new qualifier_type("volatile"); }


DECLARATOR : POINTER DECLARATOR_DIRECT        { $$ = new declarator($1, $2); }
           | DECLARATOR_DIRECT                { $$ = $1;                     }

         
DECLARATOR_DIRECT : IDENTIFIER                                              { $$ = new declarator_direct(*$1); }
                  | L_BRACKET DECLARATOR R_BRACKET                          { $$ = new declarator_direct($2); }
                  | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE          { $$ = new declarator_direct($1, $3); }
                  | DECLARATOR_DIRECT L_SQUARE R_SQUARE                     { $$ = new declarator_direct($1); }
                  | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET   { $$ = new declarator_direct($1, $3); }
                  | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET   { $$ = new declarator_direct($1, $3); }
                  | DECLARATOR_DIRECT L_BRACKET R_BRACKET                   { $$ = new declarator_direct($1); }


POINTER : OP_MUL                                { $$ = new pointer(); }
        | OP_MUL LIST_QUALIFIER_TYPE            { $$ = new pointer($2); }
        | OP_MUL POINTER                        { $$ = new pointer($2); }
        | OP_MUL LIST_QUALIFIER_TYPE POINTER    { $$ = new pointer($2, $3); }


LIST_QUALIFIER_TYPE : QUALIFIER_TYPE                        { $$ = $1;                              }
                    | LIST_QUALIFIER_TYPE QUALIFIER_TYPE    { $$ = new list_qualifier_type($1, $2); }


LIST_PARAM_TYPE : LIST_PARAMETER                { $$ = $1;                      }
                | LIST_PARAMETER ',' ELIPSIS    { $$ = new list_param_type($1); }


LIST_PARAMETER : DECLARATION_PARAMETER                      { $$ = $1;                         }
               | LIST_PARAMETER ',' DECLARATION_PARAMETER   { $$ = new list_parameter($1, $3); }


DECLARATION_PARAMETER : SPECIFIER_DECLARATION DECLARATOR                    { $$ = new declaration_parameter($1,$2);}  
                      | SPECIFIER_DECLARATION DECLARATOR_ABSTRACT           { $$ = new declaration_parameter($1,$2);}
                      | SPECIFIER_DECLARATION                               { $$ = $1;}
                   

LIST_IDENTIFIER : IDENTIFIER                        { $$ = new list_identifier(*$1);     }
                | LIST_IDENTIFIER ',' IDENTIFIER    { $$ = new list_identifier($1, *$3); }


TYPE_NAME : LIST_SPEC_QUAL                        { $$ = $1;                    }
          | LIST_SPEC_QUAL DECLARATOR_ABSTRACT    { $$ = new type_name($1, $2); }


DECLARATOR_ABSTRACT : POINTER                                   { $$ = $1; }
                    | DECLARATOR_DIRECT_ABSTRACT                { $$ = $1; }     
                    | POINTER DECLARATOR_DIRECT_ABSTRACT        { $$ = new declarator_abstract($1, $2);} 
   
                 
DECLARATOR_DIRECT_ABSTRACT : L_BRACKET DECLARATOR_ABSTRACT R_BRACKET                                { $$ = new declarator_direct_abstract($2);}
                           | L_SQUARE R_SQUARE                                                      { $$ = new declarator_direct_abstract();}
                           | L_SQUARE EXPR_CONST R_SQUARE                                           { $$ = new declarator_direct_abstract($2);}
                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE R_SQUARE                           { $$ = new declarator_direct_abstract($1);}
                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE EXPR_CONST R_SQUARE                { $$ = new declarator_direct_abstract($1, $3);}
                           | L_BRACKET R_BRACKET                                                    { $$ = new declarator_direct_abstract();}
                           | L_BRACKET LIST_PARAM_TYPE R_BRACKET                                    { $$ = new declarator_direct_abstract($2);}
                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET R_BRACKET                         { $$ = new declarator_direct_abstract($1);}
                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET LIST_PARAM_TYPE R_BRACKET         { $$ = new declarator_direct_abstract($1, $3);}
                           

INITIALIZER : EXPR_ASSIGNMENT                                 { $$ = $1; }
            | L_BRACKET LIST_INITIALIZER R_BRACKET            { $$ = new initializer($2);}
            | L_BRACKET LIST_INITIALIZER ',' R_BRACKET        { $$ = new initializer($2);} 
            

LIST_INITIALIZER : INITIALIZER                          { $$ = $1; }
                 | LIST_INITIALIZER ',' INITIALIZER     { $$ = new list_initializer($1, $3);}
                 
              
STATEMENT : STATEMENT_LABELED       { $$ = $1; }
          | STATEMENT_COMPOUND      { $$ = $1; }
          | STATEMENT_EXPR          { $$ = $1; }
          | STATEMENT_SELECTION     { $$ = $1; }
          | STATEMENT_ITERATION     { $$ = $1; }
          | STATEMENT_JUMP          { $$ = $1; }
          | LIST_DECLARATION        { $$ = $1; }

         
STATEMENT_LABELED : IDENTIFIER ':' STATEMENT       { $$ = new statement_labeled(*$1, $3);}
                  | CASE EXPR_CONST ':' STATEMENT  { $$ = new statement_labeled("case", $2, $4);}
                  | DEFAULT ':' STATEMENT          { $$ = new statement_labeled("default", $3);}
                  


STATEMENT_COMPOUND : L_BRACE R_BRACE                                  { $$ = new statement_compound(); }
                   | L_BRACE LIST_STATEMENT R_BRACE                   { $$ = new statement_compound($2); }
                   

LIST_DECLARATION : DECLARATION                          { $$ = $1;}
                 | LIST_DECLARATION DECLARATION         { $$ = new list_declaration($1, $2); }
                 
              
LIST_STATEMENT : STATEMENT                      { $$ = $1; }
               | LIST_STATEMENT STATEMENT       { $$ = new list_statement($1,$2);}
                  

STATEMENT_EXPR : ';'          {$$ = new statement_expr();}
               | EXPR ';'     {$$ = new statement_expr($1);}  
               

STATEMENT_SELECTION : IF L_BRACKET EXPR R_BRACKET STATEMENT                         { $$ = new statement_selection("if", $3, $5); }
                    | IF L_BRACKET EXPR R_BRACKET STATEMENT ELSE STATEMENT          { $$ = new statement_selection("if", $3, $5,"else" ,$7); }
                    | SWITCH L_BRACKET EXPR R_BRACKET STATEMENT                     { $$ = new statement_selection("switch", $3, $5); }
                    

STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT                             { $$ = new statement_iteration("while", $3, $5); }
                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';'                      { $$ = new statement_iteration("do", $2, "while", $5); }
                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT      { $$ = new statement_iteration("for", $3, $4, $6); }
                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMENT { $$ = new statement_iteration("for", $3, $4, $5, $7); }


STATEMENT_JUMP : GOTO IDENTIFIER ';' {$$ = new statement_jump("goto",*$2);}
               | CONTINUE ';' {$$ = new statement_jump("continue");}
               | BREAK ';'  {$$ = new statement_jump("break");}
               | RETURN ';' {$$ = new statement_jump("return");}
               | RETURN EXPR ';' {$$ = new statement_jump("return",$2);}

         
PROGRAM : DECLARATION_EXTERNAL            { $$ = $1;                  }
        | PROGRAM DECLARATION_EXTERNAL    { $$ = new program($1, $2); }


DECLARATION_EXTERNAL : DEFINITION_FUNCTION { $$ = $1; }
                     | DECLARATION         { $$ = $1; }


DEFINITION_FUNCTION : SPECIFIER_DECLARATION DECLARATOR ';'                                { $$ = new definition_function($1, $2);     } 
                    |SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND                  { $$ = new definition_function($1, $2, $3); }
         

%%


const Node *g_root; // Definition of variable (to match declaration earlier)

const Node *parseAST(char* x)
{
  yyin = fopen(x, "r");
  g_root=0;
  if(yyin) yyparse();
  fclose(yyin);
  return g_root;
}





