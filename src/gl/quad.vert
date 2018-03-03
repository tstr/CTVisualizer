/*
	Vertex quad shader
*/

#version 330 core

uniform mat4 modelView;
uniform float aspectRatio;
uniform float fov;
uniform float distance;

out vec4 origin;
out vec3 dir;

void main()
{
	vec2 quad[] = vec2[]
	(
		vec2(-1.0, -1.0),
		vec2(1.0, -1.0),
		vec2(-1.0, 1.0),
		vec2(1.0, 1.0)
	);
	
	/*
		Using the pinhole camera model,
		apply a perspective transform.
	*/
	
	//Distance from eye to image plane
	float focalLength = 1.0f / tan(fov / 2);
	
	//Ray origin
	origin = vec4(0,0,distance,1);		 //eye position
	origin = modelView * origin;
	origin = (origin + 1) * 0.5; //translate to 0-1 range
	
	//Ray direction
	dir = vec3(quad[gl_VertexID], -focalLength);
	dir.x *= aspectRatio;		 //scale width by aspect ratio
	dir.y = -dir.y;				 //flip y direction
	dir = mat3(modelView) * dir;
	dir = normalize(dir);
	
    gl_Position = vec4(quad[gl_VertexID], 0.0, 1.0);
}
