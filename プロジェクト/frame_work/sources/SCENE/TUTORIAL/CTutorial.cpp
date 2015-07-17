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
// �G�̈ړ���
static const D3DXVECTOR3 TUTORIAL_PICT_VELO = D3DXVECTOR3(100, 0, 0);
// �G�̊��������ʒu
static const float TUTORIAL_PICT_RESIST = 0.98f;
// �G�̈ʒu
static const D3DXVECTOR3 TUTORIAL_PICT_POS[TUTORIAL_PICT_MAX] = {
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f - SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
	D3DXVECTOR3(SCREEN_WIDTH * 0.5f + SCREEN_WIDTH, SCREEN_HEIGHT * 0.5f, 0),
};
// �y�[�W���ɑΉ������e�N�X�`��
static const TEXTURE_TYPE TUTORIAL_PAGE_TEXTURE[TUTORIAL_PAGE_MAX] = {
//	TEXTURE_TUTORIAL_0,
//	TEXTURE_TUTORIAL_1,
//	TEXTURE_TUTORIAL_2,
//	TEXTURE_TUTORIAL_3,
	TEXTURE_STAGE_1,
	TEXTURE_STAGE_2,
	TEXTURE_STAGE_3,
	TEXTURE_STAGE_4,
};

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
	m_vVelo = D3DXVECTOR3(0, 0, 0);
	m_nPage = 0;
	m_bRight = true;

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �w�i
	m_pBackGround = CScene2D::Create(m_pD3DDevice,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0),
		static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
		TEXTURE_BG_TUTORIAL, TYPE_PRIORITY_BG);

	// �`���[�g���A���̊G
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_p2D[n] = CScene2D::Create(m_pD3DDevice,
			TUTORIAL_PICT_POS[n],
			static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT),
			TEXTURE_TUTORIAL_0, TYPE_PRIORITY_FIELD);
	}

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
		CControllerManager::GetTriggerKey(CInputGamePad::KEY_DECIDE, 0)){

		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}


	// �G���~�܂��Ă���Ƃ��̂ݓ�������
	if (m_vVelo.x == 0){
		// ���E�L�[�Ŏ��̃y�[�W�ֈړ�
		for (int i = 0; i < PLAYER_MAX; i++){
			if (CInputKeyboard::GetKeyboardTrigger(DIK_A) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_RIGHT, i)){
				// �G�̈ړ��ʂ�����
				m_vVelo = TUTORIAL_PICT_VELO;
				// �t���O�؂�ւ�
				m_bRight = true;
				// �y�[�W�؂�ւ�
				m_nPage--;
				if (m_nPage <= -1){
					m_nPage = TUTORIAL_PAGE_MAX - 1;
	  			}
				// �e�N�X�`���؂�ւ�
				m_p2D[0]->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
				// ��񂾂��ʂ�
				break;
			}
			else if (CInputKeyboard::GetKeyboardTrigger(DIK_D) ||
				CInputGamePad::GetGamePadTrigger(CInputGamePad::LEFT_STICK_LEFT, i)){

				// �G�̈ړ��ʂ�����
				m_vVelo = -TUTORIAL_PICT_VELO;
				// �t���O�؂�ւ�
				m_bRight = false;
				// �y�[�W�؂�ւ�
				m_nPage++;
				if (m_nPage > TUTORIAL_PAGE_MAX - 1){
					m_nPage = 0;
				}
				// �e�N�X�`���؂�ւ�
				m_p2D[2]->ChangeTexture(TUTORIAL_PAGE_TEXTURE[m_nPage]);
				// ��񂾂��ʂ�
				break;
			}
		}
	}

	// �G�̓���
	for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
		m_vVelo *= TUTORIAL_PICT_RESIST;
		m_p2D[n]->AddPos(m_vVelo);
	}

	// �[���������蔻��
	if (m_bRight){
		if (m_p2D[0]->GetPos().x >= SCREEN_WIDTH * 0.5f){
			// �ړ��ʏ�����
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// �G�̃|�C���^�̓���ւ�
			CScene2D *t;
			t = m_p2D[0];
			m_p2D[0] = m_p2D[1];
			m_p2D[1] = m_p2D[2];
			m_p2D[2] = t;

			// �S�ʒu����
			for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
				m_p2D[n]->SetPos(TUTORIAL_PICT_POS[n]);
			}
		}
	}
	else{
		if (m_p2D[2]->GetPos().x <= SCREEN_WIDTH * 0.5f){
			// �ړ��ʏ�����
			m_vVelo = D3DXVECTOR3(0, 0, 0);

			// �G�̃|�C���^�̓���ւ�
			CScene2D *t;
			t = m_p2D[2];
			m_p2D[2] = m_p2D[1];
			m_p2D[1] = m_p2D[0];
			m_p2D[0] = t;

			// �S�ʒu����
			for (int n = 0; n < TUTORIAL_PICT_MAX; n++){
				m_p2D[n]->SetPos(TUTORIAL_PICT_POS[n]);
			}
		}
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