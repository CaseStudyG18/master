//=============================================================================
//
// CJudge�N���X [CJudge.cpp]
// Author : ��K�@����
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CJudge.h"
#include "../MATH/matrix.h"
#include "../MATH/vector.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/CSCENE/CScene2D.h"

//=========================================================================
// �t�B�[���h�ƃv���C���[�̂����蔻��
//=========================================================================
void CJudge::ColiFieldxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CScene2D *pPlayer[4] = {NULL};	// �v���C���[�̍ő�l�����p��
	CScene2D *pField;
	CJudge::OBB_INFO playerOBB[4];

	// �v���C���[���擾�̃��[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		int playerCount = 0;

		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_PLAYER)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// �v���C���[�������
			if (playerCount < 4)
			{
				pPlayer[playerCount] = (CScene2D*)pScene;
				D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
				float rot = pPlayer[playerCount]->GetRot().z;
				float width = pPlayer[playerCount]->GetWidth();
				float height = pPlayer[playerCount]->GetHeight();

				// OBB���쐬
				CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);

				playerCount++;
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}

	// �t�B�[���h�Ƃ̓����蔻�胋�[�v
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// �擪���w��
		pScene = CScene::GetTopAddress(priority);

		// �|�C���^��NULL�łȂ����
		while (pScene)
		{
			// ���ݑΏۂƂ��Ă���C���X�^���X�̎��̃C���X�^���X��ۑ�
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_FIELD)
			{
				// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
				pScene = pSceneNext;
				continue;
			}

			// �t�B�[���h�������
			pField = (CScene2D*)pScene;
			D3DXVECTOR2 pos(pField->GetPos().x, pField->GetPos().y);
			float rot = pField->GetRot().z;
			float width = pField->GetWidth();
			float height = pField->GetHeight();
			CJudge::OBB_INFO fieldOBB;
			// OBB���쐬
			CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);
			
			// �����蔻��
			for (int idx = 0; idx < 4; ++idx)
			{
				if (IsOBB(playerOBB[idx], fieldOBB))
				{
					// �����������̏���
					#ifdef _DEBUG
					CDebugProc::Print("HIT!!\n");
					#endif
				}
			}

			// ���̃C���X�^���X��Ώۂ̃C���X�^���X�ɂ���
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// ��`�Ƌ�`�̂����蔻��
//=========================================================================
bool CJudge::IsOBB(CJudge::OBB_INFO& rectA, CJudge::OBB_INFO& rectB)
{
	// �v�Z�p
	float A = 0.f, B = 0.f, C = 0.f;
	D3DXVECTOR2 culcVecX, culcVecY, baseAxis, V;

	V = rectB.pos - rectA.pos;

	culcVecX = rectB.axisX * rectB.lengthX;
	culcVecY = rectB.axisY * rectB.lengthY;

	// 1����
	baseAxis = rectA.axisX;

	A = rectA.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 2����
	baseAxis = rectA.axisY;

	A = rectA.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 3����
	culcVecX = rectA.axisX * rectA.lengthX;
	culcVecY = rectA.axisY * rectA.lengthY;

	baseAxis = rectB.axisX;

//	V *= -1.f;

	A = rectB.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 4����
	baseAxis = rectB.axisY;

	A = rectB.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	return true;
}

//=========================================================================
// OBB���쐬�֐�
//=========================================================================
void CJudge::CreateOBBInfo(CJudge::OBB_INFO* outOBB, D3DXVECTOR2* pos, float* rot, float* width, float* height)
{
	MATRIX3 trasrationMatrix, rotationMatrix, wMatrix;
	VECTOR2 positon(pos->x, pos->y);

	Matrix3Identity(&wMatrix);
	Matrix3Rotation(&rotationMatrix, *rot);
	Matrix3Translation(&trasrationMatrix, &positon);

	Matrix3Multiply(&wMatrix, &wMatrix, &rotationMatrix);
	Matrix3Multiply(&wMatrix, &wMatrix, &trasrationMatrix);

	outOBB->pos = *pos;
	outOBB->lengthX = *width * 0.5f;
	outOBB->lengthY = *height * 0.5f;

	outOBB->axisX.x = wMatrix.m11;
	outOBB->axisX.y = wMatrix.m12;

	outOBB->axisY.x = wMatrix.m21;
	outOBB->axisY.y = wMatrix.m22;
}

//----EOF----