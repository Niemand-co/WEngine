#pragma once
#include "Vector.h"
#include <iostream>

template<typename T, size_t RowSize, size_t ColSize>
constexpr size_t ElementCount(T(&)[RowSize][ColSize]) { return RowSize * ColSize; }

template<typename T, int RowSize, int ColSize>
struct Matrix {

	union { T data[RowSize][ColSize]; };

	T* operator[](int rowIndex) {
		return data[rowIndex];
	}

	const T* operator[](int rowIndex) const {
		return data[rowIndex];
	}

	static Matrix<T, RowSize, ColSize> GetIdentityMatrix() {

		Matrix<T, RowSize, ColSize> matrix;
		for (int r = 0; r < RowSize; ++r) {
			for (int c = 0; c < ColSize; ++c) {
				if (r == c) {
					matrix[r][c] = (T)1;
				}
				else {
					matrix[r][c] = (T)0;
				}
			}
		}

		return matrix;

	}

	operator T* () {
		return &data[0][0];
	}

	operator const T* () const {
		return static_cast<const T*>(&data[0][0]);
	}

	bool operator==(const Matrix& m) {

		for (int r = 0; r < RowSize; ++r) {
			for (int c = 0; c < ColSize; ++c) {

				if (data[r][c] != m[r][c])
					return false;

			}
		}

		return true;
	}

	bool operator!=(const Matrix& m) {

		for (int r = 0; r < RowSize; ++r) {
			for (int c = 0; c < ColSize; ++c) {

				if (data[r][c] != m[r][c])
					return true;

			}
		}

		return false;
	}

	Matrix<T, RowSize, ColSize> transpose() {

		Matrix<T, RowSize, ColSize> result = Matrix<T, RowSize, ColSize>::GetIdentityMatrix();
		for (int r = 0; r < RowSize; ++r) {
			for (int c = 0; c < ColSize; ++c) {

				result[c][r] = data[r][c];

			}
		}

		return result;

	}

	Vector4Type<float> Transform(Vector4Type<float>& vector) {

		Vec4 tmp;
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				tmp[r] += vector[c] * data[r][c];
			}
		}
		return tmp;

	}

	void operator+=(const Matrix<T, RowSize, ColSize>& m) {

		for (int r = 0; r < RowSize; ++r) {

			for (int c = 0; c < ColSize; ++c) {

				data[r][c] = data[r][c] + m[r][c];

			}

		}

	}

	void operator-=(const Matrix<T, RowSize, ColSize>& m) {

		for (int r = 0; r < RowSize; ++r) {
			for (int c = 0; c < ColSize; ++c) {

				data[r][c] = data[r][c] - m[r][c];

			}
		}

	}

};

typedef Matrix<float, 4, 4> Matrix4x4f;

typedef Matrix<float, 3, 3> Matrix3x3f;

template<typename T, int RowSize, int ColSize>
std::ostream& operator<<(std::ostream& out, const Matrix<T, RowSize, ColSize>& matrix) {

for (int r = 0; r < RowSize; ++r) {
    for (int c = 0; c < ColSize; ++c) {

        if (r != RowSize - 1 || c != ColSize - 1)
            out << matrix[r][c] << ",";
        else
            out << matrix[r][c];

    }
    out << '\n';
}

return out;

}

template<typename T, int RowSize, int ColSize>
inline void Transpose(Matrix<T, RowSize, ColSize>& result, Matrix<T, RowSize, ColSize>& m) {

    for (int r = 0; r < RowSize; ++r) {
        for (int c = 0; c < ColSize; ++c) {

            result[c][r] = m[r][c];

        }
    }

}

template<typename T, int RowSize, int ColSize>
inline Matrix<T, RowSize, ColSize> Transpose(Matrix<T, RowSize, ColSize> m) {

    Matrix<T, RowSize, ColSize> result;
    for (int r = 0; r < RowSize; ++r) {
	    for (int c = 0; c < ColSize; ++c) {

		    result[c][r] = m[r][c];

	    }
    }

    return result;

}

