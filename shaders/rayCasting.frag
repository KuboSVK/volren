uniform sampler3D data;
uniform sampler1D transfFunc;
uniform float stepSize;
uniform int iterations;
uniform vec3 volExtMin;
uniform vec3 volExtMax;
uniform float minValue;
uniform float maxValue;

void main(void) {

	float value;
	vec4 src = vec4(0.0);
	vec4 dst = vec4(0.0);
	
	vec3 position = gl_TexCoord[0].xyz;
	vec3 cam = gl_ModelViewMatrixInverse[3];
	vec3 direction = gl_TexCoord[0].xyz - cam;
	direction = normalize(direction);
	
	for (int i = 0; i < iterations; i++) {
	
		value = texture3D(data, position).x;
				
		src = texture1D(transfFunc, value);
		//src = vec4(value);
		
		/*if (value > 0.45)
		{
			dst = vec4(1.0, 1.0, 0.0, 0.0);
			break;
		}*/
		
		src.w *= 0.75;
		src.xyz *= src.w; 
		
		dst = (1.0 - dst.w) * src + dst;
		
		position = position + direction * stepSize;
				
		vec3 v1 = sign(position - volExtMin);
		vec3 v2 = sign(volExtMax - position);
		
		float isInside = dot(v1, v2);
		
		if (isInside < 3.0 || dst.w > 0.95) break;
	}
	
	gl_FragColor = dst;
}