// Std. Includes
#include <iostream>
#include <map>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <GL/glut.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Skybox.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
#include "Itens.h"
#include "GameTime.h"
#include "Sound.h"

// Properties
GLuint screenWidth = 1280, screenHeight = 720;
GLfloat theta = 0;
GLfloat alpha = 0;
bool enableKey = true;
bool enableExitKey = false;
glm::vec3 lightPos(0.0f, 3.6f, 0.0f);

GLfloat colorOffsetR = 1.0f;
GLfloat colorOffsetG = 1.0f;
GLfloat colorOffsetB = 1.0f;

glm::vec3 lightColor(colorOffsetR, colorOffsetG, colorOffsetB);

// MISSIONS
std::vector<Itens> level;

// GAME MISSIONS
int mission= 0;
int difficulty=1;
bool chooseDifficulty = false;
// GAME DIALOG BEGIN
bool showMessageStart = true;
bool canInteractStart = false;
// START GAME
bool showGame = false;
bool showGameOver= false;
// DIALOG WITH TADEU
bool showDialogTadeu = false;
bool showFinishMission = false;
bool showCollect = false;
bool showCollectOk = false;
bool positionMission = false;

// GAME TIME
bool runtime= true;
int timeGame;
float positionBoatZ = -0.83f;
float positionBoatY = -5.0f;
GameTime gameTime;
std::string showTime;

// GAME MUSIC
Sound music;

/// Holds all state information relevant to a character as loaded using FreeType
std::string message;
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;
GLuint VAO, VBO;


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

// Camera
glm::vec3 cameraBegin(5.7f,0.81f,2.0f);
Camera camera(cameraBegin);
bool keys[1024];
bool firstMouse = true;
GLfloat lastX, lastY;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int KEY = -1;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
// Initializing Window
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Island Life - The Game", nullptr, nullptr); // Windowed

    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Skybox
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
};

// Shaders - - - -
    // CREATE MISSIONS
    Itens tadeu(glm::vec3(0.0f, -2.5f, 2.0f));
    Itens madeira(glm::vec3(20.0f, -0.7f, -23.0f));
    Itens corda(glm::vec3(24.5f, -1.0f, -6.5f));
    Itens alimentos(glm::vec3(28.0f, 0.8f, -30.0f));
    Itens barco(glm::vec3(-99.0f,-99.0f,-99.0f));

    level.push_back(tadeu);
    level.push_back(madeira);
    level.push_back(corda);
    level.push_back(alimentos);
    level.push_back(barco);

    testColision.addObject(glm::vec3(0.0f, -2.5f, 2.0f));
    testColision.addObject(glm::vec3(0.81f, -2.7f, -3.7f));

    testColision.start();

    music.setMusic(music.begin, 31);

    // Setup and compile our shaders
    Shader lightingShader("resources/shaders/lighting.vs", "resources/shaders/lighting.frag");
    Shader lampShader("resources/shaders/lamp.vs", "resources/shaders/lamp.frag");
    Shader cubeShader("resources/shaders/cube.vs", "resources/shaders/cube.frag");
    Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.frag");

    // Compile and setup the shader
    Shader shader("resources/shaders/text.vs", "resources/shaders/text.frag");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "resources/fonts/Ubuntu-B.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

// skybox