template<typename T, int RowSize, int ColSize>
inline void MatrixAdd(Matrix<T, RowSize, ColSize>& result, const Matrix<T, RowSize, ColSize> m1, const Matrix<T, RowSize, ColSize> m2) {

	for (int r = 0; r < RowSize; ++r) {
		for (int c = 0; c < ColSize; ++c) {

			result[r][c] = m1[r][c] + m2[r][c];

		}
	}

}

template<typename T, int RowSize, int ColSize>
Matrix<T, RowSize, ColSize> operator+(Matrix<T, RowSize, ColSize> m1, const Matrix<T, RowSize, ColSize>& m2) {

	Matrix<T, RowSize, ColSize> result;

	for (int r = 0; r < RowSize; ++r) {

		for (int c = 0; c < ColSize; ++c) {

			result[r][c] = m1[r][c] + m2[r][c];

		}

	}

    return result;

}



template<typename T, int RowSize, int ColSize>
inline void MatrixSub(Matrix<T, RowSize, ColSize>& result, const Matrix<T, RowSize, ColSize> m1, const Matrix<T, RowSize, ColSize> m2) {

for (int r = 0; r < RowSize; ++r) {
    for (int c = 0; c < ColSize; ++c) {

        result[r][c] = m1[r][c] - m2[r][c];

    }
}

}

template<typename T, int RowSize, int ColSize>
Matrix<T, RowSize, ColSize> operator-(const Matrix<T, RowSize, ColSize>& m1, const Matrix<T, RowSize, ColSize>& m2) {

	Matrix<T, RowSize, ColSize> result;

	for (int r = 0; r < RowSize; ++r) {
		for (int c = 0; c < ColSize; ++c) {

			result[r][c] = m1[r][c] - m2[r][c];

		}
	}

	return result;

}



template<typename T, int Da, int Db, int Dc>
inline void MatrixMultiplyBy(Matrix<T, Da, Dc>& result, const Matrix<T, Da, Db>& m1, const Matrix<T, Db, Dc>& m2) {

	Matrix<T, Da, Dc> tmp = Matrix<T, Da, Dc>::GetIdentityMatrix();
	for (int a = 0; a < Da; ++a) {
		for (int c = 0; c < Dc; ++c) {

			T sum = 0;
			for (int b = 0; b < Db; ++b) {
				sum += m1[a][b] * m2[b][c];
			}
			tmp[a][c] = sum;

		}
	}
	for (int a = 0; a < Da; ++a) {
		for (int c = 0; c < Dc; ++c) {
			result[a][c] = tmp[a][c];
		}
	}

}

template<typename T, int Da, int Db, int Dc>
Matrix<T, Da, Dc> operator*(const Matrix<T, Da, Db>& m1, const Matrix<T, Db, Dc>& m2) {

	Matrix<T, Da, Dc> result;
	MatrixMultiplyBy(result, m1, m2);
	return result;

}

