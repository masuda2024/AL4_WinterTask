#include "GameScene.h"
#include "MyMath.h"
#include "CameraController.h"
#include "Fade.h"

#include "Player.h"
#include "PlayerBullet.h"

#include "Enemy.h"
#include "EnemyBullet.h"

#include"imgui.h"


using namespace KamataEngine;

#pragma region
#pragma endregion




void GameScene::Initialize()
{
	#pragma region
	
	#pragma endregion


	//サウンド
	//Springin
	
	//戦闘-4 大砲1
	P_Cannon_ = Audio::GetInstance()->LoadWave("Sounds/Cannon1.mp3");
	

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(100, 200);

	//スカイドーム
	modelskydome_ = Model::CreateFromOBJ("SkyDome", true);


	#pragma region プレイヤー
	

	// playerHPのスプライト
	playerhpHandle_ = TextureManager::Load("hp.png");
	playerhpSprite_ = KamataEngine::Sprite::Create(playerhpHandle_, {0, 680});

	_playerhpHandle_ = TextureManager::Load("hp_.png");
	_playerhpSprite_ = KamataEngine::Sprite::Create(_playerhpHandle_, {0, 680});




	//プレイヤー
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	
	// プレイヤーの弾
	modelPlayerBullet_ = Model::CreateFromOBJ("bullet", true);
	// パーティクルの3Dモデルデータの生成
	model_P_Particle_ = Model::CreateFromOBJ("deathParticle", true);

	// プレイヤーの生成
	player_ = new Player();
	
	
	// プレイヤーの座標を指定
	KamataEngine::Vector3 playerPosition = {-10, 1, 0};
	player_->Initialize(modelPlayer_, &camera_, playerPosition);
	

	// プレイヤーの弾
	playerBullet_ = new PlayerBullet();
	playerBullet_->Initialize(modelPlayerBullet_, &camera_, playerPosition, velocity_);

	// プレイヤーのデスパーティクル
	P_Particles_ = new PlayerDeathParticle();
	P_Particles_->Initialize(model_P_Particle_, &camera_, playerPosition);
	


	#pragma endregion


	#pragma region 敵

	// enemyHPのスプライト
	enemyhpHandle_ = TextureManager::Load("Ehp.png");
	enemyhpSprite_ = KamataEngine::Sprite::Create(enemyhpHandle_, {1050, 0});

	_enemyhpHandle_ = TextureManager::Load("Ehp_.png");
	_enemyhpSprite_ = KamataEngine::Sprite::Create(_enemyhpHandle_, {1050, 0});



	// 敵の3Dモデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	// 敵の弾
	modelEnemyBullet_ = Model::CreateFromOBJ("E_bullet", true);

	model_E_Particle_ = Model::CreateFromOBJ("E_deathParticle", true);

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の座標
	KamataEngine::Vector3 enemyPosition = {35, 5, 0};
	enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);

	// 敵の弾
	enemyBullet_ = new EnemyBullet();
	enemyBullet_->Initialize(modelEnemyBullet_, &camera_, enemyPosition, EnemyBulletVelocity_);

	// 敵のデスパーティクル
	E_Particles_ = new EnemyDeathParticle();
	E_Particles_->Initialize(model_E_Particle_, &camera_, enemyPosition);
	
	#pragma endregion


	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelskydome_, textureHandle_, &camera_);



	// カメラコントローラの初期化
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);


	// フェーズインから開始
	phase_ = Phase::kFadeIn;

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}


