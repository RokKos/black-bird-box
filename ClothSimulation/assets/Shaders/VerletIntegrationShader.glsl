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

layout(local_size_variable) in;

layout( location=1 ) uniform vec3 u_Gravity;
layout( location=2 ) uniform float u_DeltaTime;
layout( location=3 ) uniform int u_Itterations;


void main()
{
    uint vertex_id = gl_GlobalInvocationID.x;
	vec4 position = Positions[vertex_id];
    vec4 prev_position = PreviousPositions[vertex_id];

    int is_fixed_position = int(fixedPoints[vertex_id].x);
	if (is_fixed_position == 1) {
		Positions[vertex_id] = prev_position;
		PreviousPositions[vertex_id] = prev_position;
	} else {
		vec4 new_position = 2 * position - prev_position + vec4(u_Gravity * u_DeltaTime * u_DeltaTime, 0.0);
        Positions[vertex_id] = new_position;
		PreviousPositions[vertex_id] = position;
    }    		
}