#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "Utility.h"
#include <box2d/b2_math.h>
#include <sstream>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <locale>
#include <codecvt>
#include <string>

std::string Utility::ToString(const b2Vec2 & Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec4& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " , " << Vector.w << " )";
	return StringStream.str();
}

std::string Utility::ToString(const glm::vec3& Vector)
{
	std::ostringstream StringStream;
	StringStream << "( " << Vector.x << " , " << Vector.y << " , " << Vector.z << " )";
	return StringStream.str();
}

std::wstring Utility::StringToWString(const std::string& String)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return (converter.from_bytes(String));
}
std::string Utility::WStringToString(const std::wstring& String)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return (converter.to_bytes(String));
}

float Utility::AngleBetween(glm::vec2 From, glm::vec2 To)
{
	return atan2(To.y - From.y, To.x - From.x);
}