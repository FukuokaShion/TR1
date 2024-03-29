#include "Collision.h"
#include<math.h>


int Collision(Box* box1, Box* box2) {
	//距離
	XMFLOAT3 length = {
	  box2->worldtransform_.translation_.x - box1->worldtransform_.translation_.x,
	  box2->worldtransform_.translation_.y - box1->worldtransform_.translation_.y,
	  box2->worldtransform_.translation_.z - box1->worldtransform_.translation_.z
	};

	//接点
	XMFLOAT3 collisionPoint = {
	  box1->worldtransform_.translation_.x + (length.x / 2),
	  box1->worldtransform_.translation_.y + (length.y / 2),
	  box1->worldtransform_.translation_.z + (length.z / 2),
	};

	//box1の中心からbox2の中心の正規化
	XMFLOAT3 b1b2Vector;
	Vec3Normalize(b1b2Vector, length);

	//box1の運動量の正規化
	XMFLOAT3 b1Vector;
	Vec3Normalize(b1Vector,box1->affine.move);



	if ((length.x * length.x) + (length.y * length.y)<=4) {
		if ((length.x * length.x) + (length.z * length.z) <= 4) {
			//質量が同じ
			if (box1->m == box2->m) {
				//重心をとらえている
				if ((b1b2Vector.x == b1Vector.x) && (b1b2Vector.y == b1Vector.y) &&(b1b2Vector.z == b1Vector.z)){
					XMFLOAT3 move = box1->affine.move;
					box1->affine.move = box2->affine.move;
					box2->affine.move = move;
				} 
				//とらえてない
				else {
					box1->affine.move = {0,0,0};
					box2->affine.move = {0,0,0};
				}
			}
			//1より2の方が重い
			else if (box1->m > box2->m) {
				XMFLOAT3 move = box1->affine.move;
				box1->affine.move.x = ((box1->m - box2->m) * box1->affine.move.x + (2 * box2->m * box2->affine.move.x)) / (box1->m + box2->m);
				box1->affine.move.y = ((box1->m - box2->m) * box1->affine.move.y + (2 * box2->m * box2->affine.move.y)) / (box1->m + box2->m);
				box1->affine.move.z = ((box1->m - box2->m) * box1->affine.move.z + (2 * box2->m * box2->affine.move.z)) / (box1->m + box2->m);

				box2->affine.move.x = ((box2->m - box1->m) * box2->affine.move.x + (2 * box1->m * move.x)) / (box1->m + box2->m);
				box2->affine.move.y = ((box2->m - box1->m) * box2->affine.move.y + (2 * box1->m * move.y)) / (box1->m + box2->m);
				box2->affine.move.z = ((box2->m - box1->m) * box2->affine.move.z + (2 * box1->m * move.z)) / (box1->m + box2->m);
			}
			//1より2の方が軽い
			else if (box1->m < box2->m) {
				XMFLOAT3 move = box2->affine.move;
				box2->affine.move.x = ((box2->m - box1->m) * box2->affine.move.x + (2 * box1->m * box1->affine.move.x)) / (box1->m + box2->m);
				box2->affine.move.y = ((box2->m - box1->m) * box2->affine.move.y + (2 * box1->m * box1->affine.move.y)) / (box1->m + box2->m);
				box2->affine.move.z = ((box2->m - box1->m) * box2->affine.move.z + (2 * box1->m * box1->affine.move.z)) / (box1->m + box2->m);

				box1->affine.move.x = ((box1->m - box2->m) * box1->affine.move.x + (2 * box2->m * move.x)) / (box1->m + box2->m);
				box1->affine.move.y = ((box1->m - box2->m) * box1->affine.move.y + (2 * box2->m * move.y)) / (box1->m + box2->m);
				box1->affine.move.z = ((box1->m - box2->m) * box1->affine.move.z + (2 * box2->m * move.z)) / (box1->m + box2->m);
			}
				return 1;
		}
	}

	return 0;
}


int Wall(Box* wall, Box* box) {
	if (wall->worldtransform_.translation_.x + 1 > box->worldtransform_.translation_.x - 1 && wall->worldtransform_.translation_.x - 1 < box->worldtransform_.translation_.x + 1) {
		if (wall->worldtransform_.translation_.y + 1 > box->worldtransform_.translation_.y - 1 &&wall->worldtransform_.translation_.y - 1 < box->worldtransform_.translation_.y + 1) {
			if (wall->worldtransform_.translation_.z + 1 > box->worldtransform_.translation_.z - 1 &&wall->worldtransform_.translation_.z - 1 < box->worldtransform_.translation_.z + 1) {
				box->affine.move.x *= -1;
				box->worldtransform_.translation_.x = wall->worldtransform_.translation_.x + 2;
				return 1;
			}
		}
	}
	return 0;
}

//正規化
void Vec3Normalize(XMFLOAT3& pOut, XMFLOAT3& pV) {
	float len;
	float x, y, z;

	x = (pV.x);
	y = (pV.y);
	z = (pV.z);
	len = sqrt(x * x + y * y + z * z);

		len = 1.0 / len;
		x *= len;
		y *= len;
		z *= len;

		pOut.x = x;
		pOut.y = y;
		pOut.z = z;
	
}