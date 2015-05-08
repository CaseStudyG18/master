//=============================================================================
//
// CSceneMeshFieldクラス [CSceneMeshField.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEMESHFIELD_H_
#define _CSCENEMESHFIELD_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int SCENE_MAX_FIELD = 1;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshField : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ、オブジェタイプ
	//======================================================
	CSceneMeshField(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3, OBJTYPE m_objType = OBJTYPE_FIELD);

	// デストラクタ
	~CSceneMeshField();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャーファイルネーム、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, char* texFileName, int wblock = 1, int hblock = 1, bool useMap = true);
	
	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);
	
	// ポジションアクセサ
	D3DXVECTOR3 GetPos(void){ return m_vPos;};
	void		SetPos(D3DXVECTOR3 pos){ m_vPos = pos;};
	void		SetPos(float x,float y, float z){ m_vPos.x = x; m_vPos.y = y; m_vPos.z = z;};
	
	// ロットアクセサ
	D3DXVECTOR3 GetRot(void){return m_vRot;};
	void		SetRot(D3DXVECTOR3 rot){m_vRot = rot;};
	void		SetRot(float x,float y,float z){ m_vRot.x = x; m_vRot.y = y; m_vRot.z = z;};
	
	//=================================================
	// 高さ取得
	// 引数: 知りたい場所、(out)法線
	// 戻り値: 高さ
	//=================================================
	float GetHeight(const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	//=================================================
	// サイズゲット
	// 引数: (out)Ｘサイズ、(out)Ｚサイズ
	//=================================================
	void GetSize(float* pOutXSize, float* pOutZSize){*pOutXSize = m_fWidth; *pOutZSize = m_fHeight;};

	//=================================================
	// 角の座標ゲット
	// 引数: 角の番号
	// 戻り値: 角の座標
	//=================================================
	D3DXVECTOR3 GetCornerPos(int idx){ return m_vCornerPos[idx];};

	//=================================================
	// クリエイト関数
	// 引数: デバイス、フィールドファイルパス
	// 戻り値: 作ったやつのポインタ
	//=================================================
	static CSceneMeshField* Create(LPDIRECT3DDEVICE9 *pDevice, char* fieldFileName);
	
	//=================================================
	// アドレス取得
	// 引数: フィールド番号
	// 戻り値: アドレス
	//=================================================
	static CSceneMeshField* GetAddress(int id){return m_apMeshField[id];};

private:
	// 使ってない初期化
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1){return S_OK;};
	
	// 頂点セット
	void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	//=================================================
	// ポリゴンの高さ取得
	// 引数: 頂点座標0、頂点座標1、頂点座標2、調べたい座標、（out）法線ポインタ
	// 戻り値: 高さ
	//=================================================
	float GetHeightPolygon(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& pos, D3DXVECTOR3* pNormal);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;						// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;						// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;					// インデックス情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;						// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;								// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;								// ポリゴンの回転
	D3DXVECTOR3				m_vScl;								// 地面の大きさ（スケール）
	D3DXVECTOR3				m_vCornerPos[4];					// 角の座標
	D3DXVECTOR3*			m_vFaceNormalBuf;					// 面法線バッファ
	D3DXVECTOR3*			m_vVtxNormalBuf;					// 頂点法線バッファ
	float					m_fAngle;							// ポリゴンの対角線の角度
	float					m_fWidth;							// 幅
	float					m_fHeight;							// 高さ
	float					m_fGridWidth;						// グリッドの幅
	float					m_fGridHeight;						// グリッドの高さ
	int						m_nNumPolygon;						// ポリゴン数
	int						m_nNumVertexIndex;					// インデックス数
	int						m_nNumVtxNum;						// 頂点数
	int						m_nNumWidthBlock;					// 横の分割数
	int						m_nNumHeightBlock;					// 縦の分割数
	int						m_nID;								// ID
	static D3DXMATRIX		m_mtxWorld;							// ワールドマトリックス
	static CSceneMeshField	*m_apMeshField[SCENE_MAX_FIELD];	// フィールドバッファ
	static int				m_nNum;								// 総数
};
#endif
//----EOF----