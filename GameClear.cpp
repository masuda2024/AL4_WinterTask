#include "GameClear.h"



using namespace KamataEngine;

void GameClear::Initialize()
{
	textureHandle_ = TextureManager::Load("GameClear.png");
	clearSprite_ = Sprite::Create(textureHandle_, {0, 0});

	Botan_ = Audio::GetInstance()->LoadWave("Sounds/Decision4.mp3");

	// カメラの初期化
	camera_.Initialize();
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformPlayer_.Initialize();

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameClear::Update()
{
	switch (phase_)
	{
	case Phase::kMain:

		// タイトルシーンの終了条件
		if (Input::GetInstance()->TriggerKey(DIK_E))
		{
			Audio::GetInstance()->PlayWave(Botan_);
			// フェードアウト開始
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			finishedC_ = true;
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
			finishedC_ = true;
		}
		break;
	}
}

void GameClear::Draw() 
{
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	clearSprite_->Draw();

	Sprite::PostDraw();

	// フェード
	fade_->Draw();
}

GameClear::~GameClear()
{

	//  フェード
	delete fade_;
	delete clearSprite_;
}
