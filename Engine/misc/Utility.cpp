#include "Utility.h"

#include <sstream>

#include "Log.h"
#include "DeltaTime.h"

//Credit to Jamie for below 2 functions
std::string getOpenGLErrorString(GLenum errorCode)
{
	std::string errorMessage;
	switch (errorCode)
	{
	case GL_INVALID_ENUM:
		errorMessage = "GL_INVALID_ENUM (" + std::to_string(errorCode) + ")";
		break;
	case GL_INVALID_VALUE:
		errorMessage = "GL_INVALID_VALUE (" + std::to_string(errorCode) + ")";
		break;
	case GL_INVALID_OPERATION:
		errorMessage = "GL_INVALID_OPERATION (" + std::to_string(errorCode) + ")";
		break;
	case GL_STACK_OVERFLOW:
		errorMessage = "GL_STACK_OVERFLOW (" + std::to_string(errorCode) + ")";
		break;
	case GL_STACK_UNDERFLOW:
		errorMessage = "GL_STACK_UNDERFLOW (" + std::to_string(errorCode) + ")";
		break;
	case GL_OUT_OF_MEMORY:
		errorMessage = "GL_OUT_OF_MEMORY (" + std::to_string(errorCode) + ")";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorMessage = "GL_INVALID_FRAMEBUFFER_OPERATION (" + std::to_string(errorCode) + ")";
		break;
	case GL_CONTEXT_LOST:
		errorMessage = "GL_CONTEXT_LOST (" + std::to_string(errorCode) + ")";
		break;
	case GL_TABLE_TOO_LARGE:
		errorMessage = "GL_TABLE_TOO_LARGE (" + std::to_string(errorCode) + ")";
		break;
	default:
		//return the number if something else
		errorMessage = "UNHANDLED ENUM (" + std::to_string(errorCode) + ")";
		break;
	}
	return errorMessage;
}

void logOpenGLError(std::string file, int lineNum)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		//Process/log the error.
		Log::logE("[OPENGL] Location: " + file + ", Line " + std::to_string(lineNum) + " | Type: " + getOpenGLErrorString(errorCode));
	}
}

std::string Utility::intToString(int num)
{
	std::stringstream stream;

	stream << num;
	std::string result = stream.str();

	return result;

}

std::string Utility::intToString(unsigned int num)
{
	std::stringstream stream;

	stream << num;
	std::string result = stream.str();

	return result;
}

std::string Utility::intToString(Uint8 num)
{
	std::stringstream stream;

	stream << (int)num;
	std::string result = stream.str();

	return result;
}

std::string Utility::floatToString(float num)
{
	std::stringstream stream;

	stream << num;
	std::string result = stream.str();

	return result;

}

std::string Utility::floatToString(float num, unsigned int precision)
{
	std::stringstream stream;
	stream.precision(precision);
	stream << std::fixed << num;
	std::string result = stream.str();

	return result;
}

std::string Utility::vec2ToString(Vec2 num)
{
	std::stringstream stream;

	stream << num.x;
	stream << ", ";
	stream << num.y;
	std::string result = stream.str();

	return result;

}

std::string Utility::glmToString(glm::vec2 & vec)
{
	return std::string("glm::vec2(" + floatToString(vec.x) + ", " + floatToString(vec.y) + ")");
}

std::string Utility::glmToString(glm::vec3 & vec)
{
	return std::string("glm::vec3(" + floatToString(vec.x) + ", " + floatToString(vec.y) + ", " + floatToString(vec.z) + ")");
}

std::string Utility::glmToString(glm::vec4 & vec)
{
	return std::string("glm::vec3(" + floatToString(vec.x) + ", " + floatToString(vec.y) + ", " + floatToString(vec.z) + ", " + floatToString(vec.w) + ")");
}

