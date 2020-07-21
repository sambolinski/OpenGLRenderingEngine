#pragma once
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "MatrixStack.h"
#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <windows.h>
#include <fstream>



//UNAMED RENDERING ENGINE

//This engine was created with help from
//Joey de Vries using LearnOpenGL https://learnopengl.com, https://twitter.com/JoeyDeVriez
//David Barr using the olcPixelGameEngine https://github.com/OneLoneCoder, https://www.youtube.com/c/javidx9/
//Jason L. McKesson's matrix stack 
//VBO, Model loader, parts of shader taken from City Unversity OpenGL template by Dr Greg Slabaugh gregory.slabaugh.1@city.ac.uk



void ClearError() {
    while (glGetError() != GL_NO_ERROR);
}
void CheckError() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] :: " << error << "\n";
    }
}

//HEADERS

//Le oglr rendering engine  \(\..)
namespace oglr{

    //CONSTANTS
    struct RGBA {
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLfloat a;

        RGBA(int red, int green, int blue, int alpha = 255);
        RGBA(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1.0);
        RGBA(glm::vec3 c, GLfloat alpha = 1.0);
        RGBA(glm::vec4 c);
        RGBA();
        RGBA(const RGBA& c);


        std::string ToString();

        //Comparators
        bool operator==(const RGBA& c);
        bool operator!=(const RGBA& c);

        //Simple colour combining
        RGBA operator+(const RGBA& c);
        RGBA operator-(const RGBA& c);
    };
    static const RGBA
        WHITE               ((uint8_t)255, (uint8_t)255, (uint8_t)255),
        BLACK               ((uint8_t)  0, (uint8_t)  0, (uint8_t)  0),
        RED                 ((uint8_t)255, (uint8_t)  0, (uint8_t)  0),
        GREEN               ((uint8_t)  0, (uint8_t)255, (uint8_t)  0),
        BLUE                ((uint8_t)  0, (uint8_t)  0, (uint8_t)255),
        GREY                ((uint8_t)190, (uint8_t)190, (uint8_t)190),
        DARK_ELECTRIC_BLUE  ((uint8_t) 83, (uint8_t)104, (uint8_t)120);

    static const glm::vec3 
        DIRECTION_LEFT      (-1.0f, 0.0f, 0.0f),
        DIRECTION_RIGHT     ( 1.0f, 0.0f, 0.0f),
        DIRECTION_UP        ( 0.0f, 1.0f, 0.0f),
        DIRECTION_DOWN      ( 0.0f,-1.0f, 0.0f),
        DIRECTION_FORWARD   ( 0.0f, 0.0f, 1.0f),
        DIRECTION_BACKWARD  ( 0.0f, 0.0f,-1.0f);

    
    // Projection matrices. Do not edit in implementation class.
    static const float
        DEFAULT_FOV = 45.0f,
        DEFAULT_FNEAR = 0.5f,
        DEFAULT_FFAR = 1000.0f;
    static glm::mat4
        PERSPECTIVE_PROJECTION,     
        ORTHOGRAPHIC_PROJECTION;
    //Colour Structure

    //USER INPUT
    namespace Key{
        class Key {
        public:
            bool IsPressed = false;
            bool IsHeld = false;
            bool IsReleased = false;
            void Update(int update);
        };

        static Key
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            LEFT, RIGHT, UP, DOWN,
            LSHIFT, RSHIFT, LCTRL, RCTRL;
    }
    
    static class Mouse {
    private:
        static double x;
        static double y;
        static double xScroll;
        static double yScroll;
    public:
        static Key::Key LEFT_MOUSE, RIGHT_MOUSE, MIDDLE_MOUSE;
    public:
        static void UpdateCoordinates(double xPos, double yPos);
        static void UpdateScroll(double xOffset, double yOffset);
        static void ResetScroll();
    public://Getters
        static double GetX();
        static double GetY();
        static double GetScrollX();
        static double GetScrollY();
    };

