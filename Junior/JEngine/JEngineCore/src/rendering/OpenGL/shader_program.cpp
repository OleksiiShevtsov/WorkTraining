#include "shader_program.hpp"

jengine::JShaderProgram::JShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
	GLuint vertexShaderId{ 0 };
	if(!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderId))
	{
		JLOG_CRITICAL("VERTEX SHADER: compile-time error!");
		glDeleteShader(vertexShaderId);
		return;
	}

	GLuint fragmentShaderId{ 0 };
	if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderId))
	{
		JLOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		return;
	}

	mId = glCreateProgram();
	glAttachShader(mId, vertexShaderId);
	glAttachShader(mId, fragmentShaderId);
	glLinkProgram(mId);

	GLint success;
	glGetProgramiv(mId, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(mId, 1024, nullptr, infoLog);
		JLOG_CRITICAL("SHADER PROGRAM: link-time error:\n{}", infoLog);
		glDeleteProgram(mId);
		mId = 0;
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		return;
	}
	else
	{
		mIsCompiled = true;
	}

	glDetachShader(mId, vertexShaderId);
	glDetachShader(mId, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

jengine::JShaderProgram::JShaderProgram(JShaderProgram&& shaderProgram) noexcept
{
	mId = shaderProgram.mId;
	mIsCompiled = shaderProgram.mIsCompiled;

	shaderProgram.mId = 0;
	shaderProgram.mIsCompiled = false;
}

jengine::JShaderProgram& jengine::JShaderProgram::operator=(JShaderProgram&& shaderProgram) noexcept
{
	glDeleteProgram(mId);
	mId = shaderProgram.mId;
	mIsCompiled = shaderProgram.mIsCompiled;

	shaderProgram.mId = 0;
	shaderProgram.mIsCompiled = false;
	return *this;
}

jengine::JShaderProgram::~JShaderProgram()
{
	glDeleteProgram(mId);
}

void jengine::JShaderProgram::bind() const
{
	glUseProgram(mId);
}

void jengine::JShaderProgram::unbind()
{
	glUseProgram(0);
}

bool jengine::JShaderProgram::createShader(const char* source, const GLenum shaderType, GLuint& shaderId)
{
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, nullptr);
	glCompileShader(shaderId);

	GLint success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

		JLOG_CRITICAL("Shader compilation error:\n{}", infoLog);
		return false;
	}
	return true;
}
