#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture_t* createTexture(const char* tex)
{
	texture_t* newTex = new texture_t;
	newTex->texID = -1;//para que si sale esto de error4
	
	/*int x, y;
	x = y = 0;*/
	//llamada a stbi_load
	unsigned char* texBytes = stbi_load(tex, &newTex->w, &newTex->h, nullptr, 4);//cargados los bytes de la imagen
	//generar la textura en si
	glGenTextures(1, &newTex->texID);
	//bindear (hacerla activa)
	/*Esta línea es crítica si queremos hacer mas cosas. En nuestro código
	no utilizaremos muchas texturas y nos valdrá con un texID y un bindeo.
	Pero cada vez que queramos utilizar una textura cargada hay que repetir el bind.
	Hay que poner el glBindTexture(GL_TEXTURE_2D, texID); justo antes de hacer cosas*/
	glBindTexture(GL_TEXTURE_2D, newTex->texID);
	//parámetros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//subir bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTex->w, newTex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texBytes);
	//generar mipmaps (opcional)
	glGenerateMipmap(GL_TEXTURE_2D);
	//una vez cargada la imagen la puedo eliminar
	stbi_image_free(texBytes);

	return newTex;
}