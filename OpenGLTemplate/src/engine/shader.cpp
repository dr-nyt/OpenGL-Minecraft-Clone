#include "core.h"
#include "engine/shader.h"

namespace Engine {
	namespace Shader {
		unsigned int createVAO() {
			unsigned int vaoID;
			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			return vaoID;
		}

		void createVBO(Vertex* vertices, int verticesByteSize, int vertexLen, int usage)
		{
			unsigned int vboID;
			glGenBuffers(1, &vboID);									  // Generate Vertex Buffer Object
			glBindBuffer(GL_ARRAY_BUFFER, vboID);						  // Bind the Array Buffer to use the VBO
			glBufferData(GL_ARRAY_BUFFER, verticesByteSize, vertices, usage);	  // Bind vertices to the VBO
		}

		void addVertexAttrib(int location, int attribLen, int vertexLen, void* offset)
		{
			glVertexAttribPointer(
				location,
				attribLen,
				GL_FLOAT,
				GL_FALSE,
				vertexLen * sizeof(float),
				offset
			);
			glEnableVertexAttribArray(location);
		}

		void createEBO(unsigned int* indices, int IndicesByteSize, int usage)
		{
			unsigned int eboID;
			glGenBuffers(1, &eboID);							// Generate Element Buffer Object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);		// Bind the EBO
			// Set the EBO data to be indices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndicesByteSize, indices, usage);
		}

		void useVAO(int vaoID) {
			glBindVertexArray(vaoID);
		}

		void unbindVAO() {
			glBindVertexArray(0);
		}

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

			unsigned int vertex, fragment;
			int success;
			char infoLog[512];

			// Create vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);				// Create a vertex shader
			glShaderSource(vertex, 1, &vShaderCode, NULL);			// Attach the vertex shader source code
			glCompileShader(vertex);								// Compile the vertex shader
			// Check for vertex shader compile errors
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
				throw std::exception("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
			}

			// Create fragment shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);			// Create a fragment shader
			glShaderSource(fragment, 1, &fShaderCode, NULL);		// Attach the fragment shader source code
			glCompileShader(fragment);								// Compile the fragment shader
			// Check for fragment shader compile errors
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
				glDeleteShader(vertex);
				throw std::exception("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
			}

			// Shader program
			shaderID = glCreateProgram();
			glAttachShader(shaderID, vertex);
			glAttachShader(shaderID, fragment);
			glLinkProgram(shaderID);
			// Check for shader program linking errors
			glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
				std::cout << infoLog << std::endl;
				glDeleteShader(vertex);
				glDeleteShader(fragment);
				throw std::exception("ERROR::PROGRAM::LINKING_FAILED\n");
			}

			// Delete vertex and fragment shader instances as they have been linked
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		// Use / Activate the shader
		void Shader::use() {
			glUseProgram(shaderID);
		}

		void Shader::setBool(const std::string& name, bool value) {
			glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
		}

		void Shader::setInt(const std::string& name, int value) {
			glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
		}

		void Shader::setFloat(const std::string& name, float value) {
			glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
		}

		void Shader::setMat3(const std::string& name, const GLfloat* mat) {
			glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
		}

		void Shader::setMat4(const std::string& name, const GLfloat* mat) {
			glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
		}

		void Shader::setVec2(const std::string& name, glm::vec2 vec) {
			glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y);
		}

		void Shader::setVec3(const std::string& name, glm::vec3 vec) {
			glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);
		}

		void Shader::setVec4(const std::string& name, glm::vec4 vec) {
			glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
		}
	}
}