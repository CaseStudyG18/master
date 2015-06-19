//=============================================================================
//
// CAttacSpecialSpeedクラス [CAttacSpecialSpeed.cpp]
// Author : 佐藤　諒一
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAttackSpecialSpeed.h"
#include "../../../SCENE/CSCENE/CSceneAnime.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
const short ATTACK_SPEED_END_TIME = 10;
// 当たり判定の始まる時間
const short ATTACK_SPEED_HIT_START_TIME = 1;
// 当たり判定の終わる時間
const short ATTACK_SPEED_HIT_END_TIME = 9;

// 当たり判 定幅,高さ
const float ATTACK_SPEED_HIT_WIDTH = 50;
const float ATTACK_SPEED_HIT_HEIGHT = 50;

// プレイヤと攻撃エフェクトの距離
static const float ATTACK_SPEED_RANGE = 4.0f;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackSpecialSpeed::CAttackSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_NORMAL;

	// この攻撃の固有ステータス初期化
	m_fWidth = ATTACK_SPEED_HIT_WIDTH;
	m_fHeight = ATTACK_SPEED_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_SPEED_END_TIME;
	m_nHitStartTime = ATTACK_SPEED_HIT_START_TIME;
	m_nHitEndTime = ATTACK_SPEED_HIT_END_TIME;

	m_vVelocity = D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackSpecialSpeed ::~CAttackSpecialSpeed(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CAttackSpecialSpeed::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackSpecialSpeed::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackSpecialSpeed::Update(void)
{
	CAttackBase::Update();

	// カウントが10のとき（仮）エフェクトは発動
	if (m_nCount == 9){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, m_nEndTime);
	}
	//m_vPos += (m_vVelocity * ATTACK_SPEED_RANGE);
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAttackSpecialSpeed* CAttackSpecialSpeed::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// 作成
	CAttackSpecialSpeed* p = new CAttackSpecialSpeed(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos; // -(velocity * ATTACK_SPEED_RANGE);
	//p->m_vVelocity = velocity;

	// 初期化
	p->Init();


	return p;
}

//=============================================================================
//	プレイヤーに当たった時の処理
//=============================================================================
void CAttackSpecialSpeed::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->HPGainAndLoss(-10.0f);
}

//----EOF-------