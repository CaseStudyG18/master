//=============================================================================
//
// �e�N�X�`���[���� [CTexture.h]
// Author : ��K�@����
//
//=============================================================================
#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "../MAIN/main.h"

//*****************************************************************************
// �񋓑�
//*****************************************************************************
// �e�N�X�`���̎��
typedef enum
{
	TEXTURE_NULL = 0,			// �Ȃ�
	TEXTURE_BG_0,
	TEXTURE_BG_1,
	TEXTURE_FIRE_0,
	TEXTURE_FIRE_1,
	TEXTURE_THREAD,
	TEXTURE_PLAYER,
	TEXTURE_NUMBER,
	TEXTURE_TREASURE,
	TEXTURE_TREASURE_ICON,
	TEXTURE_BG_TITLE,
	TEXTURE_BG_RESULT,
	TEXTURE_WIN_LOGO,
	TEXTURE_DRAW_LOGO,
	TEXTURE_FIELD_01,	// ����
	TEXTURE_FIELD_02,	// �󂨂��Ă���
	TEXTURE_FIELD_03,	// �X�^�[�g���S�[��
	TEXTURE_SPAWN_YELLOW,
	TEXTURE_SPAWN_RED,
	TEXTURE_SPAWN_BLUE,
	TEXTURE_SPAWN_GREEN,
	TEXTURE_HIT_SMALL,
	TEXTURE_HIT_BIG,
	TEXTURE_ATTACK_AROUND,
	TEXTURE_ATTACK_TACKLE,
	TEXTURE_SLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_YELLOW,
	TEXTURE_PLAYER_ETAMORPHOSE_RED,
	TEXTURE_PLAYER_ETAMORPHOSE_BLUE,
	TEXTURE_PLAYER_ETAMORPHOSE_GREEN,
	TEXTURE_TREASURE_KIRAKIRA,
	TEXTURE_TREASURE_LIGHT,
	
	//**�ȉ�������***
	TEXTURE_OPTION_MENU,	// �I�v�V�������j���[
	TEXTURE_OPTION_BG,		// �I�v�V����BG
	TEXTURE_CURSOL,			// �J�[�\��
	TEXTURE_RETURN,			// �߂�
	TEXTURE_PLAYER_NUM,		// �v���C���[�ԍ�
	//***************
	TEXTURE_MAX					// �e�N�X�`���^�C�v����
}TEXTURE_TYPE;

//*****************************************************************************
// �N���X�錾
//*****************************************************************************
class CTexture
{
public:
	// �R���X�g���N�^
	CTexture(void){};

	// �f�X�g���N�^
	~CTexture(void){};

	// �I��
	static void Uninit(void);

	//============================================================
	// �N���G�C�g
	// ����:�f�o�C�X
	//============================================================
	static void CreateTexture(LPDIRECT3DDEVICE9 *pDevice);

	//============================================================
	// �e�N�X�`���Q�b�^�[
	// ����:�e�N�X�`���̎��
	// �߂�l:�e�N�X�`���̃|�C���^
	//============================================================
	static LPDIRECT3DTEXTURE9 GetTexture(const TEXTURE_TYPE type);

	//============================================================
	// �e�N�X�`���^�C�v�Q�b�^�[
	// ����:�e�N�X�`���̃t�@�C���p�X
	// �߂�l:�e�N�X�`���^�C�v
	//============================================================
	static TEXTURE_TYPE GetTextureTye(char* texFilePath);

private:
	//============================================================
	// ������
	// ����:�f�o�C�X
	// �߂�l:����
	//============================================================
	static	HRESULT Init(LPDIRECT3DDEVICE9 *pDevice);

	static	LPDIRECT3DTEXTURE9 m_pD3DTex[TEXTURE_MAX];				// �e�N�X�`���|�C���^
	static	bool m_bDoneCreateFlag;									// ���łɍ�������̃t���O
};

#endif
//----EOF----