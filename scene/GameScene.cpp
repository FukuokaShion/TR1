#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイルを検索してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mushroom.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	//大元
	worldtransform_[PartId::Root].Initialize();
	//脊髄
	worldtransform_[PartId::Spine].translation_ = {0.0f, 4.5f, 0.0f};
	worldtransform_[PartId::Spine].parent_ = &worldtransform_[PartId::Root];
	worldtransform_[PartId::Spine].Initialize();
	//---上半身---
	//胸
	worldtransform_[PartId::Chest].translation_ = {0.0f, 0.0f, 0.0f};
	worldtransform_[PartId::Chest].parent_ = &worldtransform_[PartId::Spine];
	worldtransform_[PartId::Chest].Initialize();
	//頭
	worldtransform_[PartId::Head].translation_ = {0.0f, 3.0f, 0.0f};
	worldtransform_[PartId::Head].parent_ = &worldtransform_[PartId::Chest];
	worldtransform_[PartId::Head].Initialize();
	//左腕
	worldtransform_[PartId::ArmL].translation_ = {3.0f, 0.0f, 0.0f};
	worldtransform_[PartId::ArmL].parent_ = &worldtransform_[PartId::Chest];
	worldtransform_[PartId::ArmL].Initialize();
	//右腕
	worldtransform_[PartId::ArmR].translation_ = {-3.0f, 0.0f, 0.0f};
	worldtransform_[PartId::ArmR].parent_ = &worldtransform_[PartId::Chest];
	worldtransform_[PartId::ArmR].Initialize();
	//---下半身---
	//尻
	worldtransform_[PartId::Hip].translation_ = {0.0f, -3.0f, 0.0f};
	worldtransform_[PartId::Hip].parent_ = &worldtransform_[PartId::Spine];
	worldtransform_[PartId::Hip].Initialize();
	//左腕
	worldtransform_[PartId::LegL].translation_ = {3.0f, -3.0f, 0.0f};
	worldtransform_[PartId::LegL].parent_ = &worldtransform_[PartId::Hip];
	worldtransform_[PartId::LegL].Initialize();
	//右腕
	worldtransform_[PartId::LegR].translation_ = {-3.0f, -3.0f, 0.0f};
	worldtransform_[PartId::LegR].parent_ = &worldtransform_[PartId::Hip];
	worldtransform_[PartId::LegR].Initialize();

	//ビュープロジェクションの初期化
	viewprojection_.Initialize();
}

void GameScene::Update() {
	//キャラ移動処理
	// キャラクターの移動ベクトる
	XMFLOAT3 move = {0, 0, 0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	// 上半身の回転速さ[ラジアン/frame ]
	const float kchestRotSpeed = 0.05f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldtransform_[PartId::Chest].rotation_.y -= kchestRotSpeed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldtransform_[PartId::Chest].rotation_.y += kchestRotSpeed;
	}

	// 下半身の回転速さ[ラジアン/frame]
	const float kHipRotSpeed = 0.05f;
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_J)) {
		worldtransform_[PartId::Hip].rotation_.y -= kHipRotSpeed;
	}
	else if (input_->PushKey(DIK_K)) {
		worldtransform_[ PartId::Hip].rotation_.y += kHipRotSpeed;
	}


	//注視点移動(ベクトルの加算)
	worldtransform_[PartId::Root].translation_.x += move.x;
	worldtransform_[PartId::Root].translation_.y += move.y;
	worldtransform_[PartId::Root].translation_.z += move.z;

	worldtransform_[PartId::Root].UpdateMatrix();
	worldtransform_[PartId::Spine].UpdateMatrix();
	worldtransform_[PartId::Chest].UpdateMatrix();
	worldtransform_[PartId::Head].UpdateMatrix();
	worldtransform_[PartId::ArmL].UpdateMatrix();
	worldtransform_[PartId::ArmR].UpdateMatrix();
	worldtransform_[PartId::Hip].UpdateMatrix();
	worldtransform_[PartId::LegL].UpdateMatrix();
	worldtransform_[PartId::LegR].UpdateMatrix();

	//-------デバッグ表示--------

	//キューブ座標
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "translation:(%f,%f,%f)", worldtransform_[0].translation_.x,
	  worldtransform_[0].translation_.y, worldtransform_[0].translation_.z);
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "rotatio:(%f,%f,%f)", worldtransform_[0].rotation_.x, worldtransform_[0].rotation_.y,
	  worldtransform_[0].rotation_.z);
	debugText_->SetPos(50, 110);
	debugText_->Printf(
	  "scale:(%f,%f,%f)", worldtransform_[0].scale_.x, worldtransform_[0].scale_.y,
	  worldtransform_[0].scale_.z);

	// デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root:(%f,%f %f)", worldtransform_[PartId::Root].translation_.x,
	  worldtransform_[PartId::Root].translation_.y, worldtransform_[PartId::Root].translation_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// <summary>
	// ここに3Dオブジェクトの描画処理を追加できる
	// </summary>
	model_->Draw(worldtransform_[PartId::Chest], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::Head], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::ArmL], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::ArmR], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::Hip], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::LegL], viewprojection_, textureHandle_);
	model_->Draw(worldtransform_[PartId::LegR], viewprojection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
