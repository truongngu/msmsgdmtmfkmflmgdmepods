attribute vec4 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv; 
void main(void) {
  gl_Position = vec4(a_posL.xy, 0, 1);
  v_uv=a_posL.zw;
}