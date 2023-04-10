
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

//Light Source
uniform vec3 lightPos;

// texture
uniform sampler2D texture1;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    //FragColor = texture(texture1, TexCoord);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0,1.0,1.0);

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0,1.0,1.0);

    vec3 result = (ambient + diffuse);
    FragColor = texture(texture1, TexCoord) * vec4(result, 1.0);
    //FragColor = vec4(1.0,0.5,0.5,1.0);
}