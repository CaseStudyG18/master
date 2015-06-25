//=============================================================================
//
// CPlayerクラス [CPlayer.cpp]
// Author : 佐藤　諒一
//
//=============================================================================
#include "CPlayer.h"

#include "../../../INPUT/CInputKeyboard.h"
#include "../../../TEXTURE/CTexture.h"
#include "../ATTACK/CAttackManager.h"
#include "../THREAD/CThreadManager.h"
#include "../EFFECT/CEffectManager.h"
#include "../TREASURE/CTreasure.h"
#include "../UI/CMp.h"
#include "../../../CONTROLLER/CControllerManager.h"

//-----------------------------------------------------------------------------
// 定数定義
//-----------------------------------------------------------------------------
// プレイヤーの移動速度(仮)
static const float PLAYER_SPEED = 8.0f;

// プレイヤーが鈍足状態になった時の係数(仮)
static const float PLAYER_SLOW_SPEED_COEFFICIENT = 0.4f;

// 宝物アイコンの表示位置
static const D3DXVECTOR3 TREASURE_ICON_POS_BUFF = D3DXVECTOR3(0, -50, 0);

// 1フレーム当たりのMP消費量
static const float MP_COST = 0.5f;
// 1フレーム当たりのMP回復量
static const float MP_REGAIN = 3.0f;


//-----------------------------------------------------------------------------
// コンストラクタ
//	引数　　デバイス、プライオリティ、オブジェクトタイプ
//-----------------------------------------------------------------------------
CPlayer::CPlayer(LPDIRECT3DDEVICE9 *pDevice, int nPriority, OBJTYPE objType) :CAnimation(pDevice, nPriority, objType)
{
	m_pD3DDevice = pDevice;									// デバイスオブジェクト(描画に必要)
	m_pD3DVtxBuff = NULL;									// 頂点座標情報を格納する場所のアドレスを確保する場所
	m_pD3DTexBuff = NULL;									// テクスチャ情報を格納する場所のアドレスを確保する場所
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの中央の位置
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ポリゴンの角度
	m_vRotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの目的の角度
	m_fMoveSpeedX = 0.0f;									// プレイヤーの横軸の移動速度
	m_fMoveSpeedY = 0.0f;									// プレイヤーの縦軸の移動速度
	m_Action = PLAYER_ACTION_NONE;							// プレイヤーの行動
	m_Mode = PLAYER_MODE_NONE;								// プレイヤーの現在の変形状態
	m_ModeDest = PLAYER_MODE_NONE;							// プレイヤーの目的の変形状態
	m_vPosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの中央の位置
	m_vPosDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ポリゴンの中央の位置
	m_fHP = PLAYER_DEFAULT_HP;								// プレイヤーの体力
	m_fMP = PLAYER_DEFAULT_MP;								// プレイヤーの変形用のポイント
	m_bOperation = PLAYER_COMPUTER;							// プレイヤーの操作フラグ
	m_sNumber = 0;											// マネージャーに割り振られるプレイヤー番号
	m_PlayerFacing = PLAYER_DIRECTION_UP;					// プレイヤーの初期向き
	m_PlayerFacingOld = PLAYER_DIRECTION_UP;				// プレイヤーの過去の向き
	m_nRedCount = 0;										// 赤く点滅するためのカウント
	m_bRed = false;											// 赤く点滅しているか
	m_HpState = PLAYER_HP_STATE_NORMAL;						// HPの残り状態
	m_nTextureIndex = 0;									// プレイヤの最初のインデックス
	m_nTextureCount = 0;									// テクスチャを変更するためのカウント

	// MP作る
	m_pMp = new CMp(pDevice, PLAYER_DEFAULT_MP);
	m_pMp->Init();

	m_sAnimTime = 0;										// プレイヤー変形時のアニメーションの時間
	m_sKnockBackTime = 0;									// ノックバック時間
	m_sDownTime = 0;										// ダウン時間
	m_sMatchlessTime = 0;									// 無敵状態の時間
	m_sKnockBackCount = 0;									// やられ状態になった回数
	m_sRushTime = 0;

	m_bMatchless = false;									// 無敵状態かどうか判定
	m_bMetamorphose = false;								// 変形中判定
	m_bSpeedAttack = false;									// 移動形態の攻撃中か判定
	m_bSlowSpeed = false;									// 鈍足フラグの初期設定

	m_pTreasure = NULL;										// 宝物ポインタ

	// シェーダーの初期化
	LPD3DXBUFFER code;
	LPD3DXBUFFER error;
	HRESULT hr = D3DXCompileShaderFromFile(
		"data/SHADER/redPS.hlsl", NULL, NULL, "main",
		"ps_2_0", 0, &code, &error, &m_pPSC);
	if (FAILED(hr)){
		MessageBox(nullptr, (LPSTR)error->GetBufferPointer(), "エラー", 0);
	}

	(*pDevice)->CreatePixelShader((DWORD*)code->GetBufferPointer(), &m_pPS);

}

