#pragma once

#include "KamataEngine.h"

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation);

float EaseInOut(float x1, float x2, float t);

struct AABB
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition(const AABB& aabb1, const AABB& aabb2);

struct AABB2 
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition2(const AABB2& aabb3, const AABB2& aabb4);

struct AABB3 
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition3(const AABB3& aabb5, const AABB3& aabb6);

struct AABB4
{
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};
bool IsCollition4(const AABB4& aabb7, const AABB4& aabb8);
