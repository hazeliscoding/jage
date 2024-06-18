#pragma once

#include <cstdint>

namespace jage::graphics
{
	class Mesh
	{
	public:
		Mesh(float* vertexArray, uint64_t vertexCount, uint64_t dimensions);
		~Mesh();

	private:
		uint32_t vertexCount_;
		uint32_t vao_;
		uint32_t positionVBO_;
	};
}
