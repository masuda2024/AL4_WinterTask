#pragma once
#include"KamataEngine.h"
#include "Fade.h"


class TitleScene 
{
public:
	enum class Phase 
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedTitle_ = false;
	bool IsFinishedT() const { return finishedTitle_; }

	bool finishedTitle2_ = false;
	bool IsFinishedT2() const { return finishedTitle2_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~TitleScene();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	//KamataEngine::Sprite* titleSprite_ = nullptr;

private:
	
	
	// カメラ
	KamataEngine::Camera camera_;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	

	// フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	//uint32_t Botan_ = 0;
};
