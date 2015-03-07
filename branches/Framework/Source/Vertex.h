#pragma once
#include "Utilities/utilities.h"

struct Vertex 
{
	Vector3 position;	// 0
	Vector2 texcoord;	// 12
	Vector3 normal;		// 20
	Vector3 tangent;	// 32
	Vector3 bitangent;	// 44
	Vertex()
	{
		position = Vector3(0.0f, 0.0f, 0.0f);
		normal = Vector3(0.0f, 0.0f, 0.0f);
		texcoord = Vector2(0.0f, 0.0f);
	}

	Vertex(const Vertex& src)
	{
		position = Vector3(src.position);
		normal = Vector3(src.normal);
		texcoord = Vector2(src.texcoord);
	}

	Vertex& operator=(const Vertex& src)
	{
		position = Vector3(src.position);
		normal = Vector3(src.normal);
		texcoord = Vector2(src.texcoord);
		return *this;
	}
};