#pragma once

template <typename T>
class Singleton
{
public:
	// インスタンス変数の取得
	static T& Get(void)
	{
		static T instance;
		return instance;
	}

private:
	Singleton(const Singleton&)      = delete;
	void operator=(const Singleton&) = delete;

protected:
	// コンストラクタ
	Singleton() {}
	// デストラク
	virtual ~Singleton() {}
};
