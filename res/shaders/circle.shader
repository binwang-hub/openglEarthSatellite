//
//  circle.shader
//  openglEarthSatellite
//
//  Created by Wang Bin on 2021/7/10.
//
#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Color = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0)\n;
}


#shader fragment
#version 330 core
out vec4 FragColor;
in vec3 Color;

void main()
{
    FragColor = vec4(Color, 1.0);
}

