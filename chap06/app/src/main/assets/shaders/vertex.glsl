attribute vec3 position;
attribute vec2 texCoord;
varying vec2 textureCoord;
uniform mat4 projection;
uniform mat4 modelView;

void main(){
	gl_Position = projection * modelView * vec4(position, 1.0);
	textureCoord = texCoord;
}