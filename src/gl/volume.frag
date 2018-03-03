/*
	Volume fragment shader
*/

#version 330 core

uniform sampler3D volume;

in vec4 origin;
in vec3 dir;

out vec3 color;

void main()
{
	/*
		Perform ray/volume intersection test
	*/
	
	//bounding box
	vec3 bmin = vec3(0,0,0);
	vec3 bmax = vec3(1,1,1);
	
	vec3 invDir = 1.0f / normalize(dir);
	
	//intersection vectors with planes
	vec3 tmin = (bmin - origin.xyz) * invDir;
	vec3 tmax = (bmax - origin.xyz) * invDir;
    
	vec3 tnear = min(tmin, tmax);
	vec3 tfar =  max(tmin, tmax);
    
	//entry point
	float t0 = max(max(tnear.x, 0.0f), max(tnear.y, tnear.z));
	//exit point
	float t1 = min(tfar.x, min(tfar.y, tfar.z));
    
	bool intersects = (t1 > 0.0f) && (t0 < t1);
	
	/*
		Trace between intersection points
	*/
	
	vec3 start = origin.xyz + (normalize(dir) * t0);
	vec3 end   = origin.xyz + (normalize(dir) * t1);
	
	const float stepSize = (1.0f / 300);
	vec3 step = normalize(dir) * stepSize;

	vec3 val = vec3(0,0,0);
	
	if (intersects)
	{
		vec3 cur = start;
		
		while(distance(cur, end) > stepSize)
		{
			val = max(val, texture(volume, cur).rrr);
			cur += step;
		}
	}

	color = val;
}