template <typename T, int d>
float Inverse(Matrix<T, d, d>& out, const Matrix<T, d, d>& in)
{
    Matrix<T, d, d> m = in;
    float fDet = 1.0f;
    int f = 1;
    int is[d];
    int js[d];
    int k, i, j, l;
    //int dd = d;

    for (k = 0; k < d; k++)
    {
        float fMax = 0.0f;
        for (i = k; i < d; i++)
        {
            for (j = k; j < d; j++)
            {
                const float f1 = abs(m[i][j]);
                if (f1 > fMax)
                {
                    fMax = f1;
                    is[k] = i;
                    js[k] = j;
                }
            }
        }
        if (abs(fMax) < 0.00000001f)
            return 0;
        if (is[k] != k)
        {
            f = -f;
            for (l = 0; l < d; ++l) {
                T temp = m[k][l];
                m[k][l] = m[is[k]][l];
                m[is[k]][l] = temp;
            }
        }
        if (js[k] != k)
        {
            f = -f;
            for (l = 0; l < d; ++l) {
                T temp = m[l][k];
                m[l][k] = m[l][js[k]];
                m[l][js[k]] = temp;
            }
        }
        fDet *= m[k][k];
        m[k][k] = 1.0f / m[k][k];

        for (j = 0; j < d; j++)
        {
            if (j != k)
                m[k][j] *= m[k][k];
        }

        for (i = 0; i < d; i++)
        {
            if (i != k)
            {
                for (j = 0; j < d; j++)
                {
                    if (j != k)
                        m[i][j] = m[i][j] - m[i][k] * m[k][j];
                }
            }
        }
        for (i = 0; i < d; i++)
        {
            if (i != k)
                m[i][k] *= -m[k][k];
        }
    }
    for (k = d - 1; k >= 0; k--)
    {
        if (js[k] != k)
        {
            for (l = 0; l < d; ++l) {
                T temp = m[k][l];
                m[k][l] = m[js[k]][l];
                m[js[k]][l] = temp;
            }
        }
        if (is[k] != k)
        {
            for (l = 0; l < d; ++l) {
                T temp = m[l][k];
                m[l][k] = m[l][is[k]];
                m[l][is[k]] = temp;
            }
        }
    }
    out = m;
    return fDet * f;
}

template <typename T, int d>
Matrix<T, d, d> Inverse(const Matrix<T, d, d>& in)
{
	Matrix<T, d, d> result;
	Inverse(result, in);
	return result;
}

template<template<typename> class TT, typename T, int RowSize, int ColSize>
TT<T> operator*(const Matrix<T, RowSize, ColSize>& matrix, const TT<T>& vector)
{
	TT<T> tmp;
	for (int r = 0; r < RowSize; ++r)
	{
		for (int c = 0; c < ColSize; ++c)
		{
			tmp[r] += vector[c] * matrix[r][c];
		}
	}

    return tmp;
}

inline float Det(const Matrix3x3f& M)
{
    return (M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) -
		    M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) +
		    M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));
}

inline void Transform(Vec4& vector, const Matrix4x4f& matrix)
{
    vector = matrix * vector;
}

inline void Transform(Vec3& vector, const Matrix4x4f& matrix)
{
    vector = Vec3(matrix * Vec4(vector, 1.0f));
}

inline void Translate(Matrix4x4f& matrix, const Vec3& pos) {

	Matrix4x4f translate = { {{
		{1.0f, 0.0f, 0.0f, pos.x},
		{0.0f, 1.0f, 0.0f, pos.y},
		{0.0f, 0.0f, 1.0f, pos.z},
		{0.0f, 0.0f, 0.0f, 1.0f}
	}} };

	matrix = translate * matrix;

}

inline void Scale(Matrix4x4f& matrix, const Vec3& scale)
{
	matrix[0][0] *= scale.x;
	matrix[1][1] *= scale.y;
	matrix[2][2] *= scale.z;
}

inline void Rotate(Matrix4x4f& matrix, const float& tangle, Vec3& axis)
{
	Normalize(axis);
	Matrix4x4f tmp = Matrix4x4f::GetIdentityMatrix();
	float nx = axis.x;
	float ny = axis.y;
	float nz = axis.z;
	float costheta = std::cos(tangle);
	float minus_costheta = (float)(1.0 - costheta);
	float sintheta = std::sin(tangle);
	tmp[0][0] = nx * nx * minus_costheta + costheta;
	tmp[0][1] = nx * ny * minus_costheta - nz * sintheta;
	tmp[0][2] = nx * nz * minus_costheta + ny * sintheta;

	tmp[1][0] = nx * ny * minus_costheta + nz * sintheta;
	tmp[1][1] = ny * ny * minus_costheta + costheta;
	tmp[1][2] = ny * nz * minus_costheta - nx * sintheta;

	tmp[2][0] = nx * nz * minus_costheta - ny * sintheta;
	tmp[2][1] = ny * nz * minus_costheta + nx * sintheta;
	tmp[2][2] = nz * nz * minus_costheta + costheta;

	matrix = tmp * matrix;
}

