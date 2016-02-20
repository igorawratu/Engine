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

    /**
     * @brief Gets vertex attribute location of positional vertex data
     * @return location of the positional vertex data, or -1 if there is none
     */
    int getPositionLocation(){
        return position_location_;
    }

    /**
     * @brief Gets vertex attribute location of texture coordinate data
     * @return location of texture coordinate data, or -1 if there is none
     */
    int getTexcoordLocation(){
        return texcoord_location_;
    }

    /**
     * @brief Gets vertex attribute location of colour data
     * @return location of colour data, or -1 if there is none
     */
    int getColourLocation(){
        return colour_location_;
    }

    /**
     * @brief Gets vertex attribute location of normal data
     * @return location of normal data, or -1 if there is none
     */
    int getNormalLocation(){
        return normal_location_;
    }

    /**
     * @brief Gets an observer pointer to the shader associated with the material
     * @return an observer pointer to the shader associated with the material
     */
    Shader* getShader(){
        return shader_;
    }

    /**
     * @brief Gets the lexical name of the material
     * @return the lexical name as a string
     */
    virtual std::string getMaterialName() = 0;

    /**
     * @brief Gets a copy of the current material
     * @return a unique pointer containing a copy of the current material
     */
    virtual std::unique_ptr<Material> clone() = 0;
};

#endif // MATERIAL_H