void GameScene::Update()
{
	// フェード
	fade_->Update();

	//デバッグカメラの更新
	debugCamera_->Update();

	

	#pragma region プレイヤー

	// プレイヤーHP
	float hpRatio = (float)player_->GetHP() / (float)player_->GetMaxHP();
	hpRatio = std::clamp(hpRatio, 0.0f, 1.0f);
	playerhpSprite_->SetSize({hpRatio * 300.0f, 30.0f}); // 例：幅200px、高さ20px
	playerhpSprite_->SetPosition({0, 0});         
	
	_playerhpSprite_->SetSize({300.0f, 30.0f}); // 例：幅200px、高さ20px
	_playerhpSprite_->SetPosition({0, 0});      


	player_->Update();
	//プレイヤーの攻撃を呼び出す
	PlayerAttack();
	//プレイヤーの弾を更新
	for (PlayerBullet* bullet : bullets_) 
	{
		bullet->Update();
	}
	#pragma endregion

	#pragma region 敵

	// 敵HP
	float enemyHpRatio = (float)enemy_->E_GetHP() / (float)enemy_->E_GetMaxHP();
	enemyHpRatio = std::clamp(enemyHpRatio, 0.0f, 1.0f);
	enemyhpSprite_->SetSize({enemyHpRatio * 300.0f, 30.0f}); // 幅200px、高さ20px
	enemyhpSprite_->SetPosition({980, 0});                 // 左上少し下に表示

	_enemyhpSprite_->SetSize({300.0f, 30.0f}); // 幅200px、高さ20px
	_enemyhpSprite_->SetPosition({980, 0});                 // 左上少し下に表示


	enemy_->Update();
	EnemyAttack();
	// 敵の弾を更新
	
	for (EnemyBullet* Ebullet : E_bullets_) 
	{
		Ebullet->Update();
	}
	
	



#pragma endregion

	// 天球の更新
	skydome_->Update();

	
	switch (phase_)
	{
	case Phase::kPlay:

		// 全ての当たり判定
		CheckAllCollisions();

		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
			//Audio::GetInstance()->PlayWave(PBSound_);
		}

		// ゲームプレイフェーズの処理
		if (player_->IsDead() == true)
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			P_Particles_ = new PlayerDeathParticle();
			P_Particles_->Initialize(model_P_Particle_, &camera_, deathParticlesPosition);
		}
		if (enemy_->IsEnemyDead() == true) 
		{
			// デス演出フェーズに切り替え
			phase_ = Phase::kEnemyDeath;

			// 敵の座標を取得
			const KamataEngine::Vector3 E_deathParticlesPosition = enemy_->GetWorldPosition();

			// パーティクル
			E_Particles_ = new EnemyDeathParticle();
			E_Particles_->Initialize(model_E_Particle_, &camera_, E_deathParticlesPosition);
		}
		
		break;

	case Phase::kDeath:
		// デス演出フェーズの処理

		// デスパーティクルの更新
		P_Particles_->Update();
		if (P_Particles_ && P_Particles_->isFinished_)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;

	case Phase::kEnemyDeath:

		// デスパーティクルの更新
		E_Particles_->Update();
		if (E_Particles_ && E_Particles_->isFinished_)
		{
			// フェードアウト開始
			phase_ = Phase::kFadeOut2;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;

	

	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kPlay;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			finishedGAME_ = true;
		}
		break;
	case Phase::kFadeOut2:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedGAME2_ = true;
		}
		break;
	}
	
	
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0))
	{
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG

	if (isDebugCameraActive_)
	{
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else	
	{
		camera_.UpdateMatrix();
		
	}
	
}


void GameScene::Draw()
{
	Sprite::PreDraw();
	
	_playerhpSprite_->Draw();
	playerhpSprite_->Draw();

	
	_enemyhpSprite_->Draw();
	enemyhpSprite_->Draw();


	Sprite::PostDraw();


	Model::PreDraw();
	#pragma region プレイヤー

	// 自キャラの描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kEnemyDeath)
	{
		player_->Draw();
	}
	// パーティクル(プレイヤー)
	if (phase_ == Phase::kDeath)
	{
		if ("deathParticle", true)
		{
			P_Particles_->Draw();
		}
	}

	if (phase_ == Phase::kPlay) 
	{
		// スペースキーを押して弾を撃つ
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) 
		{
			playerBulletLifeTime--;
		}

		// 弾の継続時間が0になるまで撃てる
		if (playerBulletLifeTime > 0)
		{
			for (PlayerBullet* bullet : bullets_)
			{
				bullet->Draw();
			}
		}

		// 弾の継続時間が0になったら継続時間をリセットする
		if (playerBulletLifeTime <= 0)
		{
			// delete playerBullet_;
			bullets_.clear();
			playerBulletLifeTime = 2;
		}
	}

	
	#pragma endregion

	#pragma region 敵

	// 敵の描画 下記のフェーズのみ描画
	if (phase_ == Phase::kPlay || phase_ == Phase::kFadeIn || phase_ == Phase::kDeath)
	{
		enemy_->Draw();
	}
	// パーティクル(敵)
	if (phase_ == Phase::kEnemyDeath)
	{
		if ("E_deathParticle", true) 
		{
			E_Particles_->Draw();
		}
	}
	// 弾の継続時間が0になるまで表示
	if (phase_ == Phase::kPlay)
	{
		enemyBulletLifeTime--;
		// 弾の継続時間が0になるまで撃てる
		if (enemyBulletLifeTime > 0)
		{
			
			for (EnemyBullet* Ebullet : E_bullets_)
			{
				Ebullet->Draw();
			}
			
		}

		// 弾の継続時間が0になったら継続時間をリセットする
		if (enemyBulletLifeTime <= 0)
		{

			E_bullets_.clear();
			
			enemyBulletLifeTime = 200;
		}
	}

	#pragma endregion

	skydome_->Draw();

	Model::PostDraw();


}




