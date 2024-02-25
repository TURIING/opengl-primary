#include "Camera.h"

Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch) {
    m_position = _position;
    m_worldUp = _up;
    m_yaw = _yaw;
    m_pitch = _pitch;

    updateCameraVector();
}

Camera::Camera(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) {

    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_worldUp = glm::vec3(_upX, _upY, _upZ);
    m_position = glm::vec3(_posX, _posY, _posZ);
    m_yaw = _yaw;
    m_pitch = _pitch;

    updateCameraVector();
}

void Camera::processMouseMovement(float _xOffset, float _yOffset, GLboolean _constrainPitch) {
    _xOffset *= m_mouseSensitivity;
    _yOffset *= m_mouseSensitivity;

    m_yaw += _xOffset;
    m_pitch += _yOffset;

    if (_constrainPitch) {
        if (m_pitch > 89.0f) m_pitch = 89.0f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
    }

    updateCameraVector();
}

void Camera::updateCameraVector() {
    glm::vec3 front;
    front.x = static_cast<float>(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    front.y = static_cast<float>(sin(glm::radians(m_pitch)));
    front.z = static_cast<float>(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::processMouseScroll(float _yOffset) {
    m_fov -= static_cast<float>(_yOffset);
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
}

/*
 * 处理键盘事件
 */
void Camera::processKeyboard(CAMERA_DIRECTION _direction, float _deltaTime) {
    const float velocity = m_movementSpeed * _deltaTime;

    switch (_direction)
    {
    case CAMERA_DIRECTION::FORWARD:     m_position += velocity * m_front;       break;
    case CAMERA_DIRECTION::BACKWARD:    m_position -= velocity * m_front;       break;
    case CAMERA_DIRECTION::LEFT:        m_position -= m_right * velocity;       break;
    case CAMERA_DIRECTION::RIGHT:       m_position += m_right * velocity;       break;
    default:                            break;
    }
}