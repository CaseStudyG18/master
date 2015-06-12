//=============================================================================
//
// CAttackManagerクラス [CAttackManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CAttackManager.h"
#include "CAttackNormal.h"
#include "CAttackSpecialAttack.h"
#include "CAttackSpecialSpeed.h"
#include "CAttackSpecialTrap.h"

//*****************************************************************************
// マクロ
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAttackManager::CAttackManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAttackManager ::~CAttackManager(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CAttackManager::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CAttackManager::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CAttackManager::Update(void)
{
}

//*****************************************************************************
// 攻撃を生成する
//*****************************************************************************
void CAttackManager::CreateAttack(
	AttackType type,
	short sPlayerNum,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// 通常状態の攻撃
	case ATTACK_TYPE_NORMAL:
		CAttackNormal::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// 攻撃形態の攻撃
	case ATTACK_TYPE_ATTACK:
		CAttackSpecialAttack::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// 移動形態の攻撃
	case ATTACK_TYPE_SPEED:
		CAttackSpecialSpeed::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
		// 罠形態の攻撃
	case ATTACK_TYPE_TRAP:
		CAttackSpecialTrap::Create(m_pDevice, sPlayerNum, pos, velocity);
		break;
	default:
		break;
	}
	/*
	// 普通の攻撃
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