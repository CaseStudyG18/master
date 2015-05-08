//=============================================================================
//
// CSceneMeshWallクラス [CSceneMeshWall.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENEMESHWALL_H_
#define _CSCENEMESHWALL_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
static const int SCENE_MAX_WALL = 4;				// 壁の最大数
static const int DEFAULT_WALL_SEPALATE_NUM = 5;		// 壁のデフォルト分割数
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneMeshWall : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ、オブジェタイプ
	//======================================================
	CSceneMeshWall(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3);

	// デストラクタ
	~CSceneMeshWall();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャータイプ、(横分割数、縦分割数)
	// 戻り値: 成否
	//======================================================
	HRESULT Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock = 1, int hblock = 1);
	
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
	
	//======================================================
	// 角の座標ゲット
	// 引数: 角の番号
	// 戻り値: 座標
	//======================================================
	D3DXVECTOR3 GetCornerPos(int idx){ return m_vCornerPos[idx];};


	//=================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、横分割数、縦分割数、テクスチャタイプ、角度
	// 戻り値: 作ったやつのアドレス
	//=================================================
	static CSceneMeshWall* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int wblock, int hblock, TEXTURE_TYPE texType, D3DXVECTOR3 rot = D3DXVECTOR3(0,0,0));

	//=================================================
	// アドレス取得
	// 引数: 壁の番号
	// 戻り値: アドレス
	//=================================================
	static CSceneMeshWall* GetAddress(int id){return m_apMeshWall[id];};

private:
	// 頂点反映関数
	void SetVertexPolygon(void);

	//=================================================
	// 色変更関数
	// 引数: 色
	//=================================================
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;					// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;					// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DINDEXBUFFER9	m_pD3DIndexBuff;				// インデックス情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;					// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;							// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;							// ポリゴンの回転
	D3DXVECTOR3				m_vScl;							// 地面の大きさ（スケール）
	D3DXVECTOR3				m_vCornerPos[4];				// 角の座標  四角形の角なので4つ
	float					m_fAngle;						// ポリゴンの対角線の角度
	float					m_fWidth;						// 幅
	float					m_fHeight;						// 高さ
	int						m_nNumPolygon;					// ポリゴン数
	int						m_nNumVertexIndex;				// インデックス数
	int						m_nNumVtxNum;					// 頂点数
	int						m_nNumWidthBlock;				// 横の分割数
	int						m_nNumHeightBlock;				// 縦の分割数
	int						m_nID;							// ID
	static D3DXMATRIX		m_mtxWorld;						// ワールドマトリックス
	static CSceneMeshWall	*m_apMeshWall[SCENE_MAX_WALL];	// 壁のバッファ
	static int				m_nNum;							// 総数
};
#endif
//----EOF----