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

//need to separate MVP matrix in model, view projection for the light implementation
const GLchar* vertexShaderSRC = ""\
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 projection;"\
"attribute vec3 vpos;"\
"attribute vec2 vtex;"\
"attribute vec3 vnormal;"\
"varying vec2 ftex;"\
"varying vec3 fnormal;"\
"varying vec3 fpos;"\

"void main()"\
"{	gl_Position = projection*view*model * vec4(vpos,1);"\
"	fpos = (model*vec4(vpos,1)).xyz;"\
"	ftex = vtex;"\
"	fnormal=(model*vec4(vnormal,1)).xyz;"\
" }"\
""\
"";
const GLchar* fragmentShaderSRC = ""\
"uniform sampler2D sampler;"\
"uniform float ambientStrength;"\
"uniform vec3 lightColor;"\
"uniform vec3 lightPos;"\
"uniform vec3 eyePos;"\
"uniform int lightEnable;"\
"varying vec2 ftex;"\
"varying vec3 fnormal;"\
"varying vec3 fpos;"\
"void main()"\
"{"\
	"vec3 ambiental = ambientStrength * lightColor;"\
""\
	"vec3 norm = normalize(fnormal);"\
	"vec3 lightDir = normalize(lightPos-fpos);"\
	"vec3 diffuse = max(dot(fnormal, lightDir),0.0) * lightColor;"\
""\
	"int shininess = 34;"\
	"vec3 viewDir = normalize(eyePos-fpos);"\
	"vec3 reflectDir = reflect(-lightDir,fnormal);"\
	"vec3 specular = pow(max(dot(viewDir,reflectDir),0), shininess)*lightColor;"\
""\
	"vec3 finalLight = ambiental + diffuse + specular;"\
	"if (lightEnable == 1)"\
	"	gl_FragColor = vec4(finalLight,1) * texture2D(sampler,ftex);"\
	"else"\
	"	gl_FragColor = texture2D(sampler,ftex);"\
	"}"\
"";



GLint checkShaderError(GLint shaderID)
{
	GLint success = 1;//error a trueB
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