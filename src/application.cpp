#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream> // added for reading the files
#include <string>
#include <sstream> 

struct shaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static shaderProgramSource paraseShader(const std::string& filePath)
{
    // input file stream 
    std::ifstream stream(filePath);
    if (!stream)
        std::cout << "failed to open file: " << filePath << "\n";

    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    shaderType type = shaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = shaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(),ss[1].str()};
}

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
    float position[] = {
        -0.5f, -0.5f, // Vertex 1  index 0
         0.5f, -0.5f, // Vertex 2  index 1
         0.5f,  0.5f, // Vertex 3  index 2

        // 0.5f,  0.5f, // Vertex 1
        -0.5f,  0.5f, // Vertex 2   index 3
        //-0.5f, -0.5f  // Vertex 3
    };

    // index buffer 
    int index[] =
    {
        0, 1, 2 ,// index = 0, 1, 2 for the first triangle
        2, 3, 0 // index = 2, 3, 0 for the second triangle
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

    // for making the index buffer algoritam
    // Generate a buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Upload the vertex data to the GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    shaderProgramSource source = paraseShader("C:/Users/anscer/Desktop/me/cpp/openGl/started - openGL - glfw - glew/res/shader/basic.shader");

    std::cout << "vertex shader: " << source.vertexSource << "\n";
    std::cout << "fragment shader: " << source.fragmentSource << "\n"; 

    // calling shader program
    unsigned int shader = createShader(source.vertexSource,source.fragmentSource);

    // bind the shader
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing rectangle from index array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Draw the triangle (3 vertices)
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // delete the shader program
    glDeleteProgram(shader);

    // Cleanup and close the window
    glfwTerminate();
    return 0;
}
