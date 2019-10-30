#include "shaderSource.h"

const GLchar* vertexShaderSRC = "#version 330 \n"\
"#extension GL_NV_shadow_samplers_cube : enable \n"
"uniform mat4 model;"\
"uniform mat4 view;"\
"uniform mat4 projection;"\
"uniform mat4 normalMat;"\
"uniform mat4 depthBiasMVP;"\
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
"varying vec4 fshadowMapCoords;"\

""\
"void main()"\
"{	gl_Position = projection*view*model*vec4(vpos,1.0);"\
"	fshadowMapCoords = depthBiasMVP * vec4(vpos,1.0f);"\
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
const GLchar* fragmentShaderSRC = "#version 330 \n"\
"#extension GL_NV_shadow_samplers_cube : enable \n"
"uniform sampler2D samplerColor;"\
"uniform sampler2D samplerNormal;"\
"uniform sampler2D shadowMapSampler;"\
"uniform samplerCube samplerCubeMap;"\
"uniform float ambientStrenght;"\
"uniform vec3 lightColor;"\
"uniform vec3 lightPos;"\
"uniform vec3 eyePos;"\
"uniform int  lightEnable;"\
"uniform int  useNormalTex;"\
"uniform int  useSamplerCube;"\
"uniform int  shininess;"\
"uniform int  useShadowMap;"\
""\
"varying vec2 ftex;"\
"varying vec3 fnormalIn;"\
"varying vec3 fpos;"\
"varying vec3 fposCubeMap;"\
"varying mat3 TBN;"\
"varying vec4 fshadowMapCoords;"\
""\
"void main() "\
"{"\
"		vec3 uvw = normalize(fposCubeMap);"\
"		vec3 ambiental=ambientStrenght*lightColor;"\
"		vec3 norm = vec3(0,0,0);"\
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
"		float visibility = 1.0f;"\
"       if (useShadowMap == 1) {"\
"			if (texture2D(shadowMapSampler, fshadowMapCoords.xy).r < fshadowMapCoords.z)"\
"			{"\
"				visibility = 0.0f;"\
"           }"\
"       }"\
"		vec3 viewDir=normalize(eyePos-fpos);"\
"		vec3 reflectDir=reflect(-lightDir,norm);"
"		vec3 specular=pow(max(dot(viewDir,reflectDir),0),shininess)*lightColor;"

"		vec3 finalLight= ambiental+(visibility) * (diffuse+specular);"\
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

const GLchar* vertexShaderShadowMapSRC = "#version 330 \n\
uniform mat4 MVP;\n\
attribute vec3 vpos;\n\
attribute vec2 vtex;\n\
attribute vec3 vnormal;\n\
attribute vec3 vtangent;\n\
\n\
void main()\n\
{\n\
	gl_Position = MVP * vec4(vpos, 1.0f);\n\
}\n\
";

const GLchar* fragmentShaderShadowMapSRC = "#version 330 \n\
void main()\n\
{\n\
}\n\
";

GLint checkShaderError(GLint shaderID)
{
	GLint success = 1;
	char* infoLog = new char[1024];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cout << "Error en shader\n" << infoLog << "\n";
		exit(-1);
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


GLint compileAndLinkShaderProgram(const GLchar* vShaderSrc, const GLchar* fShaderSrc)
{
	GLint programID, vertexID, fragmentID;

	programID = glCreateProgram();
	vertexID = compileShader(vShaderSrc, GL_VERTEX_SHADER);
	fragmentID = compileShader(fShaderSrc, GL_FRAGMENT_SHADER);

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);

	GLint success = 1;
	char* infoLog = new char[1024];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
		std::cout << "Error en programa al linkar\n" << infoLog << "\n";
		exit(-1);
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	return programID;


}
