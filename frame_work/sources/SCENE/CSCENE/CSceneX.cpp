//=============================================================================
//
// CSceneXクラス [CSceneX.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CSceneX.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
D3DXMATRIX CSceneX ::m_mtxWorld;

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSceneX ::CSceneX(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType):CScene(nPriority, objType)
{
	m_pD3DDevice = pDevice;
	m_ModelInfo = NULL;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	ZeroMemory(m_cModelFileName, sizeof(m_cModelFileName));
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSceneX ::~CSceneX(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneX :: Init(D3DXVECTOR3 pos, char* modelFileName)
{
	// 初期化
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// スケールは基本変えないのでこのまま固定。カメラが寄れば大きく見えるし、離れれば小さく見えるから
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// モデル情報取得
	m_ModelInfo = CModel::GetModel(modelFileName);
	return S_OK;
}
//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CSceneX :: Init(D3DXVECTOR3 pos, MODEL_TYPE type)
{
	// 初期化
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// スケールは基本変えないのでこのまま固定。カメラが寄れば大きく見えるし、離れれば小さく見えるから
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// モデル情報取得
	m_ModelInfo = CModel::GetModel(type);

	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CSceneX ::Uninit(void)
{
	this->Release();
}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CSceneX ::Update(void)
{
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CSceneX ::Draw(void)
{
	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// 元のマテリアル情報を保存しておく
	D3DXMATRIX			mtxScl,mtxRot,mtxTranslate;
	
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

	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// 現在のマテリアルの情報を取得
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// マテリアルの描画
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		(*m_pD3DDevice)->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		(*m_pD3DDevice)->SetTexture(0
									, m_ModelInfo->pD3DTexBuff[nCntMat]);	// テクスチャを使うかどうか ******注意:使わなくても書くこと!!*******
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// モデルのパーツを描画
	}

	//*********注意:以下を必ず書くこと******************
	// 書かないとすべての色がおかしくなる
	(*m_pD3DDevice)->SetMaterial(&matDef);									// マテリアル情報を元に戻す
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos,char* modelFileName)
{
	// 作成
	CSceneX* p = new CSceneX(pDevice);

	// 初期化
	p->Init(pos, modelFileName);

	return p;
}

//*****************************************************************************
// クリエイト関数
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, MODEL_TYPE type)
{
	// 作成
	CSceneX* p = new CSceneX(pDevice);

	// 初期化
	p->Init(pos, type);

	return p;
}

//*****************************************************************************
// アルファ値変更関数
//*****************************************************************************
void CSceneX::SetAlpha(float alpha)
{
	D3DXMATERIAL *pD3DXMat;
	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// マテリアルのアルファ値変更
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse.a = alpha;
	}
}

//*****************************************************************************
// スペキュラー値変更関数
//*****************************************************************************
void CSceneX::SetSpecularPow(float pow)
{
	D3DXMATERIAL *pD3DXMat;
	// マテリアルのバッファ取得
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// マテリアルのアルファ値変更
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Power = pow;
	}
}
//----EOF----