//=============================================================================
//
// CResult�N���X [CResult.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CResult.h"
#include "../../MANAGER/CManager.h"
#include "../../SCENE/CSCENE/CScene2D.h"


//*****************************************************************************
// �}�N��
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CResult ::CResult(void)
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CResult ::~CResult(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CResult::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	m_pSprite = CScene2D::Create(
		pDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		SCREEN_WIDTH, SCREEN_HEIGHT, TEXTURE_BG_RESULT);

}


//*****************************************************************************
// �I��
//*****************************************************************************
void CResult::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();

}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CResult::Update(void)
{
	CPhase::Update();

	

	if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �^�C�g����
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CResult* CResult::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CResult* pResult = NULL;

	// �쐬
	pResult = new CResult;

	// ������
	pResult->Init(mode, pDevice);

	return pResult;
}
//----EOF----