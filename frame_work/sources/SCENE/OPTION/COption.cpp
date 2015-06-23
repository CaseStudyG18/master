//=============================================================================
//
// COption�N���X [COption.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "COption.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
COption::COption(void)
{
	m_mode = OPTION_MODE_KEYCONFIG;
	m_setButton = CInputGamePad::PAD_KEY_A;
	m_nControllerID = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
COption ::~COption(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void COption::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// ���[�h�̏�����
	m_mode = OPTION_MODE_KEYCONFIG;

	// �a�f�l�Đ�
	//CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void COption::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void COption::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// �L�[�R�t�B���O�Ȃ�
	if (m_mode == OPTION_MODE_KEYCONFIG)
	{
		KeyConfig();
	}

	// ���̃V�[���ɍs���Ȃ�
	if (m_mode == OPTION_MODE_NEXT_SCENE)
	{
#ifdef _DEBUG
		CDebugProc::Print("PUSH ENTER\n");
#endif
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// �L�[�R���t�B�O���̃Z�[�u
			CControllerManager::SaveKeyCofingFile();

			// �L�[�R���t�B�O���̍X�V
			CControllerManager::UpdateKeyCord();

			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// �Q�[���w
			m_pManager->SetNextPhase(MODE_PHASE_GAME);
		}
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
COption* COption::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	COption* pTitle = NULL;

	// �쐬
	pTitle = new COption;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// �L�[�R���t�B�O
//*****************************************************************************
void COption::KeyConfig(void)
{
	int pCord = -1;
	if (CInputGamePad::CheckConectPad(m_nControllerID))
	{
#ifdef _DEBUG
		CDebugProc::Print("PLAYER_ID:%d\n", m_nControllerID);
#endif
		switch (m_setButton)
		{
		case CInputGamePad::PAD_KEY_A:
#ifdef _DEBUG
			CDebugProc::Print("A�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_B;
			}
		break;

		case CInputGamePad::PAD_KEY_B:
#ifdef _DEBUG
			CDebugProc::Print("B�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_Y;
			}
		break;

		case CInputGamePad::PAD_KEY_Y:
#ifdef _DEBUG
			CDebugProc::Print("Y�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_X;
			}
		break;

		case CInputGamePad::PAD_KEY_X:
#ifdef _DEBUG
			CDebugProc::Print("X�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_L;
			}
		break;

		case CInputGamePad::PAD_KEY_L:
#ifdef _DEBUG
			CDebugProc::Print("L�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_R;
			}
		break;

		case CInputGamePad::PAD_KEY_R:
#ifdef _DEBUG
			CDebugProc::Print("R�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_START;
			}
		break;

		case CInputGamePad::PAD_KEY_START:
#ifdef _DEBUG
			CDebugProc::Print("START�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_SELECT;
			}
		break;

		case CInputGamePad::PAD_KEY_SELECT:
#ifdef _DEBUG
			CDebugProc::Print("SELECT�{�^���ɂ������̂������Ă�������\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// �L�[�R�[�h�Z�b�g
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_MAX;
			}
		break;

		case CInputGamePad::PAD_KEY_MAX:
			m_nControllerID++;

			// �q�����Ă�R���g���[���[�S���Z�b�g������
			if (m_nControllerID >= CControllerManager::MAX_CONTROLLER_NUM)
			{
				m_mode = OPTION_MODE_NEXT_SCENE;
			}
		break;
		}
	}
	else
	{
		m_nControllerID++;

		// �q�����Ă�R���g���[���[�S���Z�b�g������
		if (m_nControllerID >= CControllerManager::MAX_CONTROLLER_NUM)
		{
			m_mode = OPTION_MODE_NEXT_SCENE;
		}
	}
}

//----EOF----