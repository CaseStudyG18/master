//=============================================================================
//
// CPlayerManager�N���X [CPlayerManager.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================
#include "CPlayerManager.h"

#include "CPlayer.h"
#include "../../../RENDERER/CRenderer.h"
#include "../../../TEXTURE/CTexture.h"
#include "../../../INPUT/CInputKeyboard.h"

//-----------------------------------------------------------------------------
//	�萔��`
//-----------------------------------------------------------------------------
static const TEXTURE_TYPE PLAYER_TEXTURE = TEXTURE_PLAYER;		// �f�o�b�O���̃v���C���[�̃e�N�X�`��

static const D3DXVECTOR3 PLAYER_START_POS[] = {
	D3DXVECTOR3(50, 50, 0),
	D3DXVECTOR3(1230, 50, 0),
	D3DXVECTOR3(50, 670, 0),
	D3DXVECTOR3(1230, 670, 0),
};

//-----------------------------------------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer, int nManualPlayer, bool *bPlayerControl)
{
	int nManual = 0;
	int nCPU = 0;

	// �}�j���A������̃v���C���[�̍쐬
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
	// CPU�̍쐬
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
// �X�V
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
// �I��
//-----------------------------------------------------------------------------
void CPlayerManager::Uninit(void)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

// EOF