#include "TitleScene.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void TitleScene::Initialize()
{
	// タイトルのスプライト
	//textureHandle_ = TextureManager::Load("title.png");
	//titleSprite_ = KamataEngine::Sprite::Create(textureHandle_, {0, 0});

	//Botan_ = Audio::GetInstance()->LoadWave("Sounds/BossBotan.mp3");

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();


	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update()
{

	switch (phase_) 
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_SPACE))
		{

			//Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTitle_ = true;
		}
		if (Input::GetInstance()->TriggerKey(DIK_T)) 
		{
			//Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedTitle2_ = true;
		}
		break;
	case Phase::kFadeIn:
		// フェード
		fade_->Update();
		if (fade_->IsFinished()) 
		{
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kFadeOut:
		// フェード
		fade_->Update();
		if (fade_->IsFinished())
		{
			finishedTitle_ = true;
		} else 
		{
			finishedTitle2_ = true;
		}
		break;
	}
}

void TitleScene::Draw() 
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	

	Sprite::PreDraw(dxCommon->GetCommandList());

	//titleSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

TitleScene::~TitleScene()
{

	//  フェード
	delete fade_;
	// タイトルのスプライト
	//delete titleSprite_;
}