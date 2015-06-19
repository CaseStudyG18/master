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
#include "CJudgeManager.h"
#include "../MATH/matrix.h"
#include "../MATH/vector.h"
#include "../MATH/math.h"
#include "../SCENE/CSCENE/CScene.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../SCENE/GAME/PLAYER/CPlayer.h"
#include "../SCENE/CSCENE/CScene2D.h"
#include "../SCENE/GAME/TREASURE/CTreasure.h"
#include "../SCENE/GAME/ATTACK/CAttackBase.h"
#include "../SCENE/GAME/THREAD/CThreadBase.h"
#include "../SCENE/GAME/GOAL/CGoal.h"
#include "../SCENE/GAME/GOAL/CGoalManager.h"

//=========================================================================
// コンストラクタ
//=========================================================================
CJudge::CJudge(CJudgeManager* pJudgeManager)
{
	m_pJudgeManager = pJudgeManager;

	for (int idx = 0; idx < MAXIMUM_NUMBER_OF_PLAYER; ++idx)
	{
		m_LastFieldColiPlayer[idx] = NULL;
	}
}

//=========================================================================
// デストラクタ
//=========================================================================
CJudge::~CJudge()
{
	m_pJudgeManager = NULL;

	for (int idx = 0; idx < MAXIMUM_NUMBER_OF_PLAYER; ++idx)
	{
		m_LastFieldColiPlayer[idx] = NULL;
	}
}

//=========================================================================
// フィールドとプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiFieldxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CScene2D *pField;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.5f;
		float height = pPlayer[playerCount]->GetHeight() * 0.25f;

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
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
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], fieldOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;

					// 最後に当たった場所更新
					m_LastFieldColiPlayer[idx] = pField;

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

	// フィールド外に居るやつの座標変更
	for (int idx = 0; idx < playerNum; ++idx)
	{
		// 当たってるやつは乗ってるので無視
		if (coli[idx])
		{
			continue;
		}

		// NULLチェック
		if (!pPlayer[idx])
		{
			continue;
		}
		if (!m_LastFieldColiPlayer[idx])
		{
			continue;
		}

		D3DXVECTOR2 playerPos, fieldPos, vertexPosA, vertexPosB, vertexPosC, vertexPosD, hitPos;
		playerPos = (D3DXVECTOR2)pPlayer[idx]->GetPos();
		playerPos.y += pPlayer[idx]->GetHeight() * 0.25f;
		if (m_LastFieldColiPlayer[idx])
		{
			fieldPos = (D3DXVECTOR2)m_LastFieldColiPlayer[idx]->GetPos();
		}
		else
		{
			fieldPos = playerPos;
		}

		// コの字なので順番注意
		vertexPosA = *m_LastFieldColiPlayer[idx]->GetVertexPos(0);
		vertexPosB = *m_LastFieldColiPlayer[idx]->GetVertexPos(1);
		vertexPosC = *m_LastFieldColiPlayer[idx]->GetVertexPos(3);
		vertexPosD = *m_LastFieldColiPlayer[idx]->GetVertexPos(2);
		
		Segment playerSegment, vertexSegment;
		playerSegment.s = fieldPos;
		playerSegment.v = playerPos - fieldPos;
		vertexSegment.s = vertexPosA;
		vertexSegment.v = vertexPosB - vertexPosA;

		if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
		{
			vertexSegment.s = vertexPosB;
			vertexSegment.v = vertexPosC - vertexPosB;
			if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
			{
				vertexSegment.s = vertexPosC;
				vertexSegment.v = vertexPosD - vertexPosC;
				if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
				{
					vertexSegment.s = vertexPosD;
					vertexSegment.v = vertexPosA - vertexPosD;
					if (!ColiRayxRay(playerSegment, vertexSegment, &hitPos))
					{
						return;
					}
				}
			}
		}
		pPlayer[idx]->SetPos(D3DXVECTOR3(hitPos.x, hitPos.y - pPlayer[idx]->GetHeight() * 0.25f, 0.f));
	}

}

//=========================================================================
// 攻撃とプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiAttackxPlayer(void){

	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CScene2D *pAttack;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight();
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth();
		float height = pPlayer[playerCount]->GetHeight();

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 攻撃との当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_ATTACK)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pAttack = (CScene2D*)pScene;
			CAttackBase* pAttackBase = (CAttackBase*)pScene;
			D3DXVECTOR2 pos(pAttack->GetPos().x, pAttack->GetPos().y);
			float rot = pAttack->GetRot().z;
			float width = pAttack->GetWidth();
			float height = pAttack->GetHeight();
			CJudge::OBB_INFO attackOBB;
			// OBB情報作成
			CreateOBBInfo(&attackOBB, &pos, &rot, &width, &height);

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (idx == (int)pAttackBase->GetPlayerNumber())
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], attackOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;

					// 当たった時の処理
					// これでいいのかな？
					pPlayer[idx]->SetPlyerKnockBack();
					pPlayer[idx]->AddHp(-1);
#ifdef _DEBUG
					CDebugProc::Print("攻撃ヒット\n");
