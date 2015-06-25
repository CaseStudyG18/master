//=============================================================================
//
// CEffectManager�N���X [CEffectManager.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "CEffectManager.h"
#include "CEffectAttackHit.h"
#include "CEffectExplosion.h"
#include "CEffectSpecialAttack.h"
#include "CEffectSpecialSpeedAttack.h"
#include "CEffectSpecialAttackThread.h"

//*****************************************************************************
// �}�N��
//*****************************************************************************


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEffectManager::CEffectManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEffectManager ::~CEffectManager(void)
{
}

//*****************************************************************************
// ������
//*****************************************************************************
void CEffectManager::Init()
{
}

//*****************************************************************************
// �I��
//*****************************************************************************
void CEffectManager::Uninit(void)
{
}

//*****************************************************************************
// �X�V
//*****************************************************************************
void CEffectManager::Update(void)
{
}

//*****************************************************************************
// �G�t�F�N�g����
//*****************************************************************************
void CEffectManager::CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// �U�����q�b�g�������̃G�t�F�N�g
	case EFFECT_ATTACK_HIT:
		CEffectAttackHit::Create(m_pDevice, pos);
		break;
		// �����G�t�F�N�g
	case EFFECT_EXPLOSION:
		CEffectExplosion::Create(m_pDevice, pos);
		break;
		// �U���`�Ԏ��̍U��
	case EFFECT_SPECIAL_ATTACK_ATTACK:
		CEffectSpecialAttack::Create(m_pDevice, pos, velocity);
		break;
		// �ړ��`�Ԏ��̍U��
	case EFFECT_SPECIAL_ATTACK_SPEED:
		CEffectSpecialSpeedAttack::Create(m_pDevice, pos, velocity);
		break;
		// �U���`�Ԏ��̎��U��
	case EFFECT_SPECIAL_THREAD_ATTACK:
		CEffectSpecialAttackThread::Create(m_pDevice, pos, velocity);
		break;
	default:
		break;
	}
}
//----EOF-------