#pragma once

template <typename T>
class Vector2
{
public:
	T x;
	T y;

	// コンストラクタ
	Vector2() {
		x = y = T(0);
	}
	Vector2(const T& x, const T& y) {
		this->x = x;
		this->y = y;
	}
	Vector2(const Vector2& vec) {
		(*this) = vec;
	}
	// デストラク
	~Vector2() {
	}

	// 演算子オーバーロード
	void operator=(const Vector2& vec) {
		x = vec.x;
		y = vec.y;
	}
};

typedef Vector2<int>Vec2;
typedef Vector2<float> Vec2f;
