#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize()
{
	camera_.Initialize();
}

void CameraController::Update() 
{

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
	const Vector3& targetVelocity = target_->GetVelocity();
	targetPosition_ = targetWorldTransform.translation_ + targetVelocity * kVelocityBias;

	// 追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	// targetPosition_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	// 座標補間によりゆったり追従
	camera_.translation_.x = Lerp(camera_.translation_.x, targetPosition_.x, kInterpolationRate);

	// 追従対象が画面外に出内容に補正
	camera_.translation_.x = max(camera_.translation_.x, targetPosition_.x + targetMaegin.left);
	camera_.translation_.x = min(camera_.translation_.x, targetPosition_.x + targetMaegin.right);
	camera_.translation_.y = max(camera_.translation_.y, targetPosition_.y + targetMaegin.bottom);
	camera_.translation_.y = min(camera_.translation_.y, targetPosition_.y + targetMaegin.top);

	// 移動範囲制限
	camera_.translation_.x = max(camera_.translation_.x, movableArea_.left);
	camera_.translation_.x = min(camera_.translation_.x, movableArea_.right);
	camera_.translation_.y = max(camera_.translation_.y, movableArea_.bottom);
	camera_.translation_.y = min(camera_.translation_.y, movableArea_.top);

	// 行列を更新する
	camera_.UpdateMatrix();
}

void CameraController::Reset()
{
	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_.translation_ = targetWorldTransform.translation_ + targetOffset_;
}
