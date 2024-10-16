// g++ main.cpp ./src/glad.c -I./include -o program `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -I./GLM/
// mike-multi-objs
#include "body/camera/Camera.cpp"

std::string LoadShaderAsString(std::string &fileName)
{
    std::string resault = "";

    std::string line = "";
    std::ifstream myFile(fileName.c_str());

    if (myFile.is_open())
    {
        while (std::getline(myFile, line))
        {
            resault += line + '\n';
        }
        myFile.close();
    }

    return resault;
}
////////////////////////////

int width = 640;
int height = 480;
SDL_Window *window = nullptr;
SDL_GLContext ctxOGL = nullptr;
bool quit = false;

struct Mesh3D
{

    // VAO
    GLuint vertexArrayObj = 0; // object vertex arrey
    // VBO
    GLuint vertexBufferObj = 0;
    GLuint indexBufferObj = 0; // obj index buffer
                                // GLuint gVertexBufferObj2 = 0;
    GLuint pipeline = 0;

    float uOffset = -2.0f;
    float uOffsetYrot = 0.0f;
    float uOffsetScale = 0.5f;
};

Camera camera;

Mesh3D mesh1;
// Mesh3D mesh2;

// ------------ errer -------------------
static void GLClearAllErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {
    }
};

static bool GLCheckErrors(const char *function, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "GL Error" << error << "\tline : " << line << "\tfunction : " << function
                  << std::endl;
        return true;
    }
    return false;
};

#define GLCheck(x)      \
    GLClearAllErrors(); \
    x;                  \
    GLCheckErrors(#x, __LINE__);

GLuint gGraphicsPipelineShaderProg = 0;

GLuint CompileShader(GLuint type, std::string &source)
{
    GLuint shaderObject;
    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char *src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    return shaderObject;
};

GLuint CreateShaderProg(std::string &vertexShaderSource, std::string &fragmentsShaderSource)
{
    GLuint programObj = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentsShaderSource);

    glAttachShader(programObj, myVertexShader);
    glAttachShader(programObj, myFragmentShader);
    glLinkProgram(programObj);

    glValidateProgram(programObj);

    return programObj;
};

void createGraphicPipeline()
{
    std::string vs = "./shaders/vert.glsl";
    std::string fs = "./shaders/frag.glsl";
    std::string shaderVertexSource = LoadShaderAsString(vs);
    std::string shaderFragmentSource = LoadShaderAsString(fs);
    gGraphicsPipelineShaderProg = CreateShaderProg(shaderVertexSource, shaderFragmentSource);
};

void GetOpenGLVersionInfo()
{
    std::cout << "vendor" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "renderer" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "version" << glGetString(GL_VERSION) << std::endl;
    std::cout << "shading lang" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
};

void MeshCreate(Mesh3D* mesh1)
{
    // on the CPU !!
    std::vector<GLfloat> vertexData{
        // 0 - vertex
        -0.5f, -0.5f, 0.0f, // ver 1
        1.0f, 0.0f, 0.0f,   // verColor1  // left
        // 1 - vertex
        0.5f, -0.5f, 0.0f, // ver2
        0.0f, 1.0f, 0.0f,  // verColor2  // right
        // 2 - vertex
        -0.5f, 0.5f, 0.0f, // ver3
        0.0f, 0.0f, 1.0f,  // verColor3  // top left
        // second tringle
        // 0.5f, -0.5f, 0.0f,  // ver2
        // 0.0f, 1.0f, 0.0f,   // verColor2  // right
        // 3- vertex
        0.5f, 0.5f, 0.0f, // ver3
        0.0f, 0.0f, 1.0f, // verColor3  // top-right
        // -0.5f, 0.5f, 0.0f, // ver 1
        // 0.0f, 0.0f, 1.0f,   // verColor1  // left-top
    };

    std::vector<GLfloat> vertexColors{
        1.0f, 0.0f, 0.0f, // verColor1

        0.0f, 1.0f, 0.0f, // verColor2

        0.0f, 0.0f, 1.0f, // verColor3
    };

    const std::vector<GLuint> indexBufferData{
        2, 0, 1, 3, 2, 1};

    glGenVertexArrays(1, &mesh1->vertexArrayObj);
    glBindVertexArray(mesh1->vertexArrayObj);
    glGenBuffers(1, &mesh1->vertexBufferObj);
    glBindBuffer(GL_ARRAY_BUFFER, mesh1->vertexBufferObj);
    glBufferData(GL_ARRAY_BUFFER,
                 vertexData.size() * sizeof(GLfloat),
                 vertexData.data(),
                 GL_STATIC_DRAW);

    // create INDEX buffer (quatro)
    glGenBuffers(1, &mesh1->indexBufferObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh1->indexBufferObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indexBufferData.size() * sizeof(GLuint),
                 indexBufferData.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 6, (void *)0);

    // --------------------- colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        false,
        sizeof(GLfloat) * 6,
        (GLvoid *)(sizeof(GLfloat) * 3));

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    //  glDisableVertexAttribArray(1);
};

// void CreateGraphicsPipeline() {};

//////////////

void Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "error" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("OGWindow", 0, 0, width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        exit(1);
        std::cout << "window is none" << std::endl;
    }
    ctxOGL = SDL_GL_CreateContext(window);
    if (ctxOGL == nullptr)
    {
        exit(1);
        std::cout << "no-context" << std::endl;
    }

    // glad init
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        exit(1);
        std::cout << "no glad init" << std::endl;
    }
    // GetOpenGLVersionInfo();
};

void Input()
{
    static float mouseX = width / 2;
    static float mouseY = height / 2;

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            std::cout << "by by!" << std::endl;
            quit = true;
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            camera.MouseLook(mouseX, mouseY);
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        camera.moveForvard(0.1f);
        // g_uOffset += 0.01f;
        // std::cout << "UP" << std::endl;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        camera.moveBack(0.1f);
        // g_uOffset -= 0.01f;
        //  std::cout << "DOWN" << std::endl;
    }
    if (state[SDL_SCANCODE_LEFT])
    {
        camera.moveLeft(0.1f);
        // g_uOffsetYrot -= 0.1f;
        //  std::cout << "DOWN" << std::endl;
    }
    if (state[SDL_SCANCODE_RIGHT])
    {
        camera.moveRight(0.1f);
        // g_uOffsetYrot += 0.1f;
        //  std::cout << "DOWN" << std::endl;
    }
};

void MeshUpdate(Mesh3D* mesh, GLuint pipeline)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   // glUseProgram(gGraphicsPipelineShaderProg);

    // move
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, mesh->uOffset));

    model = glm::rotate(model, glm::radians(mesh->uOffsetYrot), glm::vec3(0.0f, 1.0f, 0.0f));

    model = glm::scale(model, glm::vec3(mesh->uOffsetScale, mesh->uOffsetScale, mesh->uOffsetScale));

    GLint u_ModelMetrixLocation = glGetUniformLocation(pipeline, "u_ModelMatrix");
    glUniformMatrix4fv(u_ModelMetrixLocation, 1, false, &model[0][0]);

    glm::mat4 view = camera.GetViewMatrix();
    GLint viewLocation = glGetUniformLocation(pipeline, "u_ViewMatrix");
    glUniformMatrix4fv(viewLocation, 1, false, &view[0][0]);

    // poection matrix
    glm::mat4 perspective = glm::perspective(
        glm::radians(45.0f),
        (float)width / (float)height,
        0.1f,
        10.0f);
    GLint u_PerspectiveLocation = glGetUniformLocation(pipeline, "u_Perspective");
    glUniformMatrix4fv(u_PerspectiveLocation, 1, false, &perspective[0][0]);
};

void Draw(Mesh3D* mesh, GLuint pipeline)
{
    glBindVertexArray(mesh->vertexArrayObj);
    glUseProgram(pipeline);
    //  glDrawArrays(GL_TRIANGLES, 0, 6);
    // GLCheck(
    glDrawElements(
        GL_TRIANGLES,
        6,
        GL_UNSIGNED_INT,
        0);
    //  );
   
};

void MainLoop()
{
    SDL_WarpMouseInWindow(window, width / 2, height / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    while (!quit)
    {
        Input();
        MeshUpdate(&mesh1, gGraphicsPipelineShaderProg);
        Draw(&mesh1, gGraphicsPipelineShaderProg);
        SDL_GL_SwapWindow(window); // updarte screen
    }
};
void CleanUp()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
};

int main()
{
    /////////////////////////////////////////////  Math

    glm::vec4 point(1.0f, 5.0f, 1.0f, 1.0f);

    glm::mat4 model(1.0f);
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 rotating = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0));
    glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    model = scaling * rotating * move; // => move, rotate, scaling

    glm::vec4 point2 = (model * point);

    std::cout << "new point position ; " << glm::to_string(point2) << std::endl;
    // glm::vec3 A(4.0f, 0.0f, 0.0f);
    // glm::vec3 B(0.0f, 4.0f, 0.0f);

    // glm::vec3 C(glm::cross(A, B));

    // std::cout << "A * B : " << glm::to_string(C) << std::endl;

    // std::cout << glm::length(A) << std::endl;
    // std::cout << "A-norm : " << glm::to_string(glm::normalize(A)) << std::endl;
    // std::cout << "scalar vectors : A, B : " << glm::dot(A,B) << std::endl;
    // double scalAB = glm::dot(glm::normalize(A),glm::normalize(B));
    // double conorAB = std::acos(scalAB) * 180 / M_PI; // deg

    // std::cout << "scalar vectors NORM : A, B : " << scalAB << std::endl;
    // std::cout << "conor : A, B : " << conorAB << std::endl;

    //   glm::mat4 mat(1.0f);
    //     float f = glm::dot(glm::normalize(A), glm::normalize(B));

    //     A.xyz() = B.zyx();

    //    std::cout << glm::to_string(A.yz()) << std::endl;
    //    std::cout << "res : " << f << std::endl;
    //    std::cout << "mat : " << glm::to_string(mat) << std::endl;
    //    std::cout << "A : " << glm::to_string(A) << std::endl;
    //    std::cout << "norm-a : " << glm::to_string(glm::normalize(A)) << std::endl;
    //    std::cout << "norm-b : " << glm::to_string(glm::normalize(B)) << std::endl;
    ////////////////////////////////////
    Initialize();
    MeshCreate(&mesh1);
    createGraphicPipeline();
    MainLoop();
    CleanUp();

    return 0;
}
