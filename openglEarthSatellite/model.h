#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "mesh.h"

#include <iostream>
#include <vector>
#include <string>

unsigned int TextureFromFile(const char *path, const std::string &directory);

class Model
{
public:
        // Methods
    Model(const char * path);
    void Draw(Shader shader);

private:
    // Model Data
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
        std::string directory;
        
    // Methods
    void loadModel(std::string path);
    void processNode(aiNode * node, const aiScene * scene);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
        
};



#endif
