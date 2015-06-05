//=============================================================================
//
// CFieldクラス [CField.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CField.h"

#ifdef _DEBUG
#include "../../../INPUT/CInputKeyboard.h"
#endif
//=======================================================================
// コンストラクタ
//=======================================================================
CField::CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CScene2D(pDevice, nPriority, m_objType)
{

}

//=======================================================================
// デストラクタ
//=======================================================================
CField::~CField()
{
}

//=======================================================================
// 終了
//=======================================================================
void CField::Uninit(void)
{
	CScene2D::Uninit();
}

//=======================================================================
// 更新
//=======================================================================
void CField::Update(void)
{
	CScene2D::Update();

#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardPress(DIK_O))
	{
		D3DXVECTOR3 rot = GetRot();
		rot.z += 0.002f;
		NormalizeRotation(&rot);
		SetRot(rot);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_U))
	{
		D3DXVECTOR3 rot = GetRot();
		rot.z -= 0.002f;
		NormalizeRotation(&rot);
		SetRot(rot);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_L))
	{
		AddWidth_BaseLeft(1.f);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_J))
	{
		AddWidth_BaseLeft(-1.f);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_I))
	{
		AddHeight_BaseBottom(1.f);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_K))
	{
		AddHeight_BaseBottom(-1.f);
	}
#endif
}

//=======================================================================
// 描画
//=======================================================================
void CField::Draw(void)
{
	CScene2D::Draw();
}

//=======================================================================
// クリエイト関数
//=======================================================================
CField* CField::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = new CField(pDevice);

	p->Init(pos, width, height, texType);

	return p;
}

//=======================================================================
// 初期化関数
//=======================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);
	return TRUE;
}
//----EOF----