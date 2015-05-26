//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.h]
// Author : 佐藤　諒一
//
//=============================================================================
#ifndef __CPLAYERMANAGER_H__
#define __CPLAYERMANAGER_H__

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CPlayer;
class CAttackManager;
class CThreadManager;

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
// プレイヤーの最大数
static const short MAXIMUM_NUMBER_OF_PLAYER = 5;

//-----------------------------------------------------------------------------
// プレイヤーマネージャクラス定義
//-----------------------------------------------------------------------------
class CPlayerManager
{
public:
	// コンストラクタ
	CPlayerManager(
		CAttackManager *pAttackManager, 
		CThreadManager *pThreadManager);

	// デストラクタ
	~CPlayerManager();

	// クリエイト関数
	//	引数　　制作するプレイヤーの数、マニュアル操作するプレイヤーの数、CPUの数、攻撃マネージャー 糸マネージャー
	static CPlayerManager* Create(
		int nPlayerNum,
		int nManualPlayer,
		CAttackManager *pAttackPlayer,
		CThreadManager *pThreadPlayer);

	// 初期化
	// CPU人数、プレイヤー人数
	void Init(int nNumPlayer, int nManualPlayer);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// プレイヤーポインタのゲッター
	//	引数　　呼び出すプレイヤーをセットしている配列番号
	//	戻り値　プレイヤーのポインタ
	CPlayer* GetPlayer(int nNumber){ return m_apPlayer[nNumber]; }

	// プレイヤーポインタのセッター
	//	引数　　プレイヤーポインタ、セットする先の配列番号
	void SetPlayer(CPlayer* pPlayer, int nNumber){ m_apPlayer[nNumber] = pPlayer; }

private:
	// プレイヤーのポインタ格納用配列
	CPlayer* m_apPlayer[MAXIMUM_NUMBER_OF_PLAYER];
	// テスト用のタイマー
	int	m_nTimer;

	// 攻撃時に必要なマネージャー
	CAttackManager *m_pAttackManager;
	// 糸出す時に必要なマネージャー
	CThreadManager *m_pThreadManager;
};

#endif //__CPLAYERMANAGER_H__

// EOF