    //CAMERA
    class Camera {
    private:
        glm::vec3 m_Position;
        glm::vec3 m_Up;
        glm::vec3 m_View;
        glm::mat4 m_ProjectionMatrix;

    public:
        Camera();
        ~Camera();
    public://Getters and Setters
        void SetProjectionMatrix(glm::mat4 &projectionMatrix);
        void SetProjectionMatrix(float fov, float fNear, float fFar, float aspectRatio);
        void SetProjectionMatrix(float width, float height);

        void SetCamera(glm::vec3 pos, glm::vec3 view, glm::vec3 up);
        void SetPos(glm::vec3 pos);
        void SetView(glm::vec3 view);
        void SetUp(glm::vec3 up);

        glm::mat4 &GetProjectionMatrix();
        glm::vec3 &GetPosition();
        glm::vec3 &GetView();
        glm::vec3 &GetUp();
    public://Movement
        void RotateRadian(float angle, glm::vec3 up, float fDt = 1.0f);
        void RotateRadian(float angle, float fDt = 1.0f);
        void RotateDeg(float angle, glm::vec3 up, float fDt = 1.0f);
        void RotateDeg(float angle, float fDt = 1.0f);
        void Translate(glm::vec3 direction, float fDt);// Translation in global space
        void Strafe(glm::vec3 direction, float fDt); //Translation based on local camera space.
    };

    //Rendering Engine    
    class Renderer {
    static uint16_t SCREEN_WIDTH;
    static uint16_t SCREEN_HEIGHT;
    private:
        GLFWwindow* m_Window;
    protected:
        std::string sWindowTitle;
        Camera m_Camera;
    public:
        Renderer();
        ~Renderer();
        void Create(uint16_t width = 1250, uint16_t height = 850);
        void InitialiseShaders();
    public: //Window Settings
        void SetWindowTitle(std::string name);
        uint16_t GetScreenWidth();
        uint16_t GetScreenHeight();
        void SetAntiAliasingSamples(uint8_t samples);
        void EnableAntiAliasing();
        void DisableAntiAliasing();
    public: // Overriden
        virtual void OnCreate() = 0;
        virtual void OnUpdate(float fDeltaTimeSeconds) = 0;
    public: //GLFW functions
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
        static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    public:
        void Clear(RGBA colour = oglr::BLACK);
        void Run();
    public: //Render Functions
    public: //User Input        
    };



    //OPENGL RENDERING CLASSES
    class VertexBufferObject {
    private:
        unsigned int m_ID;
        std::vector<BYTE> m_Data; //maybe change data type to something more efficient
    public:
        VertexBufferObject();
        ~VertexBufferObject();
        void Create();
        void BindBuffer();
        void DeleteBuffer();
        void UploadBufferData(int usageHint);
        void AddData(void* dataPointer, unsigned int dataSize);

    };
    
    
    class Shader {
    private:
        unsigned int m_ID;
        unsigned int m_ShaderType;
        bool m_IsLoaded;
    public:
        Shader();
        Shader(std::string filePath, int type);
        ~Shader();
        void LoadShader(std::string filePath, int type);
        void DeleteShader();
        bool IsLoaded();
        unsigned int GetShaderID();
    };

    
    class ShaderProgram {
    private:
        unsigned int m_ID;
    public:
        ShaderProgram();
        void Initialise(Shader* vertexShader, Shader* fragmentShader);
        void Create();
        void BindShader(Shader* shader);
        void LinkProgram();
        void UseProgram();
        void DeleteProgram();

        void SetUniform(std::string name, float value);
        void SetUniform(std::string name, double value);
        void SetUniform(std::string name, int value);
        void SetUniform(std::string name, bool value);
        void SetUniform(std::string name, const glm::vec2 value);
        void SetUniform(std::string name, const glm::dvec2 value);
        void SetUniform(std::string name, const glm::vec3 value);
        void SetUniform(std::string name, const glm::vec4 value);
        void SetUniform(std::string name, const glm::mat3 value);
        void SetUniform(std::string name, const glm::mat4 value);
    };

    
    static Shader
        DEFAULT_VERTEX_SHADER,
        DEFAULT_FRAGMENT_SHADER,
        MANDELBROT_VERTEX_SHADER,
        MANDELBROT_FRAGMENT_SHADER,
        JULIA_VERTEX_SHADER,
        JULIA_FRAGMENT_SHADER,
        MANDELBROT_DOUBLE_VERTEX_SHADER, 
        MANDELBROT_DOUBLE_FRAGMENT_SHADER;
    
