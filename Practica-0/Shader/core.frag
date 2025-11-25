// En core.frag
#version 330 core
uniform vec3 colorUniform; // Variable para recibir el color RGB
out vec4 color;

void main()
{
    color = vec4(colorUniform, 1.0f); // Asignar el color recibido
}