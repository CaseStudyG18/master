//=============================================================================
//
// CAttacjManager�N���X [CAttacManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAttackNormal.h"
#include "../../../SCENE/CSCENE/CSceneAnime.h"
#include "../PLAYER/CPlayer.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************
// ����
const short ATTACK_NORMAL_END_TIME = 180;
// �����蔻��̎n�܂鎞��
const short ATTACK_NORMAL_HIT_START_TIME = 60;
// �����蔻��̏I��鎞��
const short ATTACK_NORMAL_HIT_END_TIME = 120;

// �����蔻 �蕝,����
const float ATTACK_NORMAL_HIT_WIDTH = 50;
const float ATTACK_NORMAL_HIT_HEIGHT = 50;

// �v���C���ƍU���G�t�F�N�g�̋���
static const float ATTACK_NORMAL_RANGE = 100;

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAttackNormal::CAttackNormal(LPDIRECT3DDEVICE9 *pDevice) : CAttackBase(pDevice)
{
	// �ϐ�������
	m_AttackType = ATTACK_TYPE_NORMAL;

	// ���̍U���̌ŗL�X�e�[�^�X������
	m_fWidth = ATTACK_NORMAL_HIT_WIDTH;
	m_fHeight = ATTACK_NORMAL_HIT_HEIGHT;
	m_vRot = D3DXVECTOR3(0, 0, 0);
	m_nEndTime = ATTACK_NORMAL_END_TIME;
	m_nHitStartTime = ATTACK_NORMAL_HIT_START_TIME;
	m_nHitEndTime = ATTACK_NORMAL_HIT_END_TIME;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAttackNormal ::~CAttackNormal(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
HRESULT CAttackNormal::Init()
{
	CAttackBase::Init();
	return S_OK;
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CAttackNormal::Uninit(void)
{
	CAttackBase::Uninit();
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CAttackNormal::Update(void)
{
	CAttackBase::Update();

	// �J�E���g��10�̂Ƃ��i���j�G�t�F�N�g�͔���
	if (m_nCount == 10){
		CSceneAnime::Create(
			m_pD3DDevice,
			m_vPos, 100, 100,
			TEXTURE_FIRE_1, 10, 1, 20);
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAttackNormal* CAttackNormal::Create(
	LPDIRECT3DDEVICE9 *pDevice,
	short nPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	// �쐬
	CAttackNormal* p = new CAttackNormal(pDevice);

	p->m_nPlayerNum = nPlayerNum;
	p->m_vPos = pos + (velocity * ATTACK_NORMAL_RANGE);

	// ������
	p->Init();


	return p;
}

//*****************************************************************************
//	�U�����q�b�g�����ۂɌĂяo�����֐�
//	 �����@�@�q�b�g�����v���C���[�̃v���C���[�ԍ�
//*****************************************************************************
void CAttackNormal::HitPlayer(CPlayer* pPlayer)
{
	pPlayer->SetDonashi(true);
}

//----EOF-------