//=============================================================================
//
// CJudgeManagerクラス [CJudgeManager.cpp]
// Author : 野尻　尚希
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "CJudgeManager.h"
#include "CJudge.h"
#include "../SCENE/GAME/PLAYER/CPlayerManager.h"

//=========================================================================
// コンストラクタ
//=========================================================================
CJudgeManager::CJudgeManager(void)
{
	m_pJudge = NULL;
	m_pPlayerManager = NULL;
}

//=========================================================================
// デストラクタ
//=========================================================================
CJudgeManager::~CJudgeManager(void)
{
	m_pJudge = NULL;
	m_pPlayerManager = NULL;
}

//=========================================================================
// クリエイト
//=========================================================================
CJudgeManager* CJudgeManager::Create(CPlayerManager* playerManager)
{
	CJudgeManager* p = new CJudgeManager;
	
	p->Init(playerManager);

	return p;
}

//=========================================================================
// 終了
//=========================================================================
void CJudgeManager::Uninit(void)
{
	if (m_pJudge)
	{
		delete m_pJudge;
		m_pJudge = NULL;
	}
}

//=========================================================================
// 更新
//=========================================================================
void CJudgeManager::Update(void)
{
	m_pJudge->ColiFieldxPlayer();
}

//=========================================================================
// 初期化
//=========================================================================
void CJudgeManager::Init(CPlayerManager* playerManager)
{
	m_pJudge = new CJudge(this);
	m_pPlayerManager = playerManager;
}


//----EOF----