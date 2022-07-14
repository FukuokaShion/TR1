#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "Struct.h"

class Box {
  public:
	Box(XMFLOAT3 pos, uint16_t texture,XMFLOAT3 move,float M);

	~Box();

	void Move();

	void Draw();

private:
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
	// 3D���f��
	Model* model_ = nullptr;

  public:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldtransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewprojection_;

	float m;

	Affine affine;
};