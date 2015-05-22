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

#ifdef _DEBUG
#define PLAYER_TEXTURE TEXTURE_PLAYER_0		// �f�o�b�O���̃v���C���[�̃e�N�X�`��
#else
#define PLAYER_TEXTURE TEXTURE_NULL		// 
#endif

//-----------------------------------------------------------------------------
// �R���X�g���N�^
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
// �f�X�g���N�^
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// �N���G�C�g�֐�
//-----------------------------------------------------------------------------
CPlayerManager* CPlayerManager::Create(int nPlayerNum, int nManualPlayer, CAttackManager *pAtatckManager)
{
	// �v���C���[�̃}�l�[�W�����쐬
	CPlayerManager* temp = new CPlayerManager(pAtatckManager);

	// �v���C���[�}�l�[�W���̏�����
	temp->Init(nPlayerNum, nManualPlayer);

	// �v���C���[�}�l�[�W���̃|�C���^��Ԃ�
	return temp;
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer, int nManualPlayer)
{
	int nManual = 0;
	int nCPU = 0;

	// �}�j���A������̃v���C���[�̍쐬
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
	// CPU�̍쐬
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
// �X�V
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