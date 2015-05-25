//=============================================================================
//
// CSceneCrinderクラス [CSceneCrinder.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneCrinder.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneCrinder ::CSceneCrinder(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene(nPriority)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fAngle = 0.0f;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneCrinder ::~CSceneCrinder(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneCrinder :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	// 使ってないので
	return E_FAIL;
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT	CSceneCrinder :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, bool In, int wblock, int hblock)
{
	VERTEX_3D *pVtx;
	
	float DistanceXVertex,DistanceZVertex; // 頂点間の距離
	float lineX=0;
	float lineZ=0;
	float nNumDig = 0;						// 角度用
	float PlusDig = 0;
	float tex_X = 1;
	float tex_Y = 1;
	float Add_tex_X = 1;
	float Add_tex_Y = 1;
	int counter =0;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fRadius = width;
	m_fHeight = height;
	// 角度の計算
	PlusDig = D3DX_PI * 2 / m_nNumWidthBlock;	// 360度にしたいので * 2
	nNumDig = 0;

	// テクスチャの増加量計算
	Add_tex_X = 1.0f / (float)(m_nNumWidthBlock * 0.5);	// 半分にしたいので * 0.5
	Add_tex_Y = 1.0f / (float)(m_nNumHeightBlock);

	// 総ポリゴン数の計算             ↓四角形は三角形２個なので
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~の部分は縮退ポリゴン分

	// 総インデックス数の計算
	m_nNumVertexIndex = NUM_POLYGON_CREATE_TRIANGLE + (m_nNumPolygon - 1);	// 最初の１個以降は１頂点プラスでポリゴンが書ける

	// 頂点間の距離算出
	DistanceXVertex = width / (float)wblock;
	DistanceZVertex = height / (float)hblock;

	// 原点の位置
	m_vPos = pos;
	
	// 頂点の数を設定
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxNum
												, D3DUSAGE_WRITEONLY
												, FVF_VERTEX_3D
												, D3DPOOL_MANAGED
												, &m_pD3DVtxBuff
												, NULL)))
	{
		return E_FAIL;	//メモリを確保できなかったらメインに返す
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
	// 頂点の設定(内側に向かって描画したいので普段と逆)
	for(int i = 0, count = 0;i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3((m_fRadius) * cosf(nNumDig)			// (半径 * 減衰率) * cos
								, (float)(DistanceZVertex * count)
								, (m_fRadius * sinf(nNumDig)));
		// 角度増加
		nNumDig += PlusDig;

		// 横一列やったら
		if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
		{
			// 行番号増加
			count++;

			// 角度を戻す
			nNumDig = 0;
		}
	}

	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		// テクスチャの設定(内側に向かって描画したいので普段と逆)
		// 1マイナスにならないようにする
		if(tex_X < 0)
		{
			tex_X = 1;
		}

		// １列やったら
		if(i-1 == m_nNumWidthBlock + count * (m_nNumWidthBlock + 1))
		{
			// 行番号増加
			count++;

			// テクスチャＸをもとにもどす
			tex_X = 1;
		
			// テクスチャＹを変更
			tex_Y -= Add_tex_Y;

			// 1マイナスにならないようにする
			if(tex_Y < 0)
			{
				tex_Y = 0.01f;
				tex_X = 0.01f;
			}
		}

		// テクスチャ設定
		pVtx[i].tex = D3DXVECTOR2(tex_X, tex_Y);

		// テクスチャＸ座標を増やす
		tex_X += Add_tex_X;

		// 法線の設定
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// 色と透明度の設定
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	
	m_pD3DTexBuff = CTexture::GetTexture(texType);
	m_pD3DVtxBuff->Unlock();							//ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	WORD *pIndex;
	// ロック
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// インデックスの設定                                                                    // 一回で２つづつ見てるので２づつ増やす
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// 横に見たときに１づつ増えていくことを利用
		pIndex[i] = first;
		pIndex[i+1] = second;

		// 外向きなら
		if(!In)
		{
			pIndex[i + 1] = first;
			pIndex[i] = second;
		}

		if(counter < hblock - 1)
		{
			// 折り返しの処理
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// 一回で２つづつ見てるので２づつ増やす
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

				if(!In)
				{
					pIndex[i + 1] = first + 1;
					pIndex[i] = second;
				}

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
void CSceneCrinder ::Uninit(void)
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

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneCrinder ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneCrinder ::Draw(void)
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
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));		//(0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);											//頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);									//テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP						//プリミティブの種類
										, 0
										, 0
										, m_nNumVtxNum								//総頂点数
										, 0
										, m_nNumPolygon);
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneCrinder :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;
	float DistanceXVertex,DistanceZVertex; // 頂点間の距離
	DistanceXVertex = m_fRadius / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		// 頂点の設定
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fRadius * 0.5f) + lineX		// 中心からの距離なので半分に
									, 0.0f
									, (m_vPos.z + m_fHeight * 0.5f) - lineZ);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineZ += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// ポリゴンカラーセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneCrinder::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;

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
CSceneCrinder* CSceneCrinder::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, bool In, D3DXVECTOR3 rot)
{
	// 作成
	CSceneCrinder* p = new CSceneCrinder(pDevice);

	// 初期化
	p->Init(pos, r, height, texType, In, wblock, hblock);

	p->SetRot(rot);

	return p;
	
}
//----EOF----