glm::vec3 Utility::vec3ToGLM(Vec3 vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

glm::vec3 Utility::bulletVec3ToGLM(const btVector3& vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}

glm::vec3 Utility::bulletVec3ToGLM(btVector3& vector)
{
	return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
}

btVector3 Utility::glmToBulletVec3(glm::vec3 & vec3)
{
	return btVector3(vec3.x, vec3.y, vec3.z);
}

Vec2 Utility::scaleTo(Vec2 vecToScale, Vec2 screenSize)
{
	vecToScale.x /= 640;
	vecToScale.y /= 480;

	return (vecToScale * screenSize);
}

int Utility::fontSizeScale(int intToScale, Vec2 screenSize)
{
	int percentageChange = (int)(screenSize.x / 640.0f);

	return (intToScale * percentageChange);
}


float Utility::normaliseBetweenMinusOneAndOne(float numberToNormalise, float max, float min)
{
	//find the new max and min number when shifted to a positive only range and the shifted number to normalise
	max += std::abs(min);
	numberToNormalise += std::abs(min);
	min = 0.0f;

	//normalise the number between zero and one
	float normalisedNumber = normaliseFloat(numberToNormalise, max, min);

	//shift it to between -0.5f and 0.5f
	normalisedNumber -= 0.5f;

	//scale it to between -1.0f and 1.0f
	normalisedNumber *= 2.0f;

	//return the normalised number
	return normalisedNumber;
}


float Utility::normaliseFloat(float numberToNormalise, float max, float min)
{
	//normalise the number between zero and one
	float normalisedNumber = (numberToNormalise - min) / (max - min);

	//return the normalised number
	return normalisedNumber;
}


Vec2 Utility::getRectCenter(SDL_Rect rect)
{
	Vec2 center;
	center.x = (float)(rect.x + (rect.w / 2));
	center.y = (float)(rect.y + (rect.h / 2));
	return center;
}

bool Utility::lineIntersection(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2)
{
	Vec2 b = a2 - a1;
	Vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	Vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	return true;
}

bool Utility::lineIntersection(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2, Vec2& intersectPoint)
{
	intersectPoint = 0;

	Vec2 b = a2 - a1;
	Vec2 d = b2 - b1;
	float bDotDPerp = b.x * d.y - b.y * d.x;

	// if b dot d == 0, it means the lines are parallel so have infinite intersection points
	if (bDotDPerp == 0)
		return false;

	Vec2 c = b1 - a1;
	float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
	if (t < 0 || t > 1)
		return false;

	float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
	if (u < 0 || u > 1)
		return false;

	intersectPoint = a1 + b * t;

	return true;
}

bool Utility::lineRectIntersection(Vec2 lineP1, Vec2 lineP2, SDL_Rect rect)
{
	//Could have used this instead: https://wiki.libsdl.org/SDL_IntersectRectAndLine

	//tl - top left/ tr - top right/ bl - bottom left/ br - bottom right
	Vec2 tl((float)rect.x, (float)rect.y);
	Vec2 tr = tl;
	tr.x += rect.w;

	Vec2 bl = tl;
	bl.y += rect.h;
	Vec2 br = tr;
	br.y += rect.h;

	if (
		lineIntersection(lineP1, lineP2, tl, tr) ||
		lineIntersection(lineP1, lineP2, tr, br) ||
		lineIntersection(lineP1, lineP2, br, bl) ||
		lineIntersection(lineP1, lineP2, bl, tl)
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}



SDL_Colour Utility::newSDLColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_Colour colour;
	colour.r = r;
	colour.g = g;
	colour.b = b;
	colour.a = a;
	return colour;
}

glm::vec3 Utility::calculateFaceNormal(glm::vec3 & a, glm::vec3 & b, glm::vec3 & c)
{
	return glm::cross((b - a),(c - a));
}



//Timer

std::unordered_map<std::string, Utility::Timer::TimerStruct> Utility::Timer::timers;
const float Utility::Timer::TIME_INTERVAL = 1.0f;

Utility::Timer::Timer() {}

void Utility::Timer::update()
{
	//Need auto& to get a reference otherwise it's just a local copy (Really Fun Bug to Find)
	for (auto& timer : timers)
	{
		timer.second.currentTime += (float)(TIME_INTERVAL * DeltaTime::getDT());
	}
}

bool Utility::Timer::createTimer(std::string id, float duration)
{
	//Unordered map can't have duplicates
	if (timers.count(id) == 1)
	{
		Log::logW("Timer id specified already exists");
		return false;
	}

	//Create the Timer
	TimerStruct timer = {0.0f, duration};

	//Push it into the timers unordered_map
	timers.insert(std::make_pair(id,timer));

	return true;
}

float Utility::Timer::stopTimer(std::string id)
{
	if (timers.count(id) == 0) { return -1.0f; }

	float finalCurrentTime = timers[id].currentTime;

	timers.erase(id);

	return finalCurrentTime;
}

bool Utility::Timer::hasTimerFinished(std::string id)
{
	if (!timers.count(id))
	{
		Log::logW("Unknown timer was asked if finished. Timer ID:" +id);
		return false;
	}

	TimerStruct timer = timers[id];
	if (timer.currentTime >= timer.duration)
	{
		//Could call stop timer but would have an extra id check (Which is wasteful)
		timers.erase(id);
		return true;
	}
	else
	{
		return false;
	}
}

void Utility::Timer::cleanup()
{
	timers.clear();
}


Utility::SimpleTimer::SimpleTimer()
	: TIME_INTERVAL(1.0f)
{
}

//Simple Timer
Utility::SimpleTimer::SimpleTimer(float duration)
	: TIME_INTERVAL(1.0f)
{
	restart(duration);
}

void Utility::SimpleTimer::restart()
{
	running = true;
	timer.currentTime = 0.0f;
}

void Utility::SimpleTimer::restart(float duration)
{
	running = true;
	timer.currentTime = 0.0f;
	timer.duration = duration;
}

bool Utility::SimpleTimer::hasTimerFinished()
{
	if (timer.currentTime >= timer.duration)
	{
		return true;
	}
	return false;
}

void Utility::SimpleTimer::update()
{
	if (running)
	{
		timer.currentTime += DeltaTime::getDT();

		if (timer.currentTime >= timer.duration)
		{
			running = false;
		}
	}
}

float Utility::convertAngleToRadian(float angle)
{
	/*return the radian*/
	return (angle * PI) / 180.0f;
}

float Utility::convertRadiansToAngle(float radians)
{
	/*return the angle*/
	return radians * (180.0f / PI);
}
