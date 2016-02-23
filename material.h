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
    int model_mat_loc_;
    int view_mat_loc_;
    int proj_mat_loc_;

    Shader* shader_;

public:
    Material() : position_location_(-1), texcoord_location_(-1), colour_location_(-1), normal_location_(-1), shader_(nullptr),
                 model_mat_loc_(-1), view_mat_loc_(-1), proj_mat_loc_(-1){

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
     * @brief Gets location of model matrix
     * @return location of model matrix, or -1 if there is none
     */
    int getModelMatLocation(){
        return model_mat_loc_;
    }

    /**
     * @brief Gets location of view matrix
     * @return  location of view matrix, or -1 if there is none
     */
    int getViewMatLocation(){
        return view_mat_loc_;
    }

    /**
     * @brief Gets location of projection matrix
     * @return location of projection matrix, or -1 if there is none
     */
    int getProjMatLocation(){
        return proj_mat_loc_;
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
