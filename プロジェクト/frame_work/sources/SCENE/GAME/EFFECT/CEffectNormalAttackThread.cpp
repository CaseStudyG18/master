//=============================================================================
//
//	爆発エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectNormalAttackThread.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int NORMAL_ATTACK_THREAD_COUNT_MAX = 1;
static const float NORMAL_ATTACK_THREAD_WIDTH = 100;
static const float NORMAL_ATTACK_THREAD_HEIGHT = 100;
static const TEXTURE_TYPE NORMAL_ATTACK_THREAD_TEXTURE = TEXTURE_TREASURE_KIRAKIRA;
static const int NORMAL_ATTACK_THREAD_TEXTURE_X = 8;
static const int NORMAL_ATTACK_THREAD_TEXTURE_Y = 1;
static const int NORMAL_ATTACK_THREAD_TEXTURE_LOOP = 100;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectNormalAttackThread::CEffectNormalAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectNormalAttackThread ::~CEffectNormalAttackThread(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectNormalAttackThread::Init(D3DXVECTOR3 pos)
{
	m_nCount = 0;
	m_nCountMax = NORMAL_ATTACK_THREAD_COUNT_MAX;
	m_vPos = pos;
	m_vPos.y = pos.y - 30.0f;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectNormalAttackThread::Uninit(void)
{
	CEffectBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectNormalAttackThread::Update(void)
{
	if (m_nCount == 0){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			NORMAL_ATTACK_THREAD_WIDTH, NORMAL_ATTACK_THREAD_HEIGHT,
			NORMAL_ATTACK_THREAD_TEXTURE, NORMAL_ATTACK_THREAD_TEXTURE_X, NORMAL_ATTACK_THREAD_TEXTURE_Y,
			NORMAL_ATTACK_THREAD_TEXTURE_LOOP);
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
CEffectNormalAttackThread* CEffectNormalAttackThread::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CEffectNormalAttackThread* p = new CEffectNormalAttackThread(pDevice);

	p->Init(pos);

	return p;
}

//EOF