//=============================================================================
//
// CJudgeクラス [CJudge.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CJudge.h"
#include "../CAMERA/CCamera.h"

//=============================================================================
// マクロ
//=============================================================================
static const float	REGARDED_0_VALUE = 0.000001f;	// 0とみなす値
static const float	INIT_LENGTH = -1.f;				// 長さの初期化値
static const int	SQUAER_CORNER_NUM = 4;			// 四角形の角の数

//=========================================================================
// 視錐台カリング判定
//=========================================================================
bool CJudge::IsViewCullFrustum(D3DXVECTOR3& BSPos, float BSRad)
{
//	// 視錐台ゲット
//	FRUSTUM frustum = CCamera::GetFrustum();
//	
//	// ビュー行列取得
//	D3DXMATRIX viewMtx = CCamera::GetMtxView(0);
//
//	float Dist;
//	D3DXVECTOR3 viewPos;
//	viewPos.z = viewMtx._13 * BSPos.x
//				+ viewMtx._23 * BSPos.y
//				+ viewMtx._33 * BSPos.z
//				+ viewMtx._43;
//	
//	// 前方投影面より手前
//	if((viewPos.z + BSRad) < frustum.NearClip)
//	{
//		return false;
//	}
//
//	// 後方投影面より奥
//	if((viewPos.z - BSRad) > frustum.FarClip)
//	{
//		return false;
//	}
//
//	viewPos.x = viewMtx._11 * BSPos.x
//				+ viewMtx._21 * BSPos.y
//				+ viewMtx._31 * BSPos.z
//				+ viewMtx._41;
//	Dist = (viewPos.x * frustum.LeftPlane.a) + (viewPos.z * frustum.LeftPlane.c);
//	// 左側面より出ている
//	if(Dist > BSRad)
//	{
//		return false;
//	}
//
//	Dist = (viewPos.x * frustum.RightPlane.a) + (viewPos.z * frustum.RightPlane.c);
//	// 右側面より出ている
//	if(Dist > BSRad)
//	{
//		return false;
//	}
//
//	viewPos.y = viewMtx._12 * BSPos.x
//				+ viewMtx._22 * BSPos.y
//				+ viewMtx._32 * BSPos.z
//				+ viewMtx._42;
//	Dist = (viewPos.y * frustum.TopPlane.b) + (viewPos.z * frustum.TopPlane.c);
//	// 上側面より出ている
//	if(Dist > BSRad)
//	{
//		return false;
//	}
//
//	Dist = (viewPos.y * frustum.BottomPlane.b) + (viewPos.z * frustum.BottomPlane.c);
//	// 下側面より出ている
//	if(Dist > BSRad)
//	{
//		return false;
//	}

	return true;
}

//----EOF----