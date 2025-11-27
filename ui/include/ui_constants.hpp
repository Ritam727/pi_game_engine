#pragma once

#include <array>
#include <string>

namespace ui {
  class Constants {
  public:
    static constexpr inline float SPEED_FAST{0.1f};
    static constexpr inline float SPEED_MEDIUM{0.01f};
    static constexpr inline float SPEED_SLOW{0.001f};
    static constexpr inline float ZERO{0.0f};
    static constexpr inline float ANGLE_MAX{360.0f};

    static constexpr inline std::string DIRECTION{"Direction"};
    static constexpr inline std::string POSITION{"Position"};
    static constexpr inline std::string AMBIENT{"Ambient"};
    static constexpr inline std::string SPECULAR{"Specular"};
    static constexpr inline std::string LINEAR{"Linear"};
    static constexpr inline std::string QUADRATIC{"Quadratic"};
    static constexpr inline std::string INNER_CUT_OFF{"Inner Cut Off"};
    static constexpr inline std::string OUTER_CUT_OFF{"Outer Cut Off"};
    static constexpr inline std::string LIGHT_COMPONENT{"Light"};
    static constexpr inline std::string LIGHT_CONTROLS_LABEL{"Light Controls"};
    static constexpr inline std::string TYPE_LABEL{"Type"};
    static constexpr inline std::string LIGHT_TYPE_DIRECTIONAL{"Directional"};
    static constexpr inline std::string LIGHT_TYPE_POINT{"Point"};
    static constexpr inline std::string LIGHT_TYPE_SPOT{"Spot"};
    static constexpr inline std::string DOCKSPACE{"Dockspace"};

    static constexpr inline std::array<std::string, 3> LIGHT_TYPES{
        LIGHT_TYPE_DIRECTIONAL, LIGHT_TYPE_POINT, LIGHT_TYPE_SPOT};
  };
}