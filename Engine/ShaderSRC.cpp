#include "shaderSource.h"

//const GLchar* vertexShaderSRC = ""\
//"uniform mat4 MVP;"\
//"attribute vec3 vpos;"\
//"attribute vec4 vcolor;"\
//"varying vec4 fcolor;"\
//"void main()"\
//"{ gl_Position = MVP * vec4(vpos, 1);"\
//	"fcolor = vcolor;"\
//" }"\
//""\
//"";
//
//const GLchar* fragmentShaderSRC = ""\
//"varying vec4 fcolor;"\
//"void main() "\
//"{ gl_FragColor = fcolor;" \
//"}"\
//"";
const GLchar* vertexShaderSRC = ""\
"uniform mat4 MVP;"\
"attribute vec4 vpos;"\
"attribute vec2 vtex;"\
"varying vec2 ftex;"\
"void main()"\
"{	gl_Position = MVP * vpos;"\
"	ftex=vtex;"\
" }"\
""\
"";
const GLchar* fragmentShaderSRC = ""\
"uniform sampler2D sampler;"\
"varying vec2 ftex;"\
"void main() "\
"{ gl_FragColor = texture2D(sampler,ftex); }" \
"";



GLint checkShaderError(GLint shaderID)
{
	GLint success = 1;//error a true
	char* infoLog = new char[1024];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cout << "Error in shader \n" << infoLog << "\n";
		exit(-1);//stop application if error in shader
	}
	return success;
}


GLint compileShader(const GLchar* src, GLenum shaderType)
{
	GLint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &src, nullptr);
	glCompileShader(shaderID);
	checkShaderError(shaderID);
	return shaderID;
}

GLint compileAndLinkShaderProgram(const GLchar* vShaderSrc,const GLchar* fShaderSrc)
{
	GLint programID, vertexID, fragmentID;//shader identifiers

	programID = glCreateProgram();
	vertexID = compileShader(vShaderSrc, GL_VERTEX_SHADER);
	fragmentID = compileShader(fShaderSrc, GL_FRAGMENT_SHADER);

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	//link
	glLinkProgram(programID);

	//check program
	GLint success = 1;//error a true
	char* infoLog = new char[1024];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
		std::cout << "Error linking program \n" << infoLog << "\n";
		exit(-1);//stop application if error in shader
	}
	//despues de linar se pueden eliminar, ya forman parte del programa
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return programID;
	
}