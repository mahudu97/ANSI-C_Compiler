%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "c_parser.tab.hpp"
#include "c_lexer_helper.hpp"

extern FILE* yyin;

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}

		
IDENTIFIER  			[_a-zA-Z][0-9_a-zA-Z]*

INTEGER_SUFFIX          ([uU][lL]?) | ([lL][uU]?)

DECIMAL_CONSTANT        [1-9][0-9]*[integer_suffix]?

OCTAL_CONSTANT		    [0][0-7]*[integer_suffix]?

HEXAD_CONSTANT    		(0x|0X)[0-9a-fA-F]+[integer_suffix]?

CHAR_CONSTANT     		L?['][.]+[']

FLOAT_CONSTANT			([0-9]+|[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?)

INCLUDE					#[.^\n]

COMMENT					"/*"[^"*/"]*"*/"

WHITESPACE				[ \n\t\v\f\r]

OTHER					.




%%

"auto"				{ fprintf(stderr, "AUTO\n");
						return AUTO; }

"double"			{ fprintf(stderr, "DOUBLE\n");
						return DOUBLE; }

"int"				{ fprintf(stderr, "INT\n");
						return INT; }

"struct"			{ fprintf(stderr, "STRUCT\n");
						return STRUCT; }

"break"				{ fprintf(stderr, "BREAK\n");
						return BREAK; }

"else"				{ fprintf(stderr, "ELSE\n");
						return ELSE; }

"long"				{ fprintf(stderr, "LONG\n");
						return LONG; }

"switch"			{ fprintf(stderr, "SWITCH\n");
						return SWITCH; }

"case"				{ fprintf(stderr, "CASE\n");
						return CASE; }

"enum"				{ fprintf(stderr, "ENUM\n");
						return ENUM; }

"..." 				{ fprintf(stderr, "ELIPSIS\n");
						return ELIPSIS; }

"register"			{ fprintf(stderr, "REGISTER\n");
						return REGISTER; }

"typedef"			{ fprintf(stderr, "TYPEDEF\n");
						return TYPEDEF; }

"char"				{ fprintf(stderr, "CHAR\n");
						return CHAR; }

"extern"			{ fprintf(stderr, "EXTERN\n");
						return EXTERN; }

"return"			{ fprintf(stderr, "RETURN\n");
						return RETURN; }

"union"				{ fprintf(stderr, "UNION\n");
						return UNION; }

"const"				{ fprintf(stderr, "CONST\n");
						return CONST; }

"float"				{ fprintf(stderr, "FLOAT\n");
						return FLOAT; }

"short"				{ fprintf(stderr, "SHORT\n");
						return SHORT; }

"unsigned"			{ fprintf(stderr, "UNSIGNED\n");
						return UNSIGNED; }

"continue"			{ fprintf(stderr, "CONTINUE\n");
						return CONTINUE; }

"for"				{ fprintf(stderr, "FOR\n");
						return FOR; }

"signed"			{ fprintf(stderr, "SIGNED\n");
						return SIGNED; }

"void"				{ fprintf(stderr, "VOID\n");
						return VOID; }

"default"			{ fprintf(stderr, "DEFAULT\n");
						return DEFAULT; }

"goto"				{ fprintf(stderr, "GOTO\n");
						return GOTO; }

"sizeof"			{ fprintf(stderr, "SIZEOF\n");
						return SIZEOF; }

"volatile"			{ fprintf(stderr, "VOLATILE\n");
						return VOLATILE; }

"do"				{ fprintf(stderr, "DO\n");
						return DO; }

"if"				{ fprintf(stderr, "IF\n");
						return IF; }

"static"			{ fprintf(stderr, "STATIC\n");
						return STATIC; }

"while"				{ fprintf(stderr, "WHILE\n");
						return WHILE; }


{IDENTIFIER}		{ fprintf(stderr, "Identifier : %s\n", yytext);
						toString();
						return IDENTIFIER; 
					}

{DECIMAL_CONSTANT}  { fprintf(stderr, "Decimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_DECIMAL(); 	return INT_C;}
						if(u && !l)		{ get_DECIMAL_U(); 	return UNSIGNED_C;}
						if(!u && l)		{ get_DECIMAL_L(); 	return LONG_C;}
						if(u && l)		{ get_DECIMAL_UL(); return UNSIGNED_LONG_C;}
					}

{OCTAL_CONSTANT}  	{ fprintf(stderr, "Octal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_OCTAL(); 		return INT_C;}
						if(u && !l)		{ get_OCTAL_U(); 	return UNSIGNED_C;}
						if(!u && l)		{ get_OCTAL_L(); 	return LONG_C;}
						if(u && l)		{ get_OCTAL_UL(); 	return UNSIGNED_LONG_C;}
					}
					
{HEXAD_CONSTANT}  	{ fprintf(stderr, "Hexadecimal : %s\n", yytext);
						bool u = false, l = false;
						checkIntSuffix(u,l);
						if(!u && !l)	{ get_HEXA(); 		return INT_C;}
						if(u && !l)		{ get_HEXA_U(); 	return UNSIGNED_C;}
						if(!u && l)		{ get_HEXA_L(); 	return LONG_C;}
						if(u && l)		{ get_HEXA_UL(); 	return UNSIGNED_LONG_C;}
					}

{FLOAT_CONSTANT}	{ fprintf(stderr, "Float constant : %s\n", yytext);
						get_FLOAT();
						return FLOAT_C;
					}

{CHAR_CONSTANT}		{ fprintf(stderr, "Character : %s\n", yytext);
						get_CHARACTER();
						return CHARACTER_C; 
					}

L?\"(\\.|[^\\"])*\"	{ fprintf(stderr, "String : %s\n", yytext);
						toString();
						return STRING_LITERAL;
					}


