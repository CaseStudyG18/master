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
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../EFFECT/CEffect.h"


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

	// �J����������
	m_pManager->InitCamera();

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// BGM�Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	int num = 0;
	const TEXTURE_TYPE *tex = {};
	const int kFireMax = 8;

	for (int n = 0; n < kFireMax; n++){
		num = rand() % 10;
		for (int i = 0; i < num; i++){
			D3DXVECTOR3 pos = D3DXVECTOR3(rand() % 400, rand() % 400, 0);
			float size = 50 + rand() % 200;
			CEffect::Create(pDevice, pos, size, size, TEXTURE_FIRE_0, 10, 1, 1, -1);
		}
	}

//	m_Fire = NULL;
//	m_Fire = CEffect::Create(pDevice, D3DXVECTOR3(200, 100, 0), 200, 300, TEXTURE_FIRE_0, 10, 1, 1, -1);
}


//*****************************************************************************
// �I��
//*****************************************************************************
void CResult::Uninit(void)
{
	CManager::StopSound();
	CPhase::Uninit();

//	m_Fire->Release();
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