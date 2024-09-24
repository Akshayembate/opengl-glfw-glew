#pragma once

#include <GL/glew.h>
// adding asser function for using error debug
#define ASSERT(x) if (!(x)) __debugbreak();

// another methode by using macro
// in this passing a variable x to  the function glcall
// first glclearerror will execute
// then x is executed which means the function which is wrapooed in sid ethe glcall
// once that is done the gllogcall will check the erros 
// if te error is there it will stop the execution otherwise resume.
// the funtion gllogcall is taiking arguments 
//  #x  (stringified version of x basically it is the function name will send as a string)
#define GLCall(x) glClearError();\
    x;\
    ASSERT(glLogCall(#x, __FILE__,__LINE__))

// error handeling
void glClearError();

// print error by using glGetError from opengl
// adding more contents to the function like the function name , line etc
bool glLogCall(const char* function, const char* file, int line);
