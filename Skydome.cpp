#include "Skydome.h"
#include"GameScene.h"
#include"cassert"

using namespace KamataEngine;

void Skydome::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera)
{
	// NULLポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Skydome::Update()
{
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { model_->Draw(worldTransform_, *camera_); }