\{				{ fprintf(stderr, "L_BRACE\n");
						return L_BRACE; }

"}"				{ fprintf(stderr, "R_BRACE\n");
						return R_BRACE; }

\(				{ fprintf(stderr, "L_BRACKET\n");
						return L_BRACKET; }

\)				{ fprintf(stderr, "R_BRACKET\n");
						return R_BRACKET; }

\[				{ fprintf(stderr, "L_SQUARE\n");
						return L_SQUARE; }

\]				{ fprintf(stderr, "R_SQUARE\n");
						return R_SQUARE; }


\+				{ fprintf(stderr, "OP_PLUS\n");
						return OP_PLUS; }

\-				{ fprintf(stderr, "OP_MINUS\n");
						return OP_MINUS; }

\*				{ fprintf(stderr, "OP_MUL\n");
						return OP_MUL; }

\/				{ fprintf(stderr, "OP_DIV\n");
						return OP_DIV; }

\%				{ fprintf(stderr, "OP_MOD\n");
						return OP_MOD; }

"++" 			{ fprintf(stderr, "OP_INC\n");
						return OP_INC; }

"--" 			{ fprintf(stderr, "OP_DEC\n");
						return OP_DEC; }


"<=" 			{ fprintf(stderr, "OP_LE\n");
						return OP_LE; }

">=" 			{ fprintf(stderr, "OP_GE\n");
						return OP_GE; }

"==" 			{ fprintf(stderr, "OP_EQ\n");
						return OP_EQ; }

"!="			{ fprintf(stderr, "OP_NE\n");
						return OP_NE; }

\<				{ fprintf(stderr, "OP_L\n");
						return OP_L; }

\>				{ fprintf(stderr, "OP_G\n");
						return OP_G; }


"&&" 			{ fprintf(stderr, "OP_LAND\n");
						return OP_LAND; }

"||" 			{ fprintf(stderr, "OP_LOR\n");
						return OP_LOR; }

\!				{ fprintf(stderr, "OP_LNOT\n");
						return OP_LNOT; }


\&				{ fprintf(stderr, "OP_BAND\n");
						return OP_BAND; }

\|				{ fprintf(stderr, "OP_BOR\n");
						return OP_BOR; }

\^				{ fprintf(stderr, "OP_BXOR\n");
						return OP_BXOR; }

\~				{ fprintf(stderr, "OP_B_ONESC\n");
						return OP_B_ONESC; }

">>"			{ fprintf(stderr, "OP_BRIGHT\n");
						return OP_BRIGHT; }

"<<" 			{ fprintf(stderr, "OP_BLEFT\n");
						return OP_BLEFT; }


"->" 			{ fprintf(stderr, "OP_PTR\n");
						return OP_PTR; }


\=				{ fprintf(stderr, "ASSIGN\n");
						return ASSIGN; }

">>=" 			{ fprintf(stderr, "RIGHT_ASSIGN\n");
						return RIGHT_ASSIGN; }

"<<="			{ fprintf(stderr, "LEFT_ASSIGN\n");
						return LEFT_ASSIGN; }

"+="			{ fprintf(stderr, "ADD_ASSIGN\n");
						return ADD_ASSIGN; }

"-="			{ fprintf(stderr, "SUB_ASSIGN\n");
						return SUB_ASSIGN; }

"*="			{ fprintf(stderr, "MUL_ASSIGN\n");
						return MUL_ASSIGN; }

"/="			{ fprintf(stderr, "DIV_ASSIGN\n");
						return DIV_ASSIGN; }

"%="			{ fprintf(stderr, "MOD_ASSIGN\n");
						return MOD_ASSIGN; }

"&="			{ fprintf(stderr, "AND_ASSIGN\n");
						return AND_ASSIGN; }

"|="			{ fprintf(stderr, "OR_ASSIGN\n");
						return OR_ASSIGN; }

"^="			{ fprintf(stderr, "XOR_ASSIGN\n");
						return XOR_ASSIGN; }


\?				{ fprintf(stderr, "?\n");
						return '?'; }

\:				{ fprintf(stderr, ":\n");
						return ':'; }

\;				{ fprintf(stderr, ";\n");
						return ';'; }

\,				{ fprintf(stderr, ",\n");
						return ','; }

\.				{ fprintf(stderr, ".\n");
						return '.'; }


{INCLUDE}		{ fprintf(stderr, "#include\n"); }

{COMMENT}		{ fprintf(stderr, "comment\n"); }

"//"[^\n]*		{ fprintf(stderr, "line comment\n"); }

{WHITESPACE}    { fprintf(stderr, "Newline, tab or space\n");  }


{OTHER}			{ yyerror(yytext); }


%%





void toString(){
 	std::string *word = new std::string; // take value out of yylex scope
	*word = yytext;
	yylval.string = word;
}


void checkIntSuffix(bool &u_exist, bool &l_exist){
	int size = strlen(yytext);
	if(size < 2){
		return;
	}
	if(yytext[size-1] == ('u' || 'U') || yytext[size-2] == ('u' || 'U')) u_exist = true;
	if(yytext[size-1] == ('l' || 'L') || yytext[size-2] == ('l' || 'L')) l_exist = true;
}


void get_DECIMAL(){
	int num;
	sscanf(yytext, "%d", &num);
	yylval.intValue = num;
}

void get_DECIMAL_U(){   
    unsigned int num;
	sscanf(yytext, "%u", &num);
	yylval.uintValue = num;
}

void get_DECIMAL_L(){   
    int num;
	sscanf(yytext, "%d", &num);
	yylval.longintValue = (long int) num;
}

void get_DECIMAL_UL(){   
    unsigned int num;
	sscanf(yytext, "%u", &num);
	yylval.longuintValue = (unsigned long) num;
}



void get_OCTAL(){
    int num;
    sscanf(yytext,"%o", &num);
    yylval.intValue = num;  
}

void get_OCTAL_U(){
    unsigned int num;
    sscanf(yytext,"%o", &num);    
    yylval.uintValue = num;   
}

void get_OCTAL_L(){
    int num;
    sscanf(yytext,"%o", &num);    
    yylval.longintValue = (long int) num;
}

void get_OCTAL_UL(){
    unsigned int num;
    sscanf(yytext,"%o", &num);    
    yylval.longuintValue = (unsigned long) num;   
}



void get_HEXA(){
    int num;
    sscanf(yytext,"%x", &num); 
    yylval.intValue = num;
}

void get_HEXA_U(){
    unsigned int num;
	sscanf(yytext, "%x", &num);
	yylval.uintValue = num;    
}

void get_HEXA_L(){   
    int num;
    sscanf(yytext,"%x",&num);
    yylval.longintValue = (long int) num;  
} 
  
void get_HEXA_UL(){
    unsigned int num;
    sscanf(yytext,"%x",&num);
    yylval.longuintValue = (unsigned long) num; 
}


void get_FLOAT(){
	double num;
	sscanf(yytext,"%lf",&num);
	yylval.floatValue = num; 

} 


void get_CHARACTER(){
    char val;
    sscanf(yytext,"%c", &val);
    yylval.characterValue = val;
}
    


/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
}
