//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
#include "CPlayerManager.h"

#include "CPlayer.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../TEXTURE/CTexture.h"

#include "../../../INPUT/CInputKeyboard.h"

#ifdef _DEBUG
#define PLAYER_TEXTURE TEXTURE_PLAYER_0		// デバッグ時のプレイヤーのテクスチャ
#else
#define PLAYER_TEXTURE TEXTURE_NULL		// 
#endif

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::CPlayerManager(CAttackManager *pAttackManager)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
	m_nTimer = 0;
	m_pAttackManager = pAttackManager;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// クリエイト関数
//-----------------------------------------------------------------------------
CPlayerManager* CPlayerManager::Create(int nPlayerNum, int nManualPlayer, CAttackManager *pAtatckManager)
{
	// プレイヤーのマネージャを作成
	CPlayerManager* temp = new CPlayerManager(pAtatckManager);

	// プレイヤーマネージャの初期化
	temp->Init(nPlayerNum, nManualPlayer);

	// プレイヤーマネージャのポインタを返す
	return temp;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer, int nManualPlayer)
{
	int nManual = 0;
	int nCPU = 0;

	// マニュアル操作のプレイヤーの作成
	for (nManual = 0; nManual < nManualPlayer; nManual++)
	{
		m_apPlayer[nManual] = CPlayer::Create(	CRenderer::GetDevice(),
												D3DXVECTOR3(50.0f, 20.0f, 0),
												50.0f,
												50.0f,
												PLAYER_TEXTURE,
												PLAYER_MANUAL,
												m_pAttackManager,
												(short)nManual);
	}
	// CPUの作成
	for (nCPU = nManual; nCPU < nNumPlayer; nCPU++)
	{
		m_apPlayer[nCPU] = CPlayer::Create(	CRenderer::GetDevice(),
											D3DXVECTOR3(50.0f, 20.0f, 0),
											50.0f,
											50.0f,
											PLAYER_TEXTURE,
											PLAYER_COMPUTER,
											m_pAttackManager,
											(short)nCPU);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerManager::Update(void)
{
	// m_nTimer++;
	// 
	// if (m_nTimer > 500)
	// {
	// 	m_apPlayer[0]->Uninit();
	// 	m_apPlayer[0] = CPlayer::Create(CRenderer::GetDevice(), D3DXVECTOR3(100.0f, 100.0f, 0), 50.0f, 50.0f, PLAYER_TEXTURE, PLAYER_COMPUTER, m_pAttackManager);
	// 
	// 	m_nTimer = 0;
	// }

	if (CInputKeyboard::GetKeyboardTrigger(DIK_3))
	{
		m_apPlayer[0]->SetPlyerKnockBack();
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_2))
	{
		m_apPlayer[0]->SetPlayerDown();
	}
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayerManager::Uninit(void)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

// EOF