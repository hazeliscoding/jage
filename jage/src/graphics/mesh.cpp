#include "graphics/mesh.h"

#include <glad/glad.h>

namespace jage::graphics
{
	Mesh::Mesh(float* vertexArray, uint64_t vertexCount, uint64_t dimensions)
		: vertexCount_(vertexCount)
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		glGenBuffers(1, &positionVBO_);
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO_);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * dimensions * sizeof(float), vertexArray, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	Mesh::~Mesh()
	{
	}
}