    static ShaderProgram
        DEFAULT_SHADER_PROGRAM,
        MANDELBROT_SHADER_PROGRAM,
        JULIA_SHADER_PROGRAM,
        MANDELBROT_DOUBLE_SHADER_PROGRAM;
      
    namespace primitive {
        //Test rendering Triangle
        class Triangle {
            VertexBufferObject m_VBO;
            unsigned int m_VAO;
        public:
            Triangle();
            void Create();
            void Render();
            void Release();
        };
        //Test rendering Triangle
        class Plane {
            VertexBufferObject m_VBO;
            unsigned int m_VAO;
        public:
            Plane();
            void Create();
            void Render();
            void Release();
        };
        //Test rendering Triangle
        class Cube {
            VertexBufferObject m_VBO;
            unsigned int m_VAO;
        public:
            Cube();
            void Create();
            void Render();
            void Release();
        };
    };
}









//IMPLEMENTATION
namespace oglr {
    RGBA::RGBA(int red, int green, int blue, int alpha) {
        r = red  /255.0f;
        g = green/255.0f;
        b = blue /255.0f;
        a = alpha/255.0f;
    }
    RGBA::RGBA(float red, float green, float blue, float alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    //assumes already normalised
    RGBA::RGBA(glm::vec3 c, GLfloat alpha) {
        r = c.x;
        g = c.y;
        b = c.z;
        a = alpha;
    }
    //assumes already normalised
    RGBA::RGBA(glm::vec4 c) {
        r = c.x;
        g = c.y;
        b = c.z;
        a = c.a;
    }
    RGBA::RGBA() {
        r = 0;
        g = 0;
        b = 0;
        a = 1.0;
    }
    RGBA::RGBA(const RGBA& c) {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
    }

    std::string RGBA::ToString() {
        return "(" + std::to_string(r) + ", " + std::to_string(r) + ", " + std::to_string(r) + ", " + std::to_string(r) + ")";
    }

    
    //
    //USER INPUT
    //

    //Keyboard
    void Key::Key::Update(int update) {
        switch (update) {
            case GLFW_PRESS:
                IsPressed = true;
                IsHeld = false;
                IsReleased = false;
                break;
            case GLFW_REPEAT:
                IsPressed = false;
                IsHeld = true;
                IsReleased = false;
                break;
            case GLFW_RELEASE:
                IsPressed = false;
                IsHeld = false;
                IsReleased = true;
                break;
        }
    }

    //Mouse
    double Mouse::x = 0;
    double Mouse::y = 0;
    double Mouse::xScroll = 0;
    double Mouse::yScroll = 0;
    Key::Key Mouse::LEFT_MOUSE = Key::Key();
    Key::Key Mouse::RIGHT_MOUSE = Key::Key();
    Key::Key Mouse::MIDDLE_MOUSE = Key::Key();
    void Mouse::UpdateCoordinates(double xPos, double yPos) {
        x = xPos;
        y = yPos;
    }
    void Mouse::UpdateScroll(double xOffset, double yOffset) {
        xScroll = xOffset;
        yScroll = yOffset;
    }
    void  Mouse::ResetScroll() {
        xScroll = 0;
        yScroll = 0;
    }
    double Mouse::GetX(){
        return x;
    }

    double Mouse::GetY(){
        return y;
    }

    double Mouse::GetScrollX() {
        return xScroll;
    }

    double Mouse::GetScrollY() {
        return yScroll;
    }
    //
    // RENDERER
    //
    // IMPLEMENTATION
    //
    //
    uint16_t Renderer::SCREEN_WIDTH = 0;
    uint16_t Renderer::SCREEN_HEIGHT = 0;
    Renderer::Renderer() {
        sWindowTitle = "";
    }

    Renderer::~Renderer() {
    }

    void Renderer::Create(uint16_t width, uint16_t height) {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;

        //Initialise Projection Maths
        ORTHOGRAPHIC_PROJECTION = glm::ortho(0.0f, float(SCREEN_WIDTH), 0.0f, float(SCREEN_HEIGHT));
        PERSPECTIVE_PROJECTION = glm::perspective(DEFAULT_FOV, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), DEFAULT_FNEAR, DEFAULT_FFAR);
        m_Camera = Camera();
        m_Camera.SetProjectionMatrix(PERSPECTIVE_PROJECTION);

        //GLFW initialisation
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //Window Creation
        m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, sWindowTitle.c_str(), NULL, NULL);
        if (m_Window == NULL) {
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_Window);

