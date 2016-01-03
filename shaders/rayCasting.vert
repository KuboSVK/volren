uniform vec3 scaleFactor;

void main(void) {

	vec4 vertexPos = gl_Vertex;
	vec4 texturePos = gl_MultiTexCoord0;
	
	vertexPos *= vec4(scaleFactor, 1.0);
	texturePos *= vec4(scaleFactor, 1.0);
	
	gl_TexCoord[0] = texturePos;
	gl_Position = gl_ModelViewProjectionMatrix * vertexPos;
    
}