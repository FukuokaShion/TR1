#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <math.h>
#include "Collision.h"

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	redBox->~Box();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	uint32_t texture = TextureManager::Load("red.png");
	XMFLOAT3 pos = {0, 0, -10};
	XMFLOAT3 move = {0, 0, 0};
	float m = 1;
	redBox = new Box(pos, texture, move, m);

	texture = TextureManager::Load("green.png");
	pos = {-5, 0, -10};
	move = {0.05, 0, 0};
	m = 1;
	greenBox = new Box(pos, texture, move, m);

	
	texture = TextureManager::Load("wall.png");
	pos = {-25, 0, -10};
	move = {0, 0, 0};
	m = 1;
	wall = new Box(pos, texture, move, m);
	wall->worldtransform_.scale_.y = 10;

	count = 0;

}

void GameScene::Update() {
	//速度を50分の1にすることですり抜けを回避
	for (int i = 0; i < 50; i++){
		redBox->Move();
		greenBox->Move();
		wall->Move();
		if (Collision(greenBox, redBox)) {
			count++;
		}
		/*if (Wall(wall, redBox)) {
			count++;
		}*/
	}
	debugText_->SetPos(600, 70);
	debugText_->SetScale(2.0f);
	//debugText_->Printf("%d", count);
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	redBox->Draw();
	greenBox->Draw();
	//wall->Draw();

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
