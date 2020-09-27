#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_ModelPosition;

void main()
{
	v_Normal = a_Normal;
	v_ModelPosition = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_ModelPosition;

uniform sampler2D u_Texture;

uniform vec3 u_CameraPosition;
uniform vec3 u_DiffuseColor;
uniform vec3 u_SpecularColor;
uniform float u_SpecularScatering;
uniform vec3 u_AmbientColor;
uniform vec3 u_AmbientIntensity;
uniform vec3 u_LightPosition;
uniform vec3 u_LightDirection;
uniform vec3 u_LightIntensity;
uniform vec3 u_LightColor;

void main()
{
	vec3 light_color = u_AmbientColor * u_AmbientIntensity;
	vec3 Normal = normalize(v_Normal);
	vec3 LightDirection = normalize(u_LightPosition - v_ModelPosition);
	float diff = max(dot(Normal, LightDirection), 0.0);
	light_color += u_LightIntensity * u_LightColor * u_DiffuseColor * diff;

	vec3 ViewDirection = normalize(u_CameraPosition - v_ModelPosition);
	
	vec3 ReflectionDirection = reflect(-LightDirection, Normal);  
	float specular = pow(max(dot(ViewDirection, ReflectionDirection), 0.0f), u_SpecularScatering);
	light_color += u_LightIntensity * u_LightColor * u_SpecularColor * specular;

	color = vec4(light_color, 1.0f); 
}