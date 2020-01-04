#pragma once
#include "../Singleton.h"
#include <list>
#include <mutex>

class Okdio;

class OkdioList :
	public Singleton<OkdioList>
{
	friend Singleton<OkdioList>;
public:
	// ���X�g�ɒǉ�
	void Add(Okdio** okdio);

	// ���X�g�̍폜
	void Delete(Okdio* okdio);

	// ���X�g�̎擾
	std::list<Okdio*>GetList(void) const;

private:
	// �R���X�g���N�^
	OkdioList();
	// �f�X�g���N�^
	~OkdioList();


	// �r������
	std::mutex mtx;

	// ���X�g
	std::list<Okdio*>okdio;
};
