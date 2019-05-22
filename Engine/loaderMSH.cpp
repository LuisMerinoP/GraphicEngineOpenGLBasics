#include "loaderMSH.h"
#include <pugixml.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

std::unique_ptr<std::vector<std::string>> splitString(const std::string& str, char delim)
{
	std::unique_ptr<std::vector<std::string>> elems = std::make_unique<std::vector<std::string>>();
	std::stringstream sstream(str);
	std::string item;
	if (str != "")
	{
		while (std::getline(sstream, item, delim))
		{
			elems->push_back(item);
		}
	}
	return elems;
}
//std::vector<std::string>* splitString(const std::string& str, char delim) {
//	std::vector<std::string>* elems = new std::vector<std::string>();
//	std::stringstream sstream(str);
//	std::string item;
//	if (str != "") {
//		while (std::getline(sstream, item, delim))
//		{
//			elems->push_back(item);
//		}
//	}
//	return elems;
//}


//template para castear a n�mero un string
template <typename T>
T numberFromString(const std::string& str)
{
	T number;
	std::istringstream stream(str);
	stream >> number;
	return number;
}

polygon* loadMSH(const char* fileName)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName);

	if (result)
	{
		polygon* pol = new polygon;
		pol->triangleCount = 1;
		memset(pol, 0, sizeof(polygon));//reseteo memoria de pol�gono por si hay basura. Memset setea todos los bytes de un puntero a un dato dado. En este caso 0.
		
		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");
		//por cada buffer, crear un pol�gono 
		/*Por cada buffer hay que ir iterando por los hermanos. */
		/*for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling())
		{

		}*/
		pugi::xml_node bufferNode = buffersNode.child("buffer");
		pugi::xml_node materialNode = bufferNode.child("material");
		std::string materialFile = materialNode.text().as_string();
		pol->textureName = new char[materialFile.length()+1];
		memcpy(pol->textureName, materialFile.c_str(), materialFile.length());
		pol->textureName[materialFile.length()] = '\0';
		
		std::unique_ptr<std::vector<std::string>> indices = splitString(bufferNode.child("indices").text().as_string(),',');
		//std::vector<std::string>* indices = splitString(bufferNode.child("indices").text().as_string(), ',');
		pol->vertexIndex = new int[indices->size()];
		pol->vertexIndexCount = indices->size();

		pol->vertexIndex = new int[indices->size()];
		int vecLength = indices->size();
		for (int i = 0; i < vecLength; i++)
		{
			pol->vertexIndex[i] = numberFromString<int>(indices->at(i));
		}

		std::unique_ptr<std::vector<std::string>> coords = splitString(bufferNode.child("coords").text().as_string(), ',');
		std::unique_ptr<std::vector<std::string>> textCoords = splitString(bufferNode.child("textCoords").text().as_string(), ',');
		/*std::vector<std::string>* coords = splitString(bufferNode.child("coords").text().as_string(), ',');
		std::vector<std::string>* texCoords = splitString(bufferNode.child("texCoords").text().as_string(), ',');*/
		std::unique_ptr<std::vector<std::string>> normals = splitString(bufferNode.child("normals").text().as_string(), ',');


		pol->vertices = new vertex_t[coords->size() + textCoords->size()];
		pol->vertexCompCount = bufferNode.child("coords").attribute("vertexCompCount").as_int();
		pol->vertexCompCount = bufferNode.child("texCoords").attribute("texCoordCompCount").as_int();
		pol->normalsCompCount = bufferNode.child("normals").attribute("normalCompCount").as_int();


		int vertexCount = coords->size() / pol->vertexCompCount;//cada coordenada ocupa 4 floats. Dividiendo entre 4 sabemos el num de v�rtices
		for (int i = 0; i < vertexCount; i++)
		{
			//Cargamos coordenadas en array de coordenadas
			for (int j=0;j<pol->vertexCompCount;j++)
				pol->vertices[i].pos[j] = numberFromString<float>(coords->at(i*pol->vertexCompCount+j));
			//Cargamos coordenadas de textura
			for (int j=0;j<pol->texCoordCompCount;j++)
				pol->vertices[i].texCoord[j] = numberFromString<float>(textCoords->at(i*pol->texCoordCompCount+j));
			for (int j = 0; j < pol->normalsCompCount; j++)
				pol->vertices[i].normal[j] = numberFromString<float>(normals->at(i*pol->normalsCompCount+j));
		}
		pol->stride = pol->vertexCompCount + pol->texCoordCompCount + pol->normalsCompCount;
		pol->normalCount = normals->size() / pol->normalsCompCount;
		pol->texCoordCount = textCoords->size() / pol->texCoordCompCount;
		pol->vertexCount = vertexCount;
		pol->triangleCount = pol->vertexCount / pol->vertexCompCount;

		//Aqui irian los deletes si hubieramos usado raw pointers e lugar de smart pointers.
		/*delete indices;
		delete coords;
		delete texCoords;*/
		return pol;
	}
	else
	{ 
		std::cout << result.description() << "\n";
		return nullptr; //o exit(); opcional que hacer con el error
	}
	return nullptr;
}
