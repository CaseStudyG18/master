//=============================================================================
//
// CThreadManager�N���X [CAttacManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CThreadSpecialAttack.h"
#include "../PLAYER/CPlayer.h"
#include "../../CSCENE/CSceneAnime.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
static const short THREAD_ATTACK_END_TIME = 180;
// �����蔻��̎n�܂鎞��
static const short THREAD_ATTACK_HIT_START_TIME = 60;
// �����蔻��̏I��鎞��
static const short THREAD_ATTACK_HIT_END_TIME = 170;

// �����蔻 �蕝,����
static const float THREAD_ATTACK_HIT_WIDTH = 50;
static const float THREAD_ATTACK_HIT_HEIGHT = 50;

// �v���C���ƍU���G�t�F�N�g�̋���
static const short THREAD_ATTACK_RANGE = 50;

// �ŏ��̃G�t�F�N�g���o������
static const short THREAD_FIRST_ATTACK_CREATE_TIME = 60;

// 2��ڂ̃G�t�F�N�g���o������
static const short THREAD_SECOND_ATTACK_CREATE_TIME = 120;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CThreadSpecialAttack::CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority, OBJTYPE type) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_ATTACK_THREAD;

	// ���̎��̌ŗL�X�e�[�^�X������
	m_fWidth = THREAD_ATTACK_HIT_WIDTH;
	m_fHeight = THREAD_ATTACK_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = THREAD_ATTACK_END_TIME;
	m_nHitStartTime = THREAD_ATTACK_HIT_START_TIME;
	m_nHitEndTime = THREAD_ATTACK_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CThreadSpecialAttack ::~CThreadSpecialAttack(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CThreadSpecialAttack::Init(D3DXVECTOR3 pos, short playerNumber , D3DXVECTOR3 velocity)
{
	CAttackBase::Init();

	m_nPlayerNum = playerNumber;
	m_vPos = pos + (velocity * THREAD_ATTACK_RANGE);
	m_vVelocity = velocity;

	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CThreadSpecialAttack::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CThreadSpecialAttack::Update(void)
{
	CAttackBase::Update();

#ifdef _DEBUG
//	CDebugProc::Print("�U�������`�Ԃ̎�������\n");
#endif

	// �J�E���g��10�̂Ƃ��i���j�G�t�F�N�g�͔���
	if (m_nCount == THREAD_FIRST_ATTACK_CREATE_TIME){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, 40);
	}

	// �J�E���g��10�̂Ƃ��i���j�G�t�F�N�g�͔���
	if (m_nCount >= THREAD_SECOND_ATTACK_CREATE_TIME){
		if (m_nCount % 11 == 0)
		{
			CSceneAnime::Create(
				m_pD3DDevice,
				m_vPos, 100, 100,
				TEXTURE_FIRE_0, 10, 1, 60);
		}
		if (m_nCount % 4 == 0)
		{
			m_vPos += m_vVelocity*50.0f;
		}

		else if (m_nCount % 4 == 1)
		{
			m_vPos -= m_vVelocity*50.0f;
		}

		else if (m_nCount % 4 == 2)
		{
			m_vPos += m_vVelocity*80.0f;
		}

		else if (m_nCount % 4 == 3)
		{
			m_vPos -= m_vVelocity*75.0f;
		}
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CThreadSpecialAttack* CThreadSpecialAttack::Create(LPDIRECT3DDEVICE9 *pDevice, short nPlayerNum, D3DXVECTOR3 pos, D3DXVECTOR3 velocity)
{
	// �쐬
	CThreadSpecialAttack* p = new CThreadSpecialAttack(pDevice);

	// ������
	p->Init(pos, nPlayerNum, velocity);


	return p;
}

//*****************************************************************************
//	�`��
//*****************************************************************************
void CThreadSpecialAttack::Draw(void)
{
	CAttackBase::Draw();
}

//*****************************************************************************
//
//*****************************************************************************
void CThreadSpecialAttack::HitPlayer(CPlayer* pPlayer)
{
	CSceneAnime::Create(
		m_pD3DDevice,
		pPlayer->GetPos(), 100, 100,
		TEXTURE_FIRE_1, 10, 1, 40);
}

//----EOF-------