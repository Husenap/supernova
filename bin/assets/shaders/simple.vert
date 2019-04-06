#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject{
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 viewproj;
	mat4 inv_proj;
	mat4 inv_viewproj;
	vec4 time;
	vec4 camera_pos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float time;
layout(location = 2) out vec3 camera_pos;
layout(location = 3) out vec3 fragRay;

void main(){
	gl_Position = vec4(inPosition, 1.0);
	fragTexCoord = inTexCoord;

	camera_pos = ubo.camera_pos.xyz;
	time = ubo.time.x;

	//vec4 ray = ubo.inv_viewproj * vec4(fragTexCoord.xy, 1.0, 1.0);
	//ray /= ray.w;
	//fragRay = (ray.xyz);
	fragRay = vec3(fragTexCoord.xy, -1.0);

	//camera_pos = (ubo.inv_viewproj * vec4(fragTexCoord.xy, 0.0, 1.0)).xyz;
	//fragRay = (ubo.inv_viewproj * vec4(fragTexCoord.xy, 1.0, 1.0)).xyz;
	//fragRay = fragRay - camera_pos;
}
