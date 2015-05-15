//=============================================================================
//
// CPlayerManagerクラス [CPlayerManager.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
#include "CPlayerManager.h"

#include "CPlayer.h"
#include "../RENDERER/CRenderer.h"
#include "../TEXTURE/CTexture.h"

#ifdef _DEBUG
#define PLAYER_TEXTURE TEXTURE_PLAYER	// デバッグ時のプレイヤーのテクスチャ
#else
#define PLAYER_TEXTURE TEXTURE_NULL		// 
#endif

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::CPlayerManager()
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CPlayerManager::~CPlayerManager()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void CPlayerManager::Init(int nNumPlayer)
{
	for (int i = 0; i < nNumPlayer; i++)
	{
		m_apPlayer[i] = CPlayer::Create(CRenderer::GetDevice(), D3DXVECTOR3(50.0f, 20.0f, 0), 50.0f, 50.0f, PLAYER_TEXTURE);
	}
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void CPlayerManager::Update(void)
{
}

//-----------------------------------------------------------------------------
// 終了
//-----------------------------------------------------------------------------
void CPlayerManager::Uninit(void)
{
	for (int i = 0; i < MAXIMUM_NUMBER_OF_PLAYER; i++)
	{
		m_apPlayer[i] = NULL;
	}
}

// EOF