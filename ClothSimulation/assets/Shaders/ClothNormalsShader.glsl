#type compute
#version 450 core

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_compute_variable_group_size : enable

layout(std430, binding=1) buffer Pos
{
	vec4 Positions[ ];
};

layout(std430, binding=4) buffer Trian
{
	uvec4 Triangles[ ];
};

layout(std430, binding=5) buffer Normals_
{
	vec4 Normals[ ];
};


layout(local_size_variable) in;


void main()
{
    uint vertex_id = gl_GlobalInvocationID.x;
	uvec4 triangle_vertecies = Triangles[vertex_id];

	vec4 vertex_a = Positions[triangle_vertecies.x];
	vec4 vertex_b = Positions[triangle_vertecies.y];
	vec4 vertex_c = Positions[triangle_vertecies.z];

	vec4 vector1 = vertex_b - vertex_a;
	vec4 vector2 = vertex_c - vertex_a;

	vec4 normal = vec4(normalize(cross(vector1.xyz, vector2.xyz)), 1.0);
    Normals[triangle_vertecies.x] = normal;
    Normals[triangle_vertecies.y] = normal;
    Normals[triangle_vertecies.z] = normal;
}