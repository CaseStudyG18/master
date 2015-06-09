//=============================================================================
//
// CThreadSpecialAttackクラス [CThreadSpecialAttack.h]
// Author : 佐藤　諒一
//
// 攻撃特化形態の糸
//
//=============================================================================
#ifndef _CTHREAD_SPECIAL_SPEED_H_
#define _CTHREAD_SPECIAL_SPEED_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadSpecialSpeed : public CThreadBase
{
	// 公開メンバ
public:
	// プライオリティとオブジェタイプはテスト
	CThreadSpecialSpeed(LPDIRECT3DDEVICE9 *pDevice, int priority = TYPE_PRIORITY_THREAD_OF_FOOTHOLD, OBJTYPE type = OBJTYPE_FIELD);
	~CThreadSpecialSpeed(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CThreadSpecialSpeed *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif
//----EOF----