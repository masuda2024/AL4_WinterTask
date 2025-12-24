#pragma once
#include "KamataEngine.h"
#include "MyMath.h"
#include "Player.h"
#include "PlayerBullet.h"


class Player;
class PlayerBullet;
class EnemyBullet 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void Update();

	void Draw();

	// 終了フラグ
	bool isFinished_ = false;

	// デスフラグ
	bool isEBDead_ = false;

	bool IsEBDead() const { return isEBDead_; }

	// 速度
	KamataEngine::Vector3 velocity_;

	// int E_Shot = false;

	// 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();



#pragma region プレイヤーと敵の弾の衝突

	// AABBを取得
	AABB3 GetAABB3();
	// 衝突応答
	void OnCollition3(const Player* player);

#pragma endregion



private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_;

	// カメラ
	KamataEngine::Camera* camera_;

	// 速度
	KamataEngine::Vector3 Bulletvelocity_;
};
