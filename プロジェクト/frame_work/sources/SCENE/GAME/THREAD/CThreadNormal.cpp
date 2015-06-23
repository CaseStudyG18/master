//=============================================================================
//
// CThreadManagerクラス [CAttacManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CThreadNormal.h"
#include "../EFFECT/CEffect.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
const short THREAD_NORMAL_END_TIME = 180;
// 当たり判定の始まる時間
const short THREAD_NORMAL_HIT_START_TIME = 60;
// 当たり判定の終わる時間
const short THREAD_NORMAL_HIT_END_TIME = 120;

// 当たり判 定幅,高さ
const float THREAD_NORMAL_HIT_WIDTH = 50;
const float THREAD_NORMAL_HIT_HEIGHT = 50;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadNormal::CThreadNormal(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// 変数初期化
	m_ThreadType = THREAD_TYPE_NORMAL;

	// この糸の固有ステータス初期化
	m_fWidth = THREAD_NORMAL_HIT_WIDTH;
	m_fHeight = THREAD_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_NORMAL_END_TIME;
	m_nHitStartTime = THREAD_NORMAL_HIT_START_TIME;
	m_nHitEndTime = THREAD_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadNormal ::~CThreadNormal(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CThreadNormal::Init()
{
	CThreadBase::Init(m_vPos, 100, 100, TEXTURE_THREAD);

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadNormal::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadNormal::Update(void)
{
	CThreadBase::Update();

	//// カウントが10のとき（仮）エフェクトは発動
	//if (m_nCount == 10){
	//	CEffect::Create(
	//		m_pD3DDevice,
	//		m_vPos, 100, 100,
	//		TEXTURE_THREAD, 10, 1, 20);
	//}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CThreadNormal* CThreadNormal::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos)
{
	// 作成
	CThreadNormal* p = new CThreadNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;

	// 初期化
	p->Init();


	return p;
}

void CThreadNormal::Draw(void)
{
	CThreadBase::Draw();
}
//----EOF-------