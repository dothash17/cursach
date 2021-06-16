#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// ���������
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// ������
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// ��������
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ���������
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// �������� ������ � ��������� ��������� ���������
float vertices[] = {
    // ����������        // �������           // ���������� ����������
    // ������ �������
   -1.3f, -2.3f, -0.7f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    1.3f, -2.3f, -0.7f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    1.3f,  1.7f, -0.7f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    1.3f,  1.7f, -0.7f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
   -1.3f,  1.7f, -0.7f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   -1.3f, -2.3f, -0.7f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

   // ��������
  -1.3f, -2.3f,  0.7f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
   1.3f, -2.3f,  0.7f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
   1.3f,  1.7f,  0.7f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   1.3f,  1.7f,  0.7f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
  -1.3f,  1.7f,  0.7f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
  -1.3f, -2.3f,  0.7f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

  // �����
  -1.3f,  1.7f,  0.7f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
  -1.3f,  1.7f, -0.7f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  -1.3f, -2.3f, -0.7f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  -1.3f, -2.3f, -0.7f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  -1.3f, -2.3f,  0.7f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  -1.3f,  1.7f,  0.7f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,

  // ������
  1.3f,  1.7f,  0.7f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  1.3f,  1.7f, -0.7f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
  1.3f, -2.3f, -0.7f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  1.3f, -2.3f, -0.7f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  1.3f, -2.3f,  0.7f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
  1.3f,  1.7f,  0.7f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

 // ������
-1.3f, -2.3f, -0.7f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
 1.3f, -2.3f, -0.7f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
 1.3f, -2.3f,  0.7f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
 1.3f, -2.3f,  0.7f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
-1.3f, -2.3f,  0.7f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
-1.3f, -2.3f, -0.7f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

// ������� 
-1.3f,  1.7f, -0.7f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
 1.3f,  1.7f, -0.7f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
 1.3f,  1.7f,  0.7f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
 1.3f,  1.7f,  0.7f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
-1.3f,  1.7f,  0.7f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
-1.3f,  1.7f, -0.7f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
};

float pointLightVertices[] = {
    // ����������        // �������           // ���������� ����������
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

// ���������� ������
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
};

// ���������� �������� ���������� �����
glm::vec3 pointLightPositions[] = {
    glm::vec3(3.0f,  3.0f,  3.5f),
    glm::vec3(2.0f, -3.0f, -3.0f),
    glm::vec3(-2.0f,  3.0f, -3.5f),
    glm::vec3(0.0f,  3.0f, -3.0f)
};

int main()
{
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Paket soka", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // �������� GLFW, ����� �� �������� ��� ������
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: �������� ���� ���������� �� OpenGL-�������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ���������������� ����������� ��������� OpenGL
    glEnable(GL_DEPTH_TEST);

    // �������������� ����� ��������� ���������
    Shader lightingShader("../6.multiple_lights_vs.glsl", "../6.multiple_lights_fs.glsl");
    Shader lightCubeShader("../6.light_cube_vs.glsl", "../6.light_cube_fs.glsl");

    // 1. ����������� VAO (� VBO) ����
    unsigned int VBOs[2], cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(2, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 2. ����������� VAO �����
    unsigned int lightCubeVAO;;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointLightVertices), pointLightVertices, GL_STATIC_DRAW);

    // ��������� ��� �������� ��������� �����, ����� �������� ����������� ������ ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // �������� �������
    stbi_set_flip_vertically_on_load(true);
    unsigned int diffuseMap = loadTexture("../textures/1.jpg");
    unsigned int specularMap = loadTexture("../textures/1.jpg");

    // ������������ ��������
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 2);

    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        // ���������� ����� ������ �� �������� ��� ������� �����
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ��������� �����
        processInput(window);

        // ���������
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ����������, ��� ������������ ������ ������, ��� ����������� uniform-����������/�������_���������
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // �������� �������� ����� �1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.19225f, 0.19225f, 0.19225f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.50754f, 0.50754f, 0.50754f);
        lightingShader.setVec3("pointLights[0].specular", 0.508273f, 0.508273f, 0.508273f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032);

        // �������� �������� ����� �2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.7);
        lightingShader.setFloat("pointLights[1].quadratic", 1.8);

        // �������� �������� ����� �3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.7);
        lightingShader.setFloat("pointLights[2].quadratic", 1.8);

        // �������� �������� ����� �4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.7);
        lightingShader.setFloat("pointLights[3].quadratic", 1.8);

        // ���������
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.19225f, 0.19225f, 0.19225f);
        lightingShader.setVec3("spotLight.diffuse", 0.50754f, 0.50754f, 0.50754f);
        lightingShader.setVec3("spotLight.specular", 0.508273f, 0.508273f, 0.508273f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09);
        lightingShader.setFloat("spotLight.quadratic", 0.032);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // �������������� ����/��������
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // ������� ��������������
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // ���������� ��������� �����
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // ���������� ����� ����������� �����
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // ���������
        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 1; i++)
        {
            // ��������� ������� ������ ��� ������� ������� � �������� � � ������
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // ����� ������������ ������ �����
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // � ������ �� ������������ ������� ����, ������� � ��� ���� �������� ���������� �����
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 1; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // ������� ���
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: ����� ���������� front- � back- �������. ������������ ������� �����/������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(3, VBOs);

    // glfw: ����������, ������������ ���� ���������� ����� GLFW-��c�����
    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ � ������ ����� ����������� ������, ��� �������, �� Retina-��������
    glViewport(0, 0, width, height);
}


// glfw: ������ ���, ����� ������������ ����, ���������� ������ callback-�������
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // �����������, ��� ��� y-���������� ���� ����� �����

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: ������ ���, ����� �������������� �������� ����, ���������� ������ callback-�������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// ������� ��� �������� 2D-�������� �� �����
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}