#include <iostream>
#include "fd.h"
#include "Scanner.h"

const char* lexemeTypeToString ( LEXEME_TYPE type) {
    static const char* lexemeTypeStrings[] = {
        "lx_identifier", "lx_integer", "lx_string", "lx_float",
        "kw_program",
        "kw_var", "kw_constant", "kw_integer", "kw_boolean", "kw_string", "kw_float",
        "kw_true", "kw_false", "kw_if", "kw_fi", "kw_then", "kw_else",
        "kw_while", "kw_do", "kw_od",
        "kw_and", "kw_or",
        "kw_read", "kw_write",
        "kw_for", "kw_from", "kw_to", "kw_by",
        "kw_function", "kw_procedure", "kw_return", "kw_not", "kw_begin", "kw_end",
        "lx_lparen", "lx_rparen", "lx_lbracket", "lx_rbracket",
        "Ix_colon", "lx_dot", "lx_semicolon", "lx_comma", "Ix_colon_eq",
        "lx_plus", "lx_minus", "lx_star", "lx_slash",
        "lx_eq", "lx_neq", "lx_lt", "lx_le", "lx_gt", "lx_ge", "lx_eof"
   
    };

    // Calculate the index of the type in the array
    int index = static_cast<int>(type);

    // Check if the index is within bounds
    if (index >= 0 && index < sizeof(lexemeTypeStrings) / sizeof(lexemeTypeStrings[0])) {
        return lexemeTypeStrings[index];
    }

    // Return "Unknown" if the index is out of bounds (should not happen if lx_eof is included as the last value)
    return "Unknown";
}

int main() {

   FileDescriptor fd("code_input.txt");
   
    SCANNER myScanner(&fd);
    TOKEN *myToken;
   

    
    while (true) {
        myToken = myScanner.Scan();
        if (myToken) {
            std::cout <<"--------"<< lexemeTypeToString(myToken->type) << "\n";
            if(myToken->type==lx_eof){
             //   std::cout << "End of File\n";
            break; // Exit the loop when there are no more tokens
            }
         //   std::cout << (myToken->str_value) << "\n";
        } else {
        //  std::cout << "illegal token \n";
          // break; // Exit the loop when there are no more tokens
        }
    }

  fd.Close();

    return 0;
}
