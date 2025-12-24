#include "MyMath.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;
using namespace MathUtility;

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation) {
	// スケーリング行列の作成
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	// 回転行列の作成
	Matrix4x4 matRotX = MakeRotateXMatrix(rotation.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列の作成
	Matrix4x4 matTrans = MakeTranslateMatrix(translation);

	// スケーリング、回転、平行移動の合成
	Matrix4x4 matWorld = matScale * matRot * matTrans;

	return matWorld;
}

// イージング
float EaseInOut(float x1, float x2, float t)
{
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

//プレイヤーと敵
bool IsCollition(const AABB& aabb1, const AABB& aabb2) 
{
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
	       (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // z軸
}
//プレイヤーの弾と敵
bool IsCollition2(const AABB2& aabb3, const AABB2& aabb4)
{
	return (aabb3.min.x <= aabb4.max.x && aabb3.max.x >= aabb4.min.x) && // x軸
	       (aabb3.min.y <= aabb4.max.y && aabb3.max.y >= aabb4.min.y) && // y軸
	       (aabb3.min.z <= aabb4.max.z && aabb3.max.z >= aabb4.min.z);   // z軸
}

//敵の弾とプレイヤーの当たり判定
bool IsCollition3(const AABB3& aabb5, const AABB3& aabb6) 
{
	return (aabb5.min.x <= aabb6.max.x && aabb5.max.x >= aabb6.min.x) && // x軸
	       (aabb5.min.y <= aabb6.max.y && aabb5.max.y >= aabb6.min.y) && // y軸
	       (aabb5.min.z <= aabb6.max.z && aabb5.max.z >= aabb6.min.z);   // z軸
}

// 敵の弾とプレイヤーの当たり判定
bool IsCollition4(const AABB4& aabb7, const AABB4& aabb8) 
{
	return (aabb7.min.x <= aabb8.max.x && aabb7.max.x >= aabb8.min.x) && // x軸
	       (aabb7.min.y <= aabb8.max.y && aabb7.max.y >= aabb8.min.y) && // y軸
	       (aabb7.min.z <= aabb8.max.z && aabb7.max.z >= aabb8.min.z);   // z軸
}
