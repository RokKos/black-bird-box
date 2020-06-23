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

layout(std430, binding=2) buffer Constr
{
	mat4 Constraints[ ];
};

layout(std430, binding=3) buffer fixedPts
{
	vec4 fixedPoints[ ];
};

layout(local_size_variable) in;

layout( location=1 ) uniform vec3 u_Gravity;
layout( location=2 ) uniform float u_DeltaTime;
layout( location=3 ) uniform int u_Itterations;


void main()
{
	uint particle_id = gl_GlobalInvocationID.x;

	vec4 position = Positions[particle_id];
	vec4 temp_position = position;
    vec4 prev_position = PreviousPositions[particle_id];

	int is_fixed_position = int(fixedPoints[particle_id].x);
	if (is_fixed_position == 1) {
		Positions[particle_id] = temp_position;
		PreviousPositions[particle_id] = temp_position;
	} else {
		vec4 new_position = 2 * position - prev_position + vec4(u_Gravity * u_DeltaTime * u_DeltaTime, 0.0);

		mat4 constrains = Constraints[particle_id];
		float horizontal_rest_lenght = fixedPoints[particle_id].y;
		float diagonal_rest_length = fixedPoints[particle_id].z;
		
		for (uint it = 0; it < u_Itterations; ++it) {
			vec4 displacement = vec4(0.0);
			float num_contraints = 0.0;
			for (int x = 0; x < 3; ++x) {
				for (int y = 0; y < 3; ++y) {
					int constraint_index = int(constrains[x][y]);
					if (constraint_index < 0) {
						continue;
					}

					uint is_diagonal = abs((x - 1) * (y - 1));
					float rest_lenght = mix(horizontal_rest_lenght, diagonal_rest_length, is_diagonal);

					vec4 other_pos = Positions[constraint_index];

					vec4 delta = new_position -  other_pos;
					float distance = length(delta);
					float diff = (rest_lenght - distance) * 0.5;
					vec4 normalized_delta = normalize(delta);
					normalized_delta *= diff;

					displacement += normalized_delta;
					num_contraints += 1.0;
				}
			}

			new_position += displacement / num_contraints;
		}



		Positions[particle_id] = new_position;
		PreviousPositions[particle_id] = temp_position;
	}
}