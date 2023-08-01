#include <cstdio>
#include <cstdlib>
#include <cstring>

#define SET 1
#define UNSET 0
#define DEFAULT_BUFFER_SIZE 256

#ifndef __FD__H
#define __FD__H
#include <stack>
#include <cstdio>
#include <iostream>
using namespace std;
class FileDescriptor {
private:
    FILE* fp;
    std::stack<char> charBuffer;

    int line_number;
    int char_number;
    int flag;
    int buf_size;
    char* buffer;
    char* file;
    int flag2;
    //string line_buffer; 
    // Helper function to double the buffer size
    void DoubleBufferSize() {
        buf_size *= 2;
        char* new_buffer = (char*)realloc(buffer, buf_size);
        if (new_buffer) {
            buffer = new_buffer;
        } else {
            // Error handling for memory allocation failure
            //  ReportError("Memory allocation error.");
            exit(EXIT_FAILURE);
        }
    }

public:
    /* Externally-visible functions: */
    FileDescriptor(const char* FileName) : line_number(1), char_number(0), flag(UNSET), flag2(UNSET) {
        if (FileName) {
            file = strdup(FileName);
            fp = fopen(FileName, "r");
        } else {
            file = nullptr;
            fp = stdin;
        }

        if (!fp) {
            // Error handling for file open failure
            //  ReportError("Error opening file.");
            exit(EXIT_FAILURE);
        }

        buf_size = DEFAULT_BUFFER_SIZE;
        buffer = (char*)malloc(buf_size);
        if (!buffer) {
            // Error handling for memory allocation failure
            // ReportError("Memory allocation error.");
            exit(EXIT_FAILURE);
        }
    }

    // Constructor and other functions...

    void UngetChar(char c) {
        if (c == '\n') {
            line_number--;
            // Note: If we are ungetting a newline, we set char_number to the length of the previous line.
            // This assumes that the newline character itself is not part of the line.
            char_number = line_number == 1 ? 0 : strlen(buffer) - 1;
        } else {
            // If we are ungetting a regular character, just decrement the character number.
            char_number--;
        }
        charBuffer.push(c);
    }

    char GetChar() {
        if (!charBuffer.empty()) {
            char c = charBuffer.top();
            charBuffer.pop();
            if (c == '\n') {
                line_number++;
                char_number = 0;
            } else {
                char_number++;
            }
            return c;
        } else {
            char c = fgetc(fp);
            if (c == EOF) {
                return '\0';
            } else {
                if (c == '\n') {
                    line_number++;
                    char_number = 0;
                    //line_buffer.clear(); // Clear the temporary buffer for a new line
                } else {
                    char_number++;
                }
                //line_buffer += c; // Add the current character to the line buffer
                return c;
            }
        }
    }

    ~FileDescriptor() {
        Close();
        free(buffer);
        free(file);
    }

    // Function to check if the file is open without errors
    bool IsOpen() {
        return fp && !ferror(fp);
    }

    // Function to get the current filename
    char* GetFileName() {
        return file;
    }

    // Function to get the current line buffer, returns nullptr if EOF
    //    const char* FileDescriptor::GetCurrLine() {
    //     return line_buffer.empty() ? nullptr : line_buffer.c_str();
    // }

    //  to get the current line number
    int GetLineNum() {
        return line_number;
    }

    //  to get the current character number in the line
    int GetCharNum() {
        return char_number;
    }

    void Close() {
        if (fp && fp != stdin) {
            fclose(fp);
            fp = nullptr;
        }
    }

    void ReportError(const char* msg) {
        for (int i=0;i<char_number-1;i++)
        {
            cout<<' ';
        }
        cout<<"^"<<endl;
        fprintf(stderr, "Error  '%s'  on line %d of %s  \n",msg, line_number,  file);
    }
void PrintCurrentLine() {
    long current_pos = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    int current_line = 1;
    int char_count = 0;
    char c;

    while ((c = fgetc(fp)) != EOF) {
        if (current_line == line_number) {
            putchar(c);
            char_count++;

            if (c == '\n') {
                // Reached the end of the current line, break the loop.
                break;
            }
        } else if (c == '\n') {
            current_line++;
        }
    }
    putchar('\n');

    // Restore the original file pointer position
    fseek(fp, current_pos, SEEK_SET);
}
};

#endif // __FD__H
