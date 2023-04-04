#version 330
uniform samplerCube skyCube;
uniform sampler2D ImageSample;

smooth in vec3 eyeDirection;
in vec3 pos_eye;
in vec3 n_eye;

out vec4 fragmentColor;
flat in int EnvMapping;

uniform mat4 view;

void main() {
    if(EnvMapping == 0) {
        fragmentColor = texture(skyCube, -eyeDirection);    
    } else {
        vec3 incident_eye = normalize (pos_eye);
        vec3 normal = normalize (n_eye);

        vec3 reflected = reflect (incident_eye, normal);
        reflected = vec3 (inverse (view) * vec4 (reflected, 0.0));
        fragmentColor = texture(skyCube, reflected);
    }
    
}