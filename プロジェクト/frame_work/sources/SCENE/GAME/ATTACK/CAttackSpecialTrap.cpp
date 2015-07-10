//=============================================================================
//
// CAttacSpecialSpeed�N���X [CAttacSpecialSpeed.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAttackSpecialTrap.h"
#include "../EFFECT/CEffectManager.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
const short ATTACK_TRAP_END_TIME = 1000;
// �����蔻��̎n�܂鎞��
const short ATTACK_TRAP_HIT_START_TIME = 100;
// �����蔻��̏I��鎞��
const short ATTACK_TRAP_HIT_END_TIME = 950;

// �����蔻 �蕝,����
const float ATTACK_TRAP_HIT_WIDTH = 30;
const float ATTACK_TRAP_HIT_HEIGHT = 30;

// �v���C���ƍU���G�t�F�N�g�̋���
static const float ATTACK_TRAP_RANGE = 0;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackSpecialTrap::CAttackSpecialTrap(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_NORMAL;

	// ���̍U���̌ŗL�X�e�[�^�X������
	m_fWidth = ATTACK_TRAP_HIT_WIDTH;
	m_fHeight = ATTACK_TRAP_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_TRAP_END_TIME;
	m_nHitStartTime = ATTACK_TRAP_HIT_START_TIME;
	m_nHitEndTime = ATTACK_TRAP_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackSpecialTrap ::~CAttackSpecialTrap(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAttackSpecialTrap::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackSpecialTrap::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackSpecialTrap::Update(void)
{
	CAttackBase::Update();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAttackSpecialTrap* CAttackSpecialTrap::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// �쐬
	CAttackSpecialTrap* p = new CAttackSpecialTrap(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos; // -(velocity * ATTACK_TRAP_RANGE);

	// ������
	p->Init();

	// �G�t�F�N�g����
	CEffectManager::CreateEffect(pos,EFFECT_TRAP_FOUNDATION,velocity);

	return p;
}

//=============================================================================
//	�v���C���[�ɓ����������̏���
//=============================================================================
void CAttackSpecialTrap::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->AddHp(-10.0f);

	// 㩂ɂ��������G�t�F�N�g����
	CEffectManager::CreateEffect(pPlayer->GetPos(), EFFECT_CAUGHT_TRAP,D3DXVECTOR3(0.0f,0.0f,0.0f));
}

//----EOF-------