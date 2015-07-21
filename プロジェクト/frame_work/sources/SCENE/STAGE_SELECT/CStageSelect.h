//=============================================================================
//
// CStageSelectクラス [CStageSelect.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSTAGE_SELECT_H_
#define _CSTAGE_SELECT_H_
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"

//*****************************************************************************
// 定数
//*****************************************************************************
static const int STAGE_MAX = 8;

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStageSelect : public CPhase
{
public:
	// コンストラクタ
	CStageSelect(void);

	// デストラクタ
	~CStageSelect(void);

	//=================================================================
	// 初期化関数
	// 引数: シーンのモード, デバイス
	//=================================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//=================================================================
	// クリエイト関数
	// 引数: シーンのモード, デバイス
	// 戻り値: このシーンのアドレス
	//=================================================================
	static CStageSelect* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

private:

	// 選択をする更新
	void UpdateSelect();

	// 背景
	CScene2D* m_pBG;
	// ロゴ
	CScene2D* m_pLogo;
	// 表示するステージ
	CScene2D* m_pStage2D[STAGE_MAX];
	// 選択中の枠
	CScene2D* m_pFrame2D;
	// 現在選択しているステージ番号(0~7)
	int m_nSelectNum;
	// 選択枠の色
	D3DXCOLOR m_FrameColor;
	// 選択枠のα加算フラグ
	bool m_bFrameAlphaAdd;

};

#endif
//----EOF----