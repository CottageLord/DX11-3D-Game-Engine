#pragma once

#include "GRAPHICS_OBJ_Surface.h"
#include <string>
#include <DirectXMath.h>


class TexturePreprocessor
{
public:
	static void FlipYAllNormalMapsInObj(const std::string& objPath);
	static void FlipYNormalMap(const std::string& pathIn, const std::string& pathOut);
	/**
	* @brief validate the input normal map for vector length, z direction, and
	*/
	static void ValidateNormalMap(const std::string& pathIn, float thresholdMin, float thresholdMax);
	static void MakeStripes(const std::string& pathOut, int size, int stripeWidth);
private:
	template<typename F>
	static void TransformFile(const std::string& pathIn, const std::string& pathOut, F&& func);
	template<typename F>
	static void TransformSurface(GPipeline::Surface& surf, F&& func);
	static DirectX::XMVECTOR ColorToVector(GPipeline::Surface::Color c) noexcept;
	static GPipeline::Surface::Color VectorToColor(DirectX::FXMVECTOR n) noexcept;
};