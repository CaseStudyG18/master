//=============================================================================
//
// CCountDown�N���X [CCountDown.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CCountDown.h"
#include "../ANIMATION/CAnimation.h"

//*****************************************************************************
// �萔
//*****************************************************************************
// �����̐����ʒu(visible)�A�o���ʒu�A�傫���A�e�N�X�`���A�e�N�X�`���̕�����
static const D3DXVECTOR3 COUNTDOWN_FIGURE_CREATE_POS = D3DXVECTOR3(0, -100, 0);
static const D3DXVECTOR3 COUNTDOWN_FIGURE_APPEAR_POS = D3DXVECTOR3(SCREEN_WIDTH*0.5f, 0, 0);
static const float COUNTDOWN_FIGURE_WIDTH = 100;
static const float COUNTDOWN_FIGURE_HEIGHT = 100;
static const TEXTURE_TYPE COUNTDOWN_FIGURE_TEXTURE = TEXTURE_NUMBER;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X = 10;
static const int COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y = 1;
// �����̗�������W
static const float COUNTDOWN_FIGURE_FALL_POSY = SCREEN_HEIGHT * 0.5f;
// �����̃t�F�[�h�X�s�[�h
static const float COUNTDOWN_FIGURE_FADE_SPEED = 0.03f;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CCountDown::CCountDown(LPDIRECT3DDEVICE9 *pDevice, bool *bPlayerControl)
{
	m_pDevice = pDevice;
	m_pFigure = NULL;
	m_nCount = 0;
	m_bPlayerControl = bPlayerControl;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CCountDown ::~CCountDown(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CCountDown::Init()
{
	m_pFigure = CAnimation::Create(
		m_pDevice,
		TYPE_PRIORITY_UI,
		COUNTDOWN_FIGURE_CREATE_POS,
		COUNTDOWN_FIGURE_WIDTH,
		COUNTDOWN_FIGURE_HEIGHT,
		COUNTDOWN_FIGURE_TEXTURE,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_X,
		COUNTDOWN_FIGURE_TEXTURE_SEPALATE_Y);
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CCountDown::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CCountDown::Update(void){

	m_nCount++;

	// �Q�[�����n�܂��Ă���J�E���g�_�E�����n�܂�܂ł̃J�E���g��
	short TIME_START = 20;
	// �����������Ă���t�F�[�h�A�E�g�����n�߂�Ԃ̃J�E���g��
	short TIME_START_TO_FADE = 60;
	// �t�F�[�h�A�E�g���n�߂Ă��玟�̐�����������Ԃ̃J�E���g��
	short TIME_NEXT_COUNT = 20;

#define COUNT_MAX 5

	short TIME[COUNT_MAX] = { 0 };

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		TIME[i] = TIME_START + (TIME_START + TIME_START_TO_FADE + TIME_NEXT_COUNT) * (3 - i);
	}

	for (int i = COUNT_MAX - 1; i >= 0; i--){
		if (m_nCount == TIME[i]){
			m_pFigure->SetIndex(i);
			m_pFigure->SetNonFall(COUNTDOWN_FIGURE_CREATE_POS);
			m_pFigure->SetNonFadeOut();
			m_pFigure->SetFall(COUNTDOWN_FIGURE_APPEAR_POS, COUNTDOWN_FIGURE_FALL_POSY);
		}
		if (m_nCount == TIME[i] + TIME_START_TO_FADE){
			m_pFigure->SetFadeOut(COUNTDOWN_FIGURE_FADE_SPEED);
			
			if (i == 0){
				*m_bPlayerControl = true;
			}
		}
	}
}

//----EOF-------