//-----------------------------------------------------------------------------
// デストラクタ
//	引数　　無し
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	// MP削除
	if (m_pMp){
		m_pMp->Uninit();
	}
	SAFE_DELETE(m_pMp);
}

//-----------------------------------------------------------------------------
// クリエイト
//	引数　　デバイス、座標、幅、高さ、テクスチャの種類、プレイヤー操作（MANUAL or AUTO）,攻撃マネージャー , プレイヤー番号, プレイヤコントロールフラグ
//	戻り値　作成したプレイヤーのポインタ
//-----------------------------------------------------------------------------
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 *pDevice,
	D3DXVECTOR3 pos,
	float fWidth,
	float fHeight,
	TEXTURE_TYPE texture,
	BOOL playerOperation,
	CAttackManager *pAttackManager,
	CThreadManager *pThreadManager,
	CEffectManager *pEffectManager,
	short sPlayerNumber,
	bool *bPlayerControl)
{
	// プレイヤーポインタの作成
	CPlayer *temp = new CPlayer(pDevice);

	// 作成したプレイヤー情報の初期化
	temp->Init(pos, fWidth, fHeight, texture, playerOperation, pAttackManager, pThreadManager, pEffectManager, sPlayerNumber, bPlayerControl);

	// 作成したプレイヤーのポインタを返す
	return temp;
}

//-----------------------------------------------------------------------------
// 初期化
//	引数　　座標、幅、高さ、テクスチャの種類
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Init(D3DXVECTOR3 pos,
	float fWidth,
	float fHeight,
	TEXTURE_TYPE texture,
	BOOL playerOperation,
	CAttackManager *pAttackManager,
	CThreadManager *pThreadManager,
	CEffectManager *pEffectManager,
	short sPlayerNumber,
	bool *bPlayerControl)
{
	// テクスチャアニメーションの初期化
	CAnimation::Init(pos, fWidth, fHeight, texture, 6, 3);

	// 向きを正面のテクスチャに
	SetIndex(5);

	// 操作フラグを変更
	m_bOperation = playerOperation;

	// プレイヤー番号のセット
	m_sNumber = sPlayerNumber;

	// 攻撃マネージャの保持
	m_pAttackManager = pAttackManager;

	// 糸マネージャの保持
	m_pThreadManager = pThreadManager;

	// プレイヤがコントロールできるかフラグ
	m_bPlayerControl = bPlayerControl;
}

//-----------------------------------------------------------------------------
// 終了
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();

	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSC);
}

