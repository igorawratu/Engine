#include "texture.h"

GLenum Texture::textureWrapOption(TextureWrapOption wrap_type){
    switch(wrap_type){
        case REPEAT: return GL_REPEAT;
        case CLAMP: return GL_CLAMP_TO_EDGE;
        case MIRROR: return GL_MIRRORED_REPEAT;
        default: return GL_REPEAT;
    }
}

GLenum Texture::textureFormat(int channels){
    switch(channels){
        case 1: return GL_R;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RGBA;
    }
}

Texture::Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, TextureOptions texture_options) : id_(id), texture_name_(0), dimensions_(1),
                                                                                                                   texture_data_(std::move(texture_dat)),
                                                                                                                   texture_options_(texture_options){
    assert(texture_data_ != nullptr);

    dimensions_[0] = w;
}

Texture::Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, int h, TextureOptions texture_options) : id_(id), texture_name_(0), dimensions_(2),
                                                                                                                          texture_data_(std::move(texture_dat)),
                                                                                                                          texture_options_(texture_options){
    assert(texture_data_ != nullptr);

    dimensions_[0] = w;
    dimensions_[1] = h;
}

Texture::Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, int h, int d, TextureOptions texture_options) : id_(id), texture_name_(0), dimensions_(3),
                                                                                                                                 texture_data_(std::move(texture_dat)),
                                                                                                                                 texture_options_(texture_options){
    assert(texture_data_ != nullptr);

    dimensions_[0] = w;
    dimensions_[1] = h;
    dimensions_[2] = d;
}

void Texture::loadTexture(int w){
    assert(texture_data_ != nullptr);

    glGenTextures(1, &texture_name_);
    glBindTexture(GL_TEXTURE_1D, texture_name_);

    auto filter_option = texture_options_.filter_type;

    switch(filter_option){
        case BILINEAR:
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TRILINEAR:
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default:
            break;
    }

    GLenum wrap_mode = textureWrapOption(texture_options_.wrap_type);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap_mode);

    GLenum format = textureFormat(texture_options_.channels);

    glTexImage1D(GL_TEXTURE_1D, 0, format, (GLsizei)w, 0, format, GL_UNSIGNED_BYTE, texture_data_.get());

    glGenerateMipmap(GL_TEXTURE_1D);

    glBindTexture(GL_TEXTURE_1D, 0);

    if(texture_options_.cache_option = DELETE_ON_GPU_TRANSFER){
        texture_data_ = nullptr;
    }
}

void Texture::loadTexture(int w, int h){
    assert(texture_data_ != nullptr);

    glGenTextures(1, &texture_name_);
    glBindTexture(GL_TEXTURE_2D, texture_name_);

    auto filter_option = texture_options_.filter_type;

    switch(filter_option){
        case BILINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TRILINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        case ANISOTROPIC:
            assert(texture_options_.anisotropy_amount > 0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            float max_aniso;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
            float aniso = std::min(max_aniso, texture_options_.anisotropy_amount);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
            break;
    }

    GLenum wrap_mode = textureWrapOption(texture_options_.wrap_type);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);

    GLenum format = textureFormat(texture_options_.channels);

    glTexImage2D(GL_TEXTURE_2D, 0, format, (GLsizei)w, (GLsizei)h, 0, format, GL_UNSIGNED_BYTE, texture_data_.get());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if(texture_options_.cache_option = DELETE_ON_GPU_TRANSFER){
        texture_data_ = nullptr;
    }
}

void Texture::loadTexture(int w, int h, int d){
    assert(texture_data_ != nullptr);

    glGenTextures(1, &texture_name_);
    glBindTexture(GL_TEXTURE_3D, texture_name_);

    auto filter_option = texture_options_.filter_type;

    switch(filter_option){
        case BILINEAR:
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TRILINEAR:
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default:
            break;
    }

    GLenum wrap_mode = textureWrapOption(texture_options_.wrap_type);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_mode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_mode);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap_mode);

    GLenum format = textureFormat(texture_options_.channels);

    glTexImage3D(GL_TEXTURE_3D, 0, format, (GLsizei)w, (GLsizei)h, (GLsizei)d, 0, format, GL_UNSIGNED_BYTE, texture_data_.get());

    glGenerateMipmap(GL_TEXTURE_3D);

    glBindTexture(GL_TEXTURE_3D, 0);

    if(texture_options_.cache_option = DELETE_ON_GPU_TRANSFER){
        texture_data_ = nullptr;
    }
}

Texture::~Texture(){
    if(texture_name_ != 0){
        glDeleteTextures(1, &texture_name_);
    }
}

GLuint Texture::getTextureName(){
    if(texture_name_ == 0 && texture_data_ != nullptr){
        if(dimensions_.size() == 1){
            loadTexture(dimensions_[0]);
        }
        else if(dimensions_.size() == 2){
            loadTexture(dimensions_[0], dimensions_[1]);
        }
        else if(dimensions_.size() == 3){
            loadTexture(dimensions_[0], dimensions_[1], dimensions_[2]);
        }
    }

    return texture_name_;
}

std::uint32_t Texture::getID(){
    return id_;
}

std::vector<unsigned int> Texture::getDimensions(){
    return dimensions_;
}

TextureOptions Texture::getOptions(){
    return texture_options_;
}

char* Texture::getRawTexDat(){
    return texture_data_.get();
}
