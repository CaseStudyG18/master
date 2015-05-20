//=============================================================================
//
// CThreadNormalクラス [CThreadNoaml.h]
// Author : 塚本　俊彦
//
// 普通糸
//
//=============================================================================
#ifndef _CTHREADNORMAL_H_
#define _CTHREADNORMAL_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../../RENDERER/CRenderer.h"
#include "CThreadBase.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CThreadNormal : public CThreadBase
{
	// 公開メンバ
public:

	CThreadNormal(LPDIRECT3DDEVICE9 *pDevice);
	~CThreadNormal(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//=======================================================================
	// クリエイト関数
	// 引数:デバイス、プレイヤ番号、座標
	// 戻り値:作成したアドレス
	//=======================================================================
	static CThreadNormal *Create(
		LPDIRECT3DDEVICE9 *pDevice,
		short nPlayerNum,
		D3DXVECTOR3 pos);

	// 非公開メンバ
private:

};

#endif
//----EOF----