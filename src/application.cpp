#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    // Cleanup and close the window
    glfwTerminate();
    return 0;
}
