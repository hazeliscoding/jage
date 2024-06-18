#include "graphics/rendercommands.h"
#include "log.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"

#include <glad/glad.h>

namespace jage::graphics::rendercommands
{
	void RenderMesh::Execute()
	{
		// Attempt to obtain a shared ownership of Mesh and Shader objects
		const std::shared_ptr<Mesh> mesh = mesh_.lock();
		const std::shared_ptr<Shader> shader = shader_.lock();

		// Check if both mesh and shader objects are still alive (i.e., not deallocated)
		if (mesh && shader)
		{
			// Bind the mesh and shader resources to prepare for drawing
			mesh->Bind();
			shader->Bind();

			// Check if the mesh has associated element data for indexed drawing
			if (mesh->GetElementCount() > 0)
			{
				// Perform indexed drawing using element array
				glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				// Perform non-indexed drawing if no elements are available
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount());
			}

			// Unbind the shader and mesh to clean up and prevent unwanted state changes
			shader->Unbind();
			mesh->Unbind();
		}
		else
		{
			// Log a warning if either the mesh or shader is no longer valid
			JAGE_WARN("Attempting to execute RenderMesh with invalid data")
		}
	}

}