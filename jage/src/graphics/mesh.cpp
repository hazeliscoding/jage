#include "graphics/mesh.h"

#include <glad/glad.h>

namespace jage::graphics
{
	Mesh::Mesh(const float* vertex_array, const uint32_t vertex_count, const uint32_t dimensions)
		: vertex_count_(vertex_count)
		, element_count_(0)
		, ebo_(0)
	{
		// Generate and bind a Vertex Array Object (VAO)
		glGenVertexArrays(1, &vao_); 
		glBindVertexArray(vao_); 

		// Generate and bind a Vertex Buffer Object (EBO) for positions
		glGenBuffers(1, &position_vbo_); 
		glBindBuffer(GL_ARRAY_BUFFER, position_vbo_);

		// Upload vertex data to the GPU
		glBufferData(GL_ARRAY_BUFFER, vertex_count * dimensions * sizeof(float), vertex_array, GL_STATIC_DRAW); 

		// Setup vertex attributes
		glEnableVertexAttribArray(0); 
		glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0);

		// Disable vertex attribute array for clean-up
		glDisableVertexAttribArray(0); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		// Unbind VAO to avoid unintentional modifications
		glBindVertexArray(0); 
	}

	Mesh::Mesh(const float* vertex_array, const uint32_t vertex_count, const uint32_t dimensions, const uint32_t* element_array, const uint32_t element_count)
		: Mesh(vertex_array, vertex_count, dimensions)
	{
		element_count_ = element_count;

		// Re-bind VAO to attach EBO
		glBindVertexArray(vao_); 

		// Generate and bind an Element Buffer Object (EBO)
		glGenBuffers(1, &ebo_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

		// Upload element data to the GPU
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_count * sizeof(uint32_t), element_array, GL_STATIC_DRAW); 

		// Unbind VAO after setup
		glBindVertexArray(0); 
	}

	Mesh::~Mesh()
	{
		// Delete VBO
		glDeleteBuffers(1, &position_vbo_);

		// Delete EBO if it exists
		if (ebo_ != 0)
		{
			glDeleteBuffers(1, &ebo_);
		}

		// Delete VAO
		glDeleteVertexArrays(1, &vao_);
	}

	void Mesh::Bind() const
	{
		glBindVertexArray(vao_);
		glEnableVertexAttribArray(0);
	}

	void Mesh::Unbind()
	{
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}
