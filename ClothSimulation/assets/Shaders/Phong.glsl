#type vertex
#version 120

attribute vec4 aPos;
attribute vec3 aNor;
attribute vec2 aTex;
uniform mat4 P;
uniform mat4 MV;
varying vec3 vPos;
varying vec3 vNor;
varying vec2 vTex;

void main()
{
	vec4 posCam = MV * aPos;
	gl_Position = P * posCam;
	vPos = posCam.xyz;
	vNor = (MV * vec4(aNor, 0.0)).xyz;
	vTex = aTex;
}

#type fragment
#version 120

varying vec3 vPos; // in camera space
varying vec3 vNor; // in camera space
varying vec2 vTex;
uniform vec3 kdFront;
uniform vec3 kdBack;

void main()
{
	vec3 lightPos = vec3(0.0, 0.0, 0.0);
	vec3 n = normalize(vNor);
	vec3 l = normalize(lightPos - vPos);
	vec3 v = -normalize(vPos);
	vec3 h = normalize(l + v);
	vec3 kd = kdFront;
	float ln = dot(l, n);
	if(ln < 0.0) {
		kd = kdBack;
		ln = -ln;
	}
	vec3 diffuse = ln * kd;
	vec3 color = diffuse;
	gl_FragColor = vec4(color, 1.0);
}
