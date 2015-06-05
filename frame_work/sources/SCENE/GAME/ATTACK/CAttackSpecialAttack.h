//=============================================================================
//
// CAttackSpecialAttackクラス [CAttackSpecialAttack.h]
// Author : 佐藤　諒一
//
// 普通攻撃
//
//=============================================================================
#ifndef _CATTACKSPECIALATTACK_H_
#define _CATTACKSPECIALATTACK_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CAttackBase.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAttackSpecialAttack : public CAttackBase
{
	// 公開メンバ
public:

	CAttackSpecialAttack(LPDIRECT3DDEVICE9 *pDevice);
	~CAttackSpecialAttack(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CAttackSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 velocity);

	// 非公開メンバ
private:

};

#endif
//----EOF----