        //Loading GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            return;
        }
        
        SetAntiAliasingSamples(4);
        EnableAntiAliasing();

        InitialiseShaders();
        
        //OpenGL initial Variables
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        char* version = (char *)glGetString(GL_VERSION);
        std::cout << version << std::endl;
        
        //Debugging
        //glEnable(GL_DEBUG_OUTPUT);
        //glDebugMessageCallback(MessageCallback, 0);
    }
    void Renderer::InitialiseShaders(){
        //Shaders
        DEFAULT_VERTEX_SHADER.LoadShader("Resources/Shaders/defaultVertexShader.vert", GL_VERTEX_SHADER);
        DEFAULT_FRAGMENT_SHADER.LoadShader("Resources/Shaders/defaultFragmentShader.frag", GL_FRAGMENT_SHADER);
        MANDELBROT_VERTEX_SHADER.LoadShader("Resources/Shaders/mandelbrotVertexShader.vert", GL_VERTEX_SHADER);
        MANDELBROT_FRAGMENT_SHADER.LoadShader("Resources/Shaders/mandelbrotFragmentShader.frag", GL_FRAGMENT_SHADER);
        JULIA_VERTEX_SHADER.LoadShader("Resources/Shaders/juliaVertexShader.vert", GL_VERTEX_SHADER);
        JULIA_FRAGMENT_SHADER.LoadShader("Resources/Shaders/juliaFragmentShader.frag", GL_FRAGMENT_SHADER);
        MANDELBROT_DOUBLE_VERTEX_SHADER.LoadShader("Resources/Shaders/mandelbrotVertexShaderDouble.vert", GL_VERTEX_SHADER);
        MANDELBROT_DOUBLE_FRAGMENT_SHADER.LoadShader("Resources/Shaders/mandelbrotFragmentShaderDouble.frag", GL_FRAGMENT_SHADER);

        //Shader Programs
        DEFAULT_SHADER_PROGRAM.Initialise(&DEFAULT_VERTEX_SHADER,&DEFAULT_FRAGMENT_SHADER);
        MANDELBROT_SHADER_PROGRAM.Initialise(&MANDELBROT_VERTEX_SHADER, &MANDELBROT_FRAGMENT_SHADER);
        JULIA_SHADER_PROGRAM.Initialise(&JULIA_VERTEX_SHADER, &JULIA_FRAGMENT_SHADER);
        MANDELBROT_DOUBLE_SHADER_PROGRAM.Initialise(&MANDELBROT_DOUBLE_VERTEX_SHADER, &MANDELBROT_DOUBLE_FRAGMENT_SHADER);
    }
    void Renderer::Clear(RGBA colour) {
        glClearColor(colour.r, colour.g, colour.b, colour.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //Window Functions
    void Renderer::SetWindowTitle(std::string title) {
        sWindowTitle = title;
        glfwSetWindowTitle(m_Window, sWindowTitle.c_str());
    }

    uint16_t Renderer::GetScreenWidth() {
        return SCREEN_WIDTH;
    }
    uint16_t Renderer::GetScreenHeight() {
        return SCREEN_HEIGHT;
    }

    void Renderer::SetAntiAliasingSamples(uint8_t samples) {
        glfwWindowHint(GLFW_SAMPLES, samples);
    }
    void Renderer::EnableAntiAliasing() {
        glEnable(GL_MULTISAMPLE);
    }
    void Renderer::DisableAntiAliasing() {
        glDisable(GL_MULTISAMPLE);
    }

    //GLFW SETTINGS
    void Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
        ORTHOGRAPHIC_PROJECTION = glm::ortho(0.0f, float(SCREEN_WIDTH), 0.0f, float(SCREEN_HEIGHT));
        PERSPECTIVE_PROJECTION = glm::perspective(DEFAULT_FOV, float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), DEFAULT_FNEAR, DEFAULT_FFAR);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    }

    void Renderer::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_A:
                Key::A.Update(action);
                break;
            case GLFW_KEY_B:
                Key::B.Update(action);
                break;
            case GLFW_KEY_C:
                Key::C.Update(action);
                break;
            case GLFW_KEY_D:
                Key::D.Update(action);
                break;
            case GLFW_KEY_E:
                Key::E.Update(action);
                break;
            case GLFW_KEY_F:
                Key::F.Update(action);
                break;
            case GLFW_KEY_G:
                Key::G.Update(action);
                break;
            case GLFW_KEY_H:
                Key::H.Update(action);
                break;
            case GLFW_KEY_I:
                Key::I.Update(action);
                break;
            case GLFW_KEY_J:
                Key::J.Update(action);
                break;
            case GLFW_KEY_K:
                Key::K.Update(action);
                break;
            case GLFW_KEY_L:
                Key::L.Update(action);
                break;
            case GLFW_KEY_M:
                Key::M.Update(action);
                break;
            case GLFW_KEY_N:
                Key::N.Update(action);
                break;
            case GLFW_KEY_O:
                Key::O.Update(action);
                break;
            case GLFW_KEY_P:
                Key::P.Update(action);
                break;
            case GLFW_KEY_Q:
                Key::Q.Update(action);
                break;
            case GLFW_KEY_R:
                Key::R.Update(action);
                break;
            case GLFW_KEY_S:
                Key::S.Update(action);
                break;
            case GLFW_KEY_T:
                Key::T.Update(action);
                break;
            case GLFW_KEY_U:
                Key::U.Update(action);
                break;
            case GLFW_KEY_V:
                Key::V.Update(action);
                break;
            case GLFW_KEY_W:
                Key::W.Update(action);
                break;
            case GLFW_KEY_X:
                Key::X.Update(action);
                break;
            case GLFW_KEY_Y:
                Key::Y.Update(action);
                break;
            case GLFW_KEY_Z:
                Key::Z.Update(action);
                break;
            case GLFW_KEY_LEFT:
                Key::LEFT.Update(action);
                break;
            case GLFW_KEY_RIGHT:
                Key::RIGHT.Update(action);
                break;
            case GLFW_KEY_UP:
                Key::UP.Update(action);
                break;
            case GLFW_KEY_DOWN:
                Key::DOWN.Update(action);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                Key::LSHIFT.Update(action);
                break;
            case GLFW_KEY_RIGHT_SHIFT:
                Key::RSHIFT.Update(action);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                Key::LCTRL.Update(action);
                break;
            case GLFW_KEY_RIGHT_CONTROL:
                Key::RCTRL.Update(action);
                break;
        }
    }

    void Renderer::CursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
        Mouse::UpdateCoordinates(xPos, yPos);
    }

    void Renderer::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
        Mouse::UpdateScroll(xOffset, yOffset);
    }
    void Renderer::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_RIGHT:
                Mouse::RIGHT_MOUSE.Update(action);
                break;
            case GLFW_MOUSE_BUTTON_LEFT:
                Mouse::LEFT_MOUSE.Update(action);
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                Mouse::MIDDLE_MOUSE.Update(action);
                break;
        }
    }

    void Renderer::Run() {
        //GLFW setup
        glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetCursorPosCallback(m_Window, CursorPositionCallback); 
        glfwSetScrollCallback(m_Window, ScrollCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);

        //Scene Initialisation
        OnCreate();

        //Time creation
        auto timeStart = std::chrono::system_clock::now();
        auto timeEnd = std::chrono::system_clock::now();

        //MAIN RENDER LOOP
        while (!glfwWindowShouldClose(m_Window)) {

            //Time handling from olcPixelGameEngine
            timeEnd = std::chrono::system_clock::now();
            std::chrono::duration<float> deltaTime = timeEnd - timeStart;
            timeStart = timeEnd;
            float fElapsedTime = deltaTime.count();


            OnUpdate(fElapsedTime);


            Mouse::ResetScroll();


            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
        glfwTerminate();
    }

    //Camera
    Camera::Camera() {
        m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
        m_View = glm::vec3(0.0f, 0.0f, -1.0f);
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    Camera::~Camera() {}
    //Getters and Setters
    void Camera::SetProjectionMatrix(glm::mat4 &projectionMatrix) {
        m_ProjectionMatrix = projectionMatrix;
    }
    void Camera::SetProjectionMatrix(float fov, float fNear, float fFar, float aspectRatio) {
        m_ProjectionMatrix = glm::perspective(fov, aspectRatio, fNear, fFar);
    }
    void Camera::SetProjectionMatrix(float width, float height){
        m_ProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height);
    }

    void Camera::SetCamera(glm::vec3 pos, glm::vec3 view, glm::vec3 up){
        m_Position = pos;
        m_View = view;
        m_Up = up;
    }
    void Camera::SetPos(glm::vec3 pos){
        m_Position = pos;
    }
    void Camera::SetView(glm::vec3 view){
        m_View = view;
    }
    void Camera::SetUp(glm::vec3 up){
        m_Up;
    }

    glm::mat4 &Camera::GetProjectionMatrix(){
        return m_ProjectionMatrix;
    }

    glm::vec3 &Camera::GetPosition() {
        return m_Position;
    }
    glm::vec3 &Camera::GetView() {
        return m_View;
    }
    glm::vec3 &Camera::GetUp() {
        return m_Up;
    }

    //Movement
    void Camera::RotateRadian(float angle, glm::vec3 up, float fDt){
        RotateDeg(glm::degrees(angle), up, fDt);
    }
    void Camera::RotateRadian(float angle, float fDt){
        RotateDeg(glm::degrees(angle), m_Up, fDt);
    }
    void Camera::RotateDeg(float angle, glm::vec3 up, float fDt){}
    void Camera::RotateDeg(float angle, float fDt){
        RotateDeg(angle, m_Up, fDt);
    }
    void Camera::Translate(glm::vec3 direction, float fDt){
        m_Position += direction * fDt;
    }// Translation in global space
    void Camera::Strafe(glm::vec3 direction, float fDt){} //Translation based on local camera space.


    //OPENGL RENDERING CLASSES

    VertexBufferObject::VertexBufferObject() {}

    VertexBufferObject::~VertexBufferObject() {}

    void VertexBufferObject::Create() {
        glGenBuffers(1, &m_ID);
    }
    void VertexBufferObject::BindBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }
    void VertexBufferObject::DeleteBuffer() {
        glDeleteBuffers(1, &m_ID);
        m_Data.clear();
    }
    void VertexBufferObject::UploadBufferData(int usageHint) {
        glBufferData(GL_ARRAY_BUFFER, m_Data.size(), &m_Data[0], usageHint);
        m_Data.clear();
    }
    void VertexBufferObject::AddData(void* dataPointer, unsigned int dataSize) {
        m_Data.insert(m_Data.end(), (BYTE*)dataPointer, (BYTE*)dataPointer + dataSize);
    }

    Shader::Shader() {}; 
    
    Shader::Shader(std::string shaderSource, int type) {
        LoadShader(shaderSource, type);
    };

    Shader::~Shader() {};

    void Shader::LoadShader(std::string filePath, int type) {
        std::ifstream shaderFile(filePath);
        if (shaderFile.is_open()) {
            std::string shaderSourceString;
            for (std::string line; std::getline(shaderFile, line);) {
                shaderSourceString += line + "\n";
            }
            const char* shaderSource = shaderSourceString.c_str();
            m_ID = glCreateShader(type);

            int bla = 2;
            glShaderSource(m_ID, 1, &shaderSource, NULL);
            glCompileShader(m_ID);
            m_IsLoaded = true;

            //Checking if compolation is successful
            int success;
            char infoLog[512];
            glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }
        } else {
            std::cout << "CANNOT OPEN SHADER FILE\n";
        }

    }
    void Shader::DeleteShader() {
        if (m_IsLoaded) {
            glDeleteShader(m_ID);
            m_IsLoaded = false;
        }
    }
    unsigned int Shader::GetShaderID() {
        return m_ID;
    }

    bool Shader::IsLoaded() {
        return m_IsLoaded;
    }

    ShaderProgram::ShaderProgram() {
    }

    void ShaderProgram::Initialise(Shader* vertexShader, Shader* fragmentShader) {
        Create();
        BindShader(vertexShader);
        BindShader(fragmentShader); 
        LinkProgram();
        vertexShader->DeleteShader();
        fragmentShader->DeleteShader();
    }

    void ShaderProgram::Create() {
        m_ID = glCreateProgram();
    }

    void ShaderProgram::BindShader(Shader* shader) {
        if (!shader->IsLoaded()) {
            return;
        }
        glAttachShader(m_ID, shader->GetShaderID());
    }

    void ShaderProgram::LinkProgram() {
        glLinkProgram(m_ID);
        int success;
        char infoLog[512];
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    void ShaderProgram::UseProgram() {
        glUseProgram(m_ID);
    }

    void ShaderProgram::DeleteProgram() {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::SetUniform(std::string name, float value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform1f(uniformLocation, value);
    }

    void ShaderProgram::SetUniform(std::string name, double value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform1d(uniformLocation, value);
    }
    void ShaderProgram::SetUniform(std::string name, int value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform1i(uniformLocation, value);
    }
    void ShaderProgram::SetUniform(std::string name, bool value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform1i(uniformLocation, (GLint)value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::vec2 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform2fv(uniformLocation, 1, (GLfloat*)&value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::dvec2 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform2dv(uniformLocation, 1, (GLdouble*)&value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::vec3 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform3fv(uniformLocation, 1, (GLfloat*)&value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::vec4 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniform4fv(uniformLocation, 1, (GLfloat*)&value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::mat3 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(uniformLocation, 1, false, (GLfloat*)&value);
    }
    void ShaderProgram::SetUniform(std::string name, const glm::mat4 value) {
        int uniformLocation = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(uniformLocation, 1, false, (GLfloat*)&value);
    }

    primitive::Triangle::Triangle() {
    }

    void primitive::Triangle::Create() {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        m_VBO.Create();
        m_VBO.BindBuffer();

        glm::vec3 triangleVertices[3] = {
            glm::vec3(-0.5f,-0.5f, 0.0f),
            glm::vec3( 0.5f,-0.5f, 0.0f),
            glm::vec3( 0.0f, 0.5f, 0.0f),
        };
        glm::vec3 triangleColours[3] = {
           glm::vec3(1.0f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3(0.0f, 0.0f, 1.0f),
        };

        for (unsigned int i = 0; i < 3; i++) {
            m_VBO.AddData(&triangleVertices[i], sizeof(glm::vec3));
            m_VBO.AddData(&triangleColours[i], sizeof(glm::vec3));
        }

        m_VBO.UploadBufferData(GL_STATIC_DRAW);
        GLsizei istride = 2*sizeof(glm::vec3);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));
    }
    void primitive::Triangle::Render() {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    void primitive::Triangle::Release() {
        glDeleteVertexArrays(1, &m_VAO);
        m_VBO.DeleteBuffer();
    }

    primitive::Plane::Plane() {
    }

    void primitive::Plane::Create() {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        m_VBO.Create();
        m_VBO.BindBuffer();

        glm::vec3 planeVertices[6] = {
            glm::vec3( 1.0f,  1.0f, 0.0f),
            glm::vec3( 1.0f, -1.0f, 0.0f),
            glm::vec3(-1.0f,  1.0f, 0.0f),
            glm::vec3( 1.0f, -1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3(-1.0f,  1.0f, 0.0f),
        };

        for (unsigned int i = 0; i < 6; i++) {
            m_VBO.AddData(&planeVertices[i], sizeof(glm::vec3));
        }

        m_VBO.UploadBufferData(GL_STATIC_DRAW);
        GLsizei istride = sizeof(glm::vec3);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);

    }
    void primitive::Plane::Render() {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    void primitive::Plane::Release() {
        glDeleteVertexArrays(1, &m_VAO);
        m_VBO.DeleteBuffer();
    }

    primitive::Cube::Cube() {
    }

    void primitive::Cube::Create() {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        m_VBO.Create();
        m_VBO.BindBuffer();

        glm::vec3 cubeVertices[8] = {
        glm::vec3(-1.0f, -1.0f, -1.0f), //0
        glm::vec3(1.0f, -1.0f, -1.0f), //1
        glm::vec3(-1.0f,  1.0f, -1.0f), //2
        glm::vec3(1.0f,  1.0f, -1.0f), //3
        glm::vec3(-1.0f, -1.0f,  1.0f), //4
        glm::vec3(1.0f, -1.0f,  1.0f), //5
        glm::vec3(-1.0f,  1.0f,  1.0f), //6
        glm::vec3(1.0f,  1.0f,  1.0f), //7
        };
        // Triangle positions
        glm::vec3 cubeTriangles[36] = {
            //Face Bottom
            cubeVertices[0],
            cubeVertices[1],
            cubeVertices[5],

            cubeVertices[5],
            cubeVertices[4],
            cubeVertices[0],

            //Face Left
            cubeVertices[1],
            cubeVertices[3],
            cubeVertices[7],

            cubeVertices[7],
            cubeVertices[5],
            cubeVertices[1],

            //Face Front
            cubeVertices[6],
            cubeVertices[4],
            cubeVertices[5],

            cubeVertices[5],
            cubeVertices[7],
            cubeVertices[6],

            //Face Right
            cubeVertices[0],
            cubeVertices[4],
            cubeVertices[6],

            cubeVertices[6],
            cubeVertices[2],
            cubeVertices[0],

            //Face Behind
            cubeVertices[0],
            cubeVertices[2],
            cubeVertices[3],

            cubeVertices[3],
            cubeVertices[1],
            cubeVertices[0],

            //Face Top
            cubeVertices[2],
            cubeVertices[6],
            cubeVertices[7],

            cubeVertices[7],
            cubeVertices[3],
            cubeVertices[2],
        };
        

        glm::vec3 triangleColours[3] = {
           glm::vec3(1.0f, 0.0f, 0.0f),
           glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3(0.0f, 0.0f, 1.0f),
        };
        for (unsigned int i = 0; i < 36; i++) {
            m_VBO.AddData(&cubeTriangles[i], sizeof(glm::vec3));
            m_VBO.AddData(&triangleColours[i%3], sizeof(glm::vec3));
        }

        m_VBO.UploadBufferData(GL_STATIC_DRAW);
        GLsizei istride = sizeof(glm::vec3);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, istride, 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, istride, (void*)sizeof(glm::vec3));

    }
    void primitive::Cube::Render() {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    void primitive::Cube::Release() {
        glDeleteVertexArrays(1, &m_VAO);
        m_VBO.DeleteBuffer();
    }
}