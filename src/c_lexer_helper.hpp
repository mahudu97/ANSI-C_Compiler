#ifndef C_LEXER_HELPER_HPP
#define C_LEXER_HELPER_HPP

void toString();
void yyerror (char const *s);

void checkIntSuffix(bool &u_exist, bool &l_exist);
void get_DECIMAL();
void get_DECIMAL_U();
void get_DECIMAL_L();
void get_DECIMAL_UL();
void get_OCTAL();
void get_OCTAL_U();
void get_OCTAL_L();
void get_OCTAL_UL();
void get_HEXA();
void get_HEXA_U();
void get_HEXA_L();
void get_HEXA_UL();

void get_FLOAT();

void get_CHARACTER();

#endif
