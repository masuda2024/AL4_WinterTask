#include <Windows.h>
#include"KamataEngine.h"


#include"TitleScene.h"
#include"Tutorial.h"
#include"GameScene.h"
#include"GameClear.h"
#include"GameOver.h"


using namespace KamataEngine;

// DirectXCommonインスタンスの取得
DirectXCommon* dxCommon = DirectXCommon::GetInstance();




enum class Scene
{
	kUnknown = 0,
	kTitle,
	kTutorial,
	kGame,
	kClear,
	kOver,
};
Scene scene = Scene::kUnknown;

void ChangeScene();
void UpdateScene();
void DrawScene();

// タイトルシーンの生成
TitleScene* titleScene = nullptr;

// チュートリアルシーンの生成
Tutorial* tutorial = nullptr;

// ゲームシーンのインスタンス生成
GameScene* gameScene = nullptr;

// ゲームクリアシーンの生成
GameClear* gameClear = nullptr;

// ゲームオーバーシーンの生成
GameOver* gameOver = nullptr;

// 音声再生ハンドル
uint32_t titleHandle_ = 0;
uint32_t TvoiceHandle_ = 0;

uint32_t gameHandle_ = 0;
uint32_t GVoiceHandle_ = 0;

uint32_t gameCHandle_ = 0;
uint32_t GCVoiceHandle_ = 0;

uint32_t gameOHandle_ = 0;
uint32_t GOVoiceHandle_ = 0;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	//タイトルバー
	KamataEngine::Initialize(L"LE2D_19_マスダ_アキヒロ_宇宙の戦い");
	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();




	#pragma region シーン


	//最初のシーンの初期化
	scene = Scene::kTitle;
	//シーンの初期化
	titleScene = new TitleScene;
	tutorial = new Tutorial;
	gameClear = new GameClear;
	gameOver = new GameOver;

	#pragma endregion




	// タイトルシーンの初期化
	titleScene->Initialize();

	// チュートリアルシーンの初期化
	tutorial->Initialize();

	// ゲームオーバーの初期化
	gameOver->Initialize();
	
	// ゲームクリアの初期化
	gameClear->Initialize();

	//BGM
	//  タイトル_Springin->BGM1->フューチャー1
	titleHandle_ = Audio::GetInstance()->LoadWave("Sounds/Future_1.mp3");
	// タイトルの音楽を再生
	TvoiceHandle_ = Audio::GetInstance()->PlayWave(titleHandle_, true);

	// ゲーム_Springin->BGM1->RPGバトル4
	gameHandle_ = Audio::GetInstance()->LoadWave("Sounds/RPG_Battle_04.mp3");

	// ゲームクリア_Springin->BGM1->ラジオ_エンディングテーマ
	gameCHandle_ = Audio::GetInstance()->LoadWave("Sounds/Springin_Radio_Ending.mp3");

	// ゲームオーバー_Springin->BGM1->ラジオ_エンディングテーマ
	gameOHandle_ = Audio::GetInstance()->LoadWave("Sounds/Springin_Radio_Ending.mp3");
	




	while (true)
	{
		// エンジンの更新
		if (KamataEngine::Update())
		{
			break;
		}


		// シーン切り替え
		ChangeScene();

		imguiManager->Begin();

		// 現在シーン更新
		UpdateScene();

		imguiManager->End();



		dxCommon->PreDraw();

		// 現在シーンの描画
		DrawScene();

		imguiManager->Draw();


		dxCommon->PostDraw();
	}


	// タイトルシーンの解放
	delete titleScene;

	// チュートリアルシーンの解放
	if (tutorial) 
	{
		delete tutorial;
	}

	// ゲームシーンの解放
	delete gameScene;

	// ゲームクリアシーンの解放
	delete gameClear;

	// ゲームオーバーシーンの解放
	delete gameOver;

	// nullptrの代入

	gameScene = nullptr;


	KamataEngine::Finalize();



	return 0;
}


void UpdateScene()
{
	switch (scene)
	{
	case Scene::kTitle:
		titleScene->Update();
		break;

	case Scene::kTutorial:
		tutorial->Update();
		break;

	case Scene::kGame:
		gameScene->Update();
		break;

	case Scene::kClear:
		gameClear->Update();
		break;

	case Scene::kOver:
		gameOver->Update();
		break;
	}
}

void ChangeScene()
{
	switch (scene)
	{
	case Scene::kTitle:

		if (titleScene->IsFinishedT()) 
		{
			// 音声停止
			Audio::GetInstance()->StopWave(TvoiceHandle_);

			// シーンの変更
			scene = Scene::kTutorial;
			// 新シーンの生成と初期化
			tutorial = new Tutorial();
			tutorial->Initialize();

			// ゲームシーンの音楽を再生
			GVoiceHandle_ = Audio::GetInstance()->PlayWave(gameHandle_, true);
		}

		break;
	case Scene::kTutorial:
		if (tutorial->IsFinishedTU())
		{

			// シーンの変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete tutorial;
			tutorial = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:

		// ゲームシーンでプレイヤーが破壊された場合
		if (gameScene->IsFinishedGAME()) 
		{

			// 音声停止
			Audio::GetInstance()->StopWave(GVoiceHandle_);

			// シーンの変更
			scene = Scene::kOver;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// クリアオーバーの生成
			gameOver = new GameOver;
			// クリアオーバーの初期化
			gameOver->Initialize();

			// ゲームオーバーシーンの音楽を再生
			GOVoiceHandle_ = Audio::GetInstance()->PlayWave(gameOHandle_, true);

		} else if (gameScene->IsFinishedGAME2())
		{
			// 音声停止
			Audio::GetInstance()->StopWave(GVoiceHandle_);

			// プレイヤーが敵を倒した場合
			// シーンの変更
			scene = Scene::kClear;

			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;

			// クリアシーンの生成
			gameClear = new GameClear;
			// クリアシーンの初期化
			gameClear->Initialize();

			// ゲームクリアシーンの音楽を再生
			GCVoiceHandle_ = Audio::GetInstance()->PlayWave(gameCHandle_, true);
		}
		break;

	case Scene::kClear:
		//ゲームクリアシーンが終了した場合
		if (gameClear->IsFinishedC())
		{
			// 音声停止
			Audio::GetInstance()->StopWave(GCVoiceHandle_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameClear;
			gameClear = nullptr;

			// タイトルシーンの生成
			titleScene = new TitleScene;
			// タイトルシーンの初期化
			titleScene->Initialize();

			// タイトルの音楽を再生
			TvoiceHandle_ = Audio::GetInstance()->PlayWave(titleHandle_, true);
		}
		break;

	case Scene::kOver:
		// ゲームオーバーシーンが終了した場合
		if (gameOver->IsFinishedO())
		{

			// 音声停止
			Audio::GetInstance()->StopWave(GOVoiceHandle_);

			// シーンの変更
			scene = Scene::kTitle;

			// 旧シーンの解放
			delete gameOver;
			gameOver = nullptr;

			// タイトルシーンの生成
			titleScene = new TitleScene;
			// タイトルシーンの初期化
			titleScene->Initialize();

			// タイトルの音楽を再生
			TvoiceHandle_ = Audio::GetInstance()->PlayWave(titleHandle_, true);
		}
		break;
	}
}

void DrawScene()
{

	switch (scene) 
	{
	case Scene::kTitle:
		titleScene->Draw();
		break;

	case Scene::kTutorial:
		tutorial->Draw();
		break;

	case Scene::kGame:
		gameScene->Draw();
		break;

	case Scene::kClear:
		gameClear->Draw();
		break;

	case Scene::kOver:
		gameOver->Draw();
		break;
	}
}