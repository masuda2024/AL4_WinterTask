#pragma once
#include "EnemyBullet.h"
#include "KamataEngine.h"
#include "MyMath.h"
#include <list>

class Player;
class PlayerBullet;
class Enemy
{
public:
	// デスフラグ
	bool isenemyDead_ = false;
	// デスフラグのgetter
	// bool IsEnemyDead() const { return isenemyDead_; }

	// 敵の当たり判定サイズ
	static inline const float kWidth = 10.0f;
	static inline const float kHeight = 10.0f;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.04f;

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 5.0f;

	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 5.0f;

	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 5.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

#pragma region プレイヤーと敵
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();
	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const Player* player);
#pragma endregion

#pragma region プレイヤーの弾と敵
	// AABBを取得
	AABB2 GetAABB2();
	// 衝突応答
	void OnCollition2(const PlayerBullet* playerBullet);
#pragma endregion






	// 体力表示
	int enemyHp;

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	int E_GetHP() const { return hp_; }
	int E_GetMaxHP() const { return maxHP_; }
	bool IsEnemyDead() const { return isenemyDead_; }



	
	EnemyBullet* E_Bullet_ = nullptr;

	static const int kFireInterval = 60;
	void AproachPhase();



private:
	///////////////
	// 敵左右移動//
	//////////////
	enum class EnemyState 
	{
		IdleMove, // 上下に揺れる状態
		MoveLeft, // 左へ移動
		MoveRight // 右へ戻る
	};

	EnemyState state_ = EnemyState::IdleMove;

	// ランダムで行動するためのタイマー
	float actionTimer_ = 0.0f;
	float nextActionTime_ = 0.0f;

	float startX_; // 初期位置X（戻るため）
	
	// 敵左右移動終
	
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};

	int maxHP_ = 10000;
	int hp_ = maxHP_;

	uint32_t Explosion_ = 0;

	int32_t shotTimer = 0;
	
};