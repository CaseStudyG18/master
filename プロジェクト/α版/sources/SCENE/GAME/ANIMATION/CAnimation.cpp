//=============================================================================
//
// �A�j���[�V�����N���X [CAnimation.cpp]
// Author : �˖{�@�r�F
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CAnimation.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CAnimation::CAnimation(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CScene2D(pDevice, nPriority)
{
	m_bDraw = true;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CAnimation ::~CAnimation(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CAnimation :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX,int sepalateY)
{
	CScene2D ::Init(pos, width, height, texType);

	// �l�̃`�F�b�N
	if(sepalateX <= 0 || sepalateY <= 0)
	{
		return E_FAIL;
	}
	m_nSepalateNumX = sepalateX;
	m_nSepalateNumY = sepalateY;
	m_nMaxIdx = sepalateX * sepalateY;

	// �e�N�X�`�����p�Z�b�g
	SetIndex(0);

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CAnimation ::Uninit(void)
{
	CScene2D :: Uninit();

}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CAnimation ::Update(void)
{

}


//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CAnimation ::Draw(void)
{
	if(m_bDraw)
	{
		CScene2D ::Draw();
	}
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 *pDevice, int nPriority, D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int sepalateX, int sepalateY)
{
	// �쐬
	CAnimation* p = new CAnimation(pDevice, nPriority);

	p->m_pD3DDevice = pDevice;

	// ������
	p->Init(pos, width, height, texType, sepalateX, sepalateY);

	return p;
}

//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CAnimation::SetIndex(int nIdx)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
	m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
	m_fTop = fV * (m_nIdx / m_nSepalateNumX);
	m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//=============================================================================
// �C���f�b�N�X�̐ݒ�
//=============================================================================
void CAnimation::SetIndex(int nIdx, bool reverse)
{
	float fU, fV;

	// �l�̃`�F�b�N
	if (nIdx < 0)
	{
		return;
	}

	m_nIdx = nIdx;

	fU = MAX_UV_VALUE / m_nSepalateNumX;
	fV = MAX_UV_VALUE / m_nSepalateNumY;

	if (reverse){
		m_fRight = fU * (m_nIdx % m_nSepalateNumX);
		m_fLeft = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}
	else{
		m_fLeft = fU * (m_nIdx % m_nSepalateNumX);
		m_fRight = fU * ((m_nIdx % m_nSepalateNumX) + 1);
		m_fTop = fV * (m_nIdx / m_nSepalateNumX);
		m_fBottom = fV * ((m_nIdx / m_nSepalateNumX) + 1);
	}

	UV_INDEX uv = { m_fLeft, m_fRight, m_fTop, m_fBottom };
	SetUV(&uv);
}
//----EOF----