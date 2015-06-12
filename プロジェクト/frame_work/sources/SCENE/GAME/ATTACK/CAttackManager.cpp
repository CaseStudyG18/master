//=============================================================================
//
// CAttackManagerNX [CAttackManager.cpp]
// Author : ห{@rF
//
//=============================================================================
//*****************************************************************************
// CN[h
//*****************************************************************************
#include <Windows.h>
#include "CAttackManager.h"
#include "CAttackNormal.h"
#include "CAttackSpecialAttack.h"
#include "CAttackSpecialSpeed.h"
#include "CAttackSpecialTrap.h"

//*****************************************************************************
// }N
//*****************************************************************************


//*****************************************************************************
// รIoฯ
//*****************************************************************************


//*****************************************************************************
// RXgN^
//*****************************************************************************
CAttackManager::CAttackManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// fXgN^
//*****************************************************************************
CAttackManager ::~CAttackManager(void)
{
}

//*****************************************************************************
// ๚ป
//*****************************************************************************
void CAttackManager::Init()
{
}

//*****************************************************************************
// Iน
//*****************************************************************************
void CAttackManager::Uninit(void)
{
}

//*****************************************************************************
// XV
//*****************************************************************************
void CAttackManager::Update(void)
{
}

//*****************************************************************************
// U๐ถฌท้
//*****************************************************************************
void CAttackManager::CreateAttack(
	AttackType type,
	short sPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// สํ๓ิฬU
	case ATTACK_TYPE_NORMAL:
		CAttackNormal::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// U`ิฬU
	case ATTACK_TYPE_ATTACK:
		CAttackSpecialAttack::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// ฺฎ`ิฬU
	case ATTACK_TYPE_SPEED:
		CAttackSpecialSpeed::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// ใฉ`ิฬU
	case ATTACK_TYPE_TRAP:
		CAttackSpecialTrap::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
	default:
		break;
	}
	/*
	// สฬU
	if (type == ATTACK_TYPE_NORMAL){
		CAttackNormal::Create(m_pDevice, sPlayerNum, pos, velocity);
	}
	else if(type == ATTACK_TYPE_ATTACK){
		CAttackSpecialAttack::Create(m_pDevice, sPlayerNum, pos, velocity);
	}
	else if (type == ATTACK_TYPE_SPEED){
		CAttackSpecialSpeed::Create(m_pDevice, sPlayerNum, pos, velocity);
	}
	else if (type == ATTACK_TYPE_TRAP){
		CAttackSpecialTrap::Create(m_pDevice, sPlayerNum, pos, velocity);
	}
	*/
}
//----EOF-------