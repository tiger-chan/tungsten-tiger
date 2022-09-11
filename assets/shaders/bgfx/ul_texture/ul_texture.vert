$input a_position, a_texcoord0
$output v_wpos, v_texcoord0

#include <bgfx_shader.sh>

void main() {
	vec4 wpos = vec4(a_position, 1.0);
	v_wpos = a_position;
	gl_Position = wpos;
	v_texcoord0 = a_texcoord0;
}
