/*
	Vertex quad shader
*/

#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//ray position/direction
out vec4 rorg;
out vec3 rdir;

void main()
{
	vec2 quad[] = vec2[]
	(
		vec2(-1.0, -1.0),
		vec2(1.0, -1.0),
		vec2(-1.0, 1.0),
		vec2(1.0, 1.0)
	);

	//center of rotation
	const vec4 center = vec4(0.5, 0.5, 0.5, 0);
	
	//Ray origin
	rorg = vec4((quad[gl_VertexID] + 1.0) / 2.0, 0.0, 1.0);
	rorg.y = 1.0f - rorg.y; //flip y component

	//Ray direction
	rdir = vec3(0, 0, 1);

	//Transform ray origin
	rorg -= center;
	rorg = model * rorg;
	rorg = view  * rorg;
	rorg = proj  * rorg;
	rorg += center;

	//Transform ray direction
	rdir = mat3(model) * rdir;
	rdir = mat3(view)  * rdir;
	rdir = mat3(proj)  * rdir;
	rdir = normalize(rdir);

    gl_Position = vec4(quad[gl_VertexID], 0.0, 1.0);
}
