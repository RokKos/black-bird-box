#type compute
#version 450 core

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_compute_variable_group_size : enable

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
	uvec4 batchIds[ ];
};

layout(local_size_variable) in;

layout( location=3 ) uniform int u_Iterations;
layout( location=4 ) uniform float u_Horizontal_Vertical_Rest_Lenght;
layout( location=5 ) uniform float u_Diagonal_Rest_Lenght;
layout( location=6 ) uniform float u_Bend_Lenght;
layout( location=7 ) uniform float u_Structural_Stiffness;
layout( location=8 ) uniform float u_Shear_Stiffness;
layout( location=9 ) uniform float u_Flexion_Stiffness;

void main()
{
	uvec4 pair_vertex = batchIds[gl_GlobalInvocationID.x];
	uint vertex_a_id_ = pair_vertex.x;
	uint vertex_b_id_ = pair_vertex.y;
    float is_diagonal = float(pair_vertex.z);
    //float is_flexion = pair_vertex.w;
    
	vec4 position_a = Positions[vertex_a_id_];
    vec4 fixed_position_a = fixedPoints[vertex_a_id_];
	vec4 position_b = Positions[vertex_b_id_];
    vec4 fixed_position_b = fixedPoints[vertex_b_id_];

    float rest_lenght = mix(u_Horizontal_Vertical_Rest_Lenght, u_Diagonal_Rest_Lenght, is_diagonal);//, u_Bend_Lenght, is_flexion);
    float stiffness = mix(u_Structural_Stiffness, u_Shear_Stiffness, is_diagonal);//, u_Flexion_Stiffness, is_flexion);

    for (uint it = 0; it < u_Iterations; ++it) {
        vec4 delta = position_a -  position_b;
        float distance = length(delta);
        float diff = (rest_lenght - distance) * 0.5;
        vec4 normalized_delta = normalize(delta);
        vec4 force_spring_offset = stiffness * diff * normalized_delta;

        position_a += force_spring_offset;
        position_b -= force_spring_offset;
    }

    position_a = mix(position_a, vec4(fixed_position_a.xyz, 0.0), fixed_position_a.w);
    position_b = mix(position_b, vec4(fixed_position_b.xyz, 0.0), fixed_position_b.w);
	Positions[vertex_a_id_] = position_a;
	Positions[vertex_b_id_] = position_b;
	
}