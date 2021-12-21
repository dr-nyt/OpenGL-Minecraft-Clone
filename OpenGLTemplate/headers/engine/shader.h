#pragma once
#include "core.h"

namespace Engine {
	namespace Shader {
		struct Vertex {
			glm::vec3 position;
			glm::vec4 color;
		};

		unsigned int createVAO();
		void createVBO(Vertex* vertices, int byteSize, int vertexLen, int usage);
		void addVertexAttrib(int location, int attribLen, int vertexLen, void* offset);
		void createEBO(unsigned int* indices, int byteSize, int usage);
		void useVAO(int vaoID);
		void unbindVAO();

		class Shader {
		private:
			unsigned int shaderID;

		public:
			Shader(const std::string& vertexPath, const std::string& fragmentPath);
			void use();
			void setBool(const std::string& name, bool value);
			void setInt(const std::string& name, int value);
			void setFloat(const std::string& name, float value);
			void setMat3(const std::string& name, const GLfloat* mat);
			void setMat4(const std::string& name, const GLfloat* mat);
			void setVec2(const std::string& name, glm::vec2 vec);
			void setVec3(const std::string& name, glm::vec3 vec);
			void setVec4(const std::string& name, glm::vec4 vec);
		};
	}
}
