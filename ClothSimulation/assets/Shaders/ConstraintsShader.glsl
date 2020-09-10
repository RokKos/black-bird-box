#type compute
#version 450 core

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_compute_variable_group_size : enable

layout(std430, binding=0) buffer PrevPos
{
	vec4 PreviousPositions[ ];
};

layout(std430, binding=1) buffer Pos
{
	vec4 Positions[ ];
};

layout(std430, binding=2) buffer fixedPts
{
	vec4 fixedPoints[ ];
};

layout(std430, binding=3) buffer btcIds
{
	vec4 batchIds[ ];
};

layout(local_size_variable) in;

layout( location=1 ) uniform vec3 u_Gravity;
layout( location=2 ) uniform float u_DeltaTime;
layout( location=3 ) uniform int u_Itterations;


void main()
{
	vec4 pair_vertex = batchIds[gl_GlobalInvocationID.x];
	uint vertex_a_id_ = uint(pair_vertex.x);
	uint vertex_b_id_ = uint(pair_vertex.y);
    uint is_diagonal = uint(pair_vertex.z);

	vec4 position_a = Positions[vertex_a_id_];
	vec4 position_b = Positions[vertex_b_id_];

	
    float horizontal_rest_lenght = fixedPoints[vertex_a_id_].y;
    float diagonal_rest_length = fixedPoints[vertex_a_id_].z;
    float rest_lenght = mix(horizontal_rest_lenght, diagonal_rest_length, is_diagonal);


    for (uint it = 0; it < u_Itterations; ++it) {
        vec4 delta = position_a -  position_b;
        float distance = length(delta);
        float diff = (rest_lenght - distance) * 0.5;
        vec4 normalized_delta = normalize(delta);
        normalized_delta *= diff;

        int is_a_fixed_position = int(fixedPoints[vertex_a_id_].x);
        if (!(is_a_fixed_position == 1)) {
            position_a += normalized_delta;
        }

        int is_b_fixed_position = int(fixedPoints[vertex_b_id_].x);
        if (!(is_b_fixed_position == 1)) {
            position_b -= normalized_delta;
        }

    }

	Positions[vertex_a_id_] = position_a;
	Positions[vertex_b_id_] = position_b;
	
}