//-----------------------------------------------------------------------------
// 更新
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// プレイヤがコントロールできないなら更新しない
	if (!(*m_bPlayerControl)){
		return;
	}

	// 宝物を持っていたらアイコンの場所更新
	if (m_pTreasure){
		m_pTreasure->SetPos(m_vPos + TREASURE_ICON_POS_BUFF);
	}

	CScene2D::Update();

	// MP更新
	m_pMp->Update(m_vPos, m_fMP);
	// アニメーション更新
	UpdatePlayerAnimation();
	// 赤く点滅する間隔更新
	UpdatePlayerRed();

	// 移動量を０にする
	m_fMoveSpeedY = 0.0f;
	m_fMoveSpeedX = 0.0f;

	// １フレーム前の座標を専用の変数にセットする
	m_vPosOld = m_vPos;
	m_vPosDest = m_vPos;

	/*----------------------------------------------------------*/
	/*プレイヤーのアクションが変形中で無かった場合のみ他の行動を*/
	/*行うことができる											*/
	/*----------------------------------------------------------*/
	if (m_Action != PLAYER_ACTION_METAMORPHOSE &&
		m_Action != PLAYER_ACTION_KNOCK_BACK &&
		m_Action != PLAYER_ACTION_DOWN &&
		m_Action != PLAYER_ACTION_ATTACK)
	{

		/*----------------------------------------------------------*/
		/*暫定的にプレイヤーの操作方法はキーボードでの操作としておく*/
		/*移動量も暫定的な数値										*/
		/*プレイヤーNo.で移動のキーを変える							*/
		/*----------------------------------------------------------*/

		/*----------------------------------------------------------*/
		/*プレイヤーの移動                                          */
		/*WSADキーでそれぞれ上下左右への移動						*/
		/*アクションの状態を全て歩行状態に変える					*/
		/*----------------------------------------------------------*/
		if (m_sNumber == 0)
		{
			// Wで画面上方向への移動
			if (CInputKeyboard::GetKeyboardPress(DIK_W) ||
				CControllerManager::GetPressKey(CInputGamePad::LEFT_STICK_UP, m_sNumber))
			{
				// 移動速度の変更
				// プレイヤーの形態が速度重視だった場合速く移動する
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedY = -PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedY = -PLAYER_SPEED;
				}

				// 行動を移動状態に変える
				m_Action = PLAYER_ACTION_WALK;

				// プレイヤーの向いている方向を変える
				SetFace(PLAYER_DIRECTION_UP);
			}
			// Sで画面下方向への移動
			else if (CInputKeyboard::GetKeyboardPress(DIK_S) ||
				CControllerManager::GetPressKey(CInputGamePad::LEFT_STICK_DOWN, m_sNumber))
			{
				// 移動速度の変更
				// プレイヤーの形態が速度重視だった場合速く移動する
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedY = PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedY = PLAYER_SPEED;
				}

				// 行動を移動状態に変える
				m_Action = PLAYER_ACTION_WALK;

				// プレイヤーの向いている方向を変える
				SetFace(PLAYER_DIRECTION_DOWN);
			}
			// Aで画面左方向への移動
			if (CInputKeyboard::GetKeyboardPress(DIK_A) ||
				CControllerManager::GetPressKey(CInputGamePad::LEFT_STICK_LEFT, m_sNumber))
			{
				// 移動速度の変更
				// プレイヤーの形態が速度重視だった場合速く移動する
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedX = -PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedX = -PLAYER_SPEED;
				}

				// 行動を移動状態に変える
				m_Action = PLAYER_ACTION_WALK;

				// プレイヤーの向いている方向を変える
				SetFace(PLAYER_DIRECTION_LEFT);
			}
			// Dで画面右方向への移動
			else if (CInputKeyboard::GetKeyboardPress(DIK_D) ||
				CControllerManager::GetPressKey(CInputGamePad::LEFT_STICK_RIGHT, m_sNumber))
			{
				// 移動速度の変更
				// プレイヤーの形態が速度重視だった場合速く移動する
				if (m_Mode == PLAYER_MODE_SPEED)
				{
					m_fMoveSpeedX = PLAYER_SPEED * 1.5f;
				}
				else
				{
					m_fMoveSpeedX = PLAYER_SPEED;
				}

				// 行動を移動状態に変える
				m_Action = PLAYER_ACTION_WALK;

				// プレイヤーの向いている方向を変える
				SetFace(PLAYER_DIRECTION_RIGHT);
			}

			/*----------------------------------------------------------*/
			/*4キーでプレイヤーの攻撃									*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_4) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_R, m_sNumber))
			{
				// アクションの状態を攻撃に変える
				m_Action = PLAYER_ACTION_ATTACK;
			}

			/*----------------------------------------------------------*/
			/*5キーで糸を出す											*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_5) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_L, m_sNumber))
			{
				// アクションの状態を糸発射状態に変える
				m_Action = PLAYER_ACTION_THREAD;
			}

			/*----------------------------------------------------------*/
			/*6キーでプレイヤー変形開始	(Attack)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_6) || 
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_A, m_sNumber))
			{
				if (m_fMP > 0.0f){
					// アクションの状態を変形に変える
					m_Action = PLAYER_ACTION_METAMORPHOSE;
					// 次に変形する状態を変更する
					m_ModeDest = PLAYER_MODE_ATTACK;
				}
			}
			/*----------------------------------------------------------*/
			/*7キーでプレイヤー変形開始	(Speed)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_7) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_X, m_sNumber))
			{
				if (m_fMP > 0.0f){
					// アクションの状態を変形に変える
					m_Action = PLAYER_ACTION_METAMORPHOSE;
					// 次に変形する状態を変更する
					m_ModeDest = PLAYER_MODE_SPEED;
				}
			}
			/*----------------------------------------------------------*/
			/*8キーでプレイヤー変形開始	(Trap)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_8) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_Y, m_sNumber))
			{
				if (m_fMP > 0.0f){
					// アクションの状態を変形に変える
					m_Action = PLAYER_ACTION_METAMORPHOSE;
					// 次に変形する状態を変更する
					m_ModeDest = PLAYER_MODE_TRAP;
				}
			}
			/*----------------------------------------------------------*/
			/*9キーでプレイヤー変形開始	(Normal)						*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_9) ||
				CControllerManager::GetTriggerKey(CInputGamePad::KEY_B, m_sNumber))
			{
				if (m_fMP > 0.0f){
					// アクションの状態を変形に変える
					m_Action = PLAYER_ACTION_METAMORPHOSE;
					// 次に変形する状態を変更する
					m_ModeDest = PLAYER_MODE_NONE;
				}
			}

			// debug
			if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE)){
				// 宝物を落とす
				FallTreasure();
			}
		}
		/*----------------------------------------------------------*/
		/*２体目の敵の動き											*/
		/*----------------------------------------------------------*/
		else if (m_sNumber == 1)
		{
			// Iで画面上方向への移動
			if (CInputKeyboard::GetKeyboardPress(DIK_I))
			{
				m_fMoveSpeedY = -5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_UP);
			}
			// Kで画面下方向への移動
			else if (CInputKeyboard::GetKeyboardPress(DIK_K))
			{
				m_fMoveSpeedY = 5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_DOWN);
			}
			// Jで画面左方向への移動
			if (CInputKeyboard::GetKeyboardPress(DIK_J))
			{
				m_fMoveSpeedX = -5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_LEFT);
			}
			// Lで画面右方向への移動
			else if (CInputKeyboard::GetKeyboardPress(DIK_L))
			{
				m_fMoveSpeedX = 5.0f;
				m_Action = PLAYER_ACTION_WALK;
				SetFace(PLAYER_DIRECTION_RIGHT);
			}

			/*----------------------------------------------------------*/
			/*0キーでプレイヤーの攻撃									*/
			/*----------------------------------------------------------*/
			if (CInputKeyboard::GetKeyboardTrigger(DIK_0))
			{
				// アクションの状態を攻撃に変える
				m_Action = PLAYER_ACTION_ATTACK;
			}
		}


		/*----------------------------------------------------------*/
		/*ここまでのものを最終的にはコントローラーで操作			*/
		/*----------------------------------------------------------*/
	}

	// 無敵状態での処理
	if (m_bMatchless)
	{
		Matchless();
	}

	// 変形している場合MPを減少させていく
	if (m_bMetamorphose)
	{
		MPReduce();

		// MPが０になったら通常状態に戻す
		if (m_fMP <= 0.0f)
		{
			m_ModeDest = PLAYER_MODE_NONE;

			m_Action = PLAYER_ACTION_METAMORPHOSE;

			m_fMP = 0.0f;
		}
	}
	else
	{
		// デフォルトMPまで回復させる
		if (m_fMP < PLAYER_DEFAULT_MP)
		{
			m_fMP += MP_REGAIN;

			// 押し戻し処理追加
			if (m_fMP > PLAYER_DEFAULT_MP){
				m_fMP = PLAYER_DEFAULT_MP;
			}
		}
	}

	/*----------------------------------------------------------*/
	/*移動形態での攻撃中だった場合アクションをNONEに変えて		*/
	/*移動形態の攻撃のエフェクトに合わせて移動する				*/
	/*----------------------------------------------------------*/
	if (m_bSpeedAttack)
	{
		Rush();
		m_Action = PLAYER_ACTION_NONE;
		return;
	}

	/*----------------------------------------------------------*/
	/*プレイヤーのアクションの状態に対応した行動を取らせる		*/
	/*歩行、攻撃、糸発射、変形									*/
	/*----------------------------------------------------------*/
	switch (m_Action)
	{
		// プレイヤーのアクションが歩行だった際の動き
	case PLAYER_ACTION_WALK:
		Move();
		break;

		// プレイヤーのアクションが攻撃だった際の動き
	case PLAYER_ACTION_ATTACK:
		Attack();
		break;

		// プレイヤーのアクションが変形だった際の動き
	case PLAYER_ACTION_METAMORPHOSE:
		// 変形先と変形元が違ったら変形開始
		if (m_Mode != m_ModeDest)
		{
			Metamorphose();
		}
		// 変形予定先と現在の形態が同じだったら行動を戻す
		else
		{
			m_Action = PLAYER_ACTION_NONE;
		}
		break;

		// プレイヤーのアクションが糸発射だった際の動き
	case PLAYER_ACTION_THREAD:
		SpidersThread();
		break;

		// プレイヤーのアクションがやられだった際
	case PLAYER_ACTION_KNOCK_BACK:
		KnockBack();
		break;

		// プレイヤーの状態がダウンだった際
	case PLAYER_ACTION_DOWN:
		PlayerDown();
		break;

	default:
		break;
	}

	// 現在のMPを表示する
