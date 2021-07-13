//
//  skybox.cpp
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/10.
//

#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 projection;
uniform mat4 view;
void main() {
   TexCoords = aPos;
   gl_Position = projection * view * vec4(aPos, 1.0);
}


#shader fragment
#version 410 core

out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;

void main() {
   FragColor = texture(skybox, TexCoords);
}
