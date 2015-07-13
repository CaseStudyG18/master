//=============================================================================
//
// CStageSelect�N���X [CStageSelect.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CStageSelect.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �X�e�[�W�G�̑傫��
static const float STAGE_SELECT_WIDTH = 400;
static const float STAGE_SELECT_HEIGHT = 250;
// �X�e�[�W�G�̍��W�i����A�E��A�����A�E���j
static const float STAGE_SELECT_WIDTH_ONE = SCREEN_WIDTH / 4;
static const D3DXVECTOR3 STAGE_SELECT_POS[] = {
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 200, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 200, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 1, 500, 0),
	D3DXVECTOR3(STAGE_SELECT_WIDTH_ONE * 3, 500, 0),
};
// �X�e�[�W�G�̃e�N�X�`���i����A�E��A�����A�E���j
static const TEXTURE_TYPE STAGE_SELECT_TEX[] = {
	TEXTURE_STAGE_0,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
};
// �I��g�̑傫���A�F
static const float STAGE_SELECT_FRAME_WIDTH = 420;
static const float STAGE_SELECT_FRAME_HEIGHT = 270;
static const D3DXCOLOR STAGE_SELECT_FRAME_COLOR = D3DXCOLOR(0.9f, 0.1f, 0.0f, 0.0f);
// �I��g�̃A���t�@�ύX�X�s�[�h
static const float STAGE_SELECT_FRAME_AALPHA_SPEED = 0.05f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CStageSelect::CStageSelect(void)
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CStageSelect::~CStageSelect(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CStageSelect::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);
	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �w�i
	m_pBG = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_BG);
	m_pBG->Init(
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_STAGE_SELECT);

	// �e�X�e�[�W�̊G
	for (int i = 0; i < STAGE_MAX; i++){
		m_pStage2D[i] = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_UI);
		m_pStage2D[i]->Init(
			STAGE_SELECT_POS[i],
			STAGE_SELECT_WIDTH, STAGE_SELECT_HEIGHT,
			STAGE_SELECT_TEX[i]);
	}

	// �I�𒆂̘g PRIORITY_UI�����O�ɕ\������������EFFECT�ɂ���
	m_pFrame2D = new CScene2D(m_pD3DDevice, TYPE_PRIORITY_EFFECT);
	m_pFrame2D->Init(
		STAGE_SELECT_POS[0],
		STAGE_SELECT_FRAME_WIDTH, STAGE_SELECT_FRAME_HEIGHT,
		TEXTURE_NULL);

	// �I�𒆂̘g�ɐF�ݒ�
	m_FrameColor = STAGE_SELECT_FRAME_COLOR;
	m_pFrame2D->SetColorPolygon(m_FrameColor);

	// �g�p�ϐ�������
	m_nSelectNum = 0;
	m_bFrameAlphaAdd = true;

	// �a�f�l�Đ�
//	CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CStageSelect::Uninit(void)
{
	// ����~
	CManager::StopSound();

	// �t�F�C�Y�̏I��
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CStageSelect::Update(void)
{
	// �t�F�C�Y�̍X�V
	CPhase::Update();

	// �I���̍X�V
	UpdateSelect();

	// ����{�^������������
	if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) ||
		CInputGamePad::GetGamePadTrigger(CInputGamePad::KEY_DECIDE, 0)){

		// �I�������X�e�[�W�ԍ����Z�b�g
		CManager::SetStageNum(m_nSelectNum);

		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		// �Q�[���w
		m_pManager->SetNextPhase(MODE_PHASE_GAME);
	}
}

//*****************************************************************************
// �N���G�C�g
//*****************************************************************************
CStageSelect* CStageSelect::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	CStageSelect* pTitle = NULL;

	// �쐬
	pTitle = new CStageSelect;

	// ������
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// �I�𒆂̍X�V
//*****************************************************************************
void CStageSelect::UpdateSelect(){
	
	// ���͂őI����ύX
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0)){
		if (m_nSelectNum == 0)
			m_nSelectNum = 1;
		else if (m_nSelectNum == 2)
			m_nSelectNum = 3;
		else
			m_nSelectNum--;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0)){
		if (m_nSelectNum == 1)
			m_nSelectNum = 0;
		else if (m_nSelectNum == 3)
			m_nSelectNum = 2;
		else
			m_nSelectNum++;
	}
	if (CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0)){
		if (m_nSelectNum == 0)
			m_nSelectNum = 2;
		else if (m_nSelectNum == 1)
			m_nSelectNum = 3;
		else
			m_nSelectNum -= 2;
	}
	else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		if (m_nSelectNum == 2)
			m_nSelectNum = 0;
		else if (m_nSelectNum == 3)
			m_nSelectNum = 1;
		else
			m_nSelectNum += 2;
	}

	// �ړ��̓��͂���������
	if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_W) ||
		CInputKeyboard::GetKeyboardTrigger(DIK_S) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_RIGHT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_LEFT, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_UP, 0) ||
		CControllerManager::GetTriggerKey(CInputGamePad::LEFT_STICK_DOWN, 0)){
		// �I��g�̈ړ�
		m_pFrame2D->SetPos(STAGE_SELECT_POS[m_nSelectNum]);
		// �Ȃ񂩕s���R����������F��������
		m_bFrameAlphaAdd = true;
		m_FrameColor.a = 0.0f;
	}

	// �I��g�̐F�ύX
	if (m_bFrameAlphaAdd){
		m_FrameColor.a += STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a > 1.0f){
			m_FrameColor.a = 1.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	else {
		m_FrameColor.a -= STAGE_SELECT_FRAME_AALPHA_SPEED;
		if (m_FrameColor.a < 0.0f){
			m_FrameColor.a = 0.0f;
			m_bFrameAlphaAdd = !m_bFrameAlphaAdd;
		}
	}
	m_pFrame2D->SetColorPolygon(m_FrameColor);
}
//----EOF----