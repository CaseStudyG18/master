//=============================================================================
//
// 汎用計算式用 [math.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _MATH_H_
#define _MATH_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float MagnitudeVector(D3DXVECTOR3 v);	// ベクトルを長さに変換
float PowVector(D3DXVECTOR3 v);			// ベクトルを長さの二乗に変換
// 角度正規化関数
void	NormalizeRotation(float *rot);
void	NormalizeRotation(D3DXVECTOR3 *rot);

#endif
//----EOF----