#include "PlayerBullet.h"
#include "KamataEngine.h"

#include "GameScene.h"
#include "Player.h"

#include <list>
#define NOMINMAX
#include "MapChipField.h"

using namespace KamataEngine;
using namespace MathUtility;

void PlayerBullet::Initialize(KamataEngine::Model* model, Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity)
{
	// NULLポイントチェック
	// assert(model);

	model_ = model;

	camera_ = camera;

	// 速度を引数で受け取って初期化
	velocity_ = velocity;

	// 引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	// ワールド変換データ初期化
	worldTransform_.Initialize();

	// Bulletvelocity_ = velocity;
}

void PlayerBullet::Update()
{
	// 弾を移動
	worldTransform_.translation_ += velocity_;

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // プレイヤーの座標の計算
}

void PlayerBullet::Draw()
{

	// モデルの描画
	model_->Draw(worldTransform_, *camera_);

	// 終了なら何もしない
	if (isFinished_) 
	{
		return;
	}
}

KamataEngine::Vector3 PlayerBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region 弾と敵の衝突

AABB2 PlayerBullet::GetAABB2() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 弾と敵の衝突応答
void PlayerBullet::OnCollition2(const Enemy* enemy) { (void)enemy; }

#pragma endregion

