#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//La variable externa me unirá la variable de SRC con el código del main.
//Lo hacemos asi de manera provisional. Luego habrá que encapsular esto dentro de la clase shader
extern const GLchar* vertexShaderSRC;
extern const GLchar* fragmentShaderSRC;
extern const GLchar* vertexShaderShadowMapSRC;
extern const GLchar* fragmentShaderShadowMapSRC;

GLint compileAndLinkShaderProgram(const GLchar* vShaderSrc, const GLchar* fShaderSrc);