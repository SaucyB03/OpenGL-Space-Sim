
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = texture(texture1, TexCoord);
    //FragColor = vec4(1.0,0.5,0.5,1.0);
}