#pragma once

#include <vector>
#include <string>

namespace ui {
  class Constants {
  public:
    static constexpr inline float SPEED_FAST{0.1f};
    static constexpr inline float SPEED_MEDIUM{0.01f};
    static constexpr inline float SPEED_SLOW{0.001f};
    static constexpr inline float ZERO{0.0f};
    static constexpr inline float ONE{1.0f};
    static constexpr inline float ANGLE_MIN{-360.0f};
    static constexpr inline float ANGLE_MAX{360.0f};

    static const inline std::string DIRECTION{"Direction"};
    static const inline std::string POSITION{"Position"};
    static const inline std::string AMBIENT{"Ambient"};
    static const inline std::string DIFFUSE{"Diffuse"};
    static const inline std::string SPECULAR{"Specular"};
    static const inline std::string LINEAR{"Linear"};
    static const inline std::string QUADRATIC{"Quadratic"};
    static const inline std::string INNER_CUT_OFF{"Inner Cut Off"};
    static const inline std::string OUTER_CUT_OFF{"Outer Cut Off"};
    static const inline std::string ENTITIES{"Entities"};
    static const inline std::string ENTITY{"Entity"};
    static const inline std::string ADD_ENTITY{"Add Entity"};
    static const inline std::string DELETE{"Delete"};
    static const inline std::string COMPONENTS{"Components"};
    static const inline std::string TRANSFORM_COMPONENT{"Transform"};
    static const inline std::string TRANSLATION{"Translation"};
    static const inline std::string ROTATION{"Rotation"};
    static const inline std::string SCALE{"Scale"};
    static const inline std::string ACTIVE{"Active"};
    static const inline std::string LIGHT_COMPONENT{"Light"};
    static const inline std::string LIGHT_CONTROLS_LABEL{"Light Controls"};
    static const inline std::string TYPE_LABEL{"Type"};
    static const inline std::string LIGHT_TYPE_DIRECTIONAL{"Directional"};
    static const inline std::string LIGHT_TYPE_POINT{"Point"};
    static const inline std::string LIGHT_TYPE_SPOT{"Spot"};
    static const inline std::string CAMERA_TRANSFORM_COMPONENT{
        "Camera Transform"};

    static const inline std::vector<std::string> LIGHT_TYPES{
        LIGHT_TYPE_DIRECTIONAL, LIGHT_TYPE_POINT, LIGHT_TYPE_SPOT};
  };
}