#ifdef _DEBUG
	CDebugProc::Print("%dプレイヤー残りMP %f\n", m_sNumber, m_fMP);
#endif

}

//-----------------------------------------------------------------------------
// 描画
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	// 赤くするフラグ
	if (m_bRed){
		// ピクセルシェーダーのセット
		(*m_pD3DDevice)->SetPixelShader(m_pPS);
	}

	CScene2D::Draw();

	// ピクセルシェーダーNULLセット
	(*m_pD3DDevice)->SetPixelShader(nullptr);
}

//-----------------------------------------------------------------------------
// プレイヤーの移動
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Move(void)
{
	// 現在の座標を更新する
	if (!m_bSlowSpeed)
	{
		m_vPos.x += m_fMoveSpeedX * 0.5f;
		m_vPos.y += m_fMoveSpeedY * 0.5f;
	}
	else
	{
		m_vPos.x += m_fMoveSpeedX * 0.5f * PLAYER_SLOW_SPEED_COEFFICIENT;
		m_vPos.y += m_fMoveSpeedY * 0.5f * PLAYER_SLOW_SPEED_COEFFICIENT;
	}

	// プレイヤーの移動方向が変わったらテクスチャのU値を変える
	if ((m_PlayerFacing == PLAYER_DIRECTION_LEFT || m_PlayerFacing == PLAYER_DIRECTION_RIGHT) &&
		m_PlayerFacing != m_PlayerFacingOld)
	{
		ChangeTextureFaceU();
		m_PlayerFacingOld = m_PlayerFacing;
	}

	m_Action = PLAYER_ACTION_NONE;
}

