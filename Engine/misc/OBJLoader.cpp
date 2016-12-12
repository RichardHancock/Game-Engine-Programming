#include "OBJLoader.h"

OBJLoader::Type OBJLoader::getType(std::string typeToConvert)
{
	/*
	Maybe rework this into a unordered map with loop umap: (string, type)
	*/
	if (typeToConvert == "v")
	{
		return V;
	}
	else if (typeToConvert == "vp")
	{
		return VP;
	}
	else if (typeToConvert == "vt")
	{
		return VT;
	}
	else if (typeToConvert == "vn")
	{
		return VN;
	}
	else if (typeToConvert == "f")
	{
		return F;
	}
	else if (typeToConvert.empty() || typeToConvert == "#")
	{
		return Ignored;
	}
	else
	{
		return Unhandled;
	}
}

void OBJLoader::readFace(std::stringstream &line, std::vector<MultiIndicies> &storage)
{
	MultiIndicies ind[3];

	for (unsigned int i = 0; i < 3; i++)
	{
		ind[i].vn = 0;
		ind[i].vt = 0;


		line >> ind[i].v;
		if (line.peek() == '/')
		{
			//std::cout << "Yep1";
			//removes '/' character
			line.get();
			///@todo handle single vertex index
			if (line.peek() != '/')
			{
				//std::cout << "Yep1";
				line >> ind[i].vt;
				if (line.peek() == '/')
				{
					line.get();
					line >> ind[i].vn;
				}
			}
			else
			{
				//removes '/' character
				line.get();

				line >> ind[i].vn;
			}
		}
	}

	for (auto i : ind)
	{
		storage.push_back(i);
	}
	
}

void OBJLoader::readFloats(std::stringstream &line, std::vector<Vec3> &storage)
{
	float floats[3];

	for (unsigned int i = 0; i < 3; i++)
	{
		line >> floats[i];
	}

	storage.push_back(Vec3(floats[0], floats[1], floats[2]));
}

void OBJLoader::readFloats(std::stringstream &line, std::vector<Vec2> &storage)
{
	float floats[2];

	for (unsigned int i = 0; i < 2; i++)
	{
		line >> floats[i];
	}

	storage.push_back(Vec2(floats[0], floats[1]));
}

bool OBJLoader::load(std::vector<Vertex> &vertexData, std::string filename)
{
	std::ifstream obj;
	obj.open(filename.c_str(), std::fstream::in);
	if (!obj.is_open())
	{
		std::cout << filename.c_str() << " failed to load." << std::endl;
		return false;
	}

	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;


	std::vector<MultiIndicies> tempIndicies;

	std::string curLine;

	while (std::getline(obj, curLine))
	{
		std::stringstream line;
		line.str(curLine);

		std::string prefix;
		line >> prefix;

		Type type = getType(prefix);

		switch (type)
		{
		case V:
			readFloats(line, vertices);
			break;
		case VN:
			readFloats(line, normals);
			break;
		case VT:
			readFloats(line, uvs);
			break;
		case F:
			//NEED to handle (or at least throw a error) different types of faces (TRIs, QUADs ...)
			readFace(line, tempIndicies);
			break;
		case Unhandled:
			/*
			std::cout << "Unhandled Line reached: " << curLine << std::endl;
			std::cout << "Loader will skip the line but the displayed model may be distorted." << std::endl;
			*/
			break;
		}


	}

	//Organise separate vectors into vertex struct

	for (auto ind : tempIndicies)
	{
		Vertex vert;

		vert.v = vertices[ind.v - 1];
		if (ind.vt != 0)
		{
			vert.vt = uvs[ind.vt - 1];
		}
		else
		{
			vert.vt = Vec2(0, 0);
		}
		
		if (ind.vn != 0)
		{
			vert.vn = normals[ind.vn - 1];
		}
		else
		{
			vert.vn = Vec3(0, 0, 0);
		}
		

		vertexData.push_back(vert);
	}

	obj.close();
	return true;
}