#include "Scanner.h"
#include <iostream>
#include <cctype>
using namespace std;

int keys = 32; /* number of keywords */
const char *keyword[] = {
"and", "begin", "boolean", "by", "constant",
"do", "else", "end", "false", "fi", "float", "for", "from",
"function", "if", "integer", "not", "od", "or", "procedure",
"program", "read", "return", "string", "then", "to", "true",
"var", "while", "write"
};

LEXEME_TYPE key_type[] = {
kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
kw_do, kw_else, kw_end, kw_false, kw_fi,kw_float,
kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
kw_od, kw_or, kw_procedure, kw_program,kw_read, kw_return,
kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write
};

bool isDigit(char x){
    if (x>='0' && x<='9')
    return true;
    return false;
}
bool isLetter(char x){
    if (x>='a' && x<='z')
    return true;
     if (x>='A' && x<='Z')
    return true;
    return false;
}

int stringToInt(const std::string& str) {
    int sign = 1;
    int idx = 0;

    if (str[0] == '-') {
        sign = -1;
        idx = 1;
    } else if (str[0] == '+') {
        idx = 1;
    }
    int result = 0;
    for (; idx < str.length(); idx++) {
        
        result = result * 10 + (str[idx] - '0');
    }
    return result * sign;
}
float stringToFloat(const std::string& str) {
    int sign = 1;
    int idx = 0;
    if (str[0] == '-') {
        sign = -1;
        idx = 1;
    } else if (str[0] == '+') {
        idx = 1;
    }

    float result = 0;
    for (; idx < str.length() && isdigit(str[idx]); idx++) {
        result = result * 10 + (str[idx] - '0');
    }
    if (idx < str.length() && str[idx] == '.') {
        float decimal = 0.1f;
        idx++;
        for (; idx < str.length() && isdigit(str[idx]); idx++) {
            result = result + (str[idx] - '0') * decimal;
            decimal *= 0.1f;
        }
    }

    return result * sign;
}
// Constructor
SCANNER::SCANNER() {
    Fd = nullptr;
}
SCANNER::SCANNER(FileDescriptor *fd) {
    Fd = fd; 
}
FileDescriptor* SCANNER::getFD(){
        return Fd;
    }

