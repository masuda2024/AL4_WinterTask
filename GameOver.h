#pragma once
#include"KamataEngine.h"
#include"Fade.h"
class GameOver
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedO_ = false;
	// デスフラグのgetter
	bool IsFinishedO() const { return finishedO_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~GameOver();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* overSprite_ = nullptr;

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;
	// モデルプレイヤー
	// KamataEngine::Model* modelPlayer_ = nullptr;
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	//uint32_t Botan_ = 0;
};
