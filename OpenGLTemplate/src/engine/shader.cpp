#include "core.h"
#include "engine/shader.h"

namespace Engine {
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
		// 1. Retrieve the vertex / fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			// Read file's buffer contents into streams
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			throw std::exception("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
		}

		// 2. Compile shaders
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertexShaderId, fragmentShaderId;
		int success;
		char infoLog[512];

		// Create vertex shader
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);				// Create a vertex shader
		glShaderSource(vertexShaderId, 1, &vShaderCode, NULL);			// Attach the vertex shader source code
		glCompileShader(vertexShaderId);								// Compile the vertex shader
		// Check for vertex shader compile errors
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(vertexShaderId);
			throw std::exception("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		}

		// Create fragment shader
		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);			// Create a fragment shader
		glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL);		// Attach the fragment shader source code
		glCompileShader(fragmentShaderId);								// Compile the fragment shader
		// Check for fragment shader compile errors
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			throw std::exception("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
		}

		// Shader program
		shaderId = glCreateProgram();
		glAttachShader(shaderId, vertexShaderId);
		glAttachShader(shaderId, fragmentShaderId);
		glLinkProgram(shaderId);
		// Check for shader program linking errors
		glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			glDeleteProgram(shaderId);
			throw std::exception("ERROR::PROGRAM::LINKING_FAILED\n");
		}

		// Delete vertex and fragment shader instances as they have been linked
		glDetachShader(shaderId, vertexShaderId);
		glDetachShader(shaderId, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		// 3. Get uniform locations, and update the hashmap
		GLint numUniforms;
		glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &numUniforms);
		GLint maxCharLength;
		glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);

		if (numUniforms > 0 && maxCharLength > 0) {
			char* charBuffer = new char[maxCharLength];
			for (int i = 0; i < numUniforms; i++) {
				int length, size;
				GLenum dataType;
				glGetActiveUniform(shaderId, i, maxCharLength, &length, &size, &dataType, charBuffer);
				GLint varLocation = glGetUniformLocation(shaderId, charBuffer);
				printf("Uniform %s has location %d\n", charBuffer, varLocation);
				uniformLocations[charBuffer] = varLocation;
			}
			delete[] charBuffer;
		}
	}

	// Use / Activate the shader
	void Shader::use() {
		glUseProgram(shaderId);
	}

	void Shader::setBool(const std::string& name, const bool value) {
		use();
		glUniform1i(uniformLocations[name], (int)value);
	}

	void Shader::setInt(const std::string& name, const int value) {
		use();
		glUniform1i(uniformLocations[name], value);
	}

	void Shader::setFloat(const std::string& name, const float value) {
		use();
		glUniform1f(uniformLocations[name], value);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3 mat) {
		use();
		glUniformMatrix3fv(uniformLocations[name], 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::setMat4(const std::string& name, const glm::mat4 mat) {
		use();
		glUniformMatrix4fv(uniformLocations[name], 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::setVec2(const std::string& name, const glm::vec2 vec) {
		use();
		glUniform2f(uniformLocations[name], vec.x, vec.y);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3 vec) {
		use();
		glUniform3f(uniformLocations[name], vec.x, vec.y, vec.z);
	}

	void Shader::setVec4(const std::string& name, const glm::vec4 vec) {
		use();
		glUniform4f(uniformLocations[name], vec.x, vec.y, vec.z, vec.w);
	}
}