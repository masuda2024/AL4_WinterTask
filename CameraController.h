#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
using namespace MathUtility;

class Player;
class CameraController 
{
public:
	void Initialize();

	void Update();

	void Reset();

	Player* target_ = nullptr;

	void SetTarget(Player* target) { target_ = target; }

	// 追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -50.0f};

	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	// 矩形
	struct Rect
	{
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	void SetMovableArea(CameraController::Rect area) { movableArea_ = area; }

	// カメラの座標目標
	KamataEngine::Vector3 targetPosition_;

	// 速度掛け率
	static inline const float kVelocityBias = 20.0f;

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	// 追従対象の各方向へのカメラ移動範囲
	static inline const Rect targetMaegin = {-9.0f, 9.0f, -5.0f, 5.0f};

private:
	KamataEngine::Camera camera_;
};
