#pragma once

#include <cstdint>

namespace jage::graphics
{
	class Mesh
	{
	public:
		Mesh(const float* vertex_array, uint32_t vertex_count, uint32_t dimensions);
		Mesh(const float* vertex_array, uint32_t vertex_count, uint32_t dimensions, const uint32_t* element_array, uint32_t element_count);
		~Mesh();

		void Bind() const;
		static void Unbind();

		[[nodiscard]] uint32_t GetVertexCount() const { return vertex_count_; }
		[[nodiscard]] uint32_t GetElementCount() const { return element_count_; }

	private:
		uint32_t vertex_count_;
		uint32_t element_count_;
		uint32_t vao_;
		uint32_t ebo_;
		uint32_t position_vbo_;
	};
}