#endif
				}
			}

			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}
}

//=========================================================================
// フィールドと足場になる糸のあたり判定
//=========================================================================
void CJudge::ColiFieldxThreadOfFoothold(void)
{
	CScene *pSceneThread = CScene::GetTopAddress(TYPE_PRIORITY_THREAD_OF_FOOTHOLD);
	CScene *pSceneNextThread;
	CScene *pSceneField;
	CScene *pSceneNextField;
	CScene2D *pThread;
	CScene2D *pField;
	CJudge::OBB_INFO threadOBB, fieldOBB;
	bool hit = false;


	// 判定すべきものがないなら
	if (!pSceneThread)
	{
		return;
	}

	// 糸ループ
	while (pSceneThread)
	{
		// 現在対象としているインスタンスの次のインスタンスを保存
		pSceneNextThread = pSceneThread->GetNextAddress();

		// 足場になってないやつなら
		if (pSceneThread->GetObjType() != CScene::OBJTYPE_FIELD)
		{
			// 糸情報取得
			pThread = (CScene2D*)pSceneThread;
			D3DXVECTOR2 pos(pThread->GetPos().x, pThread->GetPos().y);
			float rot = pThread->GetRot().z;
			float width = pThread->GetWidth();
			float height = pThread->GetHeight();
			CThreadBase* threadBase = (CThreadBase*)pSceneThread;
			int playerNum = threadBase->GetPlayerNum();

			// OBB情報作成
			CreateOBBInfo(&threadOBB, &pos, &rot, &width, &height);

			for (int prioryty = TYPE_PRIORITY_0; prioryty < TYPE_PRIORITY_MAX; ++prioryty)
			{
				pSceneField = CScene::GetTopAddress(prioryty);

				// フィールドとの当たり判定ループ
				while (pSceneField)
				{
					// 現在対象としているインスタンスの次のインスタンスを保存
					pSceneNextField = pSceneField->GetNextAddress();

					if (pSceneField->GetObjType() == CScene::OBJTYPE_FIELD)
					{
						// フィールド情報取得
						pField = (CScene2D*)pSceneField;
						D3DXVECTOR2 pos2(pField->GetPos().x, pField->GetPos().y);
						float rot2 = pField->GetRot().z;
						float width2 = pField->GetWidth();
						float height2 = pField->GetHeight();

						// 自分が乗ってる床以外ならやらない
						CScene2D* lastField = m_LastFieldColiPlayer[playerNum];
						if (lastField)
						{
							if (lastField == pField)
							{
								// 次のインスタンスを対象のインスタンスにする
								pSceneField = pSceneNextField;
								continue;
							}
						}

						// OBB情報作成
						CreateOBBInfo(&fieldOBB, &pos2, &rot2, &width2, &height2);

						// 当たり判定
						if (IsOBB(threadOBB, fieldOBB))
						{
							// フィールドにタイプ変更
							pThread->SetObjType(CScene::OBJTYPE_FIELD);

							// 足場になるアニメーションはじめ
							//
							//
							//

							hit = true;
							break;
						}
					}

					// 次のインスタンスを対象のインスタンスにする
					pSceneField = pSceneNextField;
				}

				// 既に調べている糸が当たってたら次の糸へ
				if (hit)
				{
					hit = false;
					break;
				}
			}
		}

		// 次のインスタンスを対象のインスタンスにする
		pSceneThread = pSceneNextThread;
	}
}

//=========================================================================
// 宝箱とプレイヤーのあたり判定
//=========================================================================
void CJudge::ColiTreasurexPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CTreasure *pTreasure;
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.5f;
		float height = pPlayer[playerCount]->GetHeight() * 0.25f;

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
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

			if (pScene->GetObjType() != CScene::OBJTYPE_TREASURE)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pTreasure = (CTreasure*)pScene;
			D3DXVECTOR2 pos(pTreasure->GetPos().x, pTreasure->GetPos().y);
			float rot = pTreasure->GetRot().z;
			float width = pTreasure->GetWidth();
			float height = pTreasure->GetHeight();
			CJudge::OBB_INFO treasureOBB;
			// OBB情報作成
			CreateOBBInfo(&treasureOBB, &pos, &rot, &width, &height);

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], treasureOBB))
				{
					// ヒットフラグオン
					coli[idx] = true;
#ifdef _DEBUG
					CDebugProc::Print("TREASURE x PLAYER!!\n");
#endif
					break;
				}
			}
			// 次のインスタンスを対象のインスタンスにする
			//			pScene = pSceneNext;

			// お宝は一つだから抜ける
			break;
		}
	}

	// 当たり判定なんとなく見やすいかなと思ってここに分けた
	if(pTreasure->GetTreasureState() != TREASURE_STATE_OWNED)
		for (int idx = 0; idx < playerNum; ++idx){
			if (coli[idx]){
				// プレイヤにお宝を渡す
				pPlayer[idx]->SetTreasure(pTreasure);
				pTreasure->SetTreasureState(TREASURE_STATE_OWNED);
				break;
			}
		}

}

