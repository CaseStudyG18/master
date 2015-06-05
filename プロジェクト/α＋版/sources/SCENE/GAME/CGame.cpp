//=============================================================================
//
// CGame�N���X [CGame.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "THREAD\CThreadManager.h"
#include "CGame.h"
#include "../../JUDGE/CJudgeManager.h"
#include "FIELD/CFieldManager.h"
#include "UI\CCountDown.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

// �Q�[���̐�������
static const short GAME_TIME = 300;

// �󕨂̏ꏊ
static const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(200, 100, 0);

// �S�[��
static const short GOAL_MAX = 4;
static const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(100, 100, 0),
	D3DXVECTOR3(700, 100, 0),
	D3DXVECTOR3(100, 500, 0),
	D3DXVECTOR3(700, 500, 0),
};
static const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0,1,2,3
};

// �v���C���l��
static const short MANUAL_PLAYER_NUM = 2;
static const short CPU_PLAYER_NUM = 0;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGame ::CGame(void)
{
	m_pPause = NULL;
	m_pTimeManager = NULL;
	m_pPlayerManager = NULL;
	m_pAttackManager = NULL;
	m_pThreadManager = NULL;
	m_pGoalManager = NULL;
	m_pJudgeManager = NULL;
	m_pFieldManager = NULL;
	m_pCountDown = NULL;

	// �v���C������\�t���O
	m_bPlayerControl = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGame ::~CGame(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CGame::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// �t�F�[�Y�̏�����
	CPhase::Init(pDevice, mode);

	// �t�F�[�h�쐬
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// �|�[�Y�쐬
	m_pPause = new CPause(pDevice);
	m_pPause->Init(DEFFAULT_PAUSE_POS, (float)SCREEN_WIDTH, DEFFAULT_PAUSE_HEIGHT, TEXTURE_NULL);

	// �t�F�[�h�C���J�n
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// �Q�[���̎��ԊǗ��Ǝc�莞�ԕ\��
	m_pTimeManager = new CTimeManager(pDevice, GAME_TIME);
	m_pTimeManager->Init();

	// �U���}�l�[�W������
	m_pAttackManager = new CAttackManager(pDevice);
	m_pAttackManager->Init();

	// ���}�l�[�W������
	m_pThreadManager = new CThreadManager(pDevice);
	m_pThreadManager->Init();

	// �v���C������
	m_pPlayerManager = new CPlayerManager(m_pAttackManager, m_pThreadManager);
	m_pPlayerManager->Init(CPU_PLAYER_NUM, MANUAL_PLAYER_NUM);

	// �󕨐���
	m_pTreasureManager = new CTreasureManager(pDevice);
	m_pTreasureManager->Init();
	m_pTreasureManager->CreateTreasure(TREASURE_POS);

	// �S�[������
	m_pGoalManager = new CGoalManager(pDevice);
	m_pGoalManager->Init();
	m_pGoalManager->CreateGoal(
		const_cast<D3DXVECTOR3*>(GOAL_POS),
		const_cast<short*>(GOAL_PLAYER_NUMBER),
		GOAL_MAX);

	// ���Đ�
	CManager::PlaySoundA(SOUND_LABEL_BGM000);

	// �W���b�W�쐬
	m_pJudgeManager = CJudgeManager::Create(m_pPlayerManager);

	// �t�B�[���h�쐬
	m_pFieldManager = new CFieldManager;
	m_pFieldManager->LoadField(m_pD3DDevice, CFieldManager::FIELD_TEST);
	// �J�E���g�_�E��
	m_pCountDown = new CCountDown(m_pD3DDevice, &m_bPlayerControl);
	m_pCountDown->Init();

}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	if (m_pCountDown){
		m_pCountDown->Uninit();
		SAFE_DELETE(m_pCountDown);
	}

	if (m_pJudgeManager){
		m_pJudgeManager->Uninit();
		SAFE_DELETE(m_pJudgeManager);
	}
	if (m_pFieldManager){
		m_pFieldManager->Uninit();
		SAFE_DELETE(m_pFieldManager);
	}

	if (m_pThreadManager){
		m_pThreadManager->Uninit();
		SAFE_DELETE(m_pThreadManager);
	}

	if (m_pAttackManager){
		m_pAttackManager->Uninit();
		SAFE_DELETE(m_pAttackManager);
	}

	if (m_pPlayerManager){
		m_pPlayerManager->Uninit();
		SAFE_DELETE(m_pPlayerManager);
	}

	if (m_pGoalManager){
		m_pGoalManager->Uninit();
		SAFE_DELETE(m_pGoalManager);
	}

	if (m_pTreasureManager){
		m_pTreasureManager->Uninit();
		SAFE_DELETE(m_pTreasureManager);
	}

	if (m_pTimeManager){
		m_pTimeManager->Uninit();
		SAFE_DELETE(m_pTimeManager);
	}
	
	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	// �J�E���g�_�E���̍X�V
	m_pCountDown->Update();

	// �o�������ꂽ��
	if (CInputKeyboard::GetKeyboardTrigger(DIK_P))
	{
		// �|�[�Y�t���O���]
		m_pPause->ReverceFlag();
		m_pPause->SetCursolDrawFlag(false);

		// �|�[�Y�I���Ȃ�
		if (!m_pPause->GetPauseFlag())
		{
			// �J�[�\���ʒu��߂��
			m_pPause->SetChoiceMenu(m_pPause->PAUSE_RETURN);
		}
	}

	if (!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		// �v���C���̑��삪�ł���Ȃ�
		if (m_bPlayerControl){
			// �}�l�[�W���[�X�V
			m_pPlayerManager->Update();
			m_pTimeManager->Update();
			m_pTreasureManager->Update();
			m_pJudgeManager->Update();
			m_pFieldManager->Update();
		}

		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// ���U���g��
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}
	}

	// �^�C�g���ɖ߂�I���Ȃ�
	if (m_pPause->GetReturnTitleFlag())
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �^�C�g����
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// ���g���C�ɖ߂�I���Ȃ�
	if (m_pPause->GetRetryFlag())
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