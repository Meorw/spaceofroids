uniform sampler2D Tex;

varying vec2 texcoord;


void main() {

	gl_FragColor = texture2D(Tex, texcoord);
}
