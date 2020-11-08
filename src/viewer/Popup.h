//
// Created by Vladimir A. Kiselev on 06.11.2020.
//

#pragma once

#include <string>

#include <glm/vec2.hpp>

class Popup {
 public:

    static Popup create_circle(glm::vec2 center, float radius, std::string text);
    static Popup create_rect(glm::vec2 top_left, glm::vec2 bottom_right, std::string text);

    bool hit_test(glm::vec2 point) const;

    const char *text() const;

 private:
    Popup() = default;

    bool is_circle_{};
    glm::vec2 center_{};
    float w_{};
    float h_{};
    std::string text_{};
};