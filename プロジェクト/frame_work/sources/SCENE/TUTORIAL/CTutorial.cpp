//=============================================================================
//
// CTutorial�N���X [CTutorial.h]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTutorial.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �Q�[�����S
static const float TITLE_LOGO_WIDTH = 900;
static const float TITLE_LOGO_HEIGHT = 370;
static const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 180, 0);

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorial::CTutorial(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorial ::~CTutorial(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTutorial::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �ϐ�������
	m_pBackGround = NULL;

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �w�i
	m_pBackGround = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBackGround->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
	//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTutorial::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTutorial::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// �t�F�[�h�A�E�g���ă^�C�g���w���ǂ�
	// ���͂��ꂽ��PUSH START���s
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_A, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_B, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_X, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_Y, 0)){

		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CTutorial* CTutorial::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTutorial* pTitle = NULL;

	// �쐬
	pTitle = new CTutorial;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//----EOF----