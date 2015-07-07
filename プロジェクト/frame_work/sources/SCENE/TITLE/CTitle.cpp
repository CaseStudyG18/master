//=============================================================================
//
// CTitle�N���X [CTitle.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CTitle.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �Q�[�����S
static const float TITLE_LOGO_WIDTH = 500;
static const float TITLE_LOGO_HEIGHT = 200;
static const D3DXVECTOR3 TITLE_LOGO_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 280, 0);
// PUSH START
static const float TITLE_PUSHSTART_WIDTH = 300;
static const float TITLE_PUSHSTART_HEIGHT = 100;
static const D3DXVECTOR3 TITLE_PUSHSTART_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500, 0);
// MENU
static const float TITLE_MENU_WIDTH = 500;
static const float TITLE_MENU_HEIGHT = 300;
static const D3DXVECTOR3 TITLE_MENU_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0);
// MENU�̃J�[�\��
static const int TITLE_MENU_MAX = 3;
static const float TITLE_CURSOL_WIDTH = 500;
static const float TITLE_CURSOL_HEIGHT = 100;
static const D3DXVECTOR3 TITLE_CURSOL_POS[TITLE_MENU_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 210, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 310, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 410, 0),
};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTitle ::CTitle(void)
{
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTitle ::~CTitle(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CTitle::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �w�i
	m_pBackGround = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBackGround->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_STAGE_SELECT);
	// Logo
	m_pLogo = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pLogo->Init(
		TITLE_LOGO_POS, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// PushStart
	m_pPushStart = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pPushStart->Init(
		TITLE_PUSHSTART_POS, TITLE_PUSHSTART_WIDTH, TITLE_PUSHSTART_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// ���j���[
	m_pMenu = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pMenu->Init(
		TITLE_MENU_POS, TITLE_MENU_WIDTH, TITLE_MENU_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);
	// �J�[�\��
	m_nCursol = 0;
	m_pCursol = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
	m_pCursol->Init(
		TITLE_CURSOL_POS[m_nCursol], TITLE_CURSOL_WIDTH, TITLE_CURSOL_HEIGHT,
		TEXTURE_BG_STAGE_SELECT);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �a�f�l�Đ�
//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CTitle::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CTitle::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0))
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_CHARA_SELECT);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CTitle* CTitle::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CTitle* pTitle = NULL;

	// �쐬
	pTitle = new CTitle;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//----EOF----