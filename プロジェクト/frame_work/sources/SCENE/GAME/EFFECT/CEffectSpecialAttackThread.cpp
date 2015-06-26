//=============================================================================
//
//	攻撃特化状態の糸攻撃エフェクト
//
//	Author : 佐藤　諒一
//
//=============================================================================
#include "CEffectSpecialAttackThread.h"

#include "../../CSCENE/CSceneAnime.h"

// デバッグしてみよう
#ifdef _DEBUG
#include "../../../DEBUG_PROC/CDebugProc.h"
#endif

//*****************************************************************************
// 定数
//*****************************************************************************
static const int THREAD_ATTACK_COUNT_MAX = 100;
static const float THREAD_ATTACK_WIDTH = 50;
static const float THREAD_ATTACK_HEIGHT = 50;
static const float THREAD_ATTACK_WIDTH_SECOND = 100;
static const float THREAD_ATTACK_HEIGHT_SECOND = 100;
static const TEXTURE_TYPE THREAD_ATTACK_TEXTURE = TEXTURE_FIRE_1;
static const int THREAD_ATTACK_TEXTURE_X = 10;
static const int THREAD_ATTACK_TEXTURE_Y = 1;
static const int THREAD_ATTACK_TEXTURE_LOOP = 50;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectSpecialAttackThread::CEffectSpecialAttackThread(LPDIRECT3DDEVICE9 *pDevice, int nPriority) : CEffectBase(pDevice)
{
	m_pD3DDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectSpecialAttackThread ::~CEffectSpecialAttackThread(void)
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectSpecialAttackThread::Init(D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	m_nCount = 0;
	m_nCountMax = THREAD_ATTACK_COUNT_MAX;
	m_vPos = pos;
	m_vRot.z = 0.0f;
	m_vVelocity = velocity;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectSpecialAttackThread::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectSpecialAttackThread::Update(void)
{
	if (m_nCount == 50){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH, THREAD_ATTACK_HEIGHT,
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
	}
	else if (m_nCount == 100)
	{
		m_vPos += m_vVelocity * 50.0f;
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH_SECOND + (m_vVelocity.x * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (m_vVelocity.y * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH_SECOND + (m_vVelocity.x * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (m_vVelocity.y * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;
		m_vPos.y -= 5.0f;
		m_vPos.x -= 5.0f;
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH_SECOND + (m_vVelocity.x * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (m_vVelocity.y * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
		m_vPos.x += m_vVelocity.x * 5.0f;
		m_vPos.y += m_vVelocity.y * 5.0f;
		m_vPos.y -= 10.0f;
		m_vPos.x -= 10.0f;
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos,
			THREAD_ATTACK_WIDTH_SECOND + (m_vVelocity.x * 100.0f), THREAD_ATTACK_HEIGHT_SECOND + (m_vVelocity.y * 100.0f),
			THREAD_ATTACK_TEXTURE, THREAD_ATTACK_TEXTURE_X, THREAD_ATTACK_TEXTURE_Y,
			THREAD_ATTACK_TEXTURE_LOOP);
	}

	// 自殺の更新
	CEffectBase::Update();

#ifdef _DEBUG
	CDebugProc::Print("攻撃特化状態の糸のエフェクトなう\n");
#endif
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CEffectSpecialAttackThread* CEffectSpecialAttackThread::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CEffectSpecialAttackThread* p = new CEffectSpecialAttackThread(pDevice);

	p->Init(pos, velocity);

	return p;
}

//EOF