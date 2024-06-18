#pragma once
#include <memory>

namespace jage::graphics 
{
	class Mesh;
	class Shader;

	namespace rendercommands
	{
		class RenderCommand 
		{
		public:
			virtual void Execute() = 0;
			virtual ~RenderCommand() = default;
		};

		class RenderMesh : public RenderCommand
		{
		public:
			RenderMesh(const std::weak_ptr<Mesh>& mesh, const std::weak_ptr<Shader>& shader)
				: mesh_(mesh)
				, shader_(shader)
			{}

			void Execute() override;

		private:
			std::weak_ptr<Mesh> mesh_;
			std::weak_ptr<Shader> shader_;
		};
	}
}
