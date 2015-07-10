//=============================================================================
//
// CEffectクラス [CEffect.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CEffectTrapFoundation.h"
#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#include "../../../DEBUG_PROC/CDebugProc.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int TRAP_FOUNDATION_COUNT_MAX = 180;
static const float TRAP_FOUNDATION_WIDTH = 100;
static const float TRAP_FOUNDATION_HEIGHT = 100;
static const TEXTURE_TYPE TRAP_FOUNDATION_TEXTURE = TEXTURE_HIT_SMALL;
static const int TRAP_FOUNDATION_TEXTURE_X = 10;
static const int TRAP_FOUNDATION_TEXTURE_Y = 1;
static const int TRAP_FOUNDATION_TEXTURE_LOOP = 10;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectTrapFoundation::CEffectTrapFoundation(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectTrapFoundation ::~CEffectTrapFoundation(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectTrapFoundation::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = TRAP_FOUNDATION_COUNT_MAX;
	m_vPos = pos;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectTrapFoundation::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectTrapFoundation::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			TRAP_FOUNDATION_WIDTH, TRAP_FOUNDATION_HEIGHT,
			TRAP_FOUNDATION_TEXTURE, TRAP_FOUNDATION_TEXTURE_X, TRAP_FOUNDATION_TEXTURE_Y,
			TRAP_FOUNDATION_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectTrapFoundation* CEffectTrapFoundation::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectTrapFoundation* p = new CEffectTrapFoundation(pDevice);

	p->Init(pos);

	return p;
}

//----EOF-------