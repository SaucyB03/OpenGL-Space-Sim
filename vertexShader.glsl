
//Vertex attribute information:
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

//Passing information to fragment shader:
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

//Location + Transformation, Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = vec2(-aTexCoord.x, aTexCoord.y);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}