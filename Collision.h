#pragma once
#include "Box.h"

int Collision(Box* box1,Box* box2);

void Vec3Normalize(XMFLOAT3& pOut, XMFLOAT3& pV);

int  Wall(Box* wall, Box* box);