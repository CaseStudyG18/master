//=============================================================================
//
// CTreasureManagerクラス [CTreasureManager.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTreasure.h"


//*****************************************************************************
// マクロ
//*****************************************************************************
// テクスチャ関連
static const TEXTURE_TYPE TREASURE_TEXTURE = TEXTURE_TREASURE;
static const int TREASURE_TEXTURE_X = 13;
static const int TREASURE_TEXTURE_Y = 1;
// お宝２Ｄ
static const float TREASURE_WIDTH = 70;
static const float TREASURE_HEIGHT = 70;
static const int TREASURE_ANIME_SPEED = 5;
// お宝アイコン２Ｄ
static const float TREASURE_ICON_WIDTH = 40;
static const float TREASURE_ICON_HEIGHT = 40;
static const int TREASURE_ICON_ANIME_SPEED = 3;

// お宝の所持番号（誰も持っていないときの-1）
static const short OWNER_NONE = -1;

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTreasure::CTreasure(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CSceneAnime(pDevice, nPriority, m_objType)
{
	// お宝の状態初期化
	m_TreasureState = TREASURE_STATE_OWNER_NONE;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTreasure ::~CTreasure(void)
{
}

//*****************************************************************************
// 初期化
//*****************************************************************************
void CTreasure::Init(D3DXVECTOR3 pos)
{
	// 初期化
	CSceneAnime::Init(pos, TREASURE_WIDTH, TREASURE_HEIGHT, TREASURE_TEXTURE,
		TREASURE_TEXTURE_X, TREASURE_TEXTURE_Y, TREASURE_ANIME_SPEED, -1);

	SetAutoUpdate(true);
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CTreasure::Uninit(void)
{
	CScene2D::Uninit();
}

//*****************************************************************************
// 更新
//*****************************************************************************
void CTreasure::Update(void)
{
	CSceneAnime::Update();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CTreasure* CTreasure::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos)
{
	// 作成
	CTreasure* p = new CTreasure(pDevice);

	p->Init(pos);

	return p;
}

//*****************************************************************************
// 大きさやテクスチャを最初の状態に戻す
// 宝物をプレイヤが落とした際に呼ばれる
//*****************************************************************************
void CTreasure::Reset(D3DXVECTOR3 pos){

	// 絵の変更
//	CScene2D::ChangeTexture(TREASURE_TEXTURE);
	CScene2D::SetWidth(TREASURE_WIDTH);
	CScene2D::SetHeight(TREASURE_HEIGHT);
	SetAnimeSpeed(TREASURE_ANIME_SPEED);

	// 落とす処理仮
	CScene2D::SetPos(pos + D3DXVECTOR3(100, 100, 0));

	m_TreasureState = TREASURE_STATE_OWNER_NONE;
}

//*****************************************************************************
// 大きさやテクスチャを最初の状態に戻す
// 宝物をプレイヤが落とした際に呼ばれる
//*****************************************************************************
void CTreasure::SetIcon(){

	// 絵の変更
//	CScene2D::ChangeTexture(TEXTURE_TREASURE_ICON);
	CScene2D::SetWidth(TREASURE_ICON_WIDTH);
	CScene2D::SetHeight(TREASURE_ICON_HEIGHT);
	SetAnimeSpeed(TREASURE_ICON_ANIME_SPEED);
}

//----EOF-------