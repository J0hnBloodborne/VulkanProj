#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GameObject {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::mat4 modelMatrix;

    GameObject(const glm::vec3& pos = glm::vec3(0), const glm::vec3& vel = glm::vec3(0))
        : position(pos), velocity(vel), modelMatrix(glm::mat4(1.0f)) {}
};

inline void updateGameObject(GameObject& obj, float dt) {
    const float gravity = -1.6f; // Moon gravity
    obj.velocity.y += gravity * dt;
    obj.position += obj.velocity * dt;
    // Stop at platform (assuming platform at y=0 and sphere radius = 1)
    if (obj.position.y <= 1.0f) {
        obj.position.y = 1.0f;
        obj.velocity.y = 0.0f;
    }
    obj.modelMatrix = glm::translate(glm::mat4(1.0f), obj.position);
}
