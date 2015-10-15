uniform vec2 offset;
uniform sampler2D fonttex;

varying vec2 tc;
varying vec4 col;

void main()
{
	vec4 tex = texture2D(fonttex, tc);
	gl_FragColor = vec4(col.xyz, tex.r * col.w);
}


