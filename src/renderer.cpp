#include "renderer.h"
#include <iostream>

// error handeling
void glClearError()
{
    // when there is error call this should be reset sll the other errors 
    while (glGetError() != GL_NO_ERROR)
    {
        // code for error clearing 
    }   
}

// print error by using glGetError from opengl
// adding more contents to the function like the function name , line etc
bool glLogCall(const char* function, const char* file, int line)
{
    // function for print error
    while (GLenum error = glGetError())
    {
        std::cout << "(openGl error : " << error << "), function : "
        << function << " file name : " << file << " line number : " << line 
        <<"\n";
        return false;
    }
    return true;
}