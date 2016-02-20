#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include <memory>

class Material
{
protected:
    int position_location_;
    int texcoord_location_;
    int colour_location_;
    int normal_location_;

    Shader* shader_;

public:
    Material() : position_location_(-1), texcoord_location_(-1), colour_location_(-1), normal_location_(-1), shader_(nullptr){

    }

    Material(const Material& other) = default;
    Material& operator = (const Material& other) = default;
    ~Material(){}

    int getPositionLocation(){
        return position_location_;
    }

    int getTexcoordLocation(){
        return texcoord_location_;
    }

    int getColourLocation(){
        return colour_location_;
    }

    int getNormalLocation(){
        return normal_location_;
    }

    Shader* getShader();

    std::string getMaterialName() = 0;

    std::unique_ptr<Material> clone() = 0;
};

#endif // MATERIAL_H
