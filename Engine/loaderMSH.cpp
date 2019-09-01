#include "loaderMSH.h"
#include <pugixml.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;

std::vector<string>* splitString(const std::string& str, char delim) {
	std::vector<string>* elems = new vector<string>();
	std::stringstream sstream(str);
	std::string item;
	if (str != "") {
		while (std::getline(sstream, item, delim))
		{
			elems->push_back(item);
		}
	}
	return elems;
}

template <typename T>
T numberFromString(const std::string& str)
{
	T number;
	std::istringstream stream(str);
	stream >> number;
	return number;
}



std::list<mesh_t*>* loadMSH(const char* fileName)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName);


	if (result)
	{
		std::list<mesh_t*>* meshList = new std::list<mesh_t*>();

		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");
		//por cada hijo buffer, crear un polygon
		for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling()
			)
		{
			mesh_t* pol = new mesh_t;
			pol->triangleCount = 1;
			memset(pol, 0, sizeof(mesh_t));

			//pugi::xml_node bufferNode = buffersNode.child("buffer");
			pugi::xml_node colorTextureNode = bufferNode.child("material").child("texture");

			//load colore texture			
			pol->texture = new texture_t;
			pol->texture->textureName = splitString(bufferNode.child("material").child("texture").text().as_string(), ',');
			//load normal texture			
			pol->normalTexture = new texture_t;
			pol->normalTexture->textureName = splitString(bufferNode.child("material").child("normal_texture").text().as_string(), ',');
			//carga brillo
			pol->shininess = bufferNode.child("material").child("shininess").text().as_float();
			
			//carga depthWrite
			pol->useDepthWrite = (bufferNode.child("material").child("depthwrite") && bufferNode.child("material").child("depthwrite").text().as_bool() ? 1 : 0);
			//carga vértices
			vector<string>* indices = splitString(bufferNode.child("indices").text().as_string(), ',');
			pol->vertexIndex = new int[indices->size()];
			pol->vertexIndexCount = indices->size();

			int vecLength = indices->size();

			for (int i = 0; i < vecLength; i++)
			{
				pol->vertexIndex[i] = numberFromString<int>(indices->at(i));
			}

			vector<string>* coords = splitString(bufferNode.child("coords").text().as_string(), ',');
			vector<string>* texCoords = splitString(bufferNode.child("texCoords").text().as_string(), ',');
			vector<string>* normals = splitString(bufferNode.child("normals").text().as_string(), ',');
			vector<string>* tangents = splitString(bufferNode.child("tangents").text().as_string(), ',');

			pol->vertices = new vertex_t[coords->size() + texCoords->size()];
			pol->vertexCompCount = bufferNode.child("coords").attribute("vertexCompCount").as_int();

			if (texCoords->size() != 0)
				pol->texCoordCompCount = bufferNode.child("texCoords").attribute("texCoordCompCount").as_int();
			else
				pol->texCoordCompCount = 2;

			if (normals->size() != 0)
				pol->normalsCompCount = bufferNode.child("normals").attribute("normalCompCount").as_int();
			else
				pol->normalsCompCount = 3;

			if (tangents->size() != 0)
			{
				pol->tangentsCompCount = bufferNode.child("tangents").attribute("tangentCompCount").as_int();
				pol->useNormalTex = 1;
			}
			else
			{
				pol->tangentsCompCount = 3;
				pol->useNormalTex = 0;
			}


			int vertexCount = coords->size() / pol->vertexCompCount;

			for (int i = 0; i < vertexCount; i++)
			{

				//carga coordenadas de vértice (vertexCompCount=3) desde array coords
				for (int j = 0; j < pol->vertexCompCount; j++)
					pol->vertices[i].pos[j] = numberFromString<float>(coords->at(i*pol->vertexCompCount + j));
				//carga coords de textura de vértice (texCoordCompCount=3) desde array texCoords
				if (texCoords->size() != 0)
					for (int j = 0; j < pol->texCoordCompCount; j++)
						pol->vertices[i].texCoord[j] = numberFromString<float>
						(texCoords->at(i *pol->texCoordCompCount + j));
				//carga normales de vértice (texCoordCompCount=3) desde array texCoords
				if (normals->size() != 0)
					for (int j = 0; j < pol->normalsCompCount; j++)
						pol->vertices[i].normal[j] = numberFromString<float>
						(normals->at(i *pol->normalsCompCount + j));
				//carga tangentes
				if (tangents->size() != 0)
					for (int j = 0; j < pol->tangentsCompCount; j++)
						pol->vertices[i].tangent[j] = numberFromString<float>
						(tangents->at(i *pol->tangentsCompCount + j));
			}
			pol->stride = pol->vertexCompCount + pol->texCoordCompCount + pol->normalsCompCount + pol->tangentsCompCount;
			if (texCoords->size() != 0)
				pol->texCoordCount = texCoords->size() / pol->texCoordCompCount;
			else
				pol->texCoordCount = vertexCount;

			pol->vertexCount = vertexCount;
			pol->triangleCount = pol->vertexIndexCount / 3;

			if (normals->size() != 0)
				pol->normalsCount = normals->size() / pol->normalsCompCount;
			else
				pol->normalsCount = vertexCount;

			if (tangents->size() != 0)
				pol->tangentsCount = tangents->size() / pol->tangentsCompCount;
			else
				pol->tangentsCount = vertexCount;

			delete indices;
			delete coords;
			delete texCoords;
			delete tangents;

			meshList->push_back(pol);
		}
		return meshList;
	}
	else
	{
		cout << result.description() << "\n";
		return nullptr;//exit();
	}

	return nullptr;

}