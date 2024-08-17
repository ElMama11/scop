#ifndef SHADER_HPP
# define SHADER_HPP

# include "../../resources/glad/include/glad/glad.h"
  
# include <string>
# include <fstream>
# include <sstream>
# include <iostream>

class Shader {
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		// Utility uniform functions
		void setBool(const std::string &name, bool value) const;  
		void setInt(const std::string &name, int value) const;   
		void setFloat(const std::string &name, float value) const;
		
	private:
		void checkCompileErrors(unsigned int shader, std::string type);
};
  
#endif
