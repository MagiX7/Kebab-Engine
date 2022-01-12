#pragma once

#include "Math/float3.h"

enum class LightType
{
    NONE = 0,
    DIRECTIONAL,
    POINT,
};

class Light
{
public:
    LightType type;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight()
    {
        dir = { 1,0,0 };
        ambient = { 0.25f, 0.25f, 0.25f };
        diffuse = { 0.75f, 0.75f, 0.75f };
        specular = { 1,1,1 };

        type = LightType::DIRECTIONAL;
    }

public:
    float3 dir;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

class PointLight : public Light
{
public:
    PointLight()
    {
        position = { 0,0,0 };
        type = LightType::POINT;
    }

public:
    float3 position = { 0,0,0 };

    float constant = 1.0f;
    // Linear
    float lin = 0.09f;
    float quadratic = 0.032f;

    float3 ambient = { 1,0,0 };
    float3 diffuse = { 1,1,1 };
    float3 specular = { 0,0,1 };
};