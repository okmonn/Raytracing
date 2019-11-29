#pragma once
#include "Vector.h"
#include <cstring>

class Matrix3x4
{
public:
	float mat[3][4];

	// �R���X�g���N�^
	Matrix3x4() {
		std::memset(mat, 0, sizeof(mat));
	}
	Matrix3x4(const float& x1, const float& y1, const float& z1, const float& w1,
			  const float& x2, const float& y2, const float& z2, const float& w2,
			  const float& x3, const float& y3, const float& z3, const float& w3) {
		mat[0][0] = x1; mat[0][1] = y1; mat[0][2] = z1; mat[0][3] = w1;
		mat[1][0] = x2; mat[1][1] = y2; mat[1][2] = z2; mat[1][3] = w2;
		mat[2][0] = x3; mat[2][1] = y3; mat[2][2] = z3; mat[2][3] = w3;
	}
	Matrix3x4(const Matrix3x4& matrix) {
		(*this) = matrix;
	}
	// �f�X�g���N�^
	~Matrix3x4() {
	}

	// ���s�ړ�
	void Translation(const Vec3f& vec) {
		(*this) *= Matrix3x4(1.0f, 0.0f, 0.0f, vec.x,
							 0.0f, 1.0f, 0.0f, vec.y,
							 0.0f, 0.0f, 1.0f, vec.z);
	}

	// ���Z�q�I�[�o�[���[�h
	void operator=(const Matrix3x4& matrix) {
		std::memcpy(this->mat, matrix.mat, sizeof(this->mat));
	}

	void operator*=(const Matrix3x4& matrix) {
		float tmp0x0 = (mat[0][0] * matrix.mat[0][0]) + (mat[0][1] * matrix.mat[1][0]) + (mat[0][2] * matrix.mat[2][0]);
		float tmp0x1 = (mat[0][0] * matrix.mat[0][1]) + (mat[0][1] * matrix.mat[1][1]) + (mat[0][2] * matrix.mat[2][1]);
		float tmp0x2 = (mat[0][0] * matrix.mat[0][2]) + (mat[0][1] * matrix.mat[1][2]) + (mat[0][2] * matrix.mat[2][2]);
		float tmp0x3 = (mat[0][0] * matrix.mat[0][3]) + (mat[0][1] * matrix.mat[1][3]) + (mat[0][2] * matrix.mat[2][3]);

		float tmp1x0 = (mat[1][0] * matrix.mat[0][0]) + (mat[1][1] * matrix.mat[1][0]) + (mat[1][2] * matrix.mat[2][0]);
		float tmp1x1 = (mat[1][0] * matrix.mat[0][1]) + (mat[1][1] * matrix.mat[1][1]) + (mat[1][2] * matrix.mat[2][1]);
		float tmp1x2 = (mat[1][0] * matrix.mat[0][2]) + (mat[1][1] * matrix.mat[1][2]) + (mat[1][2] * matrix.mat[2][2]);
		float tmp1x3 = (mat[1][0] * matrix.mat[0][3]) + (mat[1][1] * matrix.mat[1][3]) + (mat[1][2] * matrix.mat[2][3]);

		float tmp2x0 = (mat[2][0] * matrix.mat[0][0]) + (mat[2][1] * matrix.mat[1][0]) + (mat[2][2] * matrix.mat[2][0]);
		float tmp2x1 = (mat[2][0] * matrix.mat[0][1]) + (mat[2][1] * matrix.mat[1][1]) + (mat[2][2] * matrix.mat[2][1]);
		float tmp2x2 = (mat[2][0] * matrix.mat[0][2]) + (mat[2][1] * matrix.mat[1][2]) + (mat[2][2] * matrix.mat[2][2]);
		float tmp2x3 = (mat[2][0] * matrix.mat[0][3]) + (mat[2][1] * matrix.mat[1][3]) + (mat[2][2] * matrix.mat[2][3]);

		(*this) = Matrix3x4(tmp0x0, tmp0x1, tmp0x2, tmp0x3,
							tmp1x0, tmp1x1, tmp1x2, tmp1x3,
							tmp2x0, tmp2x1, tmp2x2, tmp2x3);
	}
};