//-----------------------------------------------------------------------------
// 攻撃マネージャを呼び出す関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Attack(void)
{
	switch (m_Mode)
	{
		// 普通攻撃
	case PLAYER_MODE_NONE:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_NORMAL,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
		// 攻撃特化の攻撃
	case PLAYER_MODE_ATTACK:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_ATTACK,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
		// 移動特化の攻撃
	case PLAYER_MODE_SPEED:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_SPEED,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		m_bSpeedAttack = true;
		break;
		// 罠型の攻撃
	case PLAYER_MODE_TRAP:
		m_pAttackManager->CreateAttack(
			ATTACK_TYPE_TRAP,
			m_sNumber,
			m_vPos,
			PLAYER_DIRECTION_VECTOR[m_PlayerFacing]);
		break;
	default:
		break;
	}

	// アクションの状態を戻す
	m_Action = PLAYER_ACTION_NONE;

}

//-----------------------------------------------------------------------------
// 変形
//	引数　　変形する状態
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Metamorphose(void)
{
	MetamorphoseAnimation();
}

//-----------------------------------------------------------------------------
// 変形アニメーション
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::MetamorphoseAnimation(void)
{
	// アニメーションの時間の増加
	m_sAnimTime++;

	if (m_sAnimTime > 60)
	{
		switch (m_ModeDest)
		{
			// 攻撃形態だったら赤くする
		case PLAYER_MODE_ATTACK:
			// 色変更
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			// フラグ変更
			m_bMetamorphose = true;
			break;
			// 移動形態だったら青くする
		case PLAYER_MODE_SPEED:
			// 色変更
			CScene2D::SetColorPolygon(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			// フラグ変更
			m_bMetamorphose = true;
			break;
			// 罠形態だったら緑にする
		case PLAYER_MODE_TRAP:
			// 色変更
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			// フラグ変更
			m_bMetamorphose = true;
			break;
			// 通常状態だったら通常の色にする
		case PLAYER_MODE_NONE:
			// 色変更
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			// フラグ変更
			m_bMetamorphose = false;
			break;
		default:
			break;
		}

		// アニメーション時間をリセットしておく
		m_sAnimTime = 0;

		// 目的の形態を現在の形態にする
		m_Mode = m_ModeDest;

		// プレイヤーの行動を初期状態にもどす
		m_Action = PLAYER_ACTION_NONE;
	}
}

//-----------------------------------------------------------------------------
// MPを減少させる関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::MPReduce(void)
{
	// MPを減らしていく
	m_fMP -= MP_COST;
}

//-----------------------------------------------------------------------------
// 糸マネージャを呼び出す関数
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SpidersThread(void)
{
	switch (m_Mode)
	{
		// 通常状態の糸
	case PLAYER_MODE_NONE:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_NORMAL,
			m_sNumber,
			m_vPos,
			m_PlayerFacing,
			m_pEffectManager);
		break;
		// 攻撃特化形態の糸
	case PLAYER_MODE_ATTACK:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_ATTACK,
			m_sNumber,
			m_vPos,
			m_PlayerFacing,
			m_pEffectManager);
		break;
		// 移動特化形態の糸
	case PLAYER_MODE_SPEED:
	{
		D3DXVECTOR3 pos = m_vPos;
		pos.y += m_fHeight *0.5f;
		m_pThreadManager->CreateThread(
			THREAD_TYPE_SPEED,
			m_sNumber,
			pos,
			m_PlayerFacing,
			m_pEffectManager);
		break;
	}
		// 罠特化形態の糸
	case PLAYER_MODE_TRAP:
		m_pThreadManager->CreateThread(
			THREAD_TYPE_TRAP,
			m_sNumber,
			m_vPos,
			m_PlayerFacing,
			m_pEffectManager);
		break;
	default:
		break;
	}

	// 行動を初期状態に戻す
	m_Action = PLAYER_ACTION_NONE;
}

