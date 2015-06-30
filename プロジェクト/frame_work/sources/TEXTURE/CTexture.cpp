//=============================================================================
//
// テクスチャ処理 [CTexture.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CTexture.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
// テクスチャのパス
static const char *TEXTURE_PATH[TEXTURE_MAX] =
{
	NULL,
	"data/TEXTURE/bg_0.png",
	"data/TEXTURE/bg_1.png",
	"data/TEXTURE/fire0.png",
	"data/TEXTURE/fire1.png",
	"data/TEXTURE/thread.png",
	"data/TEXTURE/Player.png",
	"data/TEXTURE/figure.png",
	"data/TEXTURE/treasure.png",
	"data/TEXTURE/treasure_icon.png",
	"data/TEXTURE/bg_title.png",
	"data/TEXTURE/bg_result.png",
	"data/TEXTURE/WinLogo.png",
	"data/TEXTURE/DrawLogo.png",
	"data/TEXTURE/02.png",
	"data/TEXTURE/03.png",
	"data/TEXTURE/Spawn.png",
	"data/TEXTURE/SpawnYellow.png",
	"data/TEXTURE/SpawnRed.png",
	"data/TEXTURE/SpawnBlue.png",
	"data/TEXTURE/SpawnGreen.png",
	"data/TEXTURE/hit_effect_small.png",
	"data/TEXTURE/hit_effect_big.png",
	"data/TEXTURE/attack_around.png",
	"data/TEXTURE/attack_tackle.png",
	"data/TEXTURE/slow_effect.png",
	"data/TEXTURE/trans_effect_yellow.png",
	"data/TEXTURE/trans_effect_red.png",
	"data/TEXTURE/trans_effect_blue.png",
	"data/TEXTURE/trans_effect_green.png",
	"data/TEXTURE/kirakira.png",
	"data/TEXTURE/treasure_light.png",
};

//*****************************************************************************
// スタティックメンバ変数宣言
//*****************************************************************************
bool CTexture::m_bDoneCreateFlag = false;
LPDIRECT3DTEXTURE9 CTexture::m_pD3DTex[TEXTURE_MAX] = { NULL };

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTexture::Init(LPDIRECT3DDEVICE9 *pDevice)
{
	// ゲームで使う全テクスチャを作成
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		if (TEXTURE_PATH[TexCnt] != NULL)
		{
			// テクスチャ読み込み
			if (FAILED(D3DXCreateTextureFromFile((*pDevice), TEXTURE_PATH[TexCnt], &m_pD3DTex[TexCnt])))
			{
#ifdef _DEBUG
				MessageBox(NULL, "テクスチャの読み込みに失敗しました\n", "エラー", MB_OK | MB_ICONERROR);
#endif
				m_pD3DTex[TexCnt] = NULL;
				return E_FAIL;
			}
		}
		else
		{
			m_pD3DTex[TexCnt] = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTexture::Uninit(void)
{
	// 使った全テクスチャポインタを開放
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		// テクスチャポインタが使われている場合
		if (m_pD3DTex[TexCnt])
		{
			m_pD3DTex[TexCnt]->Release();
			m_pD3DTex[TexCnt] = NULL;
		}
	}

}

//=============================================================================
// テクスチャ作成処理
//=============================================================================
void CTexture::CreateTexture(LPDIRECT3DDEVICE9 *pDevice)
{
	CTexture::Init(pDevice);
}

//=============================================================================
// テクスチャポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(const TEXTURE_TYPE type)
{
	// 正しいテクスチャタイプか判定
	if (type < 0 || type > TEXTURE_MAX)
	{
#ifdef _DEBUG
		MessageBox(NULL, "テクスチャタイプの番号が不適切です", "エラー", MB_OK | MB_ICONERROR);
#endif
		return NULL;
	}
	return m_pD3DTex[type];
}

//============================================================
// テクスチャタイプゲッター
//============================================================
TEXTURE_TYPE CTexture::GetTextureTye(char* texFilePath)
{
	for (int TexCnt = 0; TexCnt < TEXTURE_MAX; TexCnt++)
	{
		if (!TEXTURE_PATH[TexCnt])
		{
			continue;
		}
		if (strcmp(texFilePath, TEXTURE_PATH[TexCnt]) == 0)
		{
			return (TEXTURE_TYPE)TexCnt;
		}
	}

#ifdef _DEBUG
	MessageBox(NULL, "テクスチャパスが不明です\n", "エラー", MB_OK | MB_ICONERROR);
#endif
	return TEXTURE_NULL;

}

//----EOF----