//=========================================================================
// プレイヤーとゴールのあたり判定
//=========================================================================
void CJudge::ColiGoalxPlayer(void)
{
	CScene *pScene;
	CScene *pSceneNext;
	CPlayer *pPlayer[MAXIMUM_NUMBER_OF_PLAYER] = { NULL };	// プレイヤーの最大人数分用意
	CGoal *pGoal = NULL;
	CGoal *pGoalHit = NULL;
	int nHitGoalPlayerNum;
	CGoal *pGoalAll[GOAL_MAX] = { NULL };
	CJudge::OBB_INFO playerOBB[MAXIMUM_NUMBER_OF_PLAYER];
	int playerNum = 0;
	bool coli[MAXIMUM_NUMBER_OF_PLAYER] = { false };

	// プレイヤー情報入れる
	CPlayerManager* playerManager = m_pJudgeManager->GetPlayerManager();

	for (int playerCount = 0; playerCount < MAXIMUM_NUMBER_OF_PLAYER; ++playerCount)
	{
		pPlayer[playerCount] = playerManager->GetPlayer(playerCount);
		if (!pPlayer[playerCount])
		{
			continue;
		}
		D3DXVECTOR2 pos(pPlayer[playerCount]->GetPos().x, pPlayer[playerCount]->GetPos().y);
		pos.y += pPlayer[playerCount]->GetHeight() * 0.25f;
		float rot = pPlayer[playerCount]->GetRot().z;
		float width = pPlayer[playerCount]->GetWidth() * 0.5f;
		float height = pPlayer[playerCount]->GetHeight() * 0.25f;

		// OBB情報作成
		CreateOBBInfo(&playerOBB[playerCount], &pos, &rot, &width, &height);
		playerNum++;
	}

	// 当たり判定ループ
	for (int priority = 0; priority < TYPE_PRIORITY_MAX; priority++)
	{
		// 先頭を指定
		pScene = CScene::GetTopAddress(priority);

		// ポインタがNULLでなければ
		while (pScene)
		{
			// 現在対象としているインスタンスの次のインスタンスを保存
			pSceneNext = pScene->GetNextAddress();

			if (pScene->GetObjType() != CScene::OBJTYPE_GOAL)
			{
				// 次のインスタンスを対象のインスタンスにする
				pScene = pSceneNext;
				continue;
			}

			// フィールド情報入れる
			pGoal = (CGoal*)pScene;
			D3DXVECTOR2 pos(pGoal->GetPos().x, pGoal->GetPos().y);

			float rot = pGoal->GetRot().z;
			float width = pGoal->GetWidth();
			float height = pGoal->GetHeight();
			CJudge::OBB_INFO goalOBB;
			// OBB情報作成
			CreateOBBInfo(&goalOBB, &pos, &rot, &width, &height);

			// プレイヤ番号を配列番号としてゴールのポインタを格納
			pGoalAll[pGoal->GetPlayerNum()] = pGoal;

			// 当たり判定
			for (int idx = 0; idx < playerNum; ++idx)
			{
				// 宝物を持っていなかったら次へ
				if (pPlayer[idx]->GetTreasure() == NULL){
					continue;
				}
				
				// すでにあたってるなら判定しない
				if (coli[idx])
				{
					continue;
				}

				if (IsOBB(playerOBB[idx], goalOBB))
				{
					// 当たったゴールのインスタンスを保存
					pGoalHit = pGoal;
					nHitGoalPlayerNum = pGoal->GetPlayerNum();

					// ヒットフラグオン
					coli[idx] = true;
				}
			}
			// 次のインスタンスを対象のインスタンスにする
			pScene = pSceneNext;
		}
	}

	// 当たり判定チェック
	for (int idx = 0; idx < playerNum; ++idx){
		if (coli[idx]){

			// プレイヤに対応したゴールじゃなければさよなら
			if (nHitGoalPlayerNum != pPlayer[idx]->GetPlayerNum()){
				break;
			}
			pGoalHit->SetTrans();
		}
		else{
			for (int i = 0; i < GOAL_MAX; i++){
				if (i != nHitGoalPlayerNum){
					pGoalAll[i]->SetNoTrans();
				}
			}
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

//=========================================================================
// 線分と線分のあたり判定
//=========================================================================
bool CJudge::ColiRayxRay(Segment &seg1, Segment &seg2, D3DXVECTOR2* outPos)
{
	D3DXVECTOR2 v = seg2.s - seg1.s;
	float Crs_v1_v2 = D3DXVec2Cross(&seg1.v, &seg2.v);
	if (Crs_v1_v2 == 0.0f)
	{
		// 平行状態
		return false;
	}

	float Crs_v_v1 = D3DXVec2Cross(&v, &seg1.v);
	float Crs_v_v2 = D3DXVec2Cross(&v, &seg2.v);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	// 容認誤差
	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1)
	{
		// 交差していない
		return false;
	}

	if (outPos)
	{
		*outPos = seg1.s + seg1.v * t1;
#ifdef _DEBUG
		CDebugProc::Print("X:%f\nY:%f\n", outPos->x, outPos->y);
#endif
	}

	return true;
}


//----EOF----