inline void Rotation(Matrix4x4f& matrix, const float& tangle, Vec3&& axis) {

	Normalize(axis);
	Matrix4x4f tmp = Matrix4x4f::GetIdentityMatrix();
	float nx = axis.x;
	float ny = axis.y;
	float nz = axis.z;
	float costheta = std::cos(tangle);
	float minus_costheta = (float)(1.0 - costheta);
	float sintheta = std::sin(tangle);
	tmp[0][0] = nx * nx * minus_costheta + costheta;
	tmp[0][1] = nx * ny * minus_costheta - nz * sintheta;
	tmp[0][2] = nx * nz * minus_costheta + ny * sintheta;

	tmp[1][0] = nx * ny * minus_costheta + nz * sintheta;
	tmp[1][1] = ny * ny * minus_costheta + costheta;
	tmp[1][2] = ny * nz * minus_costheta - nx * sintheta;

	tmp[2][0] = nx * nz * minus_costheta - ny * sintheta;
	tmp[2][1] = ny * nz * minus_costheta + nx * sintheta;
	tmp[2][2] = nz * nz * minus_costheta + costheta;

	matrix = tmp * matrix;

}

inline void LookAt(Matrix4x4f& matrix, const Vec3& pos, const Vec3& goal, const Vec3& WorldUp) {

	Matrix4x4f translate = Matrix4x4f::GetIdentityMatrix();
	Translate(translate, -pos);

	Vec3 front = Unit(goal - pos);
	Vec3 right = Unit(CrossProduct(front, WorldUp));
	Vec3 up = Unit(CrossProduct(right, front));
	matrix = {
		right.x,  right.y, right.z, DotProduct(right, -pos),
		   up.x,     up.y,    up.z,    DotProduct(up, -pos),
		front.x,  front.y, front.z, DotProduct(front, -pos),
		0.0f, 0.0f, 0.0f, 1.0f
	};

}

inline void OrthoProjection(Matrix4x4f& matrix, const float width, const float height, const float zn, const float zf) {

	Matrix4x4f projection = {
			2 / width,       0.0f,             0.0f,          0.0f,
			0.0f, 2 / height,             0.0f,     0.0f,
			0.0f,       0.0f,  -2.0f / (zf - zn), -(zn + zf) / (zf - zn),
			0.0f,       0.0f,             0.0f,           1.0f
	};

	matrix = projection * matrix;

}

inline void PerspectiveProjection(Matrix4x4f& matrix, const float fov, const float aspect, const float zn, const float zf) {

	auto yscale = 1.0f / tanf(fov * 0.5f);
	auto xscale = yscale / aspect;

	Matrix4x4f projection = {
		xscale,	0.0f,                  0.0f,                         0.0f,
		0.0f, yscale,                  0.0f,                         0.0f,
		0.0f,   0.0f, (zn + zf) / (zn - zf), (2.0f * zf * zn) / (zf - zn),
		0.0f,	0.0f,                  1.0f,                         0.0f
	};
	matrix = projection * matrix;

}

inline Matrix4x4f ViewportMatrix(float x, float y, float width, float height)
{
	Matrix4x4f result = {
		width / 2.0f, 0.0f, 0.0f, x + width / 2.0f,
		0.0f, height / 2.0f, 0.0f, y + height / 2.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return result;
}

template<typename T, size_t RowSize, size_t ColSize>
Matrix<T, RowSize - 1, ColSize - 1> ClipDimension(const Matrix<T, RowSize, ColSize>& m)
{
	Matrix<T, RowSize - 1, ColSize - 1> result;
	for (size_t i = 0; i < RowSize - 1; ++i)
	{
		for (size_t j = 0; j < ColSize - 1; ++j)
		{
			result[i][j] = m[i][j];
		}
	}
	return result;
}