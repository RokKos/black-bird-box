#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_Position;
out vec3 v_ModelPosition;

void main()
{
	v_Normal = a_Normal;
	v_TexCoord = a_TexCoord;
    v_Position = a_Position;
	v_ModelPosition = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color_out;
layout(location = 1) out vec4 normal_out;
layout(location = 2) out vec4 tex_coord_out;
layout(location = 3) out vec4 vertex_position_out;
layout(location = 4) out vec4 model_position_out;
layout(location = 5) out vec4 diffuse_color_out;
layout(location = 6) out vec4 specular_color_out;
layout(location = 7) out vec4 light_color_out;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_Position;
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
	vec3 diffuse_color = u_LightIntensity * u_LightColor * u_DiffuseColor * diff;
    light_color += diffuse_color;

	vec3 ViewDirection = normalize(u_CameraPosition - v_ModelPosition);
	
	vec3 ReflectionDirection = reflect(-LightDirection, Normal);  
	float specular = pow(max(dot(ViewDirection, ReflectionDirection), 0.0f), u_SpecularScatering);
	vec3 specular_color = u_LightIntensity * u_LightColor * u_SpecularColor * specular;
    light_color += specular_color;

	vec4 tex_color = texture(u_Texture, v_TexCoord);

	color_out = vec4(light_color + tex_color.rgb, tex_color.a);
	normal_out = vec4(Normal, 1.0);
    tex_coord_out = vec4(v_TexCoord, 0.0, 1.0);
    vertex_position_out = vec4(v_Position, 1.0);
    model_position_out = vec4(v_ModelPosition, 1.0);
    light_color_out = vec4(light_color, 1.0);
    diffuse_color_out = vec4(diffuse_color, 1.0);
    specular_color_out = vec4(specular_color, 1.0);
}