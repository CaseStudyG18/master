//=============================================================================
//
// CThreadManager�N���X [CAttacManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CThreadNormal.h"
#include "../EFFECT/CEffect.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
const short THREAD_NORMAL_END_TIME = 180;
// �����蔻��̎n�܂鎞��
const short THREAD_NORMAL_HIT_START_TIME = 60;
// �����蔻��̏I��鎞��
const short THREAD_NORMAL_HIT_END_TIME = 120;

// �����蔻 �蕝,����
const float THREAD_NORMAL_HIT_WIDTH = 50;
const float THREAD_NORMAL_HIT_HEIGHT = 50;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadNormal::CThreadNormal(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CThreadBase(pDevice, priority, type)
{
	// �ϐ�������
	m_ThreadType = THREAD_TYPE_NORMAL;

	// ���̎��̌ŗL�X�e�[�^�X������
	m_fWidth = THREAD_NORMAL_HIT_WIDTH;
	m_fHeight = THREAD_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_NORMAL_END_TIME;
	m_nHitStartTime = THREAD_NORMAL_HIT_START_TIME;
	m_nHitEndTime = THREAD_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadNormal ::~CThreadNormal(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CThreadNormal::Init()
{
	CThreadBase::Init(m_vPos, 100, 100, TEXTURE_THREAD);

	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadNormal::Uninit(void)
{
	CThreadBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadNormal::Update(void)
{
	CThreadBase::Update();

	//// �J�E���g��10�̂Ƃ��i���j�G�t�F�N�g�͔���
	//if (m_nCount == 10){
	//	CEffect::Create(
	//		m_pD3DDevice,
	//		m_vPos, 100, 100,
	//		TEXTURE_THREAD, 10, 1, 20);
	//}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CThreadNormal* CThreadNormal::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos)
{
	// �쐬
	CThreadNormal* p = new CThreadNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos;

	// ������
	p->Init();


	return p;
}

void CThreadNormal::Draw(void)
{
	CThreadBase::Draw();
}
//----EOF-------