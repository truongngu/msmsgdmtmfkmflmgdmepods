precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
uniform vec4 color;
 
void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture2D(u_texture, v_uv).a) * color;
}