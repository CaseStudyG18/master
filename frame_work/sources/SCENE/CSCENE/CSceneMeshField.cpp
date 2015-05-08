//=============================================================================
//
// CSceneMeshFieldクラス [CSceneMeshField.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneMeshField.h"
#include <stdio.h>

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX			CSceneMeshField ::m_mtxWorld;
CSceneMeshField*	CSceneMeshField ::m_apMeshField[SCENE_MAX_FIELD] = {NULL};
int					CSceneMeshField ::m_nNum = 0;
//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneMeshField ::CSceneMeshField(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType):CScene(nPriority, objType)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fAngle = 0.0f;
	m_nID = m_nNum;
	m_fGridWidth = 0;
	m_fGridHeight = 0;
	m_apMeshField[m_nID] = this;
	m_nNum++;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneMeshField ::~CSceneMeshField(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneMeshField :: Init(D3DXVECTOR3 pos, float width, float height, char* texFileName, int wblock, int hblock, bool useMap)
{
	VERTEX_3D *pVtx;
	float DistanceXVertex,DistanceZVertex; // 頂点間の距離
	float lineX=0;
	float lineZ=0;
	float tex_X = 0;
	float tex_Y = 0;
	int counter =0;
	int i;
	float Add_tex_X = 1;
	float Add_tex_Y = 1;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fWidth = width;
	m_fHeight = height;
	m_fGridWidth = m_fWidth / m_nNumWidthBlock;
	m_fGridHeight = m_fHeight / m_nNumHeightBlock;
	Add_tex_X = 1.f / (float)(m_nNumWidthBlock);
	Add_tex_Y = 1.f / (float)(m_nNumHeightBlock);

	// 総ポリゴン数の計算             ↓四角形は三角形２個なので
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~の部分は縮退ポリゴン分

	// 総インデックス数の計算
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// 最初の１個以降は１頂点プラスでポリゴンが書ける

	// 頂点間の距離算出
	DistanceXVertex = m_fGridWidth;
	DistanceZVertex = m_fGridHeight;

	//原点の位置
	m_vPos = pos;
	
	// 頂点の数を設定
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// 面法線バッファ作成
	m_vFaceNormalBuf = new D3DXVECTOR3[wblock * hblock * 2];
	for(int i = 0; i < wblock * hblock * 2; i++)
	{
		m_vFaceNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点法線バッファの作成
	m_vVtxNormalBuf = new D3DXVECTOR3[m_nNumVtxNum];
	for(int i = 0; i < m_nNumVtxNum; i++)
	{
		m_vVtxNormalBuf[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ポリゴンのメモリ確保
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxNum	// VERTEX_2Dの型で確保
												, D3DUSAGE_WRITEONLY				// 変えない
												, FVF_VERTEX_3D						// 使用する頂点フォーマット
												, D3DPOOL_MANAGED					// 変えない
												, &m_pD3DVtxBuff					// 格納する場所
												, NULL)))							// 変えない
	{
		return E_FAIL;																// メモリを確保できなかったらメインに返す
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
	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);				// ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
	// 頂点の設定
	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3(( -width * 0.5f) + lineX		// 中心からの距離なので半分に
								, 0.f
								, ( height * 0.5f) - lineZ);

		if(useMap == false)
		{
			pVtx[i].vtx.y = pos.y;
		}
		
		pVtx[i].nor = D3DXVECTOR3(0.f, 0.f, 0.f);

		// 色と透明度の設定（テクスチャを張るのでポリゴンは白で）
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;

		lineX += DistanceXVertex;
		if(i == wblock + (wblock+1) * count)
		{
			lineX = 0;
			lineZ += DistanceZVertex;
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

	// 面法線の算出
	int faceNormalCounter = 0, hblockCounter = 0, wblockCounter = 0, vtxIndexOne = 1, vtxIndexSecond = wblock + 1;
	D3DXVECTOR3 norOne, norSecond, vecOneA, vecOneB, vecSecondA, vecSecondB;
	
	// 縦のループ
	for(hblockCounter = 0; hblockCounter < hblock; hblockCounter++)
	{
		// 横のループ
		for(wblockCounter = 0; wblockCounter < wblock; wblockCounter++, vtxIndexOne++, vtxIndexSecond++)
		{
			// 横に見ていき２個ずつ作る
			// １つ目
			vecOneA = pVtx[vtxIndexSecond - (wblock + 1)].vtx - pVtx[vtxIndexSecond].vtx;
			vecOneB = pVtx[vtxIndexSecond + 1].vtx - pVtx[vtxIndexSecond].vtx;
			D3DXVec3Cross(&norOne, &vecOneA, &vecOneB);
			D3DXVec3Normalize(&norOne, &norOne);
			m_vFaceNormalBuf[faceNormalCounter] = norOne;
			
			// 頂点の法線の設定
			pVtx[vtxIndexSecond - (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond - (wblock + 1)].nor, &pVtx[vtxIndexSecond - (wblock + 1)].nor);

			pVtx[vtxIndexSecond].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond].nor, &pVtx[vtxIndexSecond].nor);

			pVtx[vtxIndexSecond + 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexSecond + 1].nor, &pVtx[vtxIndexSecond + 1].nor);

			faceNormalCounter++;

			// ２つ目
			vecSecondA = pVtx[vtxIndexOne + (wblock + 1)].vtx - pVtx[vtxIndexOne].vtx;
			vecSecondB = pVtx[vtxIndexOne - 1].vtx - pVtx[vtxIndexOne].vtx;
			D3DXVec3Cross(&norSecond, &vecSecondA, &vecSecondB);
			D3DXVec3Normalize(&norSecond, &norSecond);
			m_vFaceNormalBuf[faceNormalCounter] = norSecond;
			
			// 頂点の法線の設定
			pVtx[vtxIndexOne + (wblock + 1)].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne + (wblock + 1)].nor, &pVtx[vtxIndexOne + (wblock + 1)].nor);

			pVtx[vtxIndexOne].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne].nor, &pVtx[vtxIndexOne].nor);

			pVtx[vtxIndexOne - 1].nor += m_vFaceNormalBuf[faceNormalCounter];
			D3DXVec3Normalize(&pVtx[vtxIndexOne - 1].nor, &pVtx[vtxIndexOne - 1].nor);
			
			faceNormalCounter++;
		}
		vtxIndexOne++;
		vtxIndexSecond++;
	}

	// 使用したいテクスチャの座標
	for(i = 0 ; i < m_nNumVtxNum ; i++)
	{
		tex_X += Add_tex_X;
		// 1を超えないようにする
		if(tex_X > MAX_UV_VALUE)
		{
			tex_X = MIN_UV_VALUE;
		}

		// １列やったら
		if(i - 1 == wblock + counter * (wblock + 1))
		{
			counter++;
			
			tex_Y += Add_tex_Y;

			if(tex_Y > MAX_UV_VALUE)
			{
				tex_Y = MIN_UV_VALUE;
			}
		}

		// テクスチャ設定
		pVtx[i].tex = D3DXVECTOR2((float)tex_X,(float)tex_Y);
	}

	D3DXCreateTextureFromFile((*m_pD3DDevice), texFileName, &m_pD3DTexBuff);
	m_pD3DVtxBuff->Unlock();												// ロックしたら必ずアンロック！！
	//********************************************************************************************************************

	WORD *pIndex;
	// ロック
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// インデックスの設定																	2つづつ見てるので２つづつ進む
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
				i += 2;	// 2つづつ見てるので２つづつ進む
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
void CSceneMeshField ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	if(m_pD3DIndexBuff)
	{
		m_pD3DIndexBuff->Release();
		m_pD3DIndexBuff = NULL;
	}

	if(m_vFaceNormalBuf)
	{
		delete[] m_vFaceNormalBuf;
		m_vFaceNormalBuf = NULL;
	}

	if(m_vVtxNormalBuf)
	{
		delete[] m_vVtxNormalBuf;
		m_vVtxNormalBuf = NULL;
	}

	if(m_pD3DTexBuff)
	{
		m_pD3DTexBuff->Release();
		m_pD3DTexBuff = NULL;
	}

	m_nNum--;
	m_apMeshField[m_nID] = NULL;

	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneMeshField ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneMeshField ::Draw(void)
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
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));	// (0,渡すものが入ってるやつ,0,データの型指定)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);								// インデックスの設定
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);										// 頂点フォーマットの設定
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);								// テクスチャの設定（２つ目の引数をNULLにするとテクスチャを描画しない
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP					// プリミティブの種類
										, 0
										, 0
										, m_nNumVtxNum							// 総頂点数
										, 0
										, m_nNumPolygon);
}