// skybox VAO
unsigned int skyboxVAO, skyboxVBO;
glGenVertexArrays(1, &skyboxVAO);
glGenBuffers(1, &skyboxVBO);
glBindVertexArray(skyboxVAO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

// load textures
// -------------
vector<std::string> faces
{
        "resources/skybox/nightsky_rt.tga",
        "resources/skybox/nightsky_lf.tga",
        "resources/skybox/nightsky_up.tga",
        "resources/skybox/nightsky_dn.tga",
        "resources/skybox/nightsky_bk.tga",
        "resources/skybox/nightsky_ft.tga"
};

unsigned int cubemapTexture = loadCubemap(faces);

// shader configuration

skyboxShader.Use();
skyboxShader.setInt("skybox", 0);

// Models - - - -


    while(!chooseDifficulty){

        glfwSetWindowTitle(window, "Inicio");
        glfwPollEvents();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(KEY==GLFW_KEY_ESCAPE){
            glfwTerminate();
            return 0;
        }

       
        RenderText(shader, "Selecione a dificuldade", screenWidth/3-60, screenHeight/2+20, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "1- Facil - 60s", screenWidth/3+10, screenHeight/4+120, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "2- Medio - 40s", screenWidth/3+10, screenHeight/4+80, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(shader, "3- Dificil - 20s", screenWidth/3+10, screenHeight/4+40, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

        if(KEY == GLFW_KEY_1){
            difficulty = 1;
            chooseDifficulty= true;
        }
        if(KEY == GLFW_KEY_2){
            difficulty = 2;
            chooseDifficulty= true;
        }
        if(KEY == GLFW_KEY_3){
            difficulty = 3;
            chooseDifficulty= true;

         }

        if(chooseDifficulty){
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //music.setMusic(music.loading);
            RenderText(shader, "Carregando...", screenWidth/3, screenHeight/2, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f));

        }

        glfwSwapBuffers(window);

    }

    glfwSetWindowTitle(window, "Island Life - The Game");

    // Load Models
    //Model Map("resources/models/prisao.obj");
		Model Statue("resources/models/EasterIslandStatue.obj");
		Model Bonfire("resources/models/bonfire.obj");
    Model Skeleton("resources/models/skeleton.obj");
		Model Wood("resources/models/wood.obj");
    Model Vines("resources/models/vines.obj");
    Model Orange("resources/models/orange.obj");
    Model Pear("resources/models/pear.obj");
    Model Apple("resources/models/apple.obj");
    Model Banana("resources/models/banana.obj");
    Model Island("resources/models/Small_Tropical_Island.obj");
    Model Ocean("resources/models/ocean.obj");
    Model Boat("resources/models/boat.obj");
    //Model Lamp("resources/models/Lamp.obj");
    /*Model Lamp2("resources/models/Lamp.obj");
    Model Lamp3("resources/models/Lamp.obj");
    Model Lamp4("resources/models/Lamp.obj");
    Model Lamp5("resources/models/Lamp.obj");
    Model Lamp6("resources/models/Lamp.obj");*/
    //Model Lamp7("resources/models/Lamp.obj");
        // std::cout << "Debug3" << '\n';
    // Model Bat1("resources/models/Huge Battery.obj");
    // Model Bat2("resources/models/Huge Battery.obj");
    // Model Bat3("resources/models/Huge Battery.obj");
    // Model Bat4("resources/models/Huge Battery.obj");
        // std::cout << "Debug4" << '\n';
    // Model Keycard("resources/models/Keycard.obj");
    // Model KeyExit("resources/models/Keycard.obj");
    // Model Gun("resources/models/The Gun.obj");
    // // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //SET GAME CONFIGURATION
    switch(difficulty){
        case 1:
            timeGame = 60;
            break;
        case 2:
            timeGame = 40;
            break;
        case 3:
            timeGame = 20;
            break;
    }

    gameTime.setTime(timeGame);
    gameTime.startTime();
    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        if(showGameOver){
            glfwPollEvents();

            // Clear the colorbuffer
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            music.setMusic(music.fail, 0);
            RenderText(shader, "VOCE ESTA COM DIFICULDADES?", screenWidth/4, screenHeight/2, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f)); 
            RenderText(shader, "Voce tera mais tempo para sobreviver", screenWidth/3, screenHeight/5, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));       

            if(KEY==GLFW_KEY_ENTER){
                showGameOver = false;
                showMessageStart = true;
                mission = 0;
                camera.Position = cameraBegin;
                for(int i=0; i < level.size(); i++){
                    level.at(i).setVisited(false);
                }
                timeGame+= 15;
                gameTime.setTime(timeGame);
                gameTime.startTime();
            }
            glDepthFunc(GL_LESS); // set depth function back to default

            // Swap the buffers
            glfwSwapBuffers(window);
            continue;
        }
        

        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lampShader.Use();

        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"),  lightColor.x, lightColor.y, lightColor.z);


        lightingShader.Use();

        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.color"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);

        // Transformation matrices
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Draw the loaded model
        //glm::mat4 model_map;
				glm::mat4 model_island;
        glm::mat4 model_ocean;
				glm::mat4 model_statue;
				glm::mat4 model_bonfire;
        glm::mat4 model_skeleton;
				glm::mat4 model_wood;
        glm::mat4 model_vine1;
        glm::mat4 model_vine2;
        glm::mat4 model_apple;
        glm::mat4 model_pear;
        glm::mat4 model_banana;
        glm::mat4 model_orange;
        glm::mat4 model_boat;
        //glm::mat4 model_lamp;
        /*glm::mat4 model_lamp2;
        glm::mat4 model_lamp3;
        glm::mat4 model_lamp4;
        glm::mat4 model_lamp5;
        glm::mat4 model_lamp6;
        glm::mat4 model_lamp7;*/

        // Draw
        /*model_lamp = glm::translate(model_lamp, glm::vec3(0.0f, -2.5f, 2.0f));
        model_lamp = glm::scale(model_lamp, glm::vec3(2.0f, 2.0f, 2.0f));*/
        //model_lamp = glm::rotate(model_lamp, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        

        model_island = glm::translate(model_island, glm::vec3(26.0f, -5.0f, -15.0f));
        model_island = glm::scale(model_island, glm::vec3(0.10f, 0.10f, 0.10f));

        model_ocean = glm::translate(model_ocean, glm::vec3(-125.0f, 2.0f, -26.0f));
        model_ocean = glm::scale(model_ocean, glm::vec3(50.0f, 50.0f, 50.0f));
        model_ocean = glm::rotate(model_ocean, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model_ocean = glm::rotate(model_ocean, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        model_statue = glm::translate(model_statue, glm::vec3(0.0f, -2.5f, 2.0f));
        //model_statue = glm::rotate(model_statue, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_statue = glm::rotate(model_statue, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    		model_bonfire = glm::translate(model_bonfire, glm::vec3(0.81f, -2.7f, -3.7f));
    		model_bonfire = glm::scale(model_bonfire, glm::vec3(0.15f, 0.15f, 0.15f));

        model_skeleton = glm::translate(model_skeleton, glm::vec3(5.0f, -2.6f, -0.91f));
        model_skeleton = glm::scale(model_skeleton, glm::vec3(0.15f, 0.15f, 0.15f));
        model_skeleton = glm::rotate(model_skeleton, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model_skeleton = glm::rotate(model_skeleton, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    		model_wood = glm::translate(model_wood, glm::vec3(20.0f, -0.7f, -23.0f));
    		model_wood = glm::scale(model_wood, glm::vec3(0.10f, 0.10f, 0.10f));

        model_vine1 = glm::translate(model_vine1, glm::vec3(24.5f, -1.0f, -6.5f));
        model_vine1 = glm::rotate(model_vine1, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		    model_vine1 = glm::scale(model_vine1, glm::vec3(0.15f, 0.15f, 0.15f));

        model_vine2 = glm::translate(model_vine2, glm::vec3(24.5f, -1.0f, -6.7f));
        model_vine2 = glm::rotate(model_vine2, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		    model_vine2 = glm::scale(model_vine2, glm::vec3(0.15f, 0.15f, 0.15f));

        model_apple = glm::translate(model_apple, glm::vec3(28.5f, 0.7f, -29.5f));
		    model_apple = glm::scale(model_apple, glm::vec3(0.005f, 0.005f, 0.005f));

        model_pear = glm::translate(model_pear, glm::vec3(28.0f, 0.80f, -29.5f));
		    model_pear = glm::scale(model_pear, glm::vec3(0.10f, 0.10f, 0.10f));

        //model_banana = glm::translate(model_banana, glm::vec3(29.0f, 0.75f, -23.0f));
        model_banana = glm::translate(model_banana, glm::vec3(28.0f, 0.8f, -30.0f));
        model_banana = glm::rotate(model_banana, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model_banana = glm::rotate(model_banana, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		    model_banana = glm::scale(model_banana, glm::vec3(0.07f, 0.07f, 0.07f));

        model_orange = glm::translate(model_orange, glm::vec3(29.0f, 1.0f, -29.0f));
				model_orange = glm::scale(model_orange, glm::vec3(0.15f, 0.15f, 0.15f));

        /*model_lamp2 = glm::translate(model_lamp2, glm::vec3(-16.0f, 0.0f, -5.2f));
        model_lamp2 = glm::rotate(model_lamp2, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        model_lamp3 = glm::translate(model_lamp3, glm::vec3(-25.0f, 0.0f, -26.0f));
        model_lamp3 = glm::rotate(model_lamp3, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        model_lamp4 = glm::translate(model_lamp4, glm::vec3(-30.0f, -0.3f, -7.6f));
        model_lamp4 = glm::rotate(model_lamp4, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        model_lamp5 = glm::translate(model_lamp5, glm::vec3(-33.0f, -0.3f, -3.2f));
        model_lamp5 = glm::rotate(model_lamp5, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        model_lamp6 = glm::translate(model_lamp6, glm::vec3(26.0f, 0.0f, 26.0f));
        model_lamp6 = glm::rotate(model_lamp6, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        model_lamp7 = glm::translate(model_lamp7, glm::vec3(9.0f, 6.8f, -11.0f));
        model_lamp7 = glm::rotate(model_lamp7, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));*/

        /*lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp));
        Lamp.Draw(lampShader);*/

        /*lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp2));
        Lamp2.Draw(lampShader);

        lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp3));
        Lamp3.Draw(lampShader);

        lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp4));
        Lamp4.Draw(lampShader);

        lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp5));
        Lamp5.Draw(lampShader);

        lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp6));
        Lamp6.Draw(lampShader);

        lampShader.Use();
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_lamp7));

        if(showMessage)
            Lamp7.Draw(lampShader);*/

        //RenderText(shader, "Pega ai o vareto!!", screenWidth/2, screenHeight/2, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
        /*lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_map));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Map.Draw(lightingShader);*/

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_statue));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Statue.Draw(lightingShader);

    		lightingShader.Use();
    		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_bonfire));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Bonfire.Draw(lightingShader);



        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_skeleton));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Skeleton.Draw(lightingShader);

    		if(!level.at(1).getVisited()){
            lightingShader.Use();
        	  glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_wood));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Wood.Draw(lightingShader);
        }

        if(!level.at(2).getVisited()){
            lightingShader.Use();
    		    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_vine1));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Vines.Draw(lightingShader);

            lightingShader.Use();
    		    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_vine2));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Vines.Draw(lightingShader);
        }

        if(!level.at(3).getVisited()){
            lightingShader.Use();
    	      glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_apple));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Apple.Draw(lightingShader);

            lightingShader.Use();
    				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_pear));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Pear.Draw(lightingShader);

            lightingShader.Use();
    				glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_banana));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Banana.Draw(lightingShader);

            lightingShader.Use();
            glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_orange));
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
            Orange.Draw(lightingShader);
        }

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_island));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Island.Draw(lightingShader);

        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_ocean));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        Ocean.Draw(lightingShader);

        colorOffsetR = 0.5 * sin(glm::radians(alpha)) + 0.5;
        colorOffsetG = 0.5 * sin(glm::radians(2*alpha)) + 0.5;
        colorOffsetB = 0.5 * sin(glm::radians(4*alpha)) + 0.5;
        //lightColor = glm::vec3(colorOffsetR, colorOffsetG, colorOffsetB);

        if(alpha > 360)
            alpha = 0;
        else
            alpha += 0.05;

        // GAME TIME
        if(!gameTime.isEndGame()){
            if(gameTime.isFinishTime()){
                showGameOver = true;
            }
            else{
                RenderText(shader, gameTime.getTime(), screenWidth/3, screenHeight/1.1, 0.8f, glm::vec3(1.0f, 1.0f, 1.0f)); 
                music.setMusic(music.game, 31);

            }
        }

        // GAME INTRO
        if(showMessageStart){
            if(!canInteractStart){
                message = "Procure o Anciao da ilha";
                RenderText(shader, message, screenWidth/3, screenHeight/4, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else{
                //music.setMusic(music.begin);
                RenderText(shader, "Eu sou o Anciao", screenWidth/4, screenHeight/5+60, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                RenderText(shader, "Voce naufragou, alguns outros ja vieram aqui, mas nao sobreviveram...", screenWidth/4, screenHeight/5+40, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                RenderText(shader, "Irei te ajudar, porem testarei sua bravura!", screenWidth/4, screenHeight/5+20, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                RenderText(shader, "Pressione Y para aceitar!", screenWidth/4, screenHeight/5, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }
        // GAME START
        if(showGame){
            if(showCollect){
                RenderText(shader, "Para coletar, pressione E", screenWidth/3, screenHeight/2, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            if(showCollectOk){
                RenderText(shader, "Coletado com sucesso!", screenWidth/3, screenHeight/2, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
            switch(mission){
                case 1:
                    if(showDialogTadeu){
                        RenderText(shader, "Sabia decisao!", screenWidth/4, screenHeight/5+40, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Vamos construir um pequeno barco.", screenWidth/4, screenHeight/5+20, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Sua primeira tarefa sera procurar e me trazer madeira!", screenWidth/4, screenHeight/5, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                    }

                    break;

                case 2:
                    if(showDialogTadeu){
                        RenderText(shader, "Excelente!", screenWidth/4, screenHeight/5+40, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Preciso de cordas.", screenWidth/4, screenHeight/5+20, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Procure algumas pela ilha e me traga", screenWidth/4, screenHeight/5, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                     }
                    break;

                case 3:
                    if(showDialogTadeu){
                        RenderText(shader, "Quase la!", screenWidth/4, screenHeight/5+40, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Seu barco esta pronto!", screenWidth/4, screenHeight/5+20, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                        RenderText(shader, "Como recompensa, quero que me traga alimentos frescos!", screenWidth/4, screenHeight/5, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
                    }
                    break;

                case 4:
                    gameTime.setEndGame(true);
                    music.setMusic(music.success, 0);
                    RenderText(shader, "Voce venceu!", screenWidth/4, screenHeight/5+40, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                    positionBoatZ = positionBoatZ + 0.01;

                    camera.Position.z = positionBoatZ -0.2;
                    camera.Position.y = 0.5f;
                    camera.Position.x = -11.0f;

                    if(camera.Position.z >= -1 && camera.Position.z < 50){
                        positionBoatY = positionBoatY + 0.0001;
                        camera.Position.y += 0.0001f;
                        camera.Position.z += 0.01f;
                    }

                    model_boat = glm::translate(model_boat, glm::vec3(-13.0f, positionBoatY, positionBoatZ));
                    model_boat = glm::rotate(model_boat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                    lightingShader.Use();
                    glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_boat));
                    glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.25f);
                    glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.3f, 0.4f);
                    glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.35f);
                    glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
                    Boat.Draw(lightingShader);
                
                    break;

                default:

                    break;

            }
        }

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.Use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void Do_Movement()
{
    //GLfloat offsetX = 0.0;//01 * cos(glm::radians(theta));
    GLfloat offsetY = 0.0004 * sin(glm::radians(theta));

    if((keys[GLFW_KEY_W])||(keys[GLFW_KEY_S])||(keys[GLFW_KEY_A])||(keys[GLFW_KEY_D])){
        // LIMIT GAME
        if(camera.Position.x < -17){
            showGameOver = true;
            std::cout<< "GAME OVER!";
        }

        // Camera controls
        if(mission!=4){
            if(keys[GLFW_KEY_W])
                camera.ProcessKeyboard(FORWARD, deltaTime, offsetY);
            if(keys[GLFW_KEY_S])
                camera.ProcessKeyboard(BACKWARD, deltaTime, offsetY);
            if(keys[GLFW_KEY_A])
                camera.ProcessKeyboard(LEFT, deltaTime, offsetY);
            if(keys[GLFW_KEY_D])
                camera.ProcessKeyboard(RIGHT, deltaTime, offsetY);
        }

        // TALK WITH TADEU
        canInteractStart = false;
        showDialogTadeu = false;
        if(level.begin()->isInside(camera.Position)){
            canInteractStart = true;
            showDialogTadeu = true;
            if(level.at(mission).getVisited()){
                mission++;
            }
            std::cout<< "aceitou!";
        }

        // MISSION
        positionMission= false;
        showCollectOk = false;
        if(level.at(mission).isInside(camera.Position) && !level.at(mission).getVisited()){
            positionMission = true;
        }



        if(theta > 360)
            theta = 0;
        else
            theta += 0.8;
    }

    // if((camera.Position.x > 0.5)&&(camera.Position.x < 1.5)&&(camera.Position.z > -1)&&(camera.Position.z < 1)&&(keys[GLFW_KEY_E]))
    //     enableKey = false;
        //
    // if((camera.Position.x > -7.5)&&(camera.Position.x < -5)&&(camera.Position.z > -5)&&(camera.Position.z < -3)&&(keys[GLFW_KEY_E])&&(!enableKey))
    //     enableExitKey = true;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    KEY = key;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(key == GLFW_KEY_Y && action == GLFW_PRESS){
        // Finish mission 1
        showMessageStart = false;
        // Start mission 2
        showGame = true;
        mission++;
        std::cout<<"Missao 0, concluida!";
    }

    if(key == GLFW_KEY_E && action == GLFW_PRESS){
        if(showCollect){
            showCollect= false;
            showCollectOk = true;
            level.at(mission).setVisited(true);
            std::cout<<"Missao concluida!";
        }
        std::cout<<"E";
    }

    if((camera.Position.x > -7.5)&&(camera.Position.x < -5)&&(camera.Position.z > -5)&&(camera.Position.z < -3)&&(keys[GLFW_KEY_E])&&(!enableKey))
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);


    //std::cout<<camera.Front.x<< " "<< camera.Front.y<< " " <<camera.Front.z<<"\n";

    showCollect= false;
    if(level.at(mission).isLooking(camera.Front.y) && positionMission && !level.at(mission).getVisited()){
        showCollect= true;
    }
    if(camera.Front.y>= -0.80)
        showCollectOk = false;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    shader.Use();
    glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
