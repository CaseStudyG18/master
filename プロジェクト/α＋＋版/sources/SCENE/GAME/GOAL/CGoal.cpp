//=============================================================================
//
// CGoalManager�N���X [CGoalManager.cpp]
// Author : �˖{�@�r�F
//
// �󕨂������ɉ^�� ���ł͊G����
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CGoal.h"
#include "../CGame.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
static const float GOAL_WIDTH = 80;
static const float GOAL_HEIGHT = 80;
static const TEXTURE_TYPE GOAL_TEXTURE = TEXTURE_GOAL;

// �]�����鎞�ԁi1�i�K��<first>�̎���2�i�K��<second>�j
static const int TRANSE_INTERVAL_FIRST = 60;
static const int TRANSE_INTERVAL_SECOND = 120;
static const int TRANSE_INTERVAL_THIRD = 180;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CGoal::CGoal(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) : CScene2D(pDevice, nPriority, m_objType)
{
	m_State = GOAL_STATE_NONE;
	m_nCount = 0;
	m_bTransFlg = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CGoal ::~CGoal(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CGoal::Init()
{

}

//*****************************************************************************
// �I��
//*****************************************************************************
void CGoal::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CGoal::Update(void)
{
#ifdef _DEBUG
	if (m_State == GOAL_STATE_ARRIVE){
		CDebugProc::Print("GOAL_STATE_ARRIVE\n");
	}
	else if (m_State == GOAL_STATE_TRANS_FIRST){
		CDebugProc::Print("GOAL_STATE_FIRST\n");
	}
	else if (m_State == GOAL_STATE_TRANS_SECOND){
		CDebugProc::Print("GOAL_STATE_SECOND\n");
	}
	else if (m_State == GOAL_STATE_TRANS_THIRD){
		CDebugProc::Print("GOAL_STATE_THIRD\n");
	}
	else if (m_State == GOAL_STATE_TRANSED){
		CDebugProc::Print("GOAL_STATE_TRANSED\n");
	}
#endif

	// ���������Ƃ�
	if(m_State == GOAL_STATE_ARRIVE){
		// ��������
		m_nCount = 0;

		// �]����i�K�ڂɂ���
		m_State = GOAL_STATE_TRANS_FIRST;
	}
	else if (m_State == GOAL_STATE_TRANS_FIRST){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_FIRST){
			m_State = GOAL_STATE_TRANS_SECOND;
		}
	}
	else if (m_State == GOAL_STATE_TRANS_SECOND){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_SECOND){
			m_State = GOAL_STATE_TRANS_THIRD;
		}
	}
	else if (m_State == GOAL_STATE_TRANS_THIRD){
		m_nCount++;
		if (m_nCount == TRANSE_INTERVAL_THIRD){
			m_State = GOAL_STATE_TRANSED;
		}
	}
	else if (m_State == GOAL_STATE_TRANSED){
		// �Q�[���I��
		m_pGame->SetGameOver();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CGoal* CGoal::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, short nPlayerNum, CGame *pGame)
{
	// �쐬
	CGoal* p = new CGoal(pDevice);

	// ������
	p->CScene2D::Init(pos, GOAL_WIDTH, GOAL_HEIGHT, GOAL_TEXTURE);

	p->m_nPlayerNum = nPlayerNum;
	p->m_pGame = pGame;
	return p;
}

//*****************************************************************************
// �]�����ɃZ�b�g����@Judge�̍X�V�ŕ���������v���C�����S�[���ɂ���Ώ�ɌĂ΂��
//*****************************************************************************
void CGoal::SetTrans(){

	// ���łɃZ�b�g����Ă���Ȃ�Ȃɂ����Ȃ�
	if (m_bTransFlg){
		return;
	}
	else{
		// �S�[���ɒ�����
		m_State = GOAL_STATE_ARRIVE;
		m_bTransFlg = true;
	}
}

//*****************************************************************************
// �]��������Ȃ��Z�b�g����@
//*****************************************************************************
void CGoal::SetNoTrans(){

	m_State = GOAL_STATE_NONE;
	m_bTransFlg = false;
}

//----EOF-------