TOKEN* SCANNER::Scan() {
 
    char c;
    char tmp;
    TOKEN* t = new TOKEN();
    string str_buffer = string(); 
    

      c = Fd->GetChar();
     //cout<<"got c is "<<c<<endl;
    while (c == ' ' || c == '\n') {
         c = Fd->GetChar();
    } 
    
    if (c == '\0') {
        t->type= lx_eof;
        return t;
    }
   // bool comment=false;
    //comments
    if (c == '#'  ){
        c= Fd->GetChar() ;  
      //  cout<<"uuuuuuuuuuuuuuuuuuuuuuu";
        if(c == '#'  ){
        //comment=true;
            while (c != '\n' && c != '\0') {
                c = Fd->GetChar();
                if (c=='#'){
                     c = Fd->GetChar();
                     if(c=='#'){
                        c = Fd->GetChar();
                        break;
                     }
                }
            }
            if (c == '\n') {
               // c = Fd->GetChar();
                return nullptr;
            }
        }
        else{
                Fd->PrintCurrentLine();
                Fd->ReportError("second # expected");
                return nullptr;
        }
    }
  
   while (c == ' ' || c == '\n') {
         c = Fd->GetChar();
    } 
    
    if (c == '\0') {
        t->type= lx_eof;
        return t;
    }
    
        //identifiers
    else if (isLetter(c)|| c=='_'){
     
        while((isDigit(c)||isLetter(c)||c=='_')){
            
            str_buffer += c;
            c = Fd->GetChar();
            
        }
        
        Fd->UngetChar(c);
       // cout<<"char unget  "<<c<<endl;
        for (int i=0;i< sizeof(keyword) / sizeof(keyword[0]);i++){
            if (str_buffer==keyword[i]) {
                t->type= key_type[i] ;
                t->str_value=str_buffer;
                return t;
            }
        }

        //otherwise (not keword)
        t->type= lx_identifier;
        t->str_value=str_buffer;
       
        return t;
    }
    // integers & floats
    bool isFloat=false;
     if (isDigit(c)==true){
        str_buffer += c;
        c = Fd->GetChar();
        if(isLetter(c)){
            Fd->PrintCurrentLine();
            Fd->ReportError("illegal identifier");
            return nullptr;
        }
        while( isDigit(c)){
            str_buffer += c;
            c = Fd->GetChar();
        }
         if(isLetter(c)){
            Fd->PrintCurrentLine();
            Fd->ReportError("illegal identifier");
            return nullptr;
        }
        if (c=='.'){
            isFloat=true;
            str_buffer += c;
            c = Fd->GetChar();
            if (isDigit(c)==false){
                Fd->PrintCurrentLine();
                Fd->ReportError("Bad Floating point number");
                 return nullptr;
            }
            while(isDigit(c)){
            str_buffer += c;
            c = Fd->GetChar();
            }
            Fd->UngetChar(c);
        }
        else{
            Fd->UngetChar(c);
        }
         if (isFloat==true){
            t->type= lx_float;
            t->float_value=stringToFloat(str_buffer);
         }
         else {
            t->type= lx_integer;
            t->value=stringToInt(str_buffer);
         }
        return t;
    }
    
    else if (c=='-' || c=='+'){
        tmp=c;
        str_buffer += c;
        c = Fd->GetChar();
        if (isDigit(c)==false ){
           Fd->UngetChar(c);
             if (tmp == '+') {
                t->type = lx_plus;
                return t;
            }
           else if (tmp== '-') {
           t->type = lx_minus;
           return t;
            }           
          }
    
        if(isDigit(c)){
        str_buffer += c;
        c = Fd->GetChar();
        while(c!=' '&& isDigit(c)){
            str_buffer += c;
            c = Fd->GetChar();
        }
          if(isLetter(c)){
            Fd->PrintCurrentLine();
            Fd->ReportError("illegal identifier");
            return nullptr;
        }
        if (c=='.'){
            isFloat=true;
            str_buffer += c;
            c = Fd->GetChar();
            if (isDigit(c)==false){
                Fd->PrintCurrentLine();
                Fd->ReportError("Bad Floating point number");
                 return nullptr;
            }
            
            while( isDigit(c)){
            str_buffer += c;
            c = Fd->GetChar();
            }
             Fd->UngetChar(c);
        }
        else{
             Fd->UngetChar(c);
        }
         if (isFloat==true){
            t->type= lx_float;
            t->float_value=stringToFloat(str_buffer);
         }
         else {
            t->type= lx_integer;
            t->value=stringToInt(str_buffer);
         }
        return t;
     }
    }
    // strings
    else if( c=='"'){
        c = Fd->GetChar();
        while (c!='\0'){
            if(c=='"'){
            t->type=lx_string;
            t->str_value =str_buffer;
            return t;
            }
            else if(c=='\n'){
                Fd->PrintCurrentLine();
                Fd->ReportError("line not alllowed in a string");
                return nullptr;
            }// throw EERORRRRRRRRR
            str_buffer+=c;
            c = Fd->GetChar();

        }
            Fd->PrintCurrentLine();
            Fd->ReportError("illegal string");
            return nullptr;
    }
    // Operators or special symbols
else if (c == '(') {
    t->type = lx_lparen;
    return t;
}
else if (c == ')') {
    t->type = lx_rparen;
    return t;
}
else if (c == ':') {
    c = Fd->GetChar();
    if (c == '=') {
        t->type = lx_colon_eq;
        return t;
    }
   
}

else if (c == '*') {
    t->type = lx_star;
    return t;
}
else if (c == '/') {
    t->type = lx_slash;
    return t;
}
else if (c == '=') {
    t->type = lx_eq;
    return t;
}
else if (c == '!') {
    c = Fd->GetChar();
    if (c == '=') {
        t->type = lx_neq;
        return t;
    }
}
else if (c == '<') {
    c = Fd->GetChar();
    if (c == '=') {
        t->type = lx_le;
        return t;
    }
    else {
        t->type = lx_lt;
        Fd->UngetChar(c);
       
        return t;
    }
}
else if (c == '>') {
    c = Fd->GetChar();
    if (c == '=') {
        t->type = lx_ge;
        return t;
    }
    else {
        t->type = lx_gt;
        Fd->UngetChar(c);
        return t;
    }
}
else if (c == '.') {
    t->type = lx_dot;
    return t;
}
else if (c == ';') {
    t->type = lx_semicolon;
    return t;
}
else if (c == '[') {
    t->type = lx_lbracket;
    return t;
}
else if (c == ']') {
    t->type = lx_rbracket;
    return t;
}
else if (c == ',') {
    t->type = lx_comma;
    return t;
}
else if (c == '{') {
    t->type = lx_lbracket;
    return t;
}
else if (c == '}') {
    t->type = lx_rbracket;
    return t;
}
        
    
    
            Fd->PrintCurrentLine();
            Fd->ReportError("illegal token");
            return nullptr;
}
