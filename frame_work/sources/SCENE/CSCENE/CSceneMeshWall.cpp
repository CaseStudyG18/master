//=============================================================================
//
// CSceneMeshWall�N���X [CSceneMeshWall.cpp]
// Author : ��K�@����
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "CSceneMeshWall.h"

//*****************************************************************************
// �X�^�e�B�b�N�����o�ϐ�
//*****************************************************************************
D3DXMATRIX		CSceneMeshWall ::m_mtxWorld;
CSceneMeshWall*	CSceneMeshWall ::m_apMeshWall[SCENE_MAX_WALL] = {NULL};
int				CSceneMeshWall ::m_nNum = 0;

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CSceneMeshWall ::CSceneMeshWall(LPDIRECT3DDEVICE9 *pDevice, int nPriority):CScene(nPriority)
{
	m_pD3DDevice = pDevice;
	m_pD3DVtxBuff = NULL;
	m_pD3DTexBuff = NULL;
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_fAngle = 0.0f;
	for(int i = 0; i < 4; i++)	// �l�p�`�̊p�Ȃ̂�4
	{
		m_vCornerPos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_nID = m_nNum;
	m_apMeshWall[m_nID] = this;
	m_nNum++;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CSceneMeshWall ::~CSceneMeshWall(void)
{
}

//*****************************************************************************
// �������֐�
//*****************************************************************************
HRESULT CSceneMeshWall :: Init(D3DXVECTOR3 pos, float width, float height, TEXTURE_TYPE texType, int wblock, int hblock)
{
	VERTEX_3D *pVtx;
	
	float DistanceXVertex, DistanceZVertex; // ���_�Ԃ̋���
	float lineX = 0;
	float lineY = 0;
	int tex_X = 0;
	int tex_Y = 0;
	int counter =0;
	int i;
	m_nNumWidthBlock = wblock;
	m_nNumHeightBlock = hblock;
	m_fWidth = width;
	m_fHeight = height;

	// ���|���S�����̌v�Z             ���l�p�`�͎O�p�`�Q�Ȃ̂�
	m_nNumPolygon = wblock * hblock * 2 + ((hblock - 1) * 4);	// +~�̕����͏k�ރ|���S����

	// ���C���f�b�N�X���̌v�Z
	m_nNumVertexIndex = 3 + (m_nNumPolygon - 1);	// 3�͍ŏ��̎O�p�`�̒��_�@�ŏ��̂P�ȍ~�͂P���_�v���X�Ń|���S����������

	// ���_�Ԃ̋����Z�o
	DistanceXVertex = width / wblock;
	DistanceZVertex = height / hblock;

	//���_�̈ʒu
	m_vPos = pos;
	
	// ���_�̐���ݒ�
	m_nNumVtxNum = (wblock + 1) * (hblock + 1);

	// �|���S���̃������m��
	if(FAILED((*m_pD3DDevice)->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVtxNum	// VERTEX_2D�̌^�łS���_*�\���������m��
												, D3DUSAGE_WRITEONLY				// �ς��Ȃ�
												, FVF_VERTEX_3D						// �g�p���钸�_�t�H�[�}�b�g
												, D3DPOOL_MANAGED					// �ς��Ȃ�
												, &m_pD3DVtxBuff					// �i�[����ꏊ
												, NULL)))							// �ς��Ȃ�
	{
		return E_FAIL;																//���������m�ۂł��Ȃ������烁�C���ɕԂ�
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
	// ���_�̐ݒ�
	for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
	{
		pVtx[i].vtx = D3DXVECTOR3(( - width * 0.5f) + lineX	// ���S����̋����Ȃ̂Ŕ�����
								, ( height * 0.5f) - lineY
								, 0.f);

		lineX += DistanceXVertex;
		if(i == wblock + (wblock+1) * count)
		{
			lineX = 0;
			lineY += DistanceZVertex;
			count++;
		}
		
		if(i == 0)
		{
			m_vCornerPos[0] = pVtx[i].vtx;
			m_vCornerPos[0].x += m_vPos.x;
			m_vCornerPos[0].y += m_vPos.y;
			m_vCornerPos[0].z += m_vPos.z;
		}

		if(i == wblock)
		{
			m_vCornerPos[1] = pVtx[i].vtx;
			m_vCornerPos[1].x += m_vPos.x;
			m_vCornerPos[1].y += m_vPos.y;
			m_vCornerPos[1].z += m_vPos.z;
		}

		if(i == m_nNumVtxNum - 1)
		{
			m_vCornerPos[2] = pVtx[i].vtx;
			m_vCornerPos[2].x += m_vPos.x;
			m_vCornerPos[2].y += m_vPos.y;
			m_vCornerPos[2].z += m_vPos.z;
		}

		if(i == m_nNumVtxNum - wblock - 1)
		{
			m_vCornerPos[3] = pVtx[i].vtx;
			m_vCornerPos[3].x += m_vPos.x;
			m_vCornerPos[3].y += m_vPos.y;
			m_vCornerPos[3].z += m_vPos.z;
		}
	}

	for(int i = 0; i < m_nNumVtxNum; i++)
	{
		// �@���̐ݒ�
		pVtx[i].nor = DEFAULT_UP_VECTOR;

		// �F�Ɠ����x�̐ݒ�i�e�N�X�`���𒣂�̂Ń|���S���͔��Łj
		pVtx[i].diffuse = DEFAULT_POLYGON_COLOR;
	}
	// �g�p�������e�N�X�`���̍��W(0�`1)
	for(i = 0 ; i < m_nNumVtxNum ; i++, tex_X++)
	{
		// 1�𒴂��Ȃ��悤�ɂ���
		if(tex_X > MAX_UV_VALUE)
		{
			tex_X = (int)MIN_UV_VALUE;
		}

		// �P��������
		if(i - 1 == wblock + counter * (wblock + 1))
		{
			counter++;

			tex_X = (int)MIN_UV_VALUE;
			
			tex_Y ++;

			if(tex_Y > MAX_UV_VALUE)
			{
				tex_Y = (int)MIN_UV_VALUE;
			}
		}

		// �e�N�X�`���ݒ�
		pVtx[i].tex = D3DXVECTOR2((float)tex_X, (float)tex_Y);
	}
	m_pD3DTexBuff = CTexture::GetTexture( texType );
	m_pD3DVtxBuff->Unlock();									//���b�N������K���A�����b�N�I�I
	//********************************************************************************************************************

	WORD *pIndex;
	// ���b�N
	m_pD3DIndexBuff->Lock(0, 0, (void**)&pIndex, 0);
	
	// �C���f�b�N�X�̐ݒ�																	// 2�Â݂Ă���̂�+2
	for(int i = 0, first = wblock + 1, second = 0, counter = 0 ; i < m_nNumVertexIndex - 1 ; i += 2, first++, second++)
	{
		// ���Ɍ����Ƃ��ɂP�Â����Ă������Ƃ𗘗p
		pIndex[i] = first;
		pIndex[i+1] = second;

		if(counter < hblock - 1)
		{
			// �܂�Ԃ��̏���
			if(second == wblock + (wblock + 1) * counter)
			{
				i += 2;					// 2�Â݂Ă���̂�+2
				pIndex[i] = second;
				pIndex[i+1] = first + 1;

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
void CSceneMeshWall ::Uninit(void)
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

	m_nNum = 0;

	this->Release();
}

//*****************************************************************************
// �X�V�֐�
//*****************************************************************************
void CSceneMeshWall ::Update(void)
{
}

//*****************************************************************************
// �`��֐�
//*****************************************************************************
void CSceneMeshWall ::Draw(void)
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
	(*m_pD3DDevice)->SetStreamSource(0, m_pD3DVtxBuff, 0, sizeof(VERTEX_3D));		// (0,�n�����̂������Ă���,0,�f�[�^�̌^�w��)
	(*m_pD3DDevice)->SetIndices(m_pD3DIndexBuff);									// �C���f�b�N�X�̐ݒ�
	(*m_pD3DDevice)->SetFVF(FVF_VERTEX_3D);											// ���_�t�H�[�}�b�g�̐ݒ�
	(*m_pD3DDevice)->SetTexture(0, m_pD3DTexBuff);									// �e�N�X�`���̐ݒ�i�Q�ڂ̈�����NULL�ɂ���ƃe�N�X�`����`�悵�Ȃ�
	(*m_pD3DDevice)->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP						// �v���~�e�B�u�̎��
										, 0
										, 0
										, m_nNumVtxNum								//�����_��
										, 0
										, m_nNumPolygon);
}

//*****************************************************************************
// �|���S���Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void CSceneMeshWall :: SetVertexPolygon(void)
{
	VERTEX_3D *pVtx;
	int lineX = 0, lineY = 0;
	float DistanceXVertex,DistanceZVertex; // ���_�Ԃ̋���
	DistanceXVertex = m_fWidth / m_nNumWidthBlock;
	DistanceZVertex = m_fHeight / m_nNumHeightBlock;

	// ���_���̐ݒ�
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < m_nNumVtxNum ; i++)
	{
		// ���_�̐ݒ�
		for(int i = 0, count = 0; i < m_nNumVtxNum; i++)
		{
			pVtx[i].vtx = D3DXVECTOR3((m_vPos.x - m_fWidth * 0.5f) + lineX	// ���S����̋����Ȃ̂Ŕ�����
									, (m_vPos.z + m_fHeight * 0.5f) - lineY
									, 0.0f);

			lineX += (int)DistanceXVertex;
			if(i == m_nNumWidthBlock + (m_nNumWidthBlock + 1) * count)
			{
				lineX = 0;
				lineY += (int)DistanceZVertex;
				count++;
			}
		}
	}
	m_pD3DVtxBuff->Unlock();
}

//*****************************************************************************
// �|���S���J���[�Z�b�g�֐�  �X�V�ŌĂ񂾂肷��
//*****************************************************************************
void	CSceneMeshWall::SetColorPolygon(D3DXCOLOR color)
{
	VERTEX_3D	*pVtx;
	int lineX = 0, lineY = 0;

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
CSceneMeshWall* CSceneMeshWall::Create(LPDIRECT3DDEVICE9 *pDevice, D3DXVECTOR3 pos, float width, float height, int wblock, int hblock, TEXTURE_TYPE texType, D3DXVECTOR3 rot)
{
	// �쐬
	CSceneMeshWall* p = new CSceneMeshWall(pDevice);

	// ������
	p->Init(pos, width, height, texType, wblock, hblock);

	p->SetRot(rot);

	return p;
	
}
//----EOF----