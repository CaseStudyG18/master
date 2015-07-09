//=============================================================================
//
// CCharaSelect�N���X [CCharaSelect.cpp]
// Author : �˖{�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CCharaSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneAnime.h"
#include "../GAME/PLAYER/CPlayer.h"

//*****************************************************************************
// �萔
//*****************************************************************************


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCharaSelect::CCharaSelect(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCharaSelect ::~CCharaSelect(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CCharaSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �w�i
	InitializeBG();

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
	//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCharaSelect::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CCharaSelect::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_STAGE_SELECT);
	}

}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CCharaSelect* CCharaSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CCharaSelect* pTitle = NULL;

	// �쐬
	pTitle = new CCharaSelect;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//-----------------------------------------------------------------------------
// �w�i������
//-----------------------------------------------------------------------------
void CCharaSelect::InitializeBG(void){
	m_pBG = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_CHARA_SELECT, TYPE_PRIORITY_BG);
}

//----EOF----