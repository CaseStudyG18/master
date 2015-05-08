//=============================================================================
//
// 汎用計算式用 [math.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MATH/math.h"

//*****************************************************************************
// ベクトルを長さに変換
//*****************************************************************************
float MagnitudeVector(D3DXVECTOR3 v)
{
	return (float) sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

//*****************************************************************************
// ベクトルを長さの二乗に変換
//*****************************************************************************
float PowVector(D3DXVECTOR3 v)
{
	return (float) (v.x*v.x + v.y*v.y + v.z*v.z);
}

//*****************************************************************************
// 角度正規関数関数
//*****************************************************************************
void NormalizeRotation(float *rot)
{
	if(*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
	if(*rot > D3DX_PI)
	{
		*rot -= D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
}
void NormalizeRotation(D3DXVECTOR3 *rot)
{
	if(rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
	if(rot->x > D3DX_PI)
	{
		rot->x -= D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}

	if(rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
	if(rot->y > D3DX_PI)
	{
		rot->y -= D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}

	if(rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
	if(rot->z > D3DX_PI)
	{
		rot->z -= D3DX_PI * 2.f;	// 360度をにしたいため2倍している
	}
}
//----EOF----