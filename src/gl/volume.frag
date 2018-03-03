/*
	Volume fragment shader
*/

#version 330 core

//ray position/direction
in vec4 rorg;
in vec3 rdir;

uniform sampler3D volume;

out vec3 color;

void main()
{
	vec3 cur = rorg.xyz;
	vec3 stop = rorg.xyz + normalize(rdir) * 2;

	float stepSize = (1.0f / 256);
	vec3 step = normalize(rdir) * stepSize;

	vec3 val = vec3(0,0,0);

	while(!(distance(cur, stop) < stepSize))
	{
		val = max(val, texture(volume, cur).rrr);
		cur += step;
	}

	color = val;
}