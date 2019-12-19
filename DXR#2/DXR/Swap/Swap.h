#pragma once

struct IDXGISwapChain4;
class Window;
class Queue;

class Swap
{
public:
	// �R���X�g���N�^
	Swap(const Window* win, const Queue* queue, const size_t& bufferNum = 2);
	// �f�X�g���N
	~Swap();

	// ��ʂ̍X�V
	void Present(const unsigned int& sync = 0, const unsigned int& flag = 0);

	// �X���b�v�`�F�C���̎擾
	IDXGISwapChain4* Get(void) const;

	// �o�b�N�o�b�t�@���̎擾
	unsigned int BufferNum(void) const;

	// ���݂̃o�b�t�@�ԍ��̎擾
	unsigned int Index(void) const;

private:
	// �X���b�v�`�F�C���̐���
	void CreateSwap(const Window* win, const Queue* queue, const size_t& bufferNum);


	// �X���b�v�`�F�C��
	IDXGISwapChain4* swap;
};
