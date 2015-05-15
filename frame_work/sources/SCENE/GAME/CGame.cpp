//=============================================================================
//
// CGame�N���X [CGame.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CGame.h"
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "PLAYER\CPlayerManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame ::CGame(void)
{
	m_pPause = NULL;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGame ::~CGame(void)
{
	m_pPause = NULL;
}

//*****************************************************************************
// ������
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �J����������
	m_pManager->InitCamera();

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �|�[�Y�쐬
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ���Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	// �v���C���[�}�l�[�W���[
	m_pPlayerManager = new CPlayerManager();
	m_pPlayerManager->Init(2);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	m_pPlayerManager->Uninit();
	SAFE_DELETE(m_pPlayerManager);
	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{

	m_pPlayerManager->Update();

	// �o�������ꂽ��
	if(CInputKeyboard::GetKeyboardTrigger(DIK_P))
	{
		// �|�[�Y�t���O���]
		m_pPause->ReverceFlag();
		m_pPause->SetCursolDrawFlag(false);

		// �|�[�Y�I���Ȃ�
		if(!m_pPause->GetPauseFlag())
		{
			// �J�[�\���ʒu��߂��
			m_pPause->SetChoiceMenu(m_pPause->PAUSE_RETURN);
		}
	}

	if(!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// ���U���g��
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}
	}

	// �^�C�g���ɖ߂�I���Ȃ�
	if(m_pPause->GetReturnTitleFlag())
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �^�C�g����
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// ���g���C�ɖ߂�I���Ȃ�
	if(m_pPause->GetRetryFlag())
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CGame* CGame::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CGame* pGame = NULL;

	// �쐬
	pGame = new CGame;

	// ������
	pGame->Init(mode, pDevice);

	return pGame;
}
//----EOF-------