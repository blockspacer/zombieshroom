#version 400

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

layout(std140) uniform PerFrame
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout(std140) uniform PerObject
{
    mat4 modelMatrix;
	mat4 normalMatrix;
};

layout(std140) uniform Lights
{
    vec3 lightDirection;
};

out vec3 vert_normal;
out vec3 vert_lightVec;
out vec3 vert_diffuseColor;

void main()
{
	vert_normal = normalize( normalMatrix * vec4(in_normal, 0.0f)).xyz;

	vec3 intensityDiffuse = vec3(0.7f);

	vert_lightVec = normalize( -lightDirection );
	vert_diffuseColor = max( 0.0f, dot( normalize( vert_lightVec ), vert_normal ) ) * intensityDiffuse;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( in_position, 1.0f );
}