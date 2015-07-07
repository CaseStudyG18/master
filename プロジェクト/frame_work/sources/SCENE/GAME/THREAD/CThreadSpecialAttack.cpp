//=============================================================================
//
// CThreadManagerクラス [CAttacManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CThreadSpecialAttack.h"
#include "../PLAYER/CPlayer.h"
#include "../../CSCENE/CSceneAnime.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
// 寿命
static const short THREAD_ATTACK_END_TIME = 180;
// 当たり判定の始まる時間
static const short THREAD_ATTACK_HIT_START_TIME = 60;
// 当たり判定の終わる時間
static const short THREAD_ATTACK_HIT_END_TIME = 170;

// 当たり判 定幅,高さ
static const float THREAD_ATTACK_HIT_WIDTH = 50;
static const float THREAD_ATTACK_HIT_HEIGHT = 50;

// プレイヤと攻撃エフェクトの距離
static const short THREAD_ATTACK_RANGE = 50;

// 最初のエフェクトを出す時間
static const short THREAD_FIRST_ATTACK_CREATE_TIME = 60;

// 2回目のエフェクトを出す時間
static const short THREAD_SECOND_ATTACK_CREATE_TIME = 120;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CThreadSpecialAttack::CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CAttackBase(pDevice)
{
	// 変数初期化
	m_AttackType = ATTACK_TYPE_ATTACK_THREAD;

	// この糸の固有ステータス初期化
	m_fWidth = THREAD_ATTACK_HIT_WIDTH;
	m_fHeight = THREAD_ATTACK_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_ATTACK_END_TIME;
	m_nHitStartTime = THREAD_ATTACK_HIT_START_TIME;
	m_nHitEndTime = THREAD_ATTACK_HIT_END_TIME;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CThreadSpecialAttack ::~CThreadSpecialAttack(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CThreadSpecialAttack::Init(D3DXVECTOR3 pos, short playerNumber , D3DXVECTOR3 velocity)
{
	CAttackBase::Init();

	m_nPlayerNum = playerNumber;
	m_vPos = pos + (velocity * THREAD_ATTACK_RANGE);
	m_vVelocity = velocity;

	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CThreadSpecialAttack::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CThreadSpecialAttack::Update(void)
{
	CAttackBase::Update();

#ifdef _DEBUG
//	CDebugProc::Print("攻撃特化形態の糸生成済\n");
#endif

	// カウントが10のとき（仮）エフェクトは発動
	if (m_nCount == THREAD_FIRST_ATTACK_CREATE_TIME){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, 40);
	}

	// カウントが10のとき（仮）エフェクトは発動
	if (m_nCount >= THREAD_SECOND_ATTACK_CREATE_TIME){
		if (m_nCount % 11 == 0)
		{
			CSceneAnime::Create(
				m_pD3DDevice,
				m_vPos, 100, 100,
				TEXTURE_FIRE_0, 10, 1, 60);
		}
		if (m_nCount % 4 == 0)
		{
			m_vPos += m_vVelocity*50.0f;
		}

		else if (m_nCount % 4 == 1)
		{
			m_vPos -= m_vVelocity*50.0f;
		}

		else if (m_nCount % 4 == 2)
		{
			m_vPos += m_vVelocity*80.0f;
		}

		else if (m_nCount % 4 == 3)
		{
			m_vPos -= m_vVelocity*75.0f;
		}
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CThreadSpecialAttack* CThreadSpecialAttack::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// 作成
	CThreadSpecialAttack* p = new CThreadSpecialAttack(pDevice);

	// 初期化
	p->Init(pos, nPlayerNum, velocity);


	return p;
}

//*****************************************************************************
//	描画
//*****************************************************************************
void CThreadSpecialAttack::Draw(void)
{
	CAttackBase::Draw();
}

//*****************************************************************************
//
//*****************************************************************************
void CThreadSpecialAttack::HitPlayer(CPlayer* pPlayer)
{
	CSceneAnime::Create(
		m_pD3DDevice,
		pPlayer->GetPos(), 100, 100,
		TEXTURE_FIRE_1, 10, 1, 40);
}

//----EOF-------