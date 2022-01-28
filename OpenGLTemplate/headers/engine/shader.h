#pragma once
#include "core.h"

namespace Engine {
	class Shader {
	private:
		unsigned int shaderID;

	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		void use();
		void setBool(const std::string& name, const bool value);
		void setInt(const std::string& name, const int value);
		void setFloat(const std::string& name, const float value);
		void setMat3(const std::string& name, const glm::mat3 mat);
		void setMat4(const std::string& name, const glm::mat4 mat);
		void setVec2(const std::string& name, const glm::vec2 vec);
		void setVec3(const std::string& name, const glm::vec3 vec);
		void setVec4(const std::string& name, const glm::vec4 vec);
	};
}


