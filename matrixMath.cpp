#include "matrixMath.h"

vec3d triangle::normalVector()
{
	vec3d u = p[1] - p[0];
	vec3d v = p[2] - p[0];
	vec3d normal(
		(u.y * v.z) - (u.z * v.y),
		(u.z * v.x) - (u.x * v.z),
		(u.x * v.y) - (u.y * v.x)
	);
	normal.normalize();
	return normal;
}

vec3d::vec3d()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

vec3d::vec3d(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void vec3d::normalize()
{
	float length = sqrtf(x * x + y * y + z * z);
	x /= length;
	y /= length;
	z /= length;
}

bool mesh::LoadOBJ(std::string fileName)
{
	tri.clear();
	tri.shrink_to_fit();
	std::ifstream f(fileName);//, std::ios::in
	if (f)
	{
		std::vector<vec3d> pts;
		std::string line, st;
		while (std::getline(f, line))
		{
			if (line.starts_with("v "))
			{
				vec3d p;
				std::stringstream s(line);
				s >> st >> p.x >> p.y >> p.z;
				pts.push_back(p);
			}
			if (line.starts_with("f "))
			{
				int p[3];
				std::stringstream s(line);
				s >> st;
				for (int i = 0; i < 3; i++)
				{
					s >> st;
					p[i] = std::stoi(st);
				}
				tri.push_back({ pts[p[0] - 1], pts[p[1] - 1], pts[p[2] - 1]
			});
			}
		}
		return true;
	}
	return false;
}

void mesh::move(float x, float y, float z)
{
	for (auto& i : tri)
		for (auto& j : i.p)
		{
			j.x += x;
			j.y += y;
			j.z += z;
		}
}

vec3d vec3d::operator-(vec3d const& obj)
{
	return { this->x - obj.x,this->y - obj.y,this->z - obj.z };
}

vec3d vec3d::operator+(vec3d const& obj)
{
	return { this->x + obj.x,this->y + obj.y,this->z + obj.z };
}

void vec3d::operator-=(vec3d const& obj)
{
	(*this) = ((*this) - obj);
}

void vec3d::operator+=(vec3d const& obj)
{
	(*this) = ((*this) + obj);
}

vec3d rotateX(vec3d point, float angle)
{
	mat4x4 rotMat = { 0 };
	rotMat.m[0][0] = 1;
	rotMat.m[1][1] = cosf(angle);
	rotMat.m[1][2] = -sinf(angle);
	rotMat.m[2][1] = sinf(angle);
	rotMat.m[2][2] = cosf(angle);
	return MultiplyMatrixVector(point, rotMat);
}

vec3d rotateY(vec3d point, float angle)
{
	mat4x4 rotMat = { 0 };
	rotMat.m[0][0] = cosf(angle);
	rotMat.m[0][2] = sinf(angle);
	rotMat.m[1][1] = 1;
	rotMat.m[2][0] = -sinf(angle);
	rotMat.m[2][2] = cosf(angle);
	return MultiplyMatrixVector(point, rotMat);
}

vec3d rotateZ(vec3d point, float angle)
{
	mat4x4 rotMat = { 0 };
	rotMat.m[0][0] = cosf(angle);
	rotMat.m[0][1] = -sinf(angle);
	rotMat.m[1][0] = sinf(angle);
	rotMat.m[1][1] = cosf(angle);
	rotMat.m[2][2] = 1;
	return MultiplyMatrixVector(point, rotMat);
}

vec3d MultiplyMatrixVector(vec3d i, mat4x4 m)
{
	vec3d o;
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
	return o;
}

vec3d project(vec3d point, float FOV)
{
	mat4x4 matProj = { 0 };

	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float fovRad = 1.0f / std::tan(FOV * 0.5f / 180.0f * 3.14159f);

	matProj.m[0][0] = AR * fovRad;
	matProj.m[1][1] = fovRad;
	matProj.m[2][2] = farPlane / (farPlane - nearPlane);
	matProj.m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
	matProj.m[2][3] = 1.0f;

	return MultiplyMatrixVector(point, matProj);
}

float dotProduct(vec3d u, vec3d v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}