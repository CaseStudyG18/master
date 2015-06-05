//=============================================================================
//
// モデル処理 [CModel.h]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CModel.h"
#include "../SCENE/CSCENE/CScene.h"

//*****************************************************************************
// 定数定義
//*****************************************************************************
// モデルのファイルパス
static const char *MODEL_PATH[ MODEL_MAX ] = 
{
	NULL,
};

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const int LENGTH_MAX = 256;	// 最大文字数

//*****************************************************************************
// スタティックメンバ変数宣言
//*****************************************************************************
X_MODEL				CModel::m_Model[MODEL_MAX] = {NULL};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init( LPDIRECT3DDEVICE9 *pDevice )
{
	// ゲームで使う全モデルを作成
	for(int modelCnt = 0; modelCnt < MODEL_MAX; modelCnt++)
	{
		// モデルの初期化
		if(FAILED(D3DXLoadMeshFromX(MODEL_PATH[modelCnt]	// 読み込むファイル名
									, D3DXMESH_SYSTEMMEM
									, *pDevice
									, NULL
									, &m_Model[modelCnt].pD3DXBuffMatModel
									, NULL
									, &m_Model[modelCnt].nNumMatModel
									, &m_Model[modelCnt].pD3DXMeshModel)))
		{
			return E_FAIL;
		}
		
		
		// マテリアルの数分作る
		m_Model[modelCnt].pD3DTexBuff = new LPDIRECT3DTEXTURE9[(int)m_Model[modelCnt].nNumMatModel];
		m_Model[modelCnt].pD3DCubeTexBuff = new LPDIRECT3DCUBETEXTURE9[(int)m_Model[modelCnt].nNumMatModel];

		D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)m_Model[modelCnt].pD3DXBuffMatModel->GetBufferPointer();

		for(int i = 0; i < (int)m_Model[modelCnt].nNumMatModel; i++)
		{
			//テクスチャをロード
			m_Model[modelCnt].pD3DTexBuff[i] = NULL;
			m_Model[modelCnt].pD3DCubeTexBuff[i] = NULL;
		
			// テクスチャの処理
			char temp[LENGTH_MAX];
			if(d3dxmatrs[i].pTextureFilename != NULL)
			{
				//中身初期化
				ZeroMemory(temp, sizeof(char) * LENGTH_MAX);

				//文字列連結
				strcat(temp, TEX_FOLDER_PATH);
				strcat(temp, d3dxmatrs[i].pTextureFilename);
				
				//テクスチャ読み込み
				D3DXCreateTextureFromFile(*pDevice, temp, &m_Model[modelCnt].pD3DTexBuff[i]);
			}
		}

		// Xファイルに法線がない場合は、法線を書き込む
		if (!(m_Model[modelCnt].pD3DXMeshModel->GetFVF() & D3DFVF_NORMAL))
		{
			ID3DXMesh* pTempMesh = NULL;
			
			m_Model[modelCnt].pD3DXMeshModel->CloneMeshFVF(m_Model[modelCnt].pD3DXMeshModel->GetOptions()
															, m_Model[modelCnt].pD3DXMeshModel->GetFVF()|D3DFVF_NORMAL
															, *pDevice
															, &pTempMesh);
		
			D3DXComputeNormals(pTempMesh, NULL);
			m_Model[modelCnt].pD3DXMeshModel->Release();
			m_Model[modelCnt].pD3DXMeshModel = pTempMesh;
		}

		// インデックス情報を保存
		LPDIRECT3DINDEXBUFFER9 index_buffer;	// インデックスバッファ
		WORD* pIndices;							// ポリゴン構成（頂点リンク）データへのポインタ
		WORD* pIndices_2;						// ポリゴン構成（頂点リンク）データへのポインタ
		
		// インデックスバッファオブジェクトへのポインタをゲット
		m_Model[modelCnt].pD3DXMeshModel->GetIndexBuffer(&index_buffer);

		// 面の数取得
		int numFace = m_Model[modelCnt].pD3DXMeshModel->GetNumFaces();

		// インデックスを頂点数分作る
		m_Model[modelCnt].pIndex = new WORD[numFace * NUM_POLYGON_CREATE_TRIANGLE];
		pIndices_2 = m_Model[modelCnt].pIndex;

		// インデックスバッファをロック
		index_buffer ->Lock(0, 0, (void**)&pIndices , 0);

		// インデックスデータをワークにコピー
		memcpy(pIndices_2, pIndices, sizeof(WORD)*numFace * NUM_POLYGON_CREATE_TRIANGLE);
		
		// インデックスバッファをアンロック
		index_buffer ->Unlock();

		// 解放
		index_buffer->Release();

		// 頂点情報保存
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer;	// 頂点バッファ
		VERTEX* pVertices;						// 頂点データへのポインタ
		VERTEX* pVertices_2;					// 頂点データへのポインタ

		// 頂点バッファオブジェクトへのポインタをゲット
		m_Model[modelCnt].pD3DXMeshModel->GetVertexBuffer(&vertex_buffer);

		// 頂点数をゲット
		int numVertex = m_Model[modelCnt].pD3DXMeshModel->GetNumVertices();
		
		// 頂点の数分作る
		m_Model[modelCnt].pVertex = new VERTEX[numVertex];
		pVertices_2 = m_Model[modelCnt].pVertex;

		// 頂点バッファをロック
		vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);

		// 頂点データをワークにコピー
		memcpy(pVertices_2, pVertices, sizeof(VERTEX)*numVertex);

		// 頂点バッファをアンロック
		vertex_buffer->Unlock();

		// 解放
		vertex_buffer->Release();
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit( void )
{
	// 使った全テクスチャポインタを開放
	for( int modelCnt = 0; modelCnt < MODEL_MAX; modelCnt++ )
	{
		if(m_Model[modelCnt].pD3DTexBuff)
		{
			for (int i = 0; i < (int)m_Model[modelCnt].nNumMatModel; i++)
			{
				if(m_Model[modelCnt].pD3DTexBuff[i])
				{
					(m_Model[modelCnt].pD3DTexBuff[i])->Release();
					(m_Model[modelCnt].pD3DTexBuff[i]) = NULL;
				}
			}
			delete[] m_Model[modelCnt].pD3DTexBuff;
			m_Model[modelCnt].pD3DTexBuff = NULL;
		}

		if(m_Model[modelCnt].pD3DCubeTexBuff)
		{
			delete[] m_Model[modelCnt].pD3DCubeTexBuff;
			m_Model[modelCnt].pD3DCubeTexBuff = NULL;
		}

		if(m_Model[modelCnt].pD3DXMeshModel)
		{
			m_Model[modelCnt].pD3DXMeshModel->Release();
			m_Model[modelCnt].pD3DXMeshModel = NULL;
		}

		if(m_Model[modelCnt].pD3DXBuffMatModel)
		{
			m_Model[modelCnt].pD3DXBuffMatModel->Release();
			m_Model[modelCnt].pD3DXBuffMatModel = NULL;
		}

		if(m_Model[modelCnt].pIndex)
		{
			delete[] m_Model[modelCnt].pIndex;
			m_Model[modelCnt].pIndex = NULL;
		}

		if(m_Model[modelCnt].pVertex)
		{
			delete[] m_Model[modelCnt].pVertex;
			m_Model[modelCnt].pVertex = NULL;
		}
	}
}

//=============================================================================
// モデル作成処理
//=============================================================================
void CModel::CreateModel( LPDIRECT3DDEVICE9 *pDevice)
{
	CModel::Init( pDevice);
}

//=============================================================================
// モデル情報ゲット
//=============================================================================
X_MODEL* CModel::GetModel(MODEL_TYPE type)
{
	return &m_Model[type];
}

X_MODEL* CModel::GetModel(char* fileName)
{
	// 作ってあるもののファイルパスと比較してあったら返す
	for(int i = 0; i < MODEL_MAX; i++)
	{
		if(strcmp(MODEL_PATH[i], fileName) == 0)
		{
			return &m_Model[i];
		}
	}
	return NULL;
}
//----EOF----