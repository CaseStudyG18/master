//=============================================================================
//
// CJudgeクラス [CJudge.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CJudge.h"
#include "../MATH/matrix.h"
#include "../MATH/vector.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/CSCENE/CScene2D.h"

//=========================================================================
// フィールドとプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiFieldxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CScene2D *pPlayer[4] = {NULL};	// プレイヤーの最大人数分用意
	CScene2D *pField;
	CJudge::OBB_INFO playerOBB[4];

	// プレイヤー情報取得のループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		int playerCount = 0;

		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_PLAYER)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// プレイヤー情報入れる
			if (playerCount < 4)
			{
				pPlayer[playerCount] = (CScene2D*)pScene;
				D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
				float rot = pPlayer[playerCount]->GetRot().z;
				float width = pPlayer[playerCount]->GetWidth();
				float height = pPlayer[playerCount]->GetHeight();

				// OBB情報作成
				CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);

				playerCount++;
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// フィールドとの当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_FIELD)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pField = (CScene2D*)pScene;
			D3DXVECTOR2 pos(pField->GetPos().x, pField->GetPos().y);
			float rot = pField->GetRot().z;
			float width = pField->GetWidth();
			float height = pField->GetHeight();
			CJudge::OBB_INFO fieldOBB;
			// OBB情報作成
			CreateOBBInfo(&fieldOBB, &pos, &rot, &width, &height);
			
			// 当たり判定
			for (int idx = 0; idx < 4; ++idx)
			{
				if (IsOBB(playerOBB[idx], fieldOBB))
				{
					// 当たった時の処理
					#ifdef _DEBUG
					CDebugProc::Print("HIT!!\n");
					#endif
				}
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// 矩形と矩形のあたり判定
//=========================================================================
bool CJudge::IsOBB(CJudge::OBB_INFO& rectA, CJudge::OBB_INFO& rectB)
{
	// 計算用
	float A = 0.f, B = 0.f, C = 0.f;
	D3DXVECTOR2 culcVecX, culcVecY, baseAxis, V;

	V = rectB.pos - rectA.pos;

	culcVecX = rectB.axisX * rectB.lengthX;
	culcVecY = rectB.axisY * rectB.lengthY;

	// 1軸目
	baseAxis = rectA.axisX;

	A = rectA.lengthX;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 2軸目
	baseAxis = rectA.axisY;

	A = rectA.lengthY;

	B = abs(D3DXVec2Dot(&culcVecX, &baseAxis))
		+ abs(D3DXVec2Dot(&culcVecY, &baseAxis));

	C = abs(D3DXVec2Dot(&V, &baseAxis));

	if (A + B < C)
	{
		return false;
	}

	// 3軸目
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

	// 4軸目
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
// OBB情報作成関数
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