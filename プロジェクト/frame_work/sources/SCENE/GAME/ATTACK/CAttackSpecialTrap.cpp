//=============================================================================
//
// CAttacSpecialSpeedクラス [CAttacSpecialSpeed.cpp]
// Author : 佐藤　諒一
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackSpecialTrap.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
const short ATTACK_TRAP_END_TIME = 1000;
// 当たり判定の始まる時間
const short ATTACK_TRAP_HIT_START_TIME = 100;
// 当たり判定の終わる時間
const short ATTACK_TRAP_HIT_END_TIME = 950;

// 当たり判 定幅,高さ
const float ATTACK_TRAP_HIT_WIDTH = 30;
const float ATTACK_TRAP_HIT_HEIGHT = 30;

// プレイヤと攻撃エフェクトの距離
static const float ATTACK_TRAP_RANGE = 0;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackSpecialTrap::CAttackSpecialTrap(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_TRAP_HIT_WIDTH;
	m_fHeight = ATTACK_TRAP_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_TRAP_END_TIME;
	m_nHitStartTime = ATTACK_TRAP_HIT_START_TIME;
	m_nHitEndTime = ATTACK_TRAP_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackSpecialTrap ::~CAttackSpecialTrap(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackSpecialTrap::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackSpecialTrap::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackSpecialTrap::Update(void)
{
	CAttackBase::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackSpecialTrap* CAttackSpecialTrap::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackSpecialTrap* p = new CAttackSpecialTrap(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos; // -(velocity * ATTACK_TRAP_RANGE);

	// 初期化
	p->Init();

	// エフェクト生成
	CEffectManager::CreateEffect(pos,EFFECT_TRAP_FOUNDATION,velocity);

	return p;
}

//=============================================================================
//	プレイヤーに当たった時の処理
//=============================================================================
void CAttackSpecialTrap::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(-10.0f);

	// 罠にかかったエフェクト生成
	CEffectManager::CreateEffect(pPlayer->GetPos(), EFFECT_CAUGHT_TRAP,D3DXVECTOR3(0.0f,0.0f,0.0f));
}

//----EOF-------