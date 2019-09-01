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
"uniform mat4 normalMat;"\
""\
"attribute vec3 vpos;"\
"attribute vec2 vtex;"\
"attribute vec3 vnormal;"\
"attribute vec3 vtangent;"\
""\
"varying vec2 ftex;"\
"varying vec3 fnormalIn;"\
"varying vec3 fpos;"\
"varying vec3 fposCubeMap;"\
"varying mat3 TBN;"\
""\
"void main()"\
"{	gl_Position = projection*view*model*vec4(vpos,1.0);"\
"	fposCubeMap=(model*vec4(vpos,1.0)).xyz;"\
"	fpos=(view*model*vec4(vpos,1.0)).xyz;"\
"	ftex=vtex;"\
"	fnormalIn=(model*vec4(vnormal,1.0)).xyz;"\
"	vec3 tangent=(normalMat*vec4(vtangent,1.0)).xyz;"\
"	vec3 bitangent=cross(fnormalIn,tangent);"\
"	TBN=transpose(mat3(tangent,bitangent,fnormalIn));"\
"	"\
" }"\
""\
"";
const GLchar* fragmentShaderSRC = ""\
"uniform sampler2D samplerColor;"\
"uniform sampler2D samplerNormal;"\
"uniform samplerCube samplerCubeMap;"\
"uniform float ambientStrenght;"\
"uniform vec3 lightColor;"\
"uniform vec3 lightPos;"\
"uniform vec3 eyePos;"\
"uniform int  lightEnable;"\
"uniform int  useNormalTex;"\
"uniform int  useSamplerCube;"\
"uniform int  shininess;"\
""\
"varying vec2 ftex;"\
"varying vec3 fnormalIn;"\
"varying vec3 fpos;"\
"varying vec3 fposCubeMap;"\
"varying mat3 TBN;"\
""\
"void main() "\
"{ "\
"		vec3 uvw      =normalize(fposCubeMap);"\
"		vec3 ambiental=ambientStrenght*lightColor;"\
"		vec3 norm	 =vec3(0,0,0);"\
""\
"		if(useNormalTex==1){"\
"			vec3 normalTex=texture(samplerNormal,ftex).rgb;"\
"			norm = normalize(normalize(normalTex*2-1.0)*TBN);"\
"		}else{"\
"			norm=normalize(fnormalIn);"\
"		}"\
""\
"		vec3 lightDir= normalize(lightPos-fpos);"\
"		vec3 diffuse= max(dot(norm,lightDir),0.0)*lightColor;"\
""\
""\
"		vec3 viewDir=normalize(eyePos-fpos);"\
"		vec3 reflectDir=reflect(-lightDir,norm);"
"		vec3 specular=pow(max(dot(viewDir,reflectDir),0),shininess)*lightColor;"

"		vec3 finalLight= ambiental+diffuse+specular;"\
"		if(lightEnable==1){"\
"			if(useSamplerCube==1){"\
"				gl_FragColor = vec4(finalLight, 1)*textureCube(samplerCubeMap, uvw); "\
"			}else{"\
"				gl_FragColor = vec4(finalLight, 1)*texture2D(samplerColor, ftex); "\
"			}"\
"		}else{"\
"			if (useSamplerCube == 1) {"\
"				gl_FragColor = textureCube(samplerCubeMap, uvw);"\
"			}"\
"			else {"\
"				gl_FragColor = texture2D(samplerColor, ftex); "\
"			}"\
"		}"\
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