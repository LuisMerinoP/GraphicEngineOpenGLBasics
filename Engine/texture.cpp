#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture_t* createTexture(const char* tex)
{
	texture_t* newTex = new texture_t;
	newTex->texID = -1;

	unsigned char* texBytes = stbi_load(tex, &newTex->w, &newTex->h, nullptr, 4);
	//generar id de textura
	glGenTextures(1, &newTex->texID);
	//hacerla activa
	glBindTexture(GL_TEXTURE_2D, newTex->texID);
	//parámetros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBytes);
	//generar mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//liberar datos
	stbi_image_free(texBytes);

	return newTex;
}