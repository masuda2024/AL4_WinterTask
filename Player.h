#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "PlayerBullet.h"
#include <list>
#include "EnemyBullet.h"
class EnemyBullet;
class Enemy;
class Player
{
public:
	// デスフラグ
	bool isDead_ = false;
	// デスフラグのgetter
	// bool IsDead() const { return isDead_; }

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// 角
	enum Corner 
	{
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCorner // 要素数

	};

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);

	// 加速度
	static inline const float kAccleration = 0.1f;
	// 減衰(ブレーキ)
	static inline const float kAttenuation = 0.5f;
	// 制限速度
	static inline const float kLimitRunSpeed = 0.25f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.0f; // 0,1f
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.0f; // 0.5f
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.8f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.9f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.1f;
	// 着地時の速度減衰率
	static inline const float kAttenuationWall = 0.9f;

	// 左右
	enum class LRDirection
	{
		kRight,
		kLeft,
	};

	// 旋回開始時の角度
	float trunFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float trunTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;

	LRDirection lrDirection_ = LRDirection::kRight;

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollition(const Enemy* enemy);

	#pragma region プレイヤーと敵の弾の衝突

	// AABBを取得
	AABB3 GetAABB3();
	// 衝突応答
	void OnCollition3(const EnemyBullet* enemyBullet);

	#pragma endregion



	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	static inline const float kBlank = 0.9f;

	void AnimateTurn();

	// 弾
	//std::list<PlayerBullet*> bullets_;

	// 攻撃
	void Attack();

	~Player();

	// Getter / 状態確認
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return maxHP_; }
	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camera_;
	// テクスチャハンドル
	// uint32_t textureHandle_ = 0u;

	// モデル
	KamataEngine::Model* model_;

	KamataEngine::Vector3 velocity_ = {};

	int maxHP_ = 10;
	int hp_ = maxHP_;
};