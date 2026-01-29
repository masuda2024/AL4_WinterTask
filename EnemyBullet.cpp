#include "EnemyBullet.h"
#include "KamataEngine.h"


#include "Enemy.h"
#include "GameScene.h"

#include <list>
#define NOMINMAX
#include "MapChipField.h"


using namespace KamataEngine;
using namespace MathUtility;

void EnemyBullet::Initialize(KamataEngine::Model* model, Camera* camera, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
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


	//NextActionTime_ = (rand() % 300) / 2.0f + 60.0f;
	// Bulletvelocity_ = velocity;
	isShot = false;
	

	shotT = 100.0f;
	shotC = 110.0f;


}

void EnemyBullet::Update()
{

	
	if (isShot == 0) 
	{
		shotC--;
		if (shotC <= 0.0f) 
		{
			isShot = true;
			shotT = 100.0f;
		}
	}

	if (isShot == 1)
	{
		worldTransform_.translation_ -= velocity_;
		shotT--;
		if (shotT <= 0.0f) 
		{
			isShot = false;
			shotC = 110.0f;
		}
	}

	/*

	
	if (isShot == 1)
	{
		bulletCount++;
		
	}

	if (bulletCount == 3)
	{
		isShot = false;
		shotC--;
		if (shotC == 0.0f)
		{
			shotC = 100.0f;
			isShot = true;
		}
	}
	




	

	



*/

	// 弾を移動
	
	/*
	// 状態ごとに処理を分ける
	switch (bulletState_) 
	{
	case EnemyBulletState::ON:
		
		
		

		// ランダム行動タイマー
		ActionTimer_ += 0.1f / 30.0f;

		isShot = true;

		if (isShot == 1) 
		{
			
		}

		// ランダム時間経過したら左移動へ
		if (ActionTimer_ >= NextActionTime_) 
		{
			bulletState_ = EnemyBulletState::OFF;
			ActionTimer_ = 0;
			// 次の発生時間を再設定
			NextActionTime_ = (rand() % 300) / 30.0f + 2.0f;
		}

		break;
	case EnemyBulletState::OFF:

		ActionTimer_ += 0.1f / 30.0f;
		if (ActionTimer_ >= NextActionTime_) 
		{
			bulletState_ = EnemyBulletState::ON;
			ActionTimer_ = 0;
			// 次の発生時間を再設定
			NextActionTime_ = (rand() % 300) / 30.0f + 2.0f;
		}
		break;
	}
*/

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix(); // 敵の座標の計算
}

void EnemyBullet::Draw() 
{
	/*
	if (bulletState_ == EnemyBulletState::ON)
	{
		

	}*/
	
	// モデルの描画
	model_->Draw(worldTransform_, *camera_);
	// 終了なら何もしない
	if (isFinished_)
	{
		return;
	}
}

KamataEngine::Vector3 EnemyBullet::GetWorldPosition()
{
	// ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


#pragma region 敵の弾とプレイヤーの当たり判定

AABB3 EnemyBullet::GetAABB3() 
{
	KamataEngine::Vector3 worldPos = GetWorldPosition();

	AABB3 aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

// プレイヤーと敵の弾の衝突
void EnemyBullet::OnCollition3(const Player* player)
{ 
	(void)player; 
}

#pragma endregion
