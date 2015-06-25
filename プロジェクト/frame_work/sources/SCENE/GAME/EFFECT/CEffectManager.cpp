//=============================================================================
//
// CEffectManagerクラス [CEffectManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "CEffectManager.h"
#include "CEffectAttackHit.h"
#include "CEffectExplosion.h"
#include "CEffectSpecialAttack.h"
#include "CEffectSpecialSpeedAttack.h"
#include "CEffectSpecialAttackThread.h"

//*****************************************************************************
// マクロ
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEffectManager::CEffectManager(LPDIRECT3DDEVICE9 *pDevice)
{
	m_pDevice = pDevice;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEffectManager ::~CEffectManager(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CEffectManager::Init()
{
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CEffectManager::Uninit(void)
{
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CEffectManager::Update(void)
{
}

//*****************************************************************************
// エフェクト生成
//*****************************************************************************
void CEffectManager::CreateEffect(D3DXVECTOR3 pos, EFFECT_TYPE type, D3DXVECTOR3 velocity)
{
	switch (type)
	{
		// 攻撃がヒットした時のエフェクト
	case EFFECT_ATTACK_HIT:
		CEffectAttackHit::Create(m_pDevice, pos);
		break;
		// 爆発エフェクト
	case EFFECT_EXPLOSION:
		CEffectExplosion::Create(m_pDevice, pos);
		break;
		// 攻撃形態時の攻撃
	case EFFECT_SPECIAL_ATTACK_ATTACK:
		CEffectSpecialAttack::Create(m_pDevice, pos, velocity);
		break;
		// 移動形態時の攻撃
	case EFFECT_SPECIAL_ATTACK_SPEED:
		CEffectSpecialSpeedAttack::Create(m_pDevice, pos, velocity);
		break;
		// 攻撃形態時の糸攻撃
	case EFFECT_SPECIAL_THREAD_ATTACK:
		CEffectSpecialAttackThread::Create(m_pDevice, pos, velocity);
		break;
	default:
		break;
	}
}
//----EOF-------