//-----------------------------------------------------------------------------
// やられ状態での処理
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::KnockBack(void)
{
	m_sKnockBackTime++;

	// 一定回数やられ状態になったらダウン状態に移行
	if (m_sKnockBackCount > 3)
	{
		SetPlayerDown();
		m_sKnockBackCount = 0;
	}

	// 一定時間経過したら初期状態にもどす
	else if (m_sKnockBackTime > 100)
	{
		// アクションの状態を初期状態に戻す
		m_Action = PLAYER_ACTION_NONE;

		// やられになった回数を増やす
		m_sKnockBackCount++;

		// やられ時間の初期化
		m_sKnockBackTime = 0;

		// 色変更
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//-----------------------------------------------------------------------------
// ダウン状態での処理
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::PlayerDown(void)
{
	m_sDownTime++;

	if (m_sDownTime > 300)
	{
		m_Action = PLAYER_ACTION_NONE;

		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_bMatchless = true;

		m_sDownTime = 0;
	}
}

//-----------------------------------------------------------------------------
// 無敵状態の際の動作
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::Matchless(void)
{
	m_sMatchlessTime++;

	// 色変更
	CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)(m_sMatchlessTime % 10) / 10.0f));

	// 一定時間経過したら初期状態へ移行
	if (m_sMatchlessTime > 500)
	{
		// 無敵状態フラグを消す
		m_bMatchless = false;

		// 色変更
		CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// 無敵時間の初期化
		m_sMatchlessTime = 0;
	}
}

