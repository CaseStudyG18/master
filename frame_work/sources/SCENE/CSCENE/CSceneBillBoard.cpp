//=============================================================================
//
// CSceneBillBoardクラス [CSceneBillBoard.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneBillBoard.h"
#include "../../CAMERA/CCamera.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX CSceneBillBoard ::m_mtxWorld;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneBillBoard ::CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene(nPriority)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneBillBoard ::~CSceneBillBoard(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneBillBoard :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_3D *pVtx;

	//原点の位置
	m_vPos = pos;
	
	//対角線の角度
	m_fAngle = D3DX_PI / 6;		// 30度にしたいため / 6

	m_vRot.z = 0;

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * 4			// 四角形の頂点数なので4
												, D3DUSAGE_WRITEONLY			// 変えない
												, FVF_VERTEX_3D					// 使用する頂点フォーマット
												, D3DPOOL_MANAGED				// 変えない
												, &m_pD3DVtxBuff				// 格納する場所
												, NULL)))						// 変えない
	{
		return E_FAIL;															// メモリを確保できなかったらメインに返す
	}

	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(-width
							, height
							, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(width
							, height
							, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(-width
							, -height
							, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(width
							, -height
							, 0.f);
	//--------------------------------------------------------------------------------
	// 法線
	pVtx[0].nor = DEFAULT_UP_VECTOR;
	pVtx[1].nor = DEFAULT_UP_VECTOR;
	pVtx[2].nor = DEFAULT_UP_VECTOR;
	pVtx[3].nor = DEFAULT_UP_VECTOR;
	//---------------------------------------------------------------------------------
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[1].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[2].diffuse = DEFAULT_POLYGON_COLOR;
	pVtx[3].diffuse = DEFAULT_POLYGON_COLOR;
	//---------------------------------------------------------------------------------
	// 使用したいテクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(MIN_UV_VALUE, MIN_UV_VALUE);
	pVtx[1].tex = D3DXVECTOR2(MAX_UV_VALUE, MIN_UV_VALUE);
	pVtx[2].tex = D3DXVECTOR2(MIN_UV_VALUE, MAX_UV_VALUE);
	pVtx[3].tex = D3DXVECTOR2(MAX_UV_VALUE, MAX_UV_VALUE);
	//----------------------------------------------------------------------------------
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	m_pD3DVtxBuff->Unlock();								// ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneBillBoard ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneBillBoard ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneBillBoard ::Draw(void)
{
	D3DXMATRIX mtxView;
	
	// カメラの位置取得
//	mtxView = CCamera::GetMtxView(0);

	D3DXMATRIX mtxScl,mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);

	// 誤差の消去
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 3Dポリゴンの描画
	// ビルボードポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);										// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawPrimitive(D3DPT_TRIANGLESTRIP							// プリミティブの種類
							, 0
							, DRAW_SQUARE_PRINITIV_NUM);						// 書きたいポリゴン数(三角形基準・・・四角形作るならポリゴン数２）
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);					// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	pVtx[0].vtx = D3DXVECTOR3(m_vPos.x - m_fLength
							, m_vPos.y + m_fLength
							, m_vPos.z);
	pVtx[1].vtx = D3DXVECTOR3(m_vPos.x + m_fLength
							, m_vPos.y + m_fLength
							, m_vPos.z);
	pVtx[2].vtx = D3DXVECTOR3(m_vPos.x - m_fLength
							, m_vPos.y - m_fLength
							, m_vPos.z);
	pVtx[3].vtx = D3DXVECTOR3(m_vPos.x + m_fLength
							, m_vPos.y - m_fLength
							, m_vPos.z);
	m_pD3DVtxBuff->Unlock();												// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].diffuse = color;
	pVtx[1].diffuse = color;
	pVtx[2].diffuse = color;
	pVtx[3].diffuse = color;
	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneBillBoard::SetTexcordPolygon(D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom)
{
	VERTEX_3D *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);			// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	//--------------------------------------------------------------------------------
	// 描画したい頂点座標の設定(座標はZ字またはN字順に設定する)
	// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
	pVtx[0].tex = leftTop;
	pVtx[1].tex = rightTop;
	pVtx[2].tex = leftBottom;
	pVtx[3].tex = rightBottom;
	m_pD3DVtxBuff->Unlock();								// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneBillBoard* CSceneBillBoard::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	// 作成
	CSceneBillBoard* p = new CSceneBillBoard(pDevice);

	// 初期化
	p->Init(pos, width, height, texType);

	return p;
}

//*****************************************************************************
// UV設定関数
//*****************************************************************************
void CSceneBillBoard::SetUV(float left, float right, float top, float bottom)
{
	VERTEX_3D *pVtx;
	//********************************************************************************************************************
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 使用したいテクスチャの座標(0～1)
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, bottom);
	pVtx[3].tex = D3DXVECTOR2(right, bottom);
	m_pD3DVtxBuff->Unlock();									// ロックしたら必ずアンロック！！
	//********************************************************************************************************************
}
//----EOF----