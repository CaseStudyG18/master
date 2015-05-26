//=============================================================================
//
// CField�N���X [CField.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CField.h"

#ifdef _DEBUG
#include "../../../INPUT/CInputKeyboard.h"
#endif
//=======================================================================
// �R���X�g���N�^
//=======================================================================
CField::CField(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE m_objType) :CScene2D(pDevice, nPriority, m_objType)
{

}

//=======================================================================
// �f�X�g���N�^
//=======================================================================
CField::~CField()
{
}

//=======================================================================
// �I��
//=======================================================================
void CField::Uninit(void)
{
	CScene2D::Uninit();
}

//=======================================================================
// �X�V
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
// �`��
//=======================================================================
void CField::Draw(void)
{
	CScene2D::Draw();
}

//=======================================================================
// �N���G�C�g�֐�
//=======================================================================
CField* CField::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType)
{
	CField* p = new CField(pDevice);

	p->Init(pos, width, height, texType);

	return p;
}

//=======================================================================
// �������֐�
//=======================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	CScene2D::Init(pos, width, height, texType);
	return TRUE;
}
//----EOF----