//*****************************************************************************
// ポリゴンセット関数  更新で呼んだりする
//*****************************************************************************
void CSceneMeshField :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;
	float DistanceXVertex,DistanceZVertex;	// 頂点間の距離
	DistanceXVertex = m_fWidth / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		// 頂点の設定
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fWidth * 0.5f) + lineX	// 中心からの距離なので半分にしている
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
void CSceneMeshField::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;

	// 頂点情報の設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum; i++)
	{
		pVtx[i].diffuse = color;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneMeshField* CSceneMeshField::Create(LPDIRECT3DDEVICE9 *pDevice, char* fieldFileName)
{
	// 作成
	CSceneMeshField* p = new CSceneMeshField(pDevice);

	VERTEX_3D *pVtx;
	int nTexNameLength = 0;
	char texFilePath[MAX_PATH] = {0};
	char* texName = NULL;

	// 読み込み用ファイル
	FILE* fpr = fopen(fieldFileName, "r");

	// 失敗したら
	if(fpr == NULL)
	{
		printf("ファイル読み込み失敗\n");
		rewind(stdin);
		getchar();
	}

	// 成功したら
	else
	{
		// 分割数取得
		int nHeightBlock = 0, nWidthBlock = 0;
		fscanf(fpr, "%d", &nHeightBlock);	// 縦
		fscanf(fpr, "%d", &nWidthBlock);	// 横

		// 縦横の長さ取得
		float fHeight = 0.f, fWidth = 0.f;
		fscanf(fpr, "%f", &fHeight);
		fscanf(fpr, "%f", &fWidth);

		// 使用テクスチャの文字数を取得
		fscanf(fpr, "%d", &nTexNameLength);
		
		// テクスチャ使ってるなら
		if(nTexNameLength > 0)
		{
			// テクスチャ名取得
			char temp[MAX_PATH] = {0};
			texName = new char[nTexNameLength + 1];	// \0分
			ZeroMemory(texName, sizeof(texName));
			fscanf(fpr, "%s", texName);

			// パス作成
			strcat(temp, TEX_FOLDER_PATH);		// TEXTUREフォルダへ
			strcat(temp, texName);				// ファイル名
			strcpy(texFilePath, temp);
		}
		
		// パラメーターをセット
		if(nTexNameLength > 0)
		{
			p->Init(D3DXVECTOR3(0.f, 0.f, 0.f), fWidth, fHeight, texFilePath, nWidthBlock, nHeightBlock);
		}
		else
		{
			p->Init(D3DXVECTOR3(0.f, 0.f, 0.f), fWidth, fHeight, NULL, nWidthBlock, nHeightBlock);
		}

		// 高さ反映
		p->m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);			//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
		for(int i = 0; i < p->m_nNumVtxNum; i++)
		{
			float y = 0.f;
			fscanf(fpr, "%f", &y);
			pVtx[i].vtx.y = y;
		}
		p->m_pD3DVtxBuff->Unlock();
	}
	fclose(fpr);

	if(texName)
	{
		delete[] texName;
	}

	return p;
	
}

