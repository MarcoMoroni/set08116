#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\projection.hpp>
#include <iostream>

using namespace std;
using namespace glm;

int main()
{



	// 12.1.1 Defining Vectors

	vec2 a(1.0f, 0.0f);
	vec2 b(0.0f, 1.0f);
	vec3 c(1.0f, 0.0f, 0.0f);
	vec3 d(0.0f, 0.0f, 1.0f);
	vec4 e(0, 0, 1, 0);
	vec4 f(0, 1, 0, 0);

	vec3 a1(a, 0);
	vec4 b1(b, 1, 0);
	vec2 c1(c);
	vec4 d1(d, 1);
	vec3 e1(e);
	vec2 f1(f);



	// 12.1.2 Vector Addition and Substraction

	vec2 a2(1.0f, 0.0f);
	vec2 b2(0.0f, 1.0f);
	vec3 c2(1.0f, 0.0f, 0.0f);
	vec3 d2(0.0f, 0.0f, 1.0f);
	vec4 e2(0, 0, 1, 0);
	vec4 f2(0, 1, 0, 0);

	vec2 add1 = a2 + b2;
	vec2 sub1 = a2 - b2;
	vec3 add2 = c2 + d2;
	vec3 sub2 = c2 - d2;
	vec4 add3 = e2 + f2;
	vec4 sub4 = e2 - f2;



	// 12.1.3 Vector Scaling

	vec2 a3(1.0f, 0.0f);
	vec2 b3(0.0f, 1.0f);
	vec3 c3(1.0f, 0.0f, 0.0f);
	vec3 d3(0.0f, 0.0f, 1.0f);
	vec4 e3(0, 0, 1, 0);
	vec4 f3(0, 1, 0, 0);

	vec2 mul1 = 5.0f * a3;
	vec2 div1 = b3 / 2.0f;
	vec3 mul2 = 3.0f * c3;
	vec3 div2 = d3 / 7.0f;
	vec4 mul3 = 4.0f * e3;
	vec4 div3 = f3 / 2.0f;



	// 12.1.4 Length of a Vector

	vec2 a4(1.0f, 0.0f);
	vec3 b4(1.0f, 0.0f, 0.0f);
	vec4 c4(0, 0, 1, 0);

	float l1 = length(a4);
	float l2 = length(b4);
	float l3 = length(c4);



	// 12.1.5 Normalizing a Vector

	vec2 a5(1.0f, 0.0f);
	vec3 b5(1.0f, 0.0f, 0.0f);
	vec4 c5(0, 0, 1, 0);

	vec2 n1 = normalize(a5);
	vec3 n2 = normalize(b5);
	vec4 n3 = normalize(c5);



	// 12.1.6 Dot Product

	vec2 a6(1.0f, 0.0f);
	vec3 b6(1.0f, 0.0f, 0.0f);
	vec4 c6(0, 0, 1, 0);

	//float dot1 = dot(a6, b6, c6);

	vec2 proj1 = proj(a3, b3);
	vec3 proj2 = proj(c3, d3);
	vec4 proj3 = proj(e3, f3);



	// 12.1.7 Cross Product

	vec3 cross1 = cross(c3, d3);



	// 12.2.1 Defining a Matrix

	mat4 m1(1.0f); // Identity matrix
	float a7 = m1[0][0];
	m1[1] = vec4(2.0); // Sets the second column to all 2.0



	// 12.2.2 Matrix Addition

	mat4 m2(1.0f, 2.0f, 3.0f, 4.0f,
		5.0f, 6.0f, 7.0f, 8.0f,
		9.0f, 10.0f, 11.0f, 12.0f,
		13.0f, 14.0f, 15.0f, 16.0f);
	mat4 m3(1.0f, 2.0f, 1.0f, 2.0f,
		5.0f, 5.0f, 5.0f, 5.0f,
		9.0f, 10.0f, 9.0f, 10.0f,
		0.0f, 0.0f, 1.0f, 2.0f);

	mat4 sum1 = m2 + m3;



	// 12.2.3 Matrix Scaling

	mat4 s = 5.0f * m2;



	// 12.2.4 Matric Multiplication

	mat2 m4(1.0f, 2.0f, 2.0f, 5.0f);
	mat3 m5(1.0f);

	mat2 mul4 = m4 * m4;
	mat3 mul5 = m5 * m5;

	vec4 a8 = m2 * c6;



	// 12.3.1 Translation Matrix

	mat4 T1 = translate(mat4(1.0f), vec3(1, 2, 3));

	//vec3 dog = vec3(T1 * vec4(b2, 1.0f));  // ?



	// 12.3.2 Rotation Matrix

	//mat4 Rx = rotate(mat4(1.0f), 90, vec3(1.0f, 0.0f, 0.0f));

}