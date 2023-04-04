#version 330

uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;
uniform float Constant;
uniform float Linear;
uniform float Exp;

uniform sampler2D PositionSample;
uniform sampler2D DiffuseSample;
uniform sampler2D NormalSample;
uniform sampler2D AmbientSample;
uniform sampler2D SpecularSample;
uniform sampler2D ShadowSample;
uniform vec2 ScreenSize;

uniform vec3 EyePos;

in vec4 worldLightPosition;

vec3 WorldPos;
vec3 WorldDiffuse;
vec3 WorldNormal;
vec3 WorldAmbient;
vec3 WorldSpecular;

float WorldShininess;

out vec4 FragColor;


vec3 specDiffuse() {

    vec3 lightDirection = normalize(worldLightPosition.xyz - WorldPos);

    float specularIntensity = 0;    
    float lambertian = max(dot(lightDirection, WorldNormal), 0.0);
    
    if(lambertian > 0.0) {
        vec3 viewDir = normalize(EyePos -WorldPos);
        vec3 halfDir = normalize(lightDirection + viewDir);
        float specAngle = max(dot(WorldNormal, halfDir), 0.0);
        specularIntensity = pow(specAngle, WorldShininess);

    }
    
    vec3 specular = specularIntensity * (WorldSpecular * LightSpecular);
    vec3 diffuse = lambertian * (WorldDiffuse * LightDiffuse);
    vec3 ambient = (WorldAmbient * LightAmbient);

    return specular + diffuse + ambient;
}


vec3 calcPointLight(float shadowFactor) {
    vec3 lightDirection = worldLightPosition.xyz - WorldPos;

    float distance = length(lightDirection); 

    vec3 color = specDiffuse();
    
    float attenuation = Constant +
                        Linear +
                        Exp * distance * distance ;
    
    color = shadowFactor * color / max(1.0, attenuation * distance * distance);
   
    if(distance < 0.15) {
        color = LightDiffuse;
    }

    return ( color);

}

vec3 calcDirLight(float shadowFactor) {
    vec3 color = specDiffuse() ;
    return color;
}


void main()
{

    vec2 worldTexCoord = gl_FragCoord.xy / ScreenSize;
    WorldPos = texture(PositionSample, worldTexCoord).xyz;
    WorldDiffuse = texture(DiffuseSample, worldTexCoord).xyz;
    
    WorldNormal = texture(NormalSample, worldTexCoord).xyz;
    WorldAmbient = texture(AmbientSample, worldTexCoord).xyz;
    WorldSpecular = texture(SpecularSample, worldTexCoord).xyz;
    WorldShininess = texture(SpecularSample, worldTexCoord).w;
    float shadowFactor = texture(ShadowSample, worldTexCoord).x;

    if(worldLightPosition.a != 0.0) {
        FragColor = vec4(calcPointLight(shadowFactor), 1.0);
    } else {
        FragColor = vec4(calcDirLight(shadowFactor), 1.0);
    }

}