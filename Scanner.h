/* scanner .h*/
#ifndef __SCANNER__H
#define __SCANNER__H

#include "fd.h"
#include <iostream> 
using namespace std;
//Token types enumeration
typedef enum
{
	/* Literals */
	lx_identifier, lx_integer, lx_string,lx_float,
	/* Keywords */
	kw_program,
	kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
	kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
	kw_while, kw_do, kw_od,
	kw_and, kw_or, 
	kw_read, kw_write,
	kw_for, kw_from, kw_to, kw_by,
	kw_function, kw_procedure,kw_return, kw_not, kw_begin, kw_end,
	/* Operators */
	lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
	lx_colon, lx_dot, lx_semicolon, lx_comma, lx_colon_eq,
	lx_plus, lx_minus, lx_star, lx_slash,
	lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,lx_eof
}
LEXEME_TYPE;

// Definition of TOKEN, you can use a clase if you  want 
struct	TOKEN
{
	LEXEME_TYPE type;
	int value;  // can be used instead of the str_ptr for IDs and strings
	float float_value;
	string str_value;
	char *str_ptr; // points to strings or Identifiers, can use value
	              //instead but with type casting
};

class SCANNER{
   private:
   FileDescriptor *Fd;
    // define your functions ..
    //I would define the following functions..
    // void skip_comments();
    // bool check_keyword(...)
    // TOKEN *get_id(..)
    // TOKEN *get_string(..)
    // TOKEN *get_int(..) // gets integers and floats

	public:
     FileDescriptor*  getFD();
	SCANNER();
	SCANNER (FileDescriptor *fd);
   

    TOKEN* Scan() ;


};
#endif



 