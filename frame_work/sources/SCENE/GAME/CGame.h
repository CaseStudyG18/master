//=============================================================================
//
// CGameクラス [CGame.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CGAME_H_
#define _CGAME_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "../../PHASE/CPhase.h"
#include "../../TEXTURE/CTexture.h"
#include "../PAUSE/CPause.h"
#include "CTimeManager.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CTimeManager;
class CTreasureManager;
class CGoalManager;
class CPlayerManager;
class CAttackManager;
class CJudgeManager;
class CFieldManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CPhase
{
	// 公開メンバ
public:
	// コンストラクタ
	CGame(void);

	// デストラクタ
	~CGame(void);

	//======================================================
	// 初期化
	// 引数: フェーズモード、デバイス
	//======================================================
	void Init(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	//======================================================
	// クリエイト
	// 引数: フェーズモード、デバイス
	// 戻り値: 作ったやつのポインタ
	//======================================================
	static CGame* Create(MODE_PHASE mode, LPDIRECT3DDEVICE9* pDevice);

	// 非公開メンバ
private:
	// ポーズ
	CPause *m_pPause;
	// ゲームのタイム管理者
	CTimeManager *m_pTimeManager;
	// 宝物管理者
	CTreasureManager *m_pTreasureManager;
	// ゴール管理者
	CGoalManager *m_pGoalManager;
	// プレイヤマネージャ
	CPlayerManager *m_pPlayerManager;
	// 攻撃マネージャ
	CAttackManager * m_pAttackManager;
	// ジャッジマネージャー
	CJudgeManager* m_pJudgeManager;
	// フィールドマネージャー
	CFieldManager* m_pFieldManager;
};

#endif
//----EOF----