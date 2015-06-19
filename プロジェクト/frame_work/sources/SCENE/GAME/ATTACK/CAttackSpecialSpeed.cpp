//=============================================================================
//
// CAttacSpecialSpeed�N���X [CAttacSpecialSpeed.cpp]
// Author : �����@�Ȉ�
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAttackSpecialSpeed.h"
#include "../../../SCENE/CSCENE/CSceneAnime.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
const short ATTACK_SPEED_END_TIME = 10;
// �����蔻��̎n�܂鎞��
const short ATTACK_SPEED_HIT_START_TIME = 1;
// �����蔻��̏I��鎞��
const short ATTACK_SPEED_HIT_END_TIME = 9;

// �����蔻 �蕝,����
const float ATTACK_SPEED_HIT_WIDTH = 50;
const float ATTACK_SPEED_HIT_HEIGHT = 50;

// �v���C���ƍU���G�t�F�N�g�̋���
static const float ATTACK_SPEED_RANGE = 4.0f;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackSpecialSpeed::CAttackSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_NORMAL;

	// ���̍U���̌ŗL�X�e�[�^�X������
	m_fWidth = ATTACK_SPEED_HIT_WIDTH;
	m_fHeight = ATTACK_SPEED_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_SPEED_END_TIME;
	m_nHitStartTime = ATTACK_SPEED_HIT_START_TIME;
	m_nHitEndTime = ATTACK_SPEED_HIT_END_TIME;

	m_vVelocity = D3DXVECTOR3(0, 0, 0);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackSpecialSpeed ::~CAttackSpecialSpeed(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAttackSpecialSpeed::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackSpecialSpeed::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackSpecialSpeed::Update(void)
{
	CAttackBase::Update();

	// �J�E���g��10�̂Ƃ��i���j�G�t�F�N�g�͔���
	if (m_nCount == 9){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, m_nEndTime);
	}
	//m_vPos += (m_vVelocity * ATTACK_SPEED_RANGE);
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAttackSpecialSpeed* CAttackSpecialSpeed::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// �쐬
	CAttackSpecialSpeed* p = new CAttackSpecialSpeed(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos; // -(velocity * ATTACK_SPEED_RANGE);
	//p->m_vVelocity = velocity;

	// ������
	p->Init();


	return p;
}

//=============================================================================
//	�v���C���[�ɓ����������̏���
//=============================================================================
void CAttackSpecialSpeed::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->HPGainAndLoss(-10.0f);
}

//----EOF-------