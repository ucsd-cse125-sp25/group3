#include "minigame.h"
#include "include/stb_image.h"
#include "shader_utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


GLuint quadVAO = 0, quadVBO = 0;
GLuint quadShader = 0;
GLuint characterShader = 0;

MiniGame::MiniGame() 
    : player(nullptr), backgroundTex(0), windowWidth(0), windowHeight(0), finished(false) {}

MiniGame::~MiniGame() {
    cleanup();
}

void MiniGame::init(GLFWwindow* window) {
    // Get framebuffer size (in case window was resized)
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    backgroundTex = loadTexture("../minigame/assets/background.png");
    if (backgroundTex == 0) {
        std::cerr << "[MiniGame] Failed to load background!" << std::endl;
        finished = true;
        return;
    }

    initQuadRenderer();
    characterShader = quadShader;

    int charW, charH, ch;
    unsigned char* data = stbi_load("../minigame/assets/thief1_right.png", &charW, &charH, &ch, 4);
    if (!data) {
        std::cerr << "Failed to load character texture!\n";
        finished = true;
        return;
    }
    stbi_image_free(data);

    float scale = 1.0f / 4.0f;
    float drawW = charW * scale;
    float drawH = charH * scale;
    float posX = windowWidth - drawW;
    float posY = windowHeight - drawH;

    player = new Character(posX, posY, 
        "../minigame/assets/thief1_right.png", 
        "../minigame/assets/thief1_left.png");

    // Add platforms
    platforms.emplace_back(330, 1700, 200, 27, 0);//1
    platforms.emplace_back(0, 1475, 270, 27, 0);//2
    platforms.emplace_back(0, 1595, 360, 32, 0);//3
    platforms.emplace_back(468, 1212, 360, 43, 0);//4
    platforms.emplace_back(1205, 1060, 520, 62, 0);//5
    platforms.emplace_back(1890, 1268, 408, 58, 0);//6
    platforms.emplace_back(2570, 930, 320, 55, 0);//7
    platforms.emplace_back(520, 750, 410, 54, 0);//8
    platforms.emplace_back(1400, 555, 480, 55, 0);//9
    platforms.emplace_back(2270, 510, 480, 37, 0);//10
}

void MiniGame::update(GLFWwindow* window) {
    //std::cout << "[MiniGame] rendering frame" << std::endl;
    if (!player) return;

    player->handleInput(window);
    player->update(1.0f / 60.0f, windowHeight, windowWidth, platforms);


    Platform& lastPlatform = platforms.back();

    float px = player->getX();
    float py = player->getY();
    float pw = player->getWidth();
    float ph = player->getHeight();

    float lx = lastPlatform.getX();
    float ly = lastPlatform.getY();
    float lw = lastPlatform.getWidth();

    bool onPlatform = 
    py + ph >= ly && py + ph <= ly + 15 &&  
    px + pw >= lx && px <= lx + lw;

     if (onPlatform) {
        timeOnLastPlatform += 1.0f / 60.0f; 
        if (timeOnLastPlatform >= 2.0f) {
            finished = true;
        }
    } else {
        timeOnLastPlatform = 0.0f;
    }
}

void MiniGame::render() {
    //std::cout << "[MiniGame] rendering frame" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    renderQuad(backgroundTex, 0, 0, windowWidth, windowHeight);

    

    if (player) 
    {
        GLuint shader = player->getShader();
        glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
        GLuint projLoc = glGetUniformLocation(shader, "uProjection");
        glUseProgram(shader);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        player->draw();

    }
    for (const auto& p : platforms)
        p.draw(quadShader);
}

void MiniGame::cleanup() {
    delete player;
    player = nullptr;
    platforms.clear();
    if (backgroundTex) {
        glDeleteTextures(1, &backgroundTex);
        backgroundTex = 0;
    }
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteProgram(quadShader);
}

bool MiniGame::isFinished() const {
    return finished;
}

GLuint MiniGame::loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

void MiniGame::renderQuad(GLuint texture, float x, float y, float width, float height) {
    glUseProgram(quadShader);

    glm::mat4 projection = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(quadShader, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(quadShader, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(quadShader, "backgroundTex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void MiniGame::initQuadRenderer() {
    float quadVertices[] = {
        // positions   // texCoords
        0.f,  0.f,    0.0f, 0.0f,
        1.f,  0.f,    1.0f, 0.0f,
        1.f,  1.f,    1.0f, 1.0f,

        0.f,  0.f,    0.0f, 0.0f,
        1.f,  1.f,    1.0f, 1.0f,
        0.f,  1.f,    0.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint vs = compileShader("minigame/shaders/quad.vert", GL_VERTEX_SHADER);
    GLuint fs = compileShader("minigame/shaders/quad.frag", GL_FRAGMENT_SHADER);
    quadShader = glCreateProgram();
    glAttachShader(quadShader, vs);
    glAttachShader(quadShader, fs);
    glLinkProgram(quadShader);
    glDeleteShader(vs);
    glDeleteShader(fs);
}