//-----------------------------------------------------------------------------
// プレイヤーの状態をやられ状態に移行既にやられ状態だった場合はダウン状態へ
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SetPlyerKnockBack(void)
{
	// 無敵状態でなければこの処理を行う
	if (m_bMatchless == false)
	{
		// 既にやられ状態だった場合にはダウン状態へ
		if (m_Action == PLAYER_ACTION_KNOCK_BACK)
		{
			m_Action = PLAYER_ACTION_DOWN;
		}
		// ダウン状態または変形状態では無かった場合はやられ状態へ
		else if (m_Action != PLAYER_ACTION_DOWN && m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_KNOCK_BACK;

			// 色変更
			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f));

			// 宝を持っていたら落とす
			if (m_pTreasure)
			{
				FallTreasure();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// プレイヤーの状態をダウン状態へ移行
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::SetPlayerDown(void)
{
	// 無敵状態でなければこの処理を行う
	if (m_bMatchless == false)
	{
		if (m_Action != PLAYER_ACTION_METAMORPHOSE)
		{
			m_Action = PLAYER_ACTION_DOWN;

			CScene2D::SetColorPolygon(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));

			// 宝を持っていたら落とす
			if (m_pTreasure)
			{
				FallTreasure();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// テクスチャのUV切り替えの処理(U値の切り替え)
//	引数　　無し
//	戻り値　無し
//-----------------------------------------------------------------------------
void CPlayer::ChangeTextureFaceU(void)
{
	VERTEX_2D *pVtx;
	float fTemp;			// 仮変数

	// ポリゴンの設定
	m_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの上部分のU値の入れ替え
	fTemp = pVtx[0].tex.x;
	pVtx[0].tex.x = pVtx[1].tex.x;
	pVtx[1].tex.x = fTemp;

	// テクスチャの下部分のU値の入れ替え
	fTemp = pVtx[2].tex.x;
	pVtx[2].tex.x = pVtx[3].tex.x;
	pVtx[3].tex.x = fTemp;

	m_pD3DVtxBuff->Unlock();
}

//-----------------------------------------------------------------------------
// プレイヤに宝物を入れて、アイコンにする
// プレイヤが宝物を取った時によばれる
//-----------------------------------------------------------------------------
void CPlayer::SetTreasure(CTreasure *pTreasure){
	// ポインタ保存
	m_pTreasure = pTreasure;

	// 宝物をアイコンにする
	m_pTreasure->SetIcon();
}

//-----------------------------------------------------------------------------
// 宝物の表示を戻して、宝物の保持ポインタをNULL
// プレイヤが宝物を落としたときに呼ばれる
//-----------------------------------------------------------------------------
void CPlayer::FallTreasure(){

	if (m_pTreasure){
		// 宝物のテクスチャや大きさをセット
		m_pTreasure->Reset(m_vPos);

		// ポインタ削除
		m_pTreasure = NULL;
	}

}


//-----------------------------------------------------------------------------
// プレイヤの向きをセットする関数
//-----------------------------------------------------------------------------
void CPlayer::SetFace(DIRECTION_PLAYER_FACING value){

	// 同じ値は連続でセットしない
	if (m_PlayerFacing == value){
		return;
	}

	// プレイヤの向きステート変更
	m_PlayerFacing = value;

	// プレイヤの向きに対応したテクスチャをセット
	m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[value];

	// プレイヤのテクスチャアニメーション用カウントをリセット
	m_nTextureCount = 0;
}

//-----------------------------------------------------------------------------
// プレイヤの向きによってテクスチャ番号処理
// 更新で一回呼ぶ
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerAnimation(void){

	if (m_PlayerFacing == PLAYER_DIRECTION_UP){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_UP]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_UP];
			}
		}
		SetIndex(m_nTextureIndex);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_DOWN){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_DOWN]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_DOWN];
			}
		}
		SetIndex(m_nTextureIndex);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_RIGHT){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_RIGHT]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_RIGHT];
			}
		}
		SetIndex(m_nTextureIndex, true);
	}
	else if (m_PlayerFacing == PLAYER_DIRECTION_LEFT){
		m_nTextureCount++;
		if (m_nTextureCount > 10){
			m_nTextureCount = 0;

			m_nTextureIndex++;
			if (m_nTextureIndex > PLAYER_TEXTURE_INDEX_MAX[PLAYER_DIRECTION_LEFT]){
				m_nTextureIndex = PLAYER_TEXTURE_INDEX_MIN[PLAYER_DIRECTION_LEFT];
			}
		}
		SetIndex(m_nTextureIndex, false);
	}

}

