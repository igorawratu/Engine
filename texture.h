#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

#include <vector>
#include <memory>

enum TextureCacheOption{DELETE_ON_GPU_TRANSFER, CACHE};
enum TextureFilterOption{BILINEAR, TRILINEAR, ANISOTROPIC};
enum TextureWrapOption{REPEAT, CLAMP, MIRROR};

struct TextureOptions{
    TextureCacheOption cache_option;
    TextureFilterOption filter_type;
    TextureWrapOption wrap_type;
    float anisotropy_amount;
    int channels;

    TextureOptions() : cache_option(DELETE_ON_GPU_TRANSFER), filter_type(ANISOTROPIC), wrap_type{CLAMP},
                        anisotropy_amount(2.f), channels(4){
    }
};

class Texture
{
friend class TextureManager;
private:
    std::uint32_t id_;
    GLuint texture_name_;
    std::vector<unsigned int> dimensions_;
    std::unique_ptr<char[]> texture_data_;
    TextureOptions texture_options_;

private:
    Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, TextureOptions texture_options);
    Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, int h, TextureOptions texture_options);
    Texture(std::uint32_t id, std::unique_ptr<char[]>&& texture_dat, int w, int h, int d, TextureOptions texture_options);

    void loadTexture(int w);
    void loadTexture(int w, int h);
    void loadTexture(int w, int h, int d);

    GLenum textureWrapOption(TextureWrapOption wrap_type);
    GLenum textureFormat(int channels);

public:
    //disallow external construction
    Texture() = delete;
    Texture(const Texture& other) = delete;
    Texture& operator = (const Texture& other) = delete;
    ~Texture();

    /**
     * @brief Gets name of texture. The texture is sent to opengl the first time this is actually called, as opposed to on construction
     * @return name of texture
     */
    GLuint getTextureName();

    /**
     * @brief Gets ID of texture
     * @return id of texture
     */
    std::uint32_t getID();

    /**
     * @brief Gets the dimension data of the texture. 1D textures will be a vector of size 1, and so on.
     * @return dimension data of texture
     */
    std::vector<unsigned int> getDimensions();

    /**
     * @brief Gets the texture options which were set on construction. Options include caching option, filter type, wrapping type, number of channels,
     * and anisotropy amount
     * @return options of texture
     */
    TextureOptions getOptions();

    /**
     * @brief Gets raw texture data. In the case that the cache option DELETE_ON_GPU_TRANSFER is set, and that the texture has already been transfered to the GPU,
     * nullptr will be returned instead.
     * @return pointer to the first element of the character array containing the raw texture data
     */
    char* getRawTexDat();
};

#endif // TEXTURE_H
