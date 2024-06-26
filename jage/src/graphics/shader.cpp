#include "jage/graphics/shader.h"
#include "jage/graphics/helpers.h"
#include "jage/log.h"

#include <glad/glad.h>

namespace jage::graphics
{
	Shader::Shader(const std::string& vertex, const std::string& fragment)
	{
		programId_ = glCreateProgram(); JAGE_CHECK_GL_ERROR

		int status = GL_FALSE;
		char errorLog[512];
		uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER); JAGE_CHECK_GL_ERROR
		{
			const GLchar* glSource = vertex.c_str();
			glShaderSource(vertexShaderId, 1, &glSource, nullptr); JAGE_CHECK_GL_ERROR
			glCompileShader(vertexShaderId); JAGE_CHECK_GL_ERROR
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status); JAGE_CHECK_GL_ERROR

			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(vertexShaderId, sizeof(errorLog), nullptr, errorLog); JAGE_CHECK_GL_ERROR
				JAGE_ERROR("Vertex shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(programId_, vertexShaderId); JAGE_CHECK_GL_ERROR
			}
		}

		uint32_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		if (status == GL_TRUE)
		{
			const GLchar* glSource = fragment.c_str();
			glShaderSource(fragmentShaderId, 1, &glSource, nullptr); JAGE_CHECK_GL_ERROR
			glCompileShader(fragmentShaderId); JAGE_CHECK_GL_ERROR
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status); JAGE_CHECK_GL_ERROR

			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(fragmentShaderId, sizeof(errorLog), nullptr, errorLog); JAGE_CHECK_GL_ERROR
				JAGE_ERROR("Fragment shader compilation error: {}", errorLog);
			}
			else
			{
				glAttachShader(programId_, fragmentShaderId); JAGE_CHECK_GL_ERROR
			}
		}

		JAGE_ASSERT(status == GL_TRUE, "Error compiling shader");

		if (status == GL_TRUE)
		{
			glLinkProgram(programId_); JAGE_CHECK_GL_ERROR
			glValidateProgram(programId_); JAGE_CHECK_GL_ERROR
			glGetProgramiv(programId_, GL_LINK_STATUS, &status); JAGE_CHECK_GL_ERROR

			if (status != GL_TRUE)
			{
				glGetProgramInfoLog(programId_, sizeof(errorLog), nullptr, errorLog); JAGE_CHECK_GL_ERROR
				JAGE_ERROR("Shader link error: {}", errorLog)
				glDeleteProgram(programId_); JAGE_CHECK_GL_ERROR
				programId_ = -1;
			}
		}

		glDeleteShader(vertexShaderId); JAGE_CHECK_GL_ERROR
		glDeleteShader(fragmentShaderId); JAGE_CHECK_GL_ERROR
	} 

	Shader::~Shader()
	{
		glUseProgram(0); JAGE_CHECK_GL_ERROR
		glDeleteProgram(programId_); JAGE_CHECK_GL_ERROR
	}

	void Shader::Bind()
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
	}

	void Shader::Unbind()
	{
		glUseProgram(0); JAGE_CHECK_GL_ERROR
	}

	void Shader::SetUniformInt(const std::string& name, int val)
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
		glUniform1i(GetUniformLocation(name), val); JAGE_CHECK_GL_ERROR
	}

	void Shader::SetUniformFloat(const std::string& name, float val)
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
		glUniform1f(GetUniformLocation(name), val); JAGE_CHECK_GL_ERROR
	}

	void Shader::SetUniformFloat2(const std::string& name, float val1, float val2)
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
		glUniform2f(GetUniformLocation(name), val1, val2); JAGE_CHECK_GL_ERROR
	}

	void Shader::SetUniformFloat3(const std::string& name, float val1, float val2, float val3)
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
		glUniform3f(GetUniformLocation(name), val1, val2, val3); JAGE_CHECK_GL_ERROR
	}

	void Shader::SetUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
	{
		glUseProgram(programId_); JAGE_CHECK_GL_ERROR
		glUniform4f(GetUniformLocation(name), val1, val2, val3, val4); JAGE_CHECK_GL_ERROR
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		auto it = uniformLocations_.find(name);
		if (it == uniformLocations_.end())
		{
			uniformLocations_[name] = glGetUniformLocation(programId_, name.c_str()); JAGE_CHECK_GL_ERROR
		}

		return uniformLocations_[name];
	}
}