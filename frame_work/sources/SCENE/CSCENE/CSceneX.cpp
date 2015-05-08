//=============================================================================
//
// CSceneX�N���X [CSceneX.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneX.h"

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
D3DXMATRIX CSceneX ::m_mtxWorld;

//*****************************************************************************
// �R���X�g���N�^
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
// �f�X�g���N�^
//*****************************************************************************
CSceneX ::~CSceneX(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneX :: Init(D3DXVECTOR3 pos, char* modelFileName)
{
	// ������
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// �X�P�[���͊�{�ς��Ȃ��̂ł��̂܂܌Œ�B�J���������Α傫�������邵�A�����Ώ����������邩��
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// ���f�����擾
	m_ModelInfo = CModel::GetModel(modelFileName);
	return S_OK;
}
//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneX :: Init(D3DXVECTOR3 pos, MODEL_TYPE type)
{
	// ������
	m_vPos = pos;
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// �X�P�[���͊�{�ς��Ȃ��̂ł��̂܂܌Œ�B�J���������Α傫�������邵�A�����Ώ����������邩��
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);

	// ���f�����擾
	m_ModelInfo = CModel::GetModel(type);

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneX ::Uninit(void)
{
	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneX ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneX ::Draw(void)
{
	D3DXMATERIAL		*pD3DXMat;
	D3DMATERIAL9		matDef;		// ���̃}�e���A������ۑ����Ă���
	D3DXMATRIX			mtxScl,mtxRot,mtxTranslate;
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_vScl.x, m_vScl.y, m_vScl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f		*****���Ԓ���*****
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.y, m_vRot.x, m_vRot.z);	// Y������_�ɉ�]���Ă���̂�Y,X,Z�̏��ɓ����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTranslate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// ���݂̃}�e���A���̏����擾
	(*m_pD3DDevice)->GetMaterial(&matDef);

	// �}�e���A���̕`��
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		(*m_pD3DDevice)->SetMaterial(&pD3DXMat[nCntMat].MatD3D);
		(*m_pD3DDevice)->SetTexture(0
									, m_ModelInfo->pD3DTexBuff[nCntMat]);	// �e�N�X�`�����g�����ǂ��� ******����:�g��Ȃ��Ă���������!!*******
		m_ModelInfo->pD3DXMeshModel->DrawSubset(nCntMat);					// ���f���̃p�[�c��`��
	}

	//*********����:�ȉ���K����������******************
	// �����Ȃ��Ƃ��ׂĂ̐F�����������Ȃ�
	(*m_pD3DDevice)->SetMaterial(&matDef);									// �}�e���A���������ɖ߂�
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos,char* modelFileName)
{
	// �쐬
	CSceneX* p = new CSceneX(pDevice);

	// ������
	p->Init(pos, modelFileName);

	return p;
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, MODEL_TYPE type)
{
	// �쐬
	CSceneX* p = new CSceneX(pDevice);

	// ������
	p->Init(pos, type);

	return p;
}

//*****************************************************************************
// �A���t�@�l�ύX�֐�
//*****************************************************************************
void CSceneX::SetAlpha(float alpha)
{
	D3DXMATERIAL *pD3DXMat;
	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// �}�e���A���̃A���t�@�l�ύX
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Diffuse.a = alpha;
	}
}

//*****************************************************************************
// �X�y�L�����[�l�ύX�֐�
//*****************************************************************************
void CSceneX::SetSpecularPow(float pow)
{
	D3DXMATERIAL *pD3DXMat;
	// �}�e���A���̃o�b�t�@�擾
	pD3DXMat = (D3DXMATERIAL*)m_ModelInfo->pD3DXBuffMatModel->GetBufferPointer();

	// �}�e���A���̃A���t�@�l�ύX
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelInfo->nNumMatModel; nCntMat++)
	{
		pD3DXMat[nCntMat].MatD3D.Power = pow;
	}
}
//----EOF----