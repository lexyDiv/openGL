#pragma once

#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#define GLM_SWIZZLE_STQP

#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>                  // glm::vec3
#include <glm/vec4.hpp>                  // glm::vec4
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/ext/matrix_transform.hpp>  // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp>  // glm::pi

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera
{
public:
    Camera();
    ~Camera();
    glm::mat4 GetViewMatrix();

    void MouseLook(int mouseX, int mouseY);

    void moveForvard(float speed);
    void moveBack(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);

private:
  glm::vec3 eye;
  glm::vec3 viewDirection;
  glm::vec3 upVector;

  glm::vec2 oldMousePosition;

};
