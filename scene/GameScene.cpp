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

	//乱数シート生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engin(seed_gen());
	//乱数範囲(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (size_t i = 0; i < _countof(worldtransform_); i++) {
		// xyz方向のスケーリングを設定
		worldtransform_[i].scale_ = {1.0f, 1.0f, 1.0f};
		// xyz軸周りの回転角を設定
		worldtransform_[i].rotation_ = {rotDist(engin), rotDist(engin), rotDist(engin)};
		// xyz軸周りの平行移動を設定
		worldtransform_[i].translation_ = {posDist(engin), posDist(engin), posDist(engin)};

		//ワールドトランスフォームの初期化
		worldtransform_[i].Initialize();
	}

	//カメラ始点座標を設定
	viewprojection_.eye = {0, 0, 0};
	//カメラ注視点座標を設定
	viewprojection_.target = {10, 0, 0};
	//カメラ上方向ベクトルを指定
	viewprojection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};

	//ビュープロジェクションの初期化
	viewprojection_.Initialize();
}

void GameScene::Update() {
	//----視点移動処理-------
	//-----視点の移動ベクトル
	XMFLOAT3 move = {0, 0, 0};
	//始点の移動速さ
	const float keySpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move = {0, 0, keySpeed};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, 0, -keySpeed};
	}

	// s辞典移動(ベクトルの加算)
	viewprojection_.eye.x += move.x;
	viewprojection_.eye.y += move.y;
	viewprojection_.eye.z += move.z;

	//行列の再試行
	viewprojection_.UpdateMatrix();


	//----注視点の移動処理
	//注視点の移動ベクトル
	XMFLOAT3 targetMove = {0, 0, 0};
	//注視点の移動速さ
	const float kTargetSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		targetMove = {-kTargetSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		targetMove = {kTargetSpeed, 0, 0};
	}

	//注視点移動(ベクトルの加算)
	viewprojection_.target.x += targetMove.x;
	viewprojection_.target.y += targetMove.y;
	viewprojection_.target.z += targetMove.z;

	//行列の再試行
	viewprojection_.UpdateMatrix();


	//------上方向回転処理-------
	//上方向の回転速さ(ラジアン/flame)
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, XM_2PI);
	}

	//上方向ベクトルを計算(半径1の円周上の座標)
	viewprojection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//行列の再試行
	viewprojection_.UpdateMatrix();


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
	//ビュー移動
	debugText_->SetPos(50, 140);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewprojection_.eye.x, viewprojection_.eye.y, viewprojection_.eye.z);
	debugText_->SetPos(50, 160);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewprojection_.target.x, viewprojection_.target.y,
	  viewprojection_.target.z);
	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewprojection_.up.x, viewprojection_.up.y, viewprojection_.up.z);
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
	for (size_t i = 0; i < _countof(worldtransform_); i++) {
		model_->Draw(worldtransform_[i], viewprojection_, textureHandle_);
	}

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