// プレイヤーの攻撃
void GameScene::PlayerAttack()
{
	// スペースキーを押して弾を撃つ
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		Audio::GetInstance()->PlayWave(P_Cannon_);
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = {kBulletSpeed, 0.0f, 0.0f};

		// 自キャラの座標を取得(弾を自キャラと同じ位置にする)
		const KamataEngine::Vector3 playerBulletPosition = player_->GetWorldPosition();

		playerBullet_ = new PlayerBullet();
		playerBullet_->Initialize(modelPlayerBullet_, &camera_, playerBulletPosition, velocity);

		bullets_.push_back(playerBullet_);
	}
}

// 敵の攻撃
void GameScene::EnemyAttack()
{
	// 弾の速度
	const float kEBulletSpeed = 1.0f;
	Vector3 E_bulletVelocity = {kEBulletSpeed, 0.0f, 0.0f};

	// 座標を取得(弾を自キャラと同じ位置にする)
	const KamataEngine::Vector3 enemyBulletPosition = enemy_->GetWorldPosition();

	enemyBullet_ = new EnemyBullet();
	enemyBullet_->Initialize(modelEnemyBullet_, &camera_, enemyBulletPosition, E_bulletVelocity);

	E_bullets_.push_back(enemyBullet_);
}

// フェーズ
void GameScene::ChangePhase() 
{

	switch (phase_)
	{
	case Phase::kPlay:
// ゲームプレイフェーズの処理
#pragma region プレイヤー
		if (player_->IsDead() == true)
		{
			
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const KamataEngine::Vector3 deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル
			P_Particles_ = new PlayerDeathParticle();
			P_Particles_->Initialize(model_P_Particle_, &camera_, deathParticlesPosition);
		}
#pragma endregion

#pragma region 敵
		if (enemy_->IsEnemyDead() == true)
		{
			
			// デス演出フェーズに切り替え
			phase_ = Phase::kEnemyDeath;
		}
#pragma endregion

		break;

	case Phase::kDeath:
		// デス演出フェーズの処理

		if (P_Particles_)
		{
			// シーン終了
			finishedGAME_ = true;
		}

		break;
	case Phase::kEnemyDeath:

		// シーン終了
		finishedGAME2_ = true;

		break;
	}
}


void GameScene::CheckAllCollisions()
{
#pragma region プレイヤーと敵
	// 判定対象1と2の座標
	AABB aabb1, aabb2;
	//  プレイヤー
	aabb1 = player_->GetAABB();
	// 敵
	aabb2 = enemy_->GetAABB();
	// AABB同士の交差判定
	if (IsCollition(aabb1, aabb2))
	{
		//  プレイヤーの衝突時関数を呼び出す
		player_->OnCollition(enemy_);
		// 敵の衝突時関数を呼び出す
		enemy_->OnCollition(player_);
	}
#pragma endregion

#pragma region プレイヤーの弾と敵
	// 判定対象1と2の座標
	AABB2 aabb3, aabb4;
	// 敵
	aabb3 = enemy_->GetAABB2();

	for (PlayerBullet* bullet : bullets_) 
	{
		// プレイヤーの弾
		aabb4 = bullet->GetAABB2();
		if (IsCollition2(aabb3, aabb4)) 
		{
			// 自キャラの衝突時関数を呼び出す
			bullet->OnCollition2(enemy_);
			// 敵の衝突時関数を呼び出す
			enemy_->OnCollition2(bullet);
		}
	}
#pragma endregion

#pragma region 敵の弾とプレイヤー

	// 判定対象1と2の座標
	AABB3 aabb5, aabb6;

	aabb5 = player_->GetAABB3();
	/**/
	for (EnemyBullet* Ebullet : E_bullets_) 
	{

		// 敵の弾
		aabb6 = Ebullet->GetAABB3();
		if (IsCollition3(aabb5, aabb6))
		{

			Ebullet->OnCollition3(player_);

			player_->OnCollition3(Ebullet);
		}
	}
#pragma endregion
}



GameScene::~GameScene()
{ 
	delete sprite_;

	delete player_;
	for (PlayerBullet* bullet : bullets_) 
	{
		delete bullet;
	}
	delete P_Particles_;


	delete enemy_;
	/*
	for (EnemyBullet* Ebullet : E_bullets_) 
	{
		delete Ebullet;
	}*/
	
	delete enemyBullet_;
	delete E_Particles_;

	delete skydome_;


	// フェード
	delete fade_;

	// 3Dモデルデータの解放
	delete model_;

	// デバッグカメラの解放
	delete debugCamera_;
}
