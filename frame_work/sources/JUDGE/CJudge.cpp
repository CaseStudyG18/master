//=============================================================================
//
// CJudge�N���X [CJudge.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CJudge.h"
#include "../CAMERA/CCamera.h"

//=============================================================================
// �}�N��
//=============================================================================
static const float	REGARDED_0_VALUE = 0.000001f;	// 0�Ƃ݂Ȃ��l
static const float	INIT_LENGTH = -1.f;				// �����̏������l
static const int	SQUAER_CORNER_NUM = 4;			// �l�p�`�̊p�̐�

//=========================================================================
// ������J�����O����
//=========================================================================
bool CJudge::IsViewCullFrustum(D3DXVECTOR3& BSPos, float BSRad)
{
//	// ������Q�b�g
//	FRUSTUM frustum = CCamera::GetFrustum();
//	
//	// �r���[�s��擾
//	D3DXMATRIX viewMtx = CCamera::GetMtxView(0);
//
//	float Dist;
//	D3DXVECTOR3 viewPos;
//	viewPos.z = viewMtx._13 * BSPos.x
//				+ viewMtx._23 * BSPos.y
//				+ viewMtx._33 * BSPos.z
//				+ viewMtx._43;
//	
//	// �O�����e�ʂ���O
//	if((viewPos.z + BSRad) < frustum.NearClip)
//	{
//		return false;
//	}
//
//	// ������e�ʂ�艜
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
//	// �����ʂ��o�Ă���
//	if(Dist > BSRad)
//	{
//		return false;
//	}
//
//	Dist = (viewPos.x * frustum.RightPlane.a) + (viewPos.z * frustum.RightPlane.c);
//	// �E���ʂ��o�Ă���
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
//	// �㑤�ʂ��o�Ă���
//	if(Dist > BSRad)
//	{
//		return false;
//	}
//
//	Dist = (viewPos.y * frustum.BottomPlane.b) + (viewPos.z * frustum.BottomPlane.c);
//	// �����ʂ��o�Ă���
//	if(Dist > BSRad)
//	{
//		return false;
//	}

	return true;
}

//----EOF----