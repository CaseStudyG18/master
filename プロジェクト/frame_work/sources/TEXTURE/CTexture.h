//=============================================================================
//
// テクスチャー処理 [CTexture.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// 列挙体
//*****************************************************************************
// テクスチャの種類
typedef enum
{
	TEXTURE_NULL = 0,			// なし
	TEXTURE_BG_0,
	TEXTURE_BG_1,
	TEXTURE_FIRE_0,
	TEXTURE_FIRE_1,
	TEXTURE_THREAD,
	TEXTURE_PLAYER,
	TEXTURE_NUMBER,
	TEXTURE_TREASURE,
	TEXTURE_TREASURE_ICON,
	TEXTURE_BG_TITLE,
	TEXTURE_BG_RESULT,
	TEXTURE_WIN_LOGO,
	TEXTURE_DRAW_LOGO,
	TEXTURE_FIELD_01,	// 浮島
	TEXTURE_FIELD_02,	// 宝おいてるやつ
	TEXTURE_FIELD_03,	// スタート兼ゴール
	TEXTURE_SPAWN_YELLOW,
	TEXTURE_SPAWN_RED,
	TEXTURE_SPAWN_BLUE,
	TEXTURE_SPAWN_GREEN,
	TEXTURE_HIT_SMALL,
	TEXTURE_HIT_BIG,
	TEXTURE_ATTACK_AROUND,
	TEXTURE_ATTACK_TACKLE,
	TEXTURE_SLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_YELLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_RED,
	TEXTURE_PLAYER_ETAMORPHOSE_BLUE,
	TEXTURE_PLAYER_ETAMORPHOSE_GREEN,
	TEXTURE_TREASURE_KIRAKIRA,
	TEXTURE_TREASURE_LIGHT,
	
	//**以下加えた***
	TEXTURE_OPTION_MENU,	// オプションメニュー
	TEXTURE_OPTION_BG,		// オプションBG
	TEXTURE_CURSOL,			// カーソル
	TEXTURE_RETURN,			// 戻る
	TEXTURE_PLAYER_NUM,		// プレイヤー番号
	//***************
	TEXTURE_MAX					// テクスチャタイプ総数
}TEXTURE_TYPE;

//*****************************************************************************
// クラス宣言
//*****************************************************************************
class CTexture
{
public:
	// コンストラクタ
	CTexture(void){};

	// デストラクタ
	~CTexture(void){};

	// 終了
	static void Uninit(void);

	//============================================================
	// クリエイト
	// 引数:デバイス
	//============================================================
	static void CreateTexture(LPDIRECT3DDEVICE9 *pDevice);

	//============================================================
	// テクスチャゲッター
	// 引数:テクスチャの種類
	// 戻り値:テクスチャのポインタ
	//============================================================
	static LPDIRECT3DTEXTURE9 GetTexture(const TEXTURE_TYPE type);

	//============================================================
	// テクスチャタイプゲッター
	// 引数:テクスチャのファイルパス
	// 戻り値:テクスチャタイプ
	//============================================================
	static TEXTURE_TYPE GetTextureTye(char* texFilePath);

private:
	//============================================================
	// 初期化
	// 引数:デバイス
	// 戻り値:成否
	//============================================================
	static	HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// テクスチャポインタ
	static	bool m_bDoneCreateFlag;									// すでに作ったかのフラグ
};

#endif
//----EOF----