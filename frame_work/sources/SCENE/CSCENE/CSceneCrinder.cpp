//=============================================================================
//
// CSceneCrinder�N���X [CSceneCrinder.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneCrinder.h"

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneCrinder ::CSceneCrinder(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene(nPriority)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vRot = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_vScl = D3DXVECTOR3(1.0f,1.0f,1.0f);
	m_fAngle = 0.0f;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneCrinder ::~CSceneCrinder(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneCrinder :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	// �g���ĂȂ��̂�
	return E_FAIL;
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT	CSceneCrinder :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, bool In, int wblock, int hblock)
{
	VERTEX_3D *pVtx;
	
	float DistanceXVertex,DistanceZVertex; // ���_�Ԃ̋���
	float lineX=0;
	float lineZ=0;
	float nNumDig = 0;						// �p�x�p
	float PlusDig = 0;
	float tex_X = 1;
	float tex_Y = 1;
	float Add_tex_X = 1;
	float Add_tex_Y = 1;
	int counter =0;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fRadius = width;
	m_fHeight = height;
	// �p�x�̌v�Z
	PlusDig = D3DX_PI * 2 / m_nNumWidthBlock;	// 360�x�ɂ������̂� * 2
	nNumDig = 0;

	// �e�N�X�`���̑����ʌv�Z
	Add_tex_X = 1.0f / (float)(m_nNumWidthBlock * 0.5);	// �����ɂ������̂� * 0.5
	Add_tex_Y = 1.0f / (float)(m_nNumHeightBlock);

	// ���|���S�����̌v�Z             ���l�p�`�͎O�p�`�Q�Ȃ̂�
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~�̕����͏k�ރ|���S����

	// ���C���f�b�N�X���̌v�Z
	m_nNumVertexIndex = NUM_POLYGON_CREATE_TRIANGLE + (m_nNumPolygon - 1);	// �ŏ��̂P�ȍ~�͂P���_�v���X�Ń|���S����������

	// ���_�Ԃ̋����Z�o
	DistanceXVertex = width / (float)wblock;
	DistanceZVertex = height / (float)hblock;

	// ���_�̈ʒu
	m_vPos = pos;
	
	// ���_�̐���ݒ�
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxNum
												, D3DUSAGE_WRITEONLY
												, FVF_VERTEX_3D
												, D3DPOOL_MANAGED
												, &m_pD3DVtxBuff
												, NULL)))
	{
		return E_FAIL;	//���������m�ۂł��Ȃ������烁�C���ɕԂ�
	}

	// �C���f�b�N�X�o�b�t�@�̐���
	if(FAILED((*m_pD3DDevice)->CreateIndexBuffer(sizeof(WORD) * m_nNumVertexIndex	// ���C���f�b�N�X��
												, D3DUSAGE_WRITEONLY
												, D3DFMT_INDEX16
												, D3DPOOL_MANAGED
												, &m_pD3DIndexBuff
												, NULL)))
	{
		return E_FAIL;
	}

	//********************************************************************************************************************
	//�|���S���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);		//���b�N���Ȃ��Ə���ɏ��������ꍇ������(�A�����b�N��Y�ꂸ��)
	// ���_�̐ݒ�(�����Ɍ������ĕ`�悵�����̂ŕ��i�Ƌt)
	for(int i = 0, count = 0;i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3((m_fRadius) * cosf(nNumDig)			// (���a * ������) * cos
								, (float)(DistanceZVertex * count)
								, (m_fRadius * sinf(nNumDig)));
		// �p�x����
		nNumDig += PlusDig;

		// �����������
		if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
		{
			// �s�ԍ�����
			count++;

			// �p�x��߂�
			nNumDig = 0;
		}
	}

	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		// �e�N�X�`���̐ݒ�(�����Ɍ������ĕ`�悵�����̂ŕ��i�Ƌt)
		// 1�}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
		if(tex_X < 0)
		{
			tex_X = 1;
		}

		// �P��������
		if(i-1 == m_nNumWidthBlock + count * (m_nNumWidthBlock + 1))
		{
			// �s�ԍ�����
			count++;

			// �e�N�X�`���w�����Ƃɂ��ǂ�
			tex_X = 1;
		
			// �e�N�X�`���x��ύX
			tex_Y -= Add_tex_Y;

			// 1�}�C�i�X�ɂȂ�Ȃ��悤�ɂ���
			if(tex_Y < 0)
			{
				tex_Y = 0.01f;
				tex_X = 0.01f;
			}
		}

		// �e�N�X�`���ݒ�
		pVtx[i].tex = D3DXVECTOR2(tex_X, tex_Y);

		// �e�N�X�`���w���W�𑝂₷
		tex_X += Add_tex_X;

		// �@���̐ݒ�
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// �F�Ɠ����x�̐ݒ�
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	
	m_pD3DTexBuff = CTexture::GetTexture(texType);
	m_pD3DVtxBuff->Unlock();							//���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************

	WORD *pIndex;
	// ���b�N
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// �C���f�b�N�X�̐ݒ�                                                                    // ���łQ�Â��Ă�̂łQ�Â��₷
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// ���Ɍ����Ƃ��ɂP�Â����Ă������Ƃ𗘗p
		pIndex[i] = first;
		pIndex[i+1] = second;

		// �O�����Ȃ�
		if(!In)
		{
			pIndex[i + 1] = first;
			pIndex[i] = second;
		}

		if(counter < hblock - 1)
		{
			// �܂�Ԃ��̏���
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;	// ���łQ�Â��Ă�̂łQ�Â��₷
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

				if(!In)
				{
					pIndex[i + 1] = first + 1;
					pIndex[i] = second;
				}

				// �܂�Ԃ����J�E���g�A�b�v
				counter++;
			}
		}
	}

	// �A�����b�N
	m_pD3DIndexBuff->Unlock();

	return S_OK;
}

//*****************************************************************************
// �I���֐�
//*****************************************************************************
void CSceneCrinder ::Uninit(void)
{
	if(m_pD3DVtxBuff)
	{
		m_pD3DVtxBuff->Release();
		m_pD3DVtxBuff = NULL;
	}

	m_pD3DTexBuff = NULL;

	if(m_pD3DIndexBuff)
	{
		m_pD3DIndexBuff->Release();
		m_pD3DIndexBuff = NULL;
	}

	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneCrinder ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneCrinder ::Draw(void)
{
	D3DXMATRIX mtxScl,mtxRot,mtxTranslate;

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
	
	// �|���S���̕`��
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));		//(0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);											//���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);									//�e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP						//�v���~�e�B�u�̎��
										, 0
										, 0
										, m_nNumVtxNum								//�����_��
										, 0
										, m_nNumPolygon);
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneCrinder :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;
	float DistanceXVertex,DistanceZVertex; // ���_�Ԃ̋���
	DistanceXVertex = m_fRadius / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// ���_���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		// ���_�̐ݒ�
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fRadius * 0.5f) + lineX		// ���S����̋����Ȃ̂Ŕ�����
									, 0.0f
									, (m_vPos.z + m_fHeight * 0.5f) - lineZ);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineZ += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneCrinder::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineZ = 0;

	// ���_���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		pVtx[i].diffuse = color;
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �N���G�C�g�֐�
//*****************************************************************************
CSceneCrinder* CSceneCrinder::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float r, float height, int wblock, int hblock, TEXTURE_TYPE texType, bool In, D3DXVECTOR3 rot)
{
	// �쐬
	CSceneCrinder* p = new CSceneCrinder(pDevice);

	// ������
	p->Init(pos, r, height, texType, In, wblock, hblock);

	p->SetRot(rot);

	return p;
	
}
//----EOF----