//=============================================================================
//
// CThreadSpecialAttackクラス [CThreadSpecialAttack.h]
// Author : 佐藤　諒一
//
// 攻撃特化形態の糸
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_ATTACK_H_
#define _CTHREAD_SPECIAL_ATTACK_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadSpecialAttack : public CThreadBase
{
	// 公開メンバ
public:
	// プライオリティとオブジェタイプはテスト
	CThreadSpecialAttack(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_FIELD);
	~CThreadSpecialAttack(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CThreadSpecialAttack *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif
//----EOF----