//-----------------------------------------------------------------------------
// 移動形態での攻撃でのタックル移動
//-----------------------------------------------------------------------------
void CPlayer::Rush(void)
{
	m_vPos += PLAYER_DIRECTION_VECTOR[m_PlayerFacing] * 4.0f;

	m_sRushTime++;

	if (m_sRushTime > 50)
	{
		m_bSpeedAttack = false;
		m_sRushTime = 0;
	}
}

//-----------------------------------------------------------------------------
// 体力セッター
//-----------------------------------------------------------------------------
void CPlayer::AddHp(float fPoint){

	// HP合計
	m_fHP += fPoint;

	// クランプ
	if (m_fHP > PLAYER_DEFAULT_HP){
		m_fHP = PLAYER_DEFAULT_HP;
	}
	else if (m_fHP < 0.0f){
		m_fHP = 0.0f;
	}

	// HP残り状態を更新
	UpdatePlayerHpState();
}

//-----------------------------------------------------------------------------
// プレイヤのHP残り状態を管理　更新で一回呼ぶ　もしくわHP変更のあったときのみでOK 
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerHpState(void){

	// 現在のHPの割合
	float ratio = m_fHP / PLAYER_DEFAULT_HP;

	// 割合に応じてステートを変更
	for (int i = 0; i < PLAYER_HP_STATE_MAX; i++){
		if (ratio >= PLAYER_HP_STATE_RATIO[i]){
			m_HpState = (PLAYER_HP_STATE)i;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// プレイヤのHP残り状態で赤く点滅する管理　更新で一回呼ぶ
//-----------------------------------------------------------------------------
void CPlayer::UpdatePlayerRed(void){

#ifdef _DEBUG
	if (CInputKeyboard::GetKeyboardPress(DIK_RCONTROL)){
		AddHp(-2);
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RSHIFT)){
		AddHp(PLAYER_DEFAULT_HP);
	}
	if (m_HpState == PLAYER_HP_STATE_NORMAL){
		CDebugProc::Print("プレイヤHP状態 = NORMAL\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_LOW){
		CDebugProc::Print("プレイヤHP状態 = LOW\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_VERY_LOW){
		CDebugProc::Print("プレイヤHP状態 = VERY_LOW\n");
	}
	else if (m_HpState == PLAYER_HP_STATE_DIE){
		CDebugProc::Print("プレイヤHP状態 = DIE\n");
	}
	CDebugProc::Print("count = %d\n", m_nRedCount);
#endif

	// 死んでいる or HPが十分あるなら更新しない
	if (m_HpState == PLAYER_HP_STATE_DIE ||
		m_HpState == PLAYER_HP_STATE_NORMAL){
		m_nRedCount = 0;
		m_bRed = false;
		return;
	}

	// 赤く点滅する間隔でフラグ切り替え
	m_nRedCount++;
	if (m_nRedCount > PLAYER_HP_STATE_FLASH_INTERVAL[m_HpState]){
		m_bRed = !m_bRed;
		m_nRedCount = 0;
	}
}
// EOF