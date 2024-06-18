#include "graphics/shader.h"
#include "log.h"

#include <glad/glad.h>

namespace jage::graphics
{
	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		programId_ = glCreateProgram();

		int status = GL_FALSE;
		char errorLog[512];
		uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		{
			const GLchar* glSource = vertex.c_str();
			glShaderSource(vertexShaderId, 1, &glSource, nullptr);
			glCompileShader(vertexShaderId);
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);

			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(vertexShaderId, sizeof(errorLog), nullptr, errorLog);
				JAGE_ERROR("Vertex shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(programId_, vertexShaderId);
			}
		}

		uint32_t fragmentShaderId = glCreateShader(GL_VERTEX_SHADER);
		if (status == GL_TRUE)
		{
			const GLchar* glSource = fragment.c_str();
			glShaderSource(fragmentShaderId, 1, &glSource, nullptr);
			glCompileShader(fragmentShaderId);
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);

			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(fragmentShaderId, sizeof(errorLog), nullptr, errorLog);
				JAGE_ERROR("Fragment shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(programId_, fragmentShaderId);
			}
		}

		JAGE_ASSERT(status == GL_TRUE, "Error compiling shader");

		if (status == GL_TRUE)
		{
			glLinkProgram(programId_);
			glValidateProgram(programId_);
			glGetProgramiv(programId_, GL_LINK_STATUS, &status);

			if (status != GL_TRUE)
			{
				glGetProgramInfoLog(programId_, sizeof(errorLog), nullptr, errorLog);
				JAGE_ERROR("Error link error: {}", errorLog);
				glDeleteProgram(programId_);
				programId_ = -1;
			}
		}

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	Shader::~Shader()
	{
		glUseProgram(0);
		glDeleteProgram(programId_);
	}

	void Shader::Bind()
	{
		glUseProgram(programId_);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::SetUniformInt(const std::string& name, int val)
	{
		glUseProgram(programId_);
		glUniform1i(GetUniformLocation(name), val);
	}

	void Shader::SetUniformFloat(const std::string& name, float val)
	{
		glUseProgram(programId_);
		glUniform1f(GetUniformLocation(name), val);
	}

	void Shader::SetUniformFloat2(const std::string& name, float val1, float val2)
	{
		glUseProgram(programId_);
		glUniform2f(GetUniformLocation(name), val1, val2);
	}

	void Shader::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
	{
		glUseProgram(programId_);
		glUniform3f(GetUniformLocation(name), val1, val2, val3);
	}

	void Shader::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
	{
		glUseProgram(programId_);
		glUniform4f(GetUniformLocation(name), val1, val2, val3, val4);
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		auto it = uniformLocations_.find(name);
		if (it == uniformLocations_.end())
		{
			uniformLocations_[name] = glGetUniformLocation(programId_, name.c_str());
		}

		return uniformLocations_[name];
	}
}