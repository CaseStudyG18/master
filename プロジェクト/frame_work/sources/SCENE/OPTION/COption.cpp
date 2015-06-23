//=============================================================================
//
// COptionクラス [COption.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "COption.h"
#include "../../MANAGER/CManager.h"
#include "../CSCENE/CScene2D.h"
#include "../../CONTROLLER/CControllerManager.h"

//*****************************************************************************
// マクロ
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
COption::COption(void)
{
	m_mode = OPTION_MODE_KEYCONFIG;
	m_setButton = CInputGamePad::PAD_KEY_A;
	m_nControllerID = 0;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
COption ::~COption(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void COption::Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	// フェーズの初期化
	CPhase::Init(pDevice, mode);

	// フェード作成
	m_pFade = new CFade(pDevice);
	m_pFade->Init(DEFFAULT_FADE_POS, DEFFAULT_FADE_WIDTH, DEFFAULT_FADE_HEIGHT, TEXTURE_NULL);

	// フェードイン開始
	m_pFade->Start(MODE_FADE_IN, DEFFAULT_FADE_IN_COLOR, DEFFAULT_FADE_TIME);

	// モードの初期化
	m_mode = OPTION_MODE_KEYCONFIG;

	// ＢＧＭ再生
	//CManager::PlaySoundA(SOUND_LABEL_BGM000);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void COption::Uninit(void)
{
	// 音停止
	CManager::StopSound();

	// フェイズの終了
	CPhase::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void COption::Update(void)
{
	// フェイズの更新
	CPhase::Update();

	// キーコフィングなら
	if (m_mode == OPTION_MODE_KEYCONFIG)
	{
		KeyConfig();
	}

	// 次のシーンに行くなら
	if (m_mode == OPTION_MODE_NEXT_SCENE)
	{
#ifdef _DEBUG
		CDebugProc::Print("PUSH ENTER\n");
#endif
		if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			// キーコンフィグ情報のセーブ
			CControllerManager::SaveKeyCofingFile();

			// キーコンフィグ情報の更新
			CControllerManager::UpdateKeyCord();

			// フェードアウト開始
			m_pFade->Start(MODE_FADE_OUT, DEFFAULT_FADE_OUT_COLOR, DEFFAULT_FADE_TIME);

			// ゲームヘ
			m_pManager->SetNextPhase(MODE_PHASE_GAME);
		}
	}
}

//*****************************************************************************
// クリエイト
//*****************************************************************************
COption* COption::Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice)
{
	COption* pTitle = NULL;

	// 作成
	pTitle = new COption;

	// 初期化
	pTitle->Init(mode, pDevice);

	return pTitle;
}

//*****************************************************************************
// キーコンフィグ
//*****************************************************************************
void COption::KeyConfig(void)
{
	int pCord = -1;
	if (CInputGamePad::CheckConectPad(m_nControllerID))
	{
#ifdef _DEBUG
		CDebugProc::Print("PLAYER_ID:%d\n", m_nControllerID);
#endif
		switch (m_setButton)
		{
		case CInputGamePad::PAD_KEY_A:
#ifdef _DEBUG
			CDebugProc::Print("Aボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_B;
			}
		break;

		case CInputGamePad::PAD_KEY_B:
#ifdef _DEBUG
			CDebugProc::Print("Bボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_Y;
			}
		break;

		case CInputGamePad::PAD_KEY_Y:
#ifdef _DEBUG
			CDebugProc::Print("Yボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_X;
			}
		break;

		case CInputGamePad::PAD_KEY_X:
#ifdef _DEBUG
			CDebugProc::Print("Xボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_L;
			}
		break;

		case CInputGamePad::PAD_KEY_L:
#ifdef _DEBUG
			CDebugProc::Print("Lボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_R;
			}
		break;

		case CInputGamePad::PAD_KEY_R:
#ifdef _DEBUG
			CDebugProc::Print("Rボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_START;
			}
		break;

		case CInputGamePad::PAD_KEY_START:
#ifdef _DEBUG
			CDebugProc::Print("STARTボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_SELECT;
			}
		break;

		case CInputGamePad::PAD_KEY_SELECT:
#ifdef _DEBUG
			CDebugProc::Print("SELECTボタンにしたいのを押してください\n");
#endif
			if (CInputGamePad::CheckTriggerAnyKey(m_nControllerID, &pCord))
			{
				// キーコードセット
				CControllerManager::SetKeyCord(m_nControllerID, m_setButton, pCord);

				m_setButton = CInputGamePad::PAD_KEY_MAX;
			}
		break;

		case CInputGamePad::PAD_KEY_MAX:
			m_nControllerID++;

			// 繋がってるコントローラー全部セットしたら
			if (m_nControllerID >= CControllerManager::MAX_CONTROLLER_NUM)
			{
				m_mode = OPTION_MODE_NEXT_SCENE;
			}
		break;
		}
	}
	else
	{
		m_nControllerID++;

		// 繋がってるコントローラー全部セットしたら
		if (m_nControllerID >= CControllerManager::MAX_CONTROLLER_NUM)
		{
			m_mode = OPTION_MODE_NEXT_SCENE;
		}
	}
}

//----EOF----