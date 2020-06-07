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
	const vec3 gravity = vec3(0.0, -1.0, 0.0);  // TODO(Rok Kos): Change to Uniform
	const float delta_time = 0.000001;  // TODO(Rok Kos): Change to Uniform
	const uint itterations = 1;  // TODO(Rok Kos): Change to Uniform
	const float rest_lenght = 1.0 / 3.0;  // TODO(Rok Kos): Change to Uniform
	
	uint particle_id = gl_GlobalInvocationID.x;

	vec3 position = Positions[particle_id];
	vec3 temp_position = position;
    vec3 prev_position = PreviousPositions[particle_id];

	int is_fixed_position = fixedPoints[particle_id];
	if (is_fixed_position == 1) {
		Positions[particle_id] = temp_position;
		PreviousPositions[particle_id] = temp_position;
	} else {
		vec3 new_position = 2 * position - prev_position + (gravity * delta_time * delta_time);

		mat3 constrains = Constraints[particle_id];
		
		for (uint it = 0; it < itterations; ++it) {
			vec3 displacement = vec3(0.0, 0.0, 0.0);
			float num_contraints = 0.0;
			for (uint x = 0; x < 3; ++x) {
				for (uint y = 0; y < 3; ++y) {
					int constraint_index = int(constrains[x][y]);
					if (constraint_index < 0) {
						continue;
					}

					vec3 other_pos = Positions[constraint_index];

					vec3 delta = new_position -  other_pos;
					float distance = length(delta);
					float diff = (rest_lenght - distance) * 0.5;

					displacement += normalize(delta) * diff;
					num_contraints += 1.0;
				}
			}

			new_position += displacement / num_contraints;
		}



		Positions[particle_id] = new_position;
		PreviousPositions[particle_id] = temp_position;
	}
}