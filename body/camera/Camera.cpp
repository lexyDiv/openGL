#include "Camera.h"

Camera::Camera()
{
    this->eye = glm::vec3(0.0f, 0.0f, 0.0f);
    this->viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    this->upVector = glm::vec3(0.0f, 1.0f, 0.0f);
};

Camera::~Camera() {
};

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->eye, this->eye + this->viewDirection, this->upVector);
}
void Camera::MouseLook(int mouseX, int mouseY)
{
    // std::cout << "X : " << mouseX << " Y : " << mouseY << std::endl;

    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);

    static bool firstLook = true;
    if (firstLook) {
        firstLook = false;
        this->oldMousePosition = currentMouse;
    }

    glm::vec2 deltaMouse = this->oldMousePosition - currentMouse;
    this->viewDirection = glm::rotate(
        this->viewDirection, glm::radians((float)deltaMouse.x),
        this->upVector);
    this->viewDirection = glm::rotateX(this->viewDirection, glm::radians((float)deltaMouse.y));    
    this->oldMousePosition = currentMouse;
}

void Camera::moveForvard(float speed)
{
   // this->eye += (this-> viewDirection * speed); // forvard on eye line
   this->eye.z -= speed;
}

void Camera::moveBack(float speed)
{
  //  this->eye -= (this-> viewDirection * speed);
  this->eye.z += speed;
}

void Camera::moveUp(float speed)
{
}

void Camera::moveDown(float speed)
{
}

void Camera::moveLeft(float speed)
{
   glm::vec3 rightVector = glm::cross(this->viewDirection, this->upVector);
    this->eye -= rightVector * speed;
};

void Camera::moveRight(float speed)
{
    glm::vec3 rightVector = glm::cross(this->viewDirection, this->upVector);
    this->eye += rightVector * speed;
}
