attribute vec2 a_uv;
attribute vec3 a_posL;
uniform float u_dx;
uniform float u_dy;
varying vec2 v_uv;
uniform mat4 m_World;
uniform mat4 m_MatrixMVP;

void main()
{
	vec4 posL = m_MatrixMVP*vec4(a_posL, 1.0);
	v_uv.x = a_uv.x + u_dx;
	v_uv.y = a_uv.y - u_dy;
	gl_Position = posL;
}
   