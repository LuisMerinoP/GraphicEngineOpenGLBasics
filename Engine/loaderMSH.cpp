#include "loaderMSH.h"
#include <pugixml.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

polygon* loadMSH(const char* fileName)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName);
	if (result)
	{
		polygon* pol = new polygon;
		memset(pol, 0, sizeof(polygon));//reseteo memoria de polígono por si hay basura. Memset setea todos los bytes de un puntero a un dato dado. En este caso 0.
		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = doc.child("buffers");
		//por cada buffer, crear un polígono 
		/*Por cada buffer hay que ir iterando por los hermanos. */
		/*for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling())
		{

		}*/


	}
	else
	{ 
		std::cout << result.description() << "\n";
		return nullptr; //o exit(); opcional que hacer con el error
	}

}
