#type compute
#version 450 core

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_compute_variable_group_size : enable

layout(std430, binding=0) buffer PrevPos
{
	vec3 PreviousPositions[ ];
};

layout(std430, binding=1) buffer Pos
{
	vec3 Positions[ ];
};

layout(std430, binding=2) buffer Constr
{
	mat3 Constraints[ ];
};

layout(std430, binding=3) buffer fixedPts
{
	int fixedPoints[ ];
};

layout(local_size_variable) in;

void main()
{
	const vec3 gravity = vec3(0.0f, -9.8f, 0.0f);
	const float delta_time = 0.01;
	
	uint particle_id = gl_GlobalInvocationID.x;

	vec3 position = Positions[particle_id];
	vec3 temp_position = position;
    vec3 prev_position = PreviousPositions[particle_id];

	mat3 constrains = Constraints[particle_id];
    int is_fixed_position = fixedPoints[particle_id];

	vec3 new_position = 2 * position - prev_position + (gravity * delta_time * delta_time);

	Positions[particle_id] = new_position;
	PreviousPositions[particle_id] = temp_position;
}