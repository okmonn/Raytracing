#pragma once

template <typename T>
class Vector2
{
public:
	T x;
	T y;

	// �R���X�g���N�^
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
	// �f�X�g���N
	~Vector2() {
	}

	// ���Z�q�I�[�o�[���[�h
	void operator=(const Vector2& vec) {
		x = vec.x;
		y = vec.y;
	}
};

typedef Vector2<int>Vec2;
typedef Vector2<float> Vec2f;
