#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_STEPS 512
#define EPSILON 1e-3
#define FAR_PLANE 100.0
#define SKY_COLOR vec4(0.1, 0.15, 0.2, 1.0)

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float time;
layout(location = 2) in vec3 camera_pos;
layout(location = 3) in vec3 fragRay;

layout(location = 0) out vec4 outColor;

float vmax(in vec2 v)
{
	return max(v.x, v.y);
}
float vmax(in vec3 v)
{
	return max(v.x, max(v.y, v.z));
}
float vmax(in vec4 v)
{
	return max(max(v.x, v.y), max(v.z, v.w));
}
float vmin(in vec2 v)
{
	return min(v.x, v.y);
}
float vmin(in vec3 v)
{
	return min(v.x, min(v.y, v.z));
}
float vmin(in vec4 v)
{
	return min(min(v.x, v.y), min(v.z, v.w));
}

float op_union(in float a, in float b)
{
	return min(a, b);
}
float op_union_round(in float a, in float b, in float r)
{
	vec2 u = (max(vec2(r-a, r-b), vec2(0.0)));
	return max(r, min(a, b)) - length(u);
}

float f_sphere(in vec3 p, float r){
	return length(p) - r;
}
float f_plane(in vec3 p, in vec3 n, in float d){
	return dot(p, n) - d;
}
float f_box(in vec3 p, in vec3 b){
	return vmax(abs(p) - b);
}

float scene_distance(in vec3 p){
	vec3 q = p;

	float d_sphere = f_sphere(q, 0.5);
	float d_box = f_box(q - vec3(0, 0, 0), vec3(0.5));
	float d_plane = f_plane(q, vec3(0,1,0), -0.5);

	float d = op_union_round(d_sphere, d_box, 0.2);
	//d = op_union_round(d, d_plane, 0.2);

	return d_sphere;
}

vec3 calculate_normal(in vec3 p){
	vec2 h = vec2(1e-4, 0.f);

	return normalize(
		vec3(
			scene_distance(p + h.xyy) - scene_distance(p - h.xyy),
			scene_distance(p + h.yxy) - scene_distance(p - h.yxy),
			scene_distance(p + h.yyx) - scene_distance(p - h.yyx)
		)
	);
}

void ray_march(in vec3 pos, in vec3 dir, out int iter, out float t){
	t = 0.0;

	for(int i = 0; i < MAX_STEPS; ++i){
		vec3 p = pos + dir * t;

		float d = abs(scene_distance(p));

		if(d < EPSILON){
			iter = i;
			return;
		}

		t += d;

		if(t >= FAR_PLANE){
			return;
		}
	}
}

vec3 mandelbrot(){
	double scale = pow(2.0, time);

	dvec2 uv = fragTexCoord / 2.0;

	dvec2 center = dvec2(0.2511000019201, 0.6360000798665);
	dvec2 z, c;
	c.x = 1.3333 * (fragTexCoord.x) / scale - center.x;
	c.y = fragTexCoord.y / scale - center.y;

	int i;
	z = c;
	for(i = 0; i < MAX_STEPS; ++i){
		double x = (z.x * z.x - z.y * z.y) + c.x;
		double y = (z.y * z.x + z.x * z.y) + c.y;
		if((x*x+y*y) > 4.0)break;
		z.x = x;
		z.y = y;
	}

	float a = (1.0/MAX_STEPS) * i;
	a = 1.0 - pow(1.0 - 2.0*a, 2.0);
	return vec3(
		pow(a, 0.5),
		pow(a, 1.0),
		pow(a, 3.0)
	);
}

void main(){
	vec3 ray = normalize(fragRay);

	outColor.rgb = mandelbrot();
	//outColor.rgb = abs(ray);
	outColor.a = 1.0;
	return;

	int i;
	float t;
	ray_march(camera_pos, ray, i, t);

	if(t >= FAR_PLANE){
		outColor = SKY_COLOR;
		return;
	}

	vec3 p = ray * t;

	vec3 n = calculate_normal(p);

	float light = dot(normalize(vec3(cos(time), 0.5, sin(time))), n);

	light = max(light, 0.1);

	outColor = vec4(
		abs(n),
		1.0
	);
}
