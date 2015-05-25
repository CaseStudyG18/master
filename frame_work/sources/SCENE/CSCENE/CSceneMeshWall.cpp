//=============================================================================
//
// CSceneMeshWallクラス [CSceneMeshWall.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneMeshWall.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX		CSceneMeshWall ::m_mtxWorld;
CSceneMeshWall*	CSceneMeshWall ::m_apMeshWall[SCENE_MAX_WALL] = {NULL};
int				CSceneMeshWall ::m_nNum = 0;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneMeshWall ::CSceneMeshWall(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene(nPriority)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fAngle = 0.0f;
	for(int i = 0; i < 4; i++)	// 四角形の角なので4
	{
		m_vCornerPos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nID = m_nNum;
	m_apMeshWall[m_nID] = this;
	m_nNum++;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneMeshWall ::~CSceneMeshWall(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneMeshWall :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_3D *pVtx;
	
	float DistanceXVertex, DistanceZVertex; // 頂点間の距離
	float lineX = 0;
	float lineY = 0;
	int tex_X = 0;
	int tex_Y = 0;
	int counter =0;
	int i;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fWidth = width;
	m_fHeight = height;

	// 総ポリゴン数の計算             ↓四角形は三角形２個なので
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~の部分は縮退ポリゴン分

	// 総インデックス数の計算
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// 3は最初の三角形の頂点　最初の１個以降は１頂点プラスでポリゴンが書ける

	// 頂点間の距離算出
	DistanceXVertex = width / wblock;
	DistanceZVertex = height / hblock;

	//原点の位置
	m_vPos = pos;
	
	// 頂点の数を設定
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxNum	// VERTEX_2Dの型で４頂点*表示桁数分確保
												, D3DUSAGE_WRITEONLY				// 変えない
												, FVF_VERTEX_3D						// 使用する頂点フォーマット
												, D3DPOOL_MANAGED					// 変えない
												, &m_pD3DVtxBuff					// 格納する場所
												, NULL)))							// 変えない
	{
		return E_FAIL;																//メモリを確保できなかったらメインに返す
	}

	// インデックスバッファの生成
	if(FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// 総インデックス数
												, D3DUSAGE_WRITEONLY
												, D3DFMT_INDEX16
												, D3DPOOL_MANAGED
												, &m_pD3DIndexBuff
												, NULL)))
	{
		return E_FAIL;
	}

	//********************************************************************************************************************
	//ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 頂点の設定
	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3(( - width * 0.5f) + lineX	// 中心からの距離なので半分に
								, ( height * 0.5f) - lineY
								, 0.f);

		lineX += DistanceXVertex;
		if(i == wblock + (wblock+1) * count)
		{
			lineX = 0;
			lineY += DistanceZVertex;
			count++;
		}
		
		if(i == 0)
		{
			m_vCornerPos[0] = pVtx[i].vtx;
			m_vCornerPos[0].x += m_vPos.x;
			m_vCornerPos[0].y += m_vPos.y;
			m_vCornerPos[0].z += m_vPos.z;
		}

		if(i == wblock)
		{
			m_vCornerPos[1] = pVtx[i].vtx;
			m_vCornerPos[1].x += m_vPos.x;
			m_vCornerPos[1].y += m_vPos.y;
			m_vCornerPos[1].z += m_vPos.z;
		}

		if(i == m_nNumVtxNum - 1)
		{
			m_vCornerPos[2] = pVtx[i].vtx;
			m_vCornerPos[2].x += m_vPos.x;
			m_vCornerPos[2].y += m_vPos.y;
			m_vCornerPos[2].z += m_vPos.z;
		}

		if(i == m_nNumVtxNum - wblock - 1)
		{
			m_vCornerPos[3] = pVtx[i].vtx;
			m_vCornerPos[3].x += m_vPos.x;
			m_vCornerPos[3].y += m_vPos.y;
			m_vCornerPos[3].z += m_vPos.z;
		}
	}

	for(int i = 0; i < m_nNumVtxNum; i++)
	{
		// 法線の設定
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	// 使用したいテクスチャの座標(0～1)
	for(i = 0 ; i < m_nNumVtxNum ; i++, tex_X++)
	{
		// 1を超えないようにする
		if(tex_X > MAX_UV_VALUE)
		{
			tex_X = (int)MIN_UV_VALUE;
		}

		// １列やったら
		if(i - 1 == wblock + counter * (wblock + 1))
		{
			counter++;

			tex_X = (int)MIN_UV_VALUE;
			
			tex_Y ++;

			if(tex_Y > MAX_UV_VALUE)
			{
				tex_Y = (int)MIN_UV_VALUE;
			}
		}

		// テクスチャ設定
		pVtx[i].tex = D3DXVECTOR2((float)tex_X, (float)tex_Y);
	}
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	m_pD3DVtxBuff->Unlock();									//ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	WORD *pIndex;
	// ロック
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// インデックスの設定																	// 2個づつみているので+2
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// 横に見たときに１づつ増えていくことを利用
		pIndex[i] = first;
		pIndex[i+1] = second;

		if(counter < hblock - 1)
		{
			// 折り返しの処理
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;					// 2個づつみているので+2
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

				// 折り返し数カウントアップ
				counter++;
			}
		}
	}

	// アンロック
	m_pD3DIndexBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneMeshWall ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	if(m_pD3DIndexBuff)
	{
		m_pD3DIndexBuff->Release();
		m_pD3DIndexBuff = NULL;
	}

	m_nNum = 0;

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneMeshWall ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneMeshWall ::Draw(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転を反映		*****順番注意*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.y, m_vRot.x, m_vRot.z);	// Y軸を基点に回転しているのでY,X,Zの順に入れる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// ポリゴンの描画
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));		// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);									// インデックスの設定
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);											// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);									// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP						// プリミティブの種類
										, 0
										, 0
										, m_nNumVtxNum								//総頂点数
										, 0
										, m_nNumPolygon);
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneMeshWall :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineY = 0;
	float DistanceXVertex,DistanceZVertex; // 頂点間の距離
	DistanceXVertex = m_fWidth / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		// 頂点の設定
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fWidth * 0.5f) + lineX	// 中心からの距離なので半分に
									, (m_vPos.z + m_fHeight * 0.5f) - lineY
									, 0.0f);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineY += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void	CSceneMeshWall::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D	*pVtx;
	int lineX = 0, lineY = 0;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		pVtx[i].diffuse = color;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneMeshWall* CSceneMeshWall::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int wblock, int hblock, TEXTURE_TYPE texType, D3DXVECTOR3 rot)
{
	// 作成
	CSceneMeshWall* p = new CSceneMeshWall(pDevice);

	// 初期化
	p->Init(pos, width, height, texType, wblock, hblock);

	p->SetRot(rot);

	return p;
	
}
//----EOF----