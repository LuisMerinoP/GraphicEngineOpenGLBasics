#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<iostream>

void createTexture(texture_t* newTex)
{
	//texture_t* newTex = new texture_t;
	newTex->texID = -1;
	newTex->textType = TEXT_2D;

	unsigned char* texBytes = stbi_load(newTex->textureName->at(0).c_str(), &newTex->w, &newTex->h, nullptr, 4);
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
}

void createCubeMapTexture(texture_t* newTex)
{
	//texture_t* newTex = new texture_t;
	newTex->texID = -1;
	newTex->textType = TEXT_CUBEMAP;

	unsigned char* texLeft = stbi_load(newTex->textureName->at(0).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	unsigned char* texRight = stbi_load(newTex->textureName->at(1).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	unsigned char* texFront = stbi_load(newTex->textureName->at(2).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	unsigned char* texBack = stbi_load(newTex->textureName->at(3).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	unsigned char* texUp = stbi_load(newTex->textureName->at(4).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	unsigned char* texDown = stbi_load(newTex->textureName->at(5).c_str(), &newTex->w, &newTex->h, nullptr, 4);
	//generar id de textura
	glGenTextures(1, &newTex->texID);
	//hacerla activa
	glBindTexture(GL_TEXTURE_CUBE_MAP, newTex->texID);
	//parámetros
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texRight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texLeft);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texFront);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBack);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texUp);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDown);


	//liberar datos
	stbi_image_free(texRight);
	stbi_image_free(texLeft);
	stbi_image_free(texFront);
	stbi_image_free(texBack);
	stbi_image_free(texUp);
	stbi_image_free(texDown);

}


bool createTextureFrameBuffer(texture_t* fbTex) {
	//texture_t* newTex = new texture_t;
	fbTex->texID = -1;
	fbTex->textType  = TEXT_FB_DEPTH;

	fbTex->fbID = 0;

	fbTex->w = 1024;
	fbTex->h = 1024;

	glGenTextures(1, &fbTex->texID);
	//hacerla activa
	glBindTexture(GL_TEXTURE_2D, fbTex->texID);
	//parámetros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, fbTex->w, fbTex->h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	
	glGenFramebuffers(1, &fbTex->fbID);
	glBindFramebuffer(GL_FRAMEBUFFER, fbTex->fbID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbTex->texID, 0);
	
	//disable colors
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//error check
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error: " << status << "\n";
		exit(-2);
		return false;
	}
	return true;
}