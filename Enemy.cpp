#include "Enemy.h"
#include"GameScene.h"
#include"cassert"
#include<numbers>
#include<algorithm>
#include<list>
#define NOMINMAX
#include "MapChipField.h"

#include "EnemyBullet.h"

using namespace KamataEngine;
using namespace MathUtility;

class EnemyBullet;

void Enemy::Initialize(Model* model, Camera* camera, KamataEngine::Vector3& position)
{
	// NULLポイントチェック
	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;

	camera_ = camera;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
	enemyHp = 10000;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;

	worldTransform_.Initialize();

	
	//敵左右移動追加
	
	startX_ = position.x;

	// 最初の「次の行動までの時間」を設定（2〜5秒）
	nextActionTime_ = (rand() % 300) / 6.0f + 2.0f;
	
	//敵左右移動追加終

	shotTimer = 100;

	// 戦闘 - 4 爆発1 
	Explosion_ = Audio::GetInstance()->LoadWave("Sounds/Explosion1.mp3");
}

void Enemy::Update()
{

	// 時間のカウンター
	walkTimer_ += 5.0f / 60.0f; // フレームごとの時間増分

	// 上下に揺れるように移動
	float amplitude = 20.0f; // 上下移動の幅（単位:座標）
	float speed = 0.1f;      // 速さ

	// Y方向にsinで移動
	worldTransform_.translation_.y = sin(walkTimer_ * speed) * amplitude;

#pragma region 敵の移動

	// X,Y をいじるので一旦変数に出す
	Vector3& pos = worldTransform_.translation_;
	Vector3& rot = worldTransform_.rotation_;

	// 状態ごとに処理を分ける
	switch (state_)
	{

	// ───────────────────────────────
	// ① 上下に揺れる状態
	// ───────────────────────────────
	case EnemyState::IdleMove:
		pos.y = sin(walkTimer_ * 0.1f) * 10.0f;
		rot.x = sin(walkTimer_);

		// ランダム行動タイマー
		actionTimer_ += 1.0f / 60.0f;

		// ランダム時間経過したら左移動へ
		if (actionTimer_ >= nextActionTime_) 
		{
			state_ = EnemyState::MoveLeft;
			actionTimer_ = 0;
			// 次の発生時間を再設定
			nextActionTime_ = (rand() % 300) / 60.0f + 2.0f;
		}

		break;

	// ───────────────────────────────
	// ② 左に移動
	// ───────────────────────────────
	case EnemyState::MoveLeft:
		pos.x -= 0.2f; // 左へ移動速度

		if (pos.x <= startX_ - 15.0f) 
		{ // 100だけ左に行ったら右へ戻る
			state_ = EnemyState::MoveRight;
		}
		break;

	// ───────────────────────────────
	// ③ 右に戻る
	// ───────────────────────────────
	case EnemyState::MoveRight:
		pos.x += 0.2f;

		if (pos.x >= startX_) 
		{ // 初期位置まで戻ったら Idle へ
			pos.x = startX_;
			state_ = EnemyState::IdleMove;
		}
		break;
	}

#pragma endregion

	if (enemyHp < 0)
	{
		isenemyDead_ = true;
		
	}

	// プレイヤーの座標の計算
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Enemy::Draw()
{
	if (isenemyDead_) 
	{
		return;
	}
	model_->Draw(worldTransform_, *camera_);
}

void Enemy::AproachPhase()
{
	shotTimer--;
	if (shotTimer == 0)
	{
		shotTimer = kFireInterval;
	}
}




KamataEngine::Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

#pragma region 自キャラと敵

AABB Enemy::GetAABB() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Enemy::OnCollition(const Player* player) { (void)player; }

#pragma endregion

#pragma region 自キャラの弾と敵

AABB2 Enemy::GetAABB2()
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB2 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// 衝突応答
void Enemy::OnCollition2(const PlayerBullet* playerBullet) 
{
	(void)playerBullet;
	enemyHp -= 10;
	hp_ -= 10;
	if (hp_ <= 0) 
	{
		hp_ = 0;
		isenemyDead_ = true;
		Audio::GetInstance()->PlayWave(Explosion_);
	}
}

#pragma endregion
