//=============================================================================
//
// CResultクラス [CResult.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CRESULT_H_
#define _CRESULT_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

class CSceneAnime;

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CPhase
{
public:
	// コンストラクタ
	CResult(void);

	// デストラクタ
	~CResult(void);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//======================================================
	// クリエイト
	// 引数: フェーズモード、デバイス
	// 戻り値: 作ったやつのポインタ
	//======================================================
	static CResult* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 非公開メンバ
private:
	//======================================================
	// 初期化
	// 引数: フェーズモード、デバイス
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	CSceneAnime *m_Fire;
};

#endif
//----EOF----