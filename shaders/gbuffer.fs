#version 330

in vec2 fragTexCoord;
in vec3 pointNormal;
in vec3 worldPos;
in vec3 worldEyePos;

uniform vec3 Ambient, Diffuse, Specular;
uniform float Shininess;
uniform int UseMaterial;
uniform int UseTexture;
uniform sampler2D Texture[NUM_TEX];
uniform samplerCube ShadowMap;


layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 AmbientOut;
layout (location = 4) out vec4 SpecularOut;
layout (location = 5) out vec3 ShadowOut;

#define EPSILON 0.5

vec3 ambientTexture() {

  if(UseTexture == 1) 
  {
    return texture2D(Texture[0], fragTexCoord).xyz;    
  } else {
    if(UseMaterial == 1 || UseMaterial == 0)
      return Ambient.xyz;
  }
}

float calcShadowFactor(vec3 lightDirection, float distance) {
  
  float SampledDistance = texture(ShadowMap, lightDirection).r;

  if (distance <= SampledDistance + 0.5) {
    return 1.0;      
  }
  else {
    return 0.5;
  }
}


void main()
{
	WorldPosOut     = worldPos;
	DiffuseOut      = Diffuse.xyz;
	NormalOut       = normalize(pointNormal);
  AmbientOut      = ambientTexture();
  SpecularOut.xyz = Specular.xyz;
  SpecularOut.a   = Shininess;  
  
  vec3 lightDirection =  worldPos - worldEyePos;
  float distance = length(lightDirection);
  float shadowFactor = calcShadowFactor(lightDirection, distance);

  ShadowOut       = vec3(shadowFactor, shadowFactor, shadowFactor);
}
