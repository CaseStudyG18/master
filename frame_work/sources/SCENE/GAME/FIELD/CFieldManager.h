//=============================================================================
//
// CFieldManagerクラス [CFieldManager.h]
// Author : 野尻　尚希
//
//=============================================================================
#ifndef _CFIELDMANAGER_H_
#define _CFIELDMANAGER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "../../../MAIN/main.h"
#include "../../../TEXTURE/CTexture.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CField;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFieldManager
{
public:
	typedef enum
	{
		FIELD_TEST = 0,
		FIELD_MAX
	}FIELD_TYPE;


	// コンストラクタ
	CFieldManager(void);

	// デストラクタ
	~CFieldManager(void);

	// フィールドクリエイト
	CField* CreateField(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType);

	// ロードフィールド
	void LoadField(LPDIRECT3DDEVICE9 *pDevice, FIELD_TYPE fieldType);

	// 初期化
	void Init(void);

	// 終了
	void Uninit(void);

	// 更新
	void Update(void);

private:

};

#endif
//----EOF----