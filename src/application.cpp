#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// function for compile shader for specified type like vertex shader or fragment type
static unsigned int compileShader(unsigned int type, const std::string& source)
{
    // returns an id from the type which will represent in opengl
    unsigned int id = glCreateShader(type);

    // converting the string to const char* because opengl functions dont accept strings directly
    const char* src = source.c_str();

    // providing source code src to shader with the identifier id and the 1 string and the length is determined by 0(null ptr)
    glShaderSource(id, 1, &src, nullptr);

    // compile the source code that was previously attached to the shader obj
    glCompileShader(id);

    // ERROR HANDELING
    int result;

    // get the information of the shader from the id as well as the status and store in the result
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        // if result isn 0 means the gl_compile_status id gl_false
        int length;
        
        // retreives the the lenghth information from the gl_info_lrngth, this tell us how long the error message is 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // allocating the message buffer
        char* message = (char*)alloca(length * sizeof(char));

        // retrieves the shader compilation log and stores in buffer
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "falied to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!\n";
        std::cout << message << "\n";

        glDeleteShader(id);
        return 0;
    }

    // return id
    return id;
}

// shader function taked two arguments vertex as well as fragment 
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // creating an empty shader program
    unsigned int program = glCreateProgram();

    // compiling the shaders
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attaching the shader with program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // link the attached shader into the final executable shader program
    glLinkProgram(program);

    // checlking the program is valid for execution
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize GLEW after creating a valid OpenGL context
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed!\n";
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

    // Define vertices for a triangle
    float position[6] = {
        -0.5f, -0.5f, // Vertex 1
         0.0f,  0.5f, // Vertex 2
         0.5f, -0.5f  // Vertex 3
    };

    // Generate a buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Upload the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

    // Enable the vertex attribute array (index 0)
    glEnableVertexAttribArray(0);

    // Specify how the vertex data is structured
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // vertex shader
    std::string vertexShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"
    "gl_Position = position;\n"
    "}\n";

    // fragment shader
    std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;\n"
    "\n"
    "void main()\n" 
    "{\n"
    "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

    // calling shader program
    unsigned int shader = createShader(vertexShader,fragmentShader);

    // bind the shader
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle (3 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // delete the shader program
    glDeleteShader(shader);

    // Cleanup and close the window
    glfwTerminate();
    return 0;
}
