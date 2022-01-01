#pragma once
#include <DirectXMath.h>

/** 
  * @brief given transform matrix, return euler angle matrix
  */
DirectX::XMFLOAT3 ExtractEulerAngles(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMFLOAT3 ExtractTranslation(const DirectX::XMFLOAT4X4& matrix);

DirectX::XMMATRIX ScaleTranslation(DirectX::XMMATRIX matrix, float scale);