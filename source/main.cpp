#include <iostream>
#include "stb_image.h"
#include "gulpp.h"
#include <GLFW/glfw3.h>

const unsigned int SCREEN_WIDTH = 720;
const unsigned int SCREEN_HEIGHT = 720;

const unsigned short OPENGL_MAJOR_VERSION = 4;
const unsigned short OPENGL_MINOR_VERSION = 6;

bool vSync = true;



GLfloat vertices[] =
                {
                        -0.5f, -0.5f , 0.0f, 0.0f, 0.0f,
                        -0.5f,  0.5f , 0.0f, 0.0f, 1.0f,
                        0.5f,  0.5f , 0.0f, 1.0f, 1.0f,
                        0.5f, -0.5f , 0.0f, 1.0f, 0.0f,
                };

GLuint indices[] =
               {
                       0, 2, 1,
                       0, 3, 2
               };

const char* vertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;
out vec2 UVs;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.000);
	UVs = uvs;
})";
const char* fragmentShaderSource = R"(#version 460 core
out vec4 FragColor;
uniform sampler2D tex;
in vec2 UVs;
void main()
{
	FragColor = vec4(0.965, 0.318, 0.000, 1.000);
	FragColor = texture(tex, UVs);
})";

int main() {
    // Initializing GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Initializing the GLFW window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Context", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vSync);

    // Initializing glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
    }

    // Initializing the OpenGL viewport
    gl::Viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Compiling the shader
    gl::u32 vertexShader = gl::CreateShader(gl::ShaderType::eVertexShader);
    gl::ShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    gl::CompileShader(vertexShader);

    gl::u32 fragmentShader = gl::CreateShader(gl::ShaderType::eFragmentShader);
    gl::ShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    gl::CompileShader(fragmentShader);

    gl::u32 shaderProgram = gl::CreateProgram();
    gl::AttachShader(shaderProgram, vertexShader);
    gl::AttachShader(shaderProgram, fragmentShader);
    gl::LinkProgram(shaderProgram);

    gl::DeleteShader(vertexShader);
    gl::DeleteShader(fragmentShader);

    // Setting up the buffers
    gl::u32 VAO, VBO, EBO;
    gl::CreateVertexArrays(1, &VAO);
    gl::CreateBuffers(1, &VBO);
    gl::CreateBuffers(1, &EBO);

    gl::NamedBufferData(VBO, sizeof(vertices), vertices, gl::VertexBufferObjectUsage::eStaticDraw);
    gl::NamedBufferData(EBO, sizeof(indices), indices, gl::VertexBufferObjectUsage::eStaticDraw);

    gl::EnableVertexArrayAttrib(VAO, 0);
    gl::VertexArrayAttribBinding(VAO, 0, 0);
    gl::VertexArrayAttribFormat(VAO, 0, 3, gl::VertexAttribType::eFloat, gl::Boolean::eFalse, 0);

    gl::EnableVertexArrayAttrib(VAO, 1);
    gl::VertexArrayAttribBinding(VAO, 1, 0);
    gl::VertexArrayAttribFormat(VAO, 1, 2, gl::VertexAttribType::eFloat, gl::Boolean::eFalse, 3 * sizeof(gl::f32));

    gl::VertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(gl::f32));
    gl::VertexArrayElementBuffer(VAO, EBO);

    // Setting up the texture
    int32_t widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("assets/textures/gulpp.png", &widthImg, &heightImg, &numColCh, 0);

    gl::u32 tex;
    gl::CreateTextures(gl::TextureTarget::eTexture2d, 1, &tex);
    gl::TextureParameteri(tex, gl::TextureParameterName::eTextureMinFilter, (gl::i32)gl::TextureMinFilter::eLinear);
    gl::TextureParameteri(tex, gl::TextureParameterName::eTextureMagFilter, (gl::i32)gl::TextureMinFilter::eLinear);
    gl::TextureParameteri(tex, gl::TextureParameterName::eTextureWrapS, (gl::i32)gl::TextureWrapMode::eClampToEdge);
    gl::TextureParameteri(tex, gl::TextureParameterName::eTextureWrapT, (gl::i32)gl::TextureWrapMode::eClampToEdge);
    gl::TextureStorage2D(tex, 1, gl::SizedInternalFormat::eRgba8, widthImg, heightImg);
    gl::TextureSubImage2D(tex, 0, 0, 0, widthImg, heightImg, gl::PixelFormat::eRgba, gl::PixelType::eUnsignedByte, bytes);
    gl::GenerateTextureMipmap(tex);

    stbi_image_free(bytes);

    // Main draw loop
    while (!glfwWindowShouldClose(window)){
	// Clear screen
        gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl::Clear(gl::ClearBufferMask::eColorBufferBit);

	// Set up and draw sprite
        gl::UseProgram(shaderProgram);
        gl::BindTextureUnit(0, tex);
        gl::Uniform1i(gl::GetUniformLocation(shaderProgram, "tex"), 0);
        gl::BindVertexArray(VAO);
        gl::DrawElements(gl::PrimitiveType::eTriangles, sizeof(indices)/ sizeof(indices[0]), gl::DrawElementsType::eUnsignedInt, nullptr);

	// Swap the swapbuffers
        glfwSwapBuffers(window);
	// Poll for user input
        glfwPollEvents();
    }

    return 0;
}
