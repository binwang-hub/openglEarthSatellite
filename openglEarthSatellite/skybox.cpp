//
//  skybox.cpp
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/10.
//

#include "skybox.h"


void Skybox::loadTexture(GLenum target, std::string path)
{
    int width, height, nrChannels;
    unsigned char * data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cerr << "ERROR: Cubemap texture failed to load at path: " << path << std::endl;
    }

    stbi_image_free(data);
}
