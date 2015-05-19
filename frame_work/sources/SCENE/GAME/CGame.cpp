//=============================================================================
//
// CGame�N���X [CGame.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../../CAMERA/CCamera.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../CSCENE/CSceneScore.h"
#include "CTimeManager.h"
#include "PLAYER\CPlayerManager.h"
#include "TREASURE/CTreasureManager.h"
#include "GOAL/CGoalManager.h"
#include "ATTACK\CAttackManager.h"
#include "CGame.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************

// �Q�[���̐�������
const short GAME_TIME = 300;

// �󕨂̏ꏊ
const D3DXVECTOR3 TREASURE_POS = D3DXVECTOR3(200, 100, 0);

// �S�[��
const short GOAL_MAX = 4;
const D3DXVECTOR3 GOAL_POS[GOAL_MAX] = {
	D3DXVECTOR3(100, 100, 0),
	D3DXVECTOR3(700, 100, 0),
	D3DXVECTOR3(100, 500, 0),
	D3DXVECTOR3(700, 500, 0),
};
const short GOAL_PLAYER_NUMBER[GOAL_MAX] = {
	0,1,2,3
};

// �v���C���l��
const short MANUAL_PLAYER_NUM = 1;
const short CPU_PLAYER_NUM = 3;

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

	// �J����������
	m_pManager->InitCamera();

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

	// �v���C������
	m_pPlayerManager = new CPlayerManager(m_pAttackManager);
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
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGame::Uninit(void)
{
	m_pAttackManager->Uninit();
	SAFE_DELETE(m_pAttackManager);

	m_pPlayerManager->Uninit();
	SAFE_DELETE(m_pPlayerManager);

	m_pGoalManager->Uninit();
	SAFE_DELETE(m_pGoalManager);

	m_pTreasureManager->Uninit();
	SAFE_DELETE(m_pTreasureManager);

	m_pTimeManager->Uninit();
	SAFE_DELETE(m_pTimeManager);

	CManager::StopSound();
	CPhase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGame::Update(void)
{
	// �}�l�[�W���[�X�V
	m_pPlayerManager->Update();
	m_pTimeManager->Update();
	m_pTreasureManager->Update();

	// �o�������ꂽ��
	if(CInputKeyboard::GetKeyboardTrigger(DIK_P))
	{
		// �|�[�Y�t���O���]
		m_pPause->ReverceFlag();
		m_pPause->SetCursolDrawFlag(false);

		// �|�[�Y�I���Ȃ�
		if(!m_pPause->GetPauseFlag())
		{
			// �J�[�\���ʒu��߂��
			m_pPause->SetChoiceMenu(m_pPause->PAUSE_RETURN);
		}
	}

	if(!m_pPause->GetPauseFlag())
	{
		CPhase::Update();

		if(CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// �t�F�[�h�A�E�g�J�n
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// ���U���g��
			m_pManager->SetNextPhase(MODE_PHASE_RESULT);
		}
	}

	// �^�C�g���ɖ߂�I���Ȃ�
	if(m_pPause->GetReturnTitleFlag())
	{
		// �t�F�[�h�A�E�g�J�n
		m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

		// �^�C�g����
		m_pManager->SetNextPhase(MODE_PHASE_TITLE);
	}

	// ���g���C�ɖ߂�I���Ȃ�
	if(m_pPause->GetRetryFlag())
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