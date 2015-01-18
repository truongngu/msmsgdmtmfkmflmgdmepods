precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_uv;
void main()
{
	//vec4 tex=texture2D(u_texture, v_uv);
	//tex.a=0.5;
	//gl_FragColor = vec4(tex.rgb, tex.a);
	gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
