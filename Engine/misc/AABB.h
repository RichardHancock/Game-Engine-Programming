#pragma once

#include <glm/glm.hpp>

/** @brief	An Axis Aligned Bounding Box */
struct AABB
{
public:
	/** @brief	The center of the box. */
	glm::vec3 center;
	/** @brief	The size of the box. */
	glm::vec3 size;
	/** @brief	size / 2. */
	glm::vec3 halfSize;

	/** @brief	The minimum point of the box. */
	glm::vec3 min;
	/** @brief	The maximum point of the box. */
	glm::vec3 max;

	/** @brief	Default constructor. */
	AABB();

	/**
	 @brief	Constructor.
	
	 @param	center	The center point.
	 @param	size  	The size of the box.
	 */
	AABB(glm::vec3 center, glm::vec3 size);

	/**
	 @brief	Query if this object intersects the given other.
	
	 @param	other	The other AABB.
	
	 @return	true if it intersect, else false.
	 */
	bool intersects(AABB other);

	/**
	 @brief	Gets the estimated radius using center and halfsize.
	
	 @return	The radius.
	 */
	float getRadius();
};