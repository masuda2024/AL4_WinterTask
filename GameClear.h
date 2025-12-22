#pragma once
#include "Fade.h"
#include "KamataEngine.h"

class GameClear
{
public:
	enum class Phase
	{
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	// 終了フラグ
	bool finishedC_ = false;
	// デスフラグのgetter
	bool IsFinishedC() const { return finishedC_; }

	void Initialize();
	void Update();
	void Draw();

	// デストラクタ
	~GameClear();
	// void ChangeScene();
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_; // stdでエラーが起きたらKamataEngine::をいれる

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* clearSprite_ = nullptr;

	// マップチップフィールド
	//MapChipField* mapChipField_;

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
