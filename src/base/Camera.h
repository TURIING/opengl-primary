#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * 摄像机移动的方向
 */
enum class CAMERA_DIRECTION {
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FOV = 45.0f;

class Camera {
public:
    Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH);

    Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch);


    glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_position + m_front, m_up); }

    glm::vec3 getPosition() const { return m_position; }

    glm::vec3 getFront() const { return m_front; }

    void processKeyboard(CAMERA_DIRECTION _direction, float _deltaTime);

    /**
     * \brief 处理鼠标移动事件
     * \param _xOffset 
     * \param _yOffset 
     * \param _constrainPitch 
     */
    void processMouseMovement(float _xOffset, float _yOffset, GLboolean _constrainPitch = true);


    /*
     * 处理鼠标滚动事件
     */
    void processMouseScroll(float _yOffset);


    float getFov() const { return m_fov; }

private:
    void updateCameraVector();


private:
    glm::vec3 m_position;
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);                     // 摄像机指向的向量
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;                                                                    // 偏航角
    float m_pitch;                                                                  // 俯仰角

    float m_movementSpeed = SPEED;
    float m_mouseSensitivity = SENSITIVITY;
    float m_fov = FOV;                                                              // 视野（Field of View），定义了我们可以看到场景中多大的范围
};

