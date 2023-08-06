#version 460 core

uniform float u_time;
uniform vec4 u_color;
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

in vec2 v_uv;

out vec4 color;


void main()
{
    color = vec4(u_color.rgb * ((sin(u_time) + 1.0) * 0.5), 1.0);
    color = mix(color, texture(u_texture0, v_uv), 0.5);
    color = mix(color, texture(u_texture1, v_uv), 0.3);
}
