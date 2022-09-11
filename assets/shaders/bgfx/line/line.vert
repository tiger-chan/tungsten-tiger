$input a_position, a_texcoord0, a_color0
$output v_wpos, v_texcoord0, v_color0

#include <bgfx_shader.sh>

void main() {
	vec4 wpos = mul(u_modelViewProj, vec4(a_position, 1.0));
	v_wpos = wpos.xyz;
	gl_Position = wpos;
	v_texcoord0 = a_texcoord0;
	v_color0 = a_color0;
}
