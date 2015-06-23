//=============================================================================
//
// アニメーションクラス [CAnimation.cpp]
// Author : 塚本　俊彦
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CAnimation.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CAnimation::CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority)
{
	m_bDraw = true;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CAnimation ::~CAnimation(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CAnimation :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY)
{
	CScene2D ::Init(pos, width, height, texType);

	// 値のチェック
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;

	// テクスチャ座用セット
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CAnimation ::Uninit(void)
{
	CScene2D :: Uninit();

}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CAnimation ::Update(void)
{

}


//*****************************************************************************
// 描画関数
//*****************************************************************************
void CAnimation ::Draw(void)
{
	if(m_bDraw)
	{
		CScene2D ::Draw();
	}
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY)
{
	// 作成
	CAnimation* p = new CAnimation(pDevice, nPriority);

	p->m_pD3DDevice = pDevice;

	// 初期化
	p->Init(pos, width, height, texType, sepalateX, sepalateY);

	return p;
}

//=============================================================================
// インデックスの設定
//=============================================================================
void CAnimation::SetIndex(int nIdx)
{
	float fU, fV;

	// 値のチェック
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
	m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
	m_fTop = fV * (m_nIdx / m_nSepalateNumX);
	m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//=============================================================================
// インデックスの設定
//=============================================================================
void CAnimation::SetIndex(int nIdx, bool reverse)
{
	float fU, fV;

	// 値のチェック
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	if (reverse){
		m_fRight = fU * (m_nIdx % m_nSepalateNumX);
		m_fLeft = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}
	else{
		m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
		m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//----EOF----