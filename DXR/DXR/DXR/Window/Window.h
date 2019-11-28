#pragma once
#include "../Vector.h"

struct HWND__;

class Window
{
public:
	// �R���X�g���N�^
	Window(const Vec2& winSize);
	// �f�X�g���N
	~Window();

	// �E�B���h�E�n���h���̎擾
	HWND__* Get(void) const;

	// �E�B���h�E�T�C�Y�̎擾
	Vec2 WinSize(void) const;

	// ���b�Z�[�W�̊m�F
	static bool CheckMsg(void);

private:
	// �E�B���h�E����
	void CreateWin(const Vec2& winSize);
	

	// �E�B���h�E�n���h��
	void* handle;
};
