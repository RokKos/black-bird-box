#type compute
#version 450 core

#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
#extension GL_ARB_compute_variable_group_size : enable

layout(std430, binding=0) buffer Pos
{
	vec3 Positions[ ];
};

layout(std430, binding=1) buffer Vel
{
	vec3 Velocities[ ];
};

layout(std430, binding=1) buffer Color
{
	vec3 Colors[ ];
};

layout(local_size_variable) in;

void main()
{
	const vec3 gravity = vec3(0.0f, -9.8f, 0.0f);
	const float delta_time = 0.01;
	
	uint particle_id = gl_GlobalInvocationID.x;

	vec3 position = Positions[particle_id].xyz;
	vec3 velocity = Velocities[particle_id].xyz;

	vec3 new_position = position + velocity * delta_time + 0.5 * delta_time * delta_time * gravity;
	vec3 new_velocity = velocity + delta_time * gravity;

	Positions[particle_id].xyz = new_position;
	Velocities[particle_id].xyz = new_velocity;



}