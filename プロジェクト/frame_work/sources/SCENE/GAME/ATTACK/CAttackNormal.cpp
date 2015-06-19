//=============================================================================
//
// CAttacjManagerクラス [CAttacManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackNormal.h"
#include "../../../SCENE/CSCENE/CSceneAnime.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
const short ATTACK_NORMAL_END_TIME = 180;
// 当たり判定の始まる時間
const short ATTACK_NORMAL_HIT_START_TIME = 60;
// 当たり判定の終わる時間
const short ATTACK_NORMAL_HIT_END_TIME = 120;

// 当たり判 定幅,高さ
const float ATTACK_NORMAL_HIT_WIDTH = 50;
const float ATTACK_NORMAL_HIT_HEIGHT = 50;

// プレイヤと攻撃エフェクトの距離
static const float ATTACK_NORMAL_RANGE = 100;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackNormal::CAttackNormal(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_NORMAL_HIT_WIDTH;
	m_fHeight = ATTACK_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_NORMAL_END_TIME;
	m_nHitStartTime = ATTACK_NORMAL_HIT_START_TIME;
	m_nHitEndTime = ATTACK_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackNormal ::~CAttackNormal(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackNormal::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackNormal::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackNormal::Update(void)
{
	CAttackBase::Update();

	// カウントが10のとき（仮）エフェクトは発動
	if (m_nCount == 10){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, 20);
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackNormal* CAttackNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackNormal* p = new CAttackNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos + (velocity * ATTACK_NORMAL_RANGE);

	// 初期化
	p->Init();


	return p;
}

//*****************************************************************************
//	攻撃がヒットした際に呼び出される関数
//	 引数　　ヒットしたプレイヤーのプレイヤー番号
//*****************************************************************************
void CAttackNormal::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->SetDonashi(true);
}

//----EOF-------