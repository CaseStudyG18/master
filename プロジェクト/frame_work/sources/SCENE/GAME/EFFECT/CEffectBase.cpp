//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectBase.h"


//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectBase::CEffectBase(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CScene2D(pDevice, nPriority)
{
	// 描画しない
	CScene2D::SetDrawFlag(false);
	m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectBase ::~CEffectBase(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectBase::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectBase::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectBase::Update(void)
{
	m_nCount++;

	// 自殺
	if (m_nCount > m_nCountMax){
		Release();
	}
	
}

//----EOF-------