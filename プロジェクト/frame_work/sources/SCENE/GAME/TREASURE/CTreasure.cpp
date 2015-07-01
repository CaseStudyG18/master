//=============================================================================
//
// CTreasureクラス [CTreasure.cpp]
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

// きらきら絵
static const D3DXVECTOR3 TREASURE_KIRA_SIZE = D3DXVECTOR3(80, 80, 0);
static const int TREASURE_KIRA_TEX_X = 8;
static const int TREASURE_KIRA_TEX_Y = 1;
static const int TREASURE_KIRA_ANIME_SPEED = 2;
// 神々しいエフェクト
static const D3DXVECTOR3 TREASURE_LIGHT_SIZE = D3DXVECTOR3(100, 100, 0);
static const int TREASURE_LIGHT_TEX_X = 10;
static const int TREASURE_LIGHT_TEX_Y = 1;
static const int TREASURE_LIGHT_ANIME_SPEED = 4;

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

	// エフェクトの作成
	p->m_pKira = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_KIRA_SIZE.x, TREASURE_KIRA_SIZE.y,
		TEXTURE_TREASURE_KIRAKIRA, TREASURE_KIRA_TEX_X, TREASURE_KIRA_TEX_Y, TREASURE_KIRA_ANIME_SPEED, -1);
	p->m_pLight = CSceneAnime::Create(
		pDevice,
		pos, TREASURE_LIGHT_SIZE.x, TREASURE_LIGHT_SIZE.y,
		TEXTURE_TREASURE_LIGHT, TREASURE_LIGHT_TEX_X, TREASURE_LIGHT_TEX_Y, TREASURE_LIGHT_ANIME_SPEED, -1);

	p->m_pLight->SetDrawFlag(false);
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
	m_TreasureState = TREASURE_STATE_OWNER_NONE;
	
	// 落とす処理仮
	CScene2D::SetPos(pos + D3DXVECTOR3(100, 100, 0));
	m_pKira->SetPos(pos + D3DXVECTOR3(100, 100, 0));
	m_pLight->SetPos(pos + D3DXVECTOR3(100, 100, 0));
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

	// エフェクトの変更
	m_pLight->SetHeight(150);
}


//*****************************************************************************
// 宝物の場所をセット エフェクトとかも一緒にセットする
//*****************************************************************************
void CTreasure::SetPos(D3DXVECTOR3 pos){

	// 座標セット
	m_vPos = pos;
	m_pKira->SetPos(pos);
	m_pLight->SetPos(pos + D3DXVECTOR3(0, 40, 0));
}
//----EOF-------