#pragma once
#include <DirectXMath.h>
using namespace DirectX;


struct Affine {
	XMFLOAT3 ext = {1,1,1};
	XMFLOAT3 rota = {0,0,0};
	XMFLOAT3 move = {0,0,0};
};