//=============================================================================
//
// CAttacSpecialAttackクラス [CAttacSpecialAttack.cpp]
// Author : 佐藤　諒一
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackSpecialAttack.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
const short ATTACK_ATTACK_END_TIME = 50;
// 当たり判定の始まる時間
const short ATTACK_ATTACK_HIT_START_TIME = 20;
// 当たり判定の終わる時間
const short ATTACK_ATTACK_HIT_END_TIME = 40;

// 当たり判 定幅,高さ
const float ATTACK_ATTACK_HIT_WIDTH = 50;
const float ATTACK_ATTACK_HIT_HEIGHT = 50;

// プレイヤと攻撃エフェクトの距離
static const float ATTACK_ATTACK_RANGE = 50;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackSpecialAttack::CAttackSpecialAttack(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_ATTACK_HIT_WIDTH;
	m_fHeight = ATTACK_ATTACK_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_ATTACK_END_TIME;
	m_nHitStartTime = ATTACK_ATTACK_HIT_START_TIME;
	m_nHitEndTime = ATTACK_ATTACK_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackSpecialAttack ::~CAttackSpecialAttack(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackSpecialAttack::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackSpecialAttack::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackSpecialAttack::Update(void)
{
	CAttackBase::Update();

	m_vRot.z += D3DX_PI*2.0f;

	m_vPos.x += ATTACK_ATTACK_RANGE * cosf(m_vRot.z * D3DX_PI);
	m_vPos.y -= ATTACK_ATTACK_RANGE * sinf(m_vRot.z * D3DX_PI);
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackSpecialAttack* CAttackSpecialAttack::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackSpecialAttack* p = new CAttackSpecialAttack(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	//p->m_vPos = pos;// +(velocity * ATTACK_ATTACK_RANGE);
	p->m_vPos.y = pos.y + ATTACK_ATTACK_RANGE;
	p->m_vPos.x = pos.x + 25.0f;

	// 初期化
	p->Init();

	// 攻撃エフェクト生成
	CEffectManager::CreateEffect(pos, EFFECT_SPECIAL_ATTACK_ATTACK, velocity);

	return p;
}

//=============================================================================
//	プレイヤーに当たった時の処理
//=============================================================================
void CAttackSpecialAttack::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(-20.0f);
}

//----EOF-------