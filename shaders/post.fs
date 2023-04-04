#version 330

uniform sampler2D ImageSample;
uniform vec2 ScreenSize;
uniform float Sine;

out vec4 FragColor;

void main()
{

    vec2 worldTexCoord = gl_FragCoord.xy / ScreenSize;
    vec4 color = texture(ImageSample, worldTexCoord.xy + worldTexCoord.xy *  Sine);
    FragColor = color;
    if (int(gl_FragCoord.x) % 5 == 0) FragColor *= vec4(0.2, 0.2, 0.2, 1);

 
    
}