//*****************************************************************************
// 高さ取得関数
//*****************************************************************************
float CSceneMeshField::GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal)
{
	if(pos.x >= m_vCornerPos[0].x && pos.x <= m_vCornerPos[1].x)
	{
		if(pos.z <= m_vCornerPos[0].z && pos.z >= m_vCornerPos[2].z)
		{
			D3DXVECTOR3 vec0,vec1;
			D3DXVECTOR3 vtxPos[4] = {D3DXVECTOR3(0.f, 0.f, 0.f)};	// 四角形の頂点なので4つ
			int indexX, indexZ;
			float ans;

			// グリッドの指標を出す
			indexX = (int)((pos.x - m_vCornerPos[0].x) / m_fGridWidth);
			indexZ = (int)((pos.z - m_vCornerPos[0].z) / m_fGridHeight);
			if(indexX < 0)
			{
				indexX = -indexX;
			}
			if(indexZ < 0)
			{
				indexZ = -indexZ;
			}

			VERTEX_3D *pVtx;
			//ポリゴンの設定
			m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		//ロックしないと勝手に書き換わる場合がある(アンロックを忘れずに)
			
			// 調べる頂点決定
			vtxPos[0] = D3DXVECTOR3(m_vCornerPos[0].x + m_fGridWidth * (float)indexX
									, pVtx[indexX + indexZ * (m_nNumWidthBlock + 1)].vtx.y
									, m_vCornerPos[0].z - m_fGridHeight * (float)indexZ);
			vtxPos[1] = D3DXVECTOR3(m_vCornerPos[0].x + m_fGridWidth * (float)(indexX + 1)
									, pVtx[(indexX + 1) + indexZ * (m_nNumWidthBlock + 1)].vtx.y
									, m_vCornerPos[0].z - m_fGridHeight * (float)indexZ);
			vtxPos[2] = D3DXVECTOR3(m_vCornerPos[0].x + m_fGridWidth * (float)(indexX + 1)
									, pVtx[(indexX + 1) + (indexZ + 1) * (m_nNumWidthBlock + 1)].vtx.y
									, m_vCornerPos[0].z - m_fGridHeight * (float)(indexZ + 1));
			vtxPos[3] = D3DXVECTOR3(m_vCornerPos[0].x + m_fGridWidth * (float)indexX
									, pVtx[indexX + (indexZ + 1) * (m_nNumWidthBlock + 1)].vtx.y
									, m_vCornerPos[0].z - m_fGridHeight * (float)(indexZ + 1));

			m_pD3DVtxBuff->Unlock();

			vec0 = vtxPos[2] - vtxPos[0];
			vec1 = pos - vtxPos[0];

			// 外積
			if((vec0.z * vec1.x) - (vec0.x * vec1.z) > 0.f)
			{
				vec0 = vtxPos[3] - vtxPos[2];
				vec1 = pos - vtxPos[2];

				// 外積
				if((vec0.z * vec1.x) - (vec0.x * vec1.z) > 0.f)
				{
					vec0 = vtxPos[0] - vtxPos[3];
					vec1 = pos - vtxPos[3];

					// 外積
					if((vec0.z * vec1.x) - (vec0.x * vec1.z) > 0.f)
					{
						// 立っている場所のＹ座標を返す
						ans = GetHeightPolygon(vtxPos[0], vtxPos[2], vtxPos[3], pos, pNormal);
						return ans;
					}
				}
			}

			// 立っている場所のＹ座標を返す
			ans = GetHeightPolygon(vtxPos[0], vtxPos[1], vtxPos[2], pos, pNormal);
			return ans;
		}
	}
	return 0.f;
}

//*****************************************************************************
// 高さ取得関数
//*****************************************************************************
float CSceneMeshField::GetHeightPolygon(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal)
{
	D3DXVECTOR3 vec0, vec1, normal;
	vec0 = p1 - p0;
	vec1 = p2 - p0;

	float ans;	// 計算結果保持用

	D3DXVec3Cross(&normal, &vec0, &vec1);
	D3DXVec3Normalize(&normal, &normal);

	if(normal.y == 0.0f)
	{
		return 0.f;
	}

	ans = p0.y - (normal.x * (pos.x - p0.x) + normal.z * (pos.z - p0.z)) / normal.y;

	return ans;
}

//----EOF----