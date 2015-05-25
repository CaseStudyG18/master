//=============================================================================
//
// CSceneBillBoardクラス [CSceneBillBoard.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CSCENE_BILLBOARD_H_
#define _CSCENE_BILLBOARD_H_

//=============================================================================
// インクルード
//=============================================================================
#include "../../MAIN/main.h"
#include "CScene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneBillBoard : public CScene
{
public:
	//======================================================
	// コンストラクタ
	// 引数: デバイス、プライオリティ
	//======================================================
	CSceneBillBoard(LPDIRECT3DDEVICE9 *pDevice, int nPriority = TYPE_PRIORITY_3);

	// デストラクタ
	~CSceneBillBoard();

	//======================================================
	// 初期化
	// 引数: 座標、幅、高さ、テクスチャの種類、(横分割数、縦分割数)
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

	// UV値変更
	void SetTexcordPolygon(D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom);
	void SetUV(float left, float right, float top, float bottom);

	//==================================================
	// クリエイト関数
	// 引数: デバイス、座標、幅、高さ、テクスチャータイプ
	// 戻り値: 作ったやつのポインタ
	//==================================================
	static CSceneBillBoard* Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

private:
	// 頂点反映関数
	void SetVertexPolygon(void);

	// 色設定関数
	void SetColorPolygon(D3DXCOLOR color);

	LPDIRECT3DDEVICE9		*m_pD3DDevice;			// デバイスオブジェクト(描画に必要)
	LPDIRECT3DVERTEXBUFFER9	m_pD3DVtxBuff;			// 頂点座標情報を格納する場所のアドレスを確保する場所
	LPDIRECT3DTEXTURE9		m_pD3DTexBuff;			// テクスチャ情報を格納する場所のアドレスを確保する場所
	D3DXVECTOR3				m_vPos;					// ポリゴンの中央の位置
	D3DXVECTOR3				m_vRot;					// ポリゴンの回転
	D3DXVECTOR3				m_vScl;					// 地面の大きさ（スケール）
	float					m_fLength;				// ポリゴンの対角線の長さ
	float					m_fAngle;				// ポリゴンの対角線の角度
	static D3DXMATRIX		m_mtxWorld;				// ワールドマトリックス
};
#endif
//----EOF----