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

//-----------------------------------------------------------------------------
//	定数定義
//-----------------------------------------------------------------------------
static const TEXTURE_TYPE PLAYER_TEXTURE = TEXTURE_PLAYER;		// デバッグ時のプレイヤーのテクスチャ

static const D3DXVECTOR3 PLAYER_START_POS[] = {
	D3DXVECTOR3(50, 50, 0),
	D3DXVECTOR3(1230, 50, 0),
	D3DXVECTOR3(50, 670, 0),
	D3DXVECTOR3(1230, 670, 0),
};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::CPlayerManager(CAttackManager *pAttackManager, CThreadManager *pThreadManager)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
	m_nTimer = 0;
	m_pAttackManager = pAttackManager;
	m_pThreadManager = pThreadManager;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer, int nManualPlayer, bool *bPlayerControl)
{
	int nManual = 0;
	int nCPU = 0;

	// マニュアル操作のプレイヤーの作成
	for (nManual = 0; nManual < nManualPlayer; nManual++)
	{
		m_apPlayer[nManual] = CPlayer::Create(CRenderer::GetDevice(),
			PLAYER_START_POS[nManual],
			50.0f,
			80.0f,
			PLAYER_TEXTURE,
			PLAYER_MANUAL,
			m_pAttackManager,
			m_pThreadManager,
			(short)nManual,
			bPlayerControl);
	}
	// CPUの作成
	for (nCPU = nManual; nCPU < nNumPlayer; nCPU++)
	{
		m_apPlayer[nCPU] = CPlayer::Create(CRenderer::GetDevice(),
			PLAYER_START_POS[nCPU],
			1050.0f,
			80.0f,
			PLAYER_TEXTURE,
			PLAYER_COMPUTER,
			m_pAttackManager,
			m_pThreadManager,
			(short)nCPU,
			bPlayerControl);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerManager::Update(void)
{
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