#include "Fade.h"
#include "algorithm"
using namespace KamataEngine;

// 初期化
void Fade::Initialize() 
{
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("white1x1.png");
	// スプライトインデックスの生成
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, {0, 0});
	// 画面サイズ
	sprite_->SetSize(Vector2(1280, 720));
	// 黒色
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}
// 更新
void Fade::Update() 
{
	// フェード状態による分岐
	switch (status_) 
	{
	case Fade::Status::None:
		// 何もしない
		break;
	case Fade::Status::FadeIn:

		// 1フレーム分の秒数カウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_)
		{
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、
		//  経過時間がフェード継続時間に近づくほど
		//  アルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, 1.0f - std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	case Fade::Status::FadeOut:

		// 1フレーム分の秒数カウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_)
		{
			counter_ = duration_;
		}
		// 0.0fから1.0fの間で、
		//  経過時間がフェード継続時間に近づくほど
		//  アルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}
// 描画
void Fade::Draw()
{
	if (status_ == Status::None)
	{
		return;
	}
	// DirectXCommonインスタンスの取得

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	KamataEngine::Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	KamataEngine::Sprite::PostDraw();
}

void Fade::Start(Status status, float duration)
{
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const
{
	switch (status_) 
	{
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:
		if (counter_ >= duration_) 
		{
			return true;
		} else 
		{
			return false;
		}
	}
	return true;
}
