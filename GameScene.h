#pragma once
#include"KamataEngine.h"


#include <Windows.h>
#include <vector>
#define NOMINMAX
#include <list>
#include <algorithm>

#include "Fade.h"

#include "CameraController.h"

#include "Player.h"
#include "PlayerBullet.h"
#include "PlayerDeathParticle.h"

#include "Enemy.h"
#include "EnemyBullet.h"
#include "EnemyDeathParticle.h"

#include "Skydome.h"




#pragma region
#pragma endregion



class GameScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	// デストラクタ
	~GameScene();


	
	// 終了フラグ
	bool finishedGAME_ = false;
	// デスフラグのgetter
	bool IsFinishedGAME() const { return finishedGAME_; } // ゲームオーバー

	// 敵を倒した場合のフラグ
	bool finishedGAME2_ = false;
	bool IsFinishedGAME2() const { return finishedGAME2_; } ////ゲームクリア





private:


	#pragma region プレイヤー
	
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	

	#pragma region プレイヤーの弾
	// 自キャラの弾
	KamataEngine::Model* modelPlayerBullet_ = nullptr;
	// 弾
	std::list<PlayerBullet*> bullets_;
	// 速度
	KamataEngine::Vector3 velocity_;
	// 弾の寿命(フレーム数)
	int playerBulletLifeTime = 20; // 変更可能な左辺値にするためconstを外し型をintに変更    // スペースキーを押して弾を撃つ

	#pragma endregion 

	// プレイヤーの弾の生成
	PlayerBullet* playerBullet_ = nullptr;
	void PlayerAttack();

	// プレイヤーのパーティクル
	PlayerDeathParticle* P_Particles_ = nullptr;
	KamataEngine::Model* model_P_Particle_ = nullptr;

	// プレイヤーHPテクスチャハンドル
	uint32_t playerhpHandle_ = 0;
	// スプライト
	Sprite* playerhpSprite_ = nullptr;

	uint32_t _playerhpHandle_ = 0;
	Sprite* _playerhpSprite_ = nullptr;
	#pragma endregion

	#pragma region 敵

	// 敵
	Enemy* enemy_ = nullptr;
	KamataEngine::Model* modelEnemy_ = nullptr;

	#pragma region 敵の弾
	// 敵の弾
	KamataEngine::Model* modelEnemyBullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> E_bullets_;
	
	
	
	// 速度
	KamataEngine::Vector3 EnemyBulletVelocity_;
	// 弾の寿命(フレーム数)
	int enemyBulletLifeTime = 20; 

	#pragma endregion

	// 敵の弾の生成
	EnemyBullet* enemyBullet_ = nullptr;
	void EnemyAttack();

	// 敵のパーティクル
	EnemyDeathParticle* E_Particles_ = nullptr;
	KamataEngine::Model* model_E_Particle_ = nullptr;

	// 敵HPテクスチャハンドル
	uint32_t enemyhpHandle_ = 0;
	// スプライト
	Sprite* enemyhpSprite_ = nullptr;
	
	uint32_t _enemyhpHandle_ = 0;
	Sprite* _enemyhpSprite_ = nullptr;
	#pragma endregion 



    
	//スカイドーム
	Skydome* skydome_ = nullptr;
	KamataEngine::Model* modelskydome_ = nullptr;



	#pragma region システム

	

	// 全ての当たり判定
	void CheckAllCollisions();

	// ゲームのフェーズ(型)
	enum class Phase
	{
		kFadeIn,       // フェードイン
		kPlay,         // ゲームプレイ
		kDeath,        // プレイヤーのデス演出
		kEnemyDeath,   // 敵のデス演出
		kFadeOut,      // フェードアウト(オーバー)
		kFadeOut2,     // フェードアウト(クリア)
	};

	// ゲームの現在フェーズから開始
	Phase phase_;

	// フェーズの切り替え
	void ChangePhase();

	// フェード
	Fade* fade_ = nullptr;


	#pragma endregion

	// カメラコントロール
	CameraController* cameraController_ = nullptr;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;


	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;


	uint32_t P_Cannon_ = 0;
	

};