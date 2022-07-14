#pragma once
#include "DirectXCommon.h"
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include <DirectXMath.h>

#include"Box.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	Box* redBox = nullptr;
	Box* greenBox = nullptr;

	Box* wall = nullptr;

	int32_t count;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
