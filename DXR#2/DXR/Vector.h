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
using Vec2 = Vector2<int>;
using Vec2f = Vector2<float>;

template <typename T>
class Vector3
{
public:
	T x;
	T y;
	T z;

	// コンストラクタ
	Vector3() {
		x = y = z = T(0);
	}
	Vector3(const T& x, const T& y, const T& z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(const Vector3& vec) {
		(*this) = vec;
	}
	// デストラクタ
	~Vector3() {
	}

	// 演算子オーバーロード
	void operator=(const Vector3& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
};
using Vec3 = Vector3<int>;
using Vec3f = Vector3<float>;
