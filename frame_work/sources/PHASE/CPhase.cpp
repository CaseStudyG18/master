//=============================================================================
//
// CPhaseクラス [CPhase.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CPhase.h"
#include "../SCENE/TITLE/CTitle.h"
#include "../SCENE/GAME/CGame.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/RESULT/CResult.h"
#include "../SCENE/OPTION/COption.h"
#include "../MANAGER/CManager.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager*		CPhase::m_pManager = NULL;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPhase ::CPhase(void)
{
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPhase ::~CPhase(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CPhase ::Init(LPDIRECT3DDEVICE9* pD3DDevice, MODE_PHASE mode)
{
	m_pD3DDevice = pD3DDevice;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CPhase ::Uninit(void)
{

}

//*****************************************************************************
// 更新
//*****************************************************************************
void CPhase ::Update(void)
{
}


//*****************************************************************************
// クリエイト
//*****************************************************************************
CPhase* CPhase::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice, CManager* pManager)
{
	m_pManager = pManager;
	switch(mode)
	{
		// タイトル
		case MODE_PHASE_TITLE:
		{
			CTitle* pTitle;
			pTitle = CTitle::Create(mode, pDevice);
			return pTitle;

			break;
		}

		// ゲーム
		case MODE_PHASE_GAME:
		{
			CGame* pGame;
			pGame = CGame::Create(mode, pDevice);
			return pGame;

			break;
		}

		// リザルト
		case MODE_PHASE_RESULT:
		{
			CResult* pResult;
			pResult = CResult::Create(mode, pDevice);
			return pResult;

			break;
		}

		// オプション
		case MODE_PHASE_OPTION:
		{
			COption* pOption;
			pOption = COption::Create(mode, pDevice);
			return pOption;

			break;
		}

	}
	return NULL;
}
//----EOF----