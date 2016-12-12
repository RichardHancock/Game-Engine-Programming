#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Vertex.h"
#include "Vec2.h"
#include "Vec3.h"

///Contains methods used to load an OBJ File (no MTL support (MTL lines are ignored))
namespace OBJLoader
{
	/** @brief A struct that contains all the indices for a point (equivalent to a (num/num/num) in a face). */
	struct MultiIndicies
	{
		unsigned int v; //< Vertex Position index
		unsigned int vn; //< Vertex Normal index
		unsigned int vt; //< Vertex Texture Position index
	};

	/** @brief The types of prefixes that an OBJ file can have */
	enum Type
	{
		V,
		VP,
		F,
		VN,
		VT,
		Ignored = -1, //< Comments etc...
		Unhandled = -2, //< Types that might get handled in the future (MTL format)
	};

	/**
	 @brief Determines which prefix was passed in.
	
	 @param typeToConvert The prefix to define.
	
	 @return The type.
	 */
	Type getType(std::string typeToConvert);

	/**
	 @brief Reads a face.
	
	 @param [in,out] line    The current line.
	 @param [in,out] storage The vector the data will be stored in.
	 */
	void readFace(std::stringstream &line, std::vector<MultiIndicies> &storage);

	/**
	 @brief Reads 3 floats from a line.
	
	 @param [in,out] line     The current line.
	 @param [in,out] storage The vector the data will be stored in.
	 */
	void readFloats(std::stringstream &line, std::vector<Vec3> &storage);

	/**
	@brief Reads 2 floats from a line.

	@param [in,out] line     The current line.
	@param [in,out] storage The vector the data will be stored in.
	*/
	void readFloats(std::stringstream &line, std::vector<Vec2> &storage);

	/**
	 @brief Loads and processes an OBJ file and returns the Vertices.
	
	 @param [in,out] vertexData Array of vertices.
	 @param filename		    Filename of the file.
	
	 @return true if it succeeds, false if it fails.
	 */
	bool load(std::vector<Vertex> &vertexData, std::string filename);
}