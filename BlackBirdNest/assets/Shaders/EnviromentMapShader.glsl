#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    TexCoords = a_Position;
    vec4 pos = u_ViewProjection * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec3 TexCoords;

uniform samplerCube u_EnviromentMap;

void main()
{    
    color = texture(u_EnviromentMap, TexCoords);  // vec4(TexCoords, 1.0);
}