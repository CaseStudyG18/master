//=============================================================================
//
// CAttackNormalクラス [CAttackNoaml.h]
// Author : 塚本　俊彦
//
// 普通攻撃
//
//=============================================================================
#ifndef _CATTACKNORMAL_H_
#define _CATTACKNORMAL_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackNormal : public CAttackBase
{
	// 公開メンバ
public:

	CAttackNormal(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackNormal(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CAttackNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:

};

#endif
//----EOF----