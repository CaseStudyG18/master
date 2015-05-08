//=============================================================================
//
// CSceneクラス [CScene.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "CScene.h"
#include "../../FADE/CFade.h"

//*****************************************************************************
// スタティックメンバ変数
//*****************************************************************************
CScene*		CScene::m_apTop[TYPE_PRIORITY_MAX] = {NULL};		// リストの先頭アドレス
CScene*		CScene::m_apCur[TYPE_PRIORITY_MAX] = {NULL};		// リストの終端アドレス

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CScene ::CScene(int nPriority, OBJTYPE objType)
{
	// オブジェタイプ設定
	m_objType = objType;

	// プライオリティ設定
	m_nPriority = nPriority;

	// 消去フラグ初期化
	m_bDelete = false;

	// １個目のとき
	if(CScene::m_apTop[nPriority] == NULL)
	{
		CScene ::m_apTop[nPriority] = this;
		CScene ::m_apCur[nPriority] = this;
		m_pNext = NULL;
		m_pPrev = NULL;
	}

	// それ以外
	else
	{
		// １つ前のインスタンス保存用
		CScene *prev = NULL;
		
		// １つ前のインスタンス保存
		prev = CScene ::m_apCur[nPriority];

		// 現在が終端なのでNULL
		m_pNext = NULL;

		// 前ポインタは保存してあるやつ
		m_pPrev = prev;

		// 現在が終端
		m_apCur[nPriority] = this;
		
		// 前ポインタの次ポインタを変更
		prev->m_pNext = this;
	}
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CScene ::~CScene(void)
{
}

//*****************************************************************************
// 初期化関数
//*****************************************************************************
HRESULT CScene :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	return S_OK;
}

//*****************************************************************************
// 終了関数
//*****************************************************************************
void CScene ::Uninit(void)
{

}

//*****************************************************************************
// 更新関数
//*****************************************************************************
void CScene ::Update(void)
{

}

//*****************************************************************************
// 全て更新関数
//*****************************************************************************
void CScene ::UpdateAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;

	for(int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 更新
			pScene->Update();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];

		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// デスフラグONなら
			if(pScene->m_bDelete)
			{
				// リストから除外
				pScene->UnLinkList();
				
				// 削除
				delete pScene;
				pScene = NULL;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// 指定プライオリティ更新関数
//*****************************************************************************
void CScene ::UpdateChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while(pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// 更新
		pScene->Update();

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}

	// 先頭を指定
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while(pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// デスフラグONなら
		if(pScene->m_bDelete)
		{
			// リストから除外
			pScene->UnLinkList();
			
			// 削除
			delete pScene;
			pScene = NULL;
		}

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}
	
}

//*****************************************************************************
// 描画関数
//*****************************************************************************
void CScene ::Draw(void)
{
}

//*****************************************************************************
// 全て描画関数
//*****************************************************************************
void CScene ::DrawAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for(int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		pScene = m_apTop[priority];	// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 描画
			pScene->Draw();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//*****************************************************************************
// 指定プライオリティ描画関数
//*****************************************************************************
void CScene ::DrawChoice(int priority)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	pScene = m_apTop[priority];

	// ポインタがNULLでなければ
	while(pScene)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNext = pScene->m_pNext;

		// 描画
		pScene->Draw();

		// 次のインスタンスを対象のインスタンスにする
		pScene = pSceneNext;
	}
	
}

//*****************************************************************************
// リリース関数
//*****************************************************************************
void CScene::Release(void)
{
	// デスフラグON
	m_bDelete = true;
}

//*****************************************************************************
// 全てリリース関数
//*****************************************************************************
void CScene ::ReleaseAll(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	
	for(int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = m_apTop[priority];
		
		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// 解放
			pScene->Uninit();

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭を指定
		pScene = m_apTop[priority];
		
		// ポインタがNULLでなければ
		while(pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->m_pNext;

			// デスフラグONなら
			if(pScene->m_bDelete)
			{
				// 解放
				delete pScene;
				pScene = NULL;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}

		// 先頭,終端アドレス初期化
		m_apTop[priority] = NULL;
		m_apCur[priority] = NULL;
	}
}

//*****************************************************************************
// リストから除外関数
//*****************************************************************************
void CScene::UnLinkList(void)
{
	CScene* pScene = this;

	if(this->m_pPrev && this->m_pNext)
	{
		CScene *pPrev = this->m_pPrev;
		CScene *pNext = this->m_pNext;

		// 前のインスタンスの次ポインタを変更
		pPrev->m_pNext = this->m_pNext;
		
		// 次のインスタンスの前ポインタを変更
		pNext->m_pPrev = this->m_pPrev;
		
	}

	// 先頭なら
	if(this->m_pPrev == NULL)
	{
		CScene *pNext = this->m_pNext;

		if(pNext)
		{
			// 先頭アドレス変更
			m_apTop[this->m_nPriority] = pNext;

			// 先頭へ
			pNext->m_pPrev = NULL;
		}
	}

	// 最後なら
	if(this->m_pNext == NULL)
	{
		CScene *pPrev = this->m_pPrev;

		// 終端アドレス変更
		m_apCur[this->m_nPriority] = pPrev;

		if(pPrev)
		{
			// 終端に
			pPrev->m_pNext = NULL;
		}
	}

	// 現在のインスタンスをリストから除外
	this->m_pNext = NULL;
	this->m_pPrev = NULL;

	pScene->Uninit();
}

//----EOF----