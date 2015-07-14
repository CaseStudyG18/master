//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectMPAttack.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int MP_ATTACK_COUNT_MAX = 1;
static const float MP_ATTACK_WIDTH = 100;
static const float MP_ATTACK_HEIGHT = 100;
static const TEXTURE_TYPE MP_ATTACK_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int MP_ATTACK_TEXTURE_X = 8;
static const int MP_ATTACK_TEXTURE_Y = 1;
static const int MP_ATTACK_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectMPAttack::CEffectMPAttack(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectMPAttack ::~CEffectMPAttack(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectMPAttack::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = MP_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectMPAttack::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectMPAttack::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			MP_ATTACK_WIDTH, MP_ATTACK_HEIGHT,
			MP_ATTACK_TEXTURE, MP_ATTACK_TEXTURE_X, MP_ATTACK_TEXTURE_Y,
			MP_ATTACK_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("道作成エフェクトなう\n");
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectMPAttack* CEffectMPAttack::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectMPAttack* p = new CEffectMPAttack(pDevice);

	p->Init(pos);

	return p;
}

//EOF