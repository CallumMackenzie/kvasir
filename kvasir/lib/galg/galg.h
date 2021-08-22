#ifndef GALG_H
#define GALG_H 1

#include <initializer_list>
#include <string.h>
#include <cmath>

#define FORCE_INLINE inline

#if defined(_WIN32) || defined(__APPLE__)
typedef unsigned int uint;
#endif

#define ALGEBRAIC_VEC(CLASS, NELEMS)                           \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] * rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] / rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] + rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] - rhs[i];                          \
		return ret;                                            \
	}                                                          \
	friend CLASS operator*(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] * rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] / rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] + rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const CLASS &lhs, const T &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs[i] - rhs;                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator*(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs * rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator/(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs / rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator+(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs + rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend CLASS operator-(const T &lhs, const CLASS &rhs)     \
	{                                                          \
		CLASS ret;                                             \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			ret[i] = lhs - rhs[i];                             \
		return ret;                                            \
	}                                                          \
	friend bool operator==(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		bool res = true;                                       \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			res &= (lhs.v[i] == rhs.v[i]);                     \
		return res;                                            \
	}                                                          \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs) \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			if (lhs[i] != rhs[i])                              \
				return true;                                   \
		return false;                                          \
	}                                                          \
	CLASS &operator+=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] += rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator-=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] -= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator/=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] /= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator*=(const CLASS &rhs)                        \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] *= rhs[i];                                    \
		return *this;                                          \
	}                                                          \
	CLASS &operator+=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] += rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator-=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] -= rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator/=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] /= rhs;                                       \
		return *this;                                          \
	}                                                          \
	CLASS &operator*=(const T &rhs)                            \
	{                                                          \
		for (size_s i = 0; i < NELEMS; ++i)                    \
			v[i] *= rhs;                                       \
		return *this;                                          \
	}

#define VEC_STD_OPS(CLASS, NELEMS)              \
	T dot(const CLASS &v2) const                \
	{                                           \
		T sum = 0;                              \
		for (size_s i = 0; i < NELEMS; ++i)     \
			sum += (v[i] * v2[i]);              \
		return sum;                             \
	}                                           \
	T len() const                               \
	{                                           \
		return (T)sqrtf((float)dot(*this));     \
	}                                           \
	CLASS &normalize()                          \
	{                                           \
		T l = len();                            \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				v[i] /= l;                      \
		return *this;                           \
	}                                           \
	CLASS normalized() const                    \
	{                                           \
		T l = len();                            \
		CLASS ret;                              \
		if (l != 0)                             \
			for (size_s i = 0; i < NELEMS; ++i) \
				ret.v[i] = v[i] / l;            \
		return ret;                             \
	}

#define MAT_NUM_OP(CLASS, SZ, OP)                            \
	friend CLASS operator OP(const CLASS &lhs, const T &rhs) \
	{                                                        \
		CLASS ret = lhs;                                     \
		for (size_s i = 0; i < SZ; ++i)                      \
			for (size_s j = 0; j < SZ; ++j)                  \
				ret[i][j] OP## = rhs;                        \
		return ret;                                          \
	}

#define ALGEBRAIC_MAT_SQUARE(CLASS, SZ)                                  \
	MAT_NUM_OP(CLASS, SZ, +)                                             \
	MAT_NUM_OP(CLASS, SZ, -)                                             \
	MAT_NUM_OP(CLASS, SZ, /)                                             \
	MAT_NUM_OP(CLASS, SZ, *)                                             \
	friend CLASS operator*(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		CLASS ret;                                                       \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				for (size_s i = 0; i < SZ; ++i)                          \
					ret[r][c] += lhs[r][i] * rhs[i][c];                  \
		return ret;                                                      \
	}                                                                    \
	friend bool operator==(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				if (lhs[r][c] != rhs[r][c])                              \
					return false;                                        \
		return true;                                                     \
	}                                                                    \
	friend bool operator!=(const CLASS &lhs, const CLASS &rhs)           \
	{                                                                    \
		return !(lhs == rhs);                                            \
	}                                                                    \
	friend vec##SZ<T> operator*(const CLASS &lhs, const vec##SZ<T> &rhs) \
	{                                                                    \
		vec##SZ<T> ret;                                                  \
		for (size_s r = 0; r < SZ; ++r)                                  \
			for (size_s c = 0; c < SZ; ++c)                              \
				ret.v[c] += rhs[r] * lhs[r][c];                          \
		return ret;                                                      \
	}                                                                    \
	CLASS get_cofactor(int p, int q, int n) const                        \
	{                                                                    \
		CLASS temp;                                                      \
		int i = 0, j = 0;                                                \
		for (int row = 0; row < n; ++row)                                \
			for (int col = 0; col < n; ++col)                            \
				if (row != p && col != q)                                \
				{                                                        \
					temp[i][j] = m[row][col];                            \
					if ((++j) == n - 1)                                  \
					{                                                    \
						j = 0;                                           \
						++i;                                             \
					}                                                    \
				}                                                        \
		return temp;                                                     \
	}                                                                    \
	T determinant(int n = SZ) const                                      \
	{                                                                    \
		if (n == 1)                                                      \
			return m[0][0];                                              \
		T D = 0;                                                         \
		CLASS temp;                                                      \
		int sign = 1;                                                    \
		for (int f = 0; f < n; ++f)                                      \
		{                                                                \
			temp = get_cofactor(0, f, n);                                \
			D += ((T)sign) * m[0][f] * temp.determinant(n - 1);          \
			sign = -sign;                                                \
		}                                                                \
		return D;                                                        \
	}                                                                    \
	CLASS adjoint() const                                                \
	{                                                                    \
		int sign = 1;                                                    \
		CLASS temp;                                                      \
		CLASS adj;                                                       \
		for (size_s i = 0; i < 4; ++i)                                   \
			for (size_s j = 0; j < 4; ++j)                               \
			{                                                            \
				temp = get_cofactor(i, j, 4);                            \
				sign = ((i + j) % 2 == 0) ? 1 : -1;                      \
				adj.m[j][i] = (T)sign * temp.determinant(3);             \
			}                                                            \
		return adj;                                                      \
	}                                                                    \
	CLASS inverse() const                                                \
	{                                                                    \
		return adjoint() / determinant();                                \
	}

namespace galg
{
	typedef float fp_num;
	typedef unsigned short size_s;

	template <typename T>
	struct vec4;
	template <typename T>
	struct vec3;
	template <typename T>
	struct vec2;
	template <typename T>
	struct quaternion;
	template <typename T>
	struct mat4;
	template <typename T>
	struct mat3;
	template <typename T>
	struct mat2;

	typedef vec4<fp_num> vec4fp;
	typedef vec4<float> vec4f;
	typedef vec4<double> vec4d;
	typedef vec4<int> vec4i;
	typedef vec4<uint> vec4ui;
	typedef vec4<size_t> vec4st;
	typedef vec4<unsigned short> vec4us;
	typedef vec4<short> vec4s;

	typedef vec3<fp_num> vec3fp;
	typedef vec3<float> vec3f;
	typedef vec3<double> vec3d;
	typedef vec3<int> vec3i;
	typedef vec3<uint> vec3ui;
	typedef vec3<size_t> vec3st;
	typedef vec3<unsigned short> vec3us;
	typedef vec3<short> vec3s;

	typedef vec2<fp_num> vec2fp;
	typedef vec2<float> vec2f;
	typedef vec2<double> vec2d;
	typedef vec2<int> vec2i;
	typedef vec2<uint> vec2ui;
	typedef vec2<size_t> vec2st;
	typedef vec2<unsigned short> vec2us;
	typedef vec2<short> vec2s;

	typedef quaternion<fp_num> quaternionfp;
	typedef quaternion<float> quaternionf;
	typedef quaternion<double> quaterniond;
	typedef quaternion<int> quaternioni;
	typedef quaternion<uint> quaternionui;
	typedef quaternion<size_t> quaternionst;
	typedef quaternion<unsigned short> quaternionus;
	typedef quaternion<short> quaternions;

	typedef mat4<fp_num> mat4fp;
	typedef mat4<float> mat4f;
	typedef mat4<double> mat4d;
	typedef mat4<int> mat4i;
	typedef mat4<uint> mat4ui;
	typedef mat4<size_t> mat4st;
	typedef mat4<unsigned short> mat4us;
	typedef mat4<short> mat4s;

	typedef mat3<fp_num> mat3fp;
	typedef mat3<float> mat3f;
	typedef mat3<double> mat3d;
	typedef mat3<int> mat3i;
	typedef mat3<uint> mat3ui;
	typedef mat3<size_t> mat3st;
	typedef mat3<unsigned short> mat3us;
	typedef mat3<short> mat3s;

	typedef mat2<fp_num> mat2fp;
	typedef mat2<float> mat2f;
	typedef mat2<double> mat2d;
	typedef mat2<int> mat2i;
	typedef mat2<uint> mat2ui;
	typedef mat2<size_t> mat2st;
	typedef mat2<unsigned short> mat2us;
	typedef mat2<short> mat2s;

	template <typename T = fp_num>
	struct vec4
	{
		T v[4];

		vec4(std::initializer_list<T> lst)
		{
			v[3] = 1;
			short ctr = -1;
			for (auto i : lst)
				if (ctr <= 2)
					v[++ctr] = i;
				else
					break;
		}

		vec4(T x = 0, T y = 0, T z = 0, T w = (T)1)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		vec4(vec3<T> v, T w = 1) : vec4(v.x(), v.y(), v.z(), w) {}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE T &z() { return v[2]; }
		FORCE_INLINE T &w() { return v[3]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }
		FORCE_INLINE const T &z() const { return v[2]; }
		FORCE_INLINE const T &w() const { return v[3]; }

		FORCE_INLINE vec3<T> xyz() const { return vec3<T>(x(), y(), z()); }
		FORCE_INLINE vec2<T> xy() const { return vec2<T>(x(), y()); }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
		{
			return v[index];
		}

		FORCE_INLINE vec4 cross(vec4 &v2) const
		{
			return vec4(
				(y() * v2.z()) - (z() * v2.y()),
				(z() * v2.x()) - (x() * v2.z()),
				(x() * v2.y()) - (y() * v2.x()));
		}

		void set(T x, T y, T z, T w)
		{
			x() = x;
			y() = y;
			z() = z;
			w() = w;
		}

		void set(T x, T y, T z)
		{
			x() = x;
			y() = y;
			z() = z;
		}

		void set(T x, T y)
		{
			x() = x;
			y() = y;
		}

		void set(vec4 v)
		{
			set(v.x(), v.y(), v.z(), v.w());
		}

		ALGEBRAIC_VEC(vec4, 4)
		VEC_STD_OPS(vec4, 4)
	};

	template <typename T = fp_num>
	struct vec3
	{
		T v[3];

		vec3(std::initializer_list<T> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec3(T x = 0, T y = 0, T z = 0)
		{
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE T &z() { return v[2]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }
		FORCE_INLINE const T &z() const { return v[2]; }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
		{
			return v[index];
		}

		vec3 cross(vec3 &v2) const
		{
			return vec3(
				(y() * v2.z()) - (z() * v2.y()),
				(z() * v2.x()) - (x() * v2.z()),
				(x() * v2.y()) - (y() * v2.x()));
		}

		FORCE_INLINE vec4<T> xyz1() { return vec4<T>(x(), y(), z(), (T)1); }
		FORCE_INLINE vec2<T> xy() const { return vec2<T>(x(), y()); }

		ALGEBRAIC_VEC(vec3, 3)
		VEC_STD_OPS(vec3, 3)
	};

	template <typename T = fp_num>
	struct vec2
	{
		T v[2];

		vec2(std::initializer_list<T> lst)
		{
			short ctr = -1;
			for (auto i : lst)
				v[++ctr] = i;
		}
		vec2(T x = 0, T y = 0)
		{
			v[0] = x;
			v[1] = y;
		}

		FORCE_INLINE T &x() { return v[0]; }
		FORCE_INLINE T &y() { return v[1]; }
		FORCE_INLINE const T &x() const { return v[0]; }
		FORCE_INLINE const T &y() const { return v[1]; }

		FORCE_INLINE T &operator[](size_t index)
		{
			return v[index];
		}
		FORCE_INLINE const T &operator[](size_t index) const
		{
			return v[index];
		}

		ALGEBRAIC_VEC(vec2, 2)
		VEC_STD_OPS(vec2, 2)
	};

	template <typename T>
	struct quaternion : vec4<T>
	{
		quaternion() : vec4<T>(0, 0, 0, 1) {}

		quaternion(const vec4<T> &v)
		{
			this->v[0] = v.x();
			this->v[1] = v.y();
			this->v[2] = v.z();
			this->v[3] = v.w();
		}

		// NOT axis angle
		quaternion(const vec3<T> &v, T r)
		{
			this->v[0] = v.x();
			this->v[1] = v.y();
			this->v[2] = v.z();
			this->v[3] = r;
		}

		quaternion(T x, T y, T z, T w)
		{
			this->v[0] = x;
			this->v[1] = y;
			this->v[2] = z;
			this->v[3] = w;
		}

		vec3<T> complex() const { return this->xyz(); }
		void complex(const vec3<T> &c)
		{
			this->v[0] = c[0];
			this->v[1] = c[1];
			this->v[2] = c[2];
		}

		T real() const { return this->v[3]; }
		void real(T r) { this->v[3] = r; }

		quaternion conjugate(void) const
		{
			return quaternion(-complex(), real());
		}
		quaternion inverse(void) const
		{
			return conjugate() / this->len();
		}
		quaternion product(const quaternion &rhs) const
		{
			return quaternion(this->y() * rhs.z() - this->z() * rhs.y() + this->x() * rhs.w() + this->w() * rhs.x(),
							  this->z() * rhs.x() - this->x() * rhs.z() + this->y() * rhs.w() + this->w() * rhs.y(),
							  this->x() * rhs.y() - this->y() * rhs.x() + this->z() * rhs.w() + this->w() * rhs.z(),
							  this->w() * rhs.w() - this->x() * rhs.x() - this->y() * rhs.y() - this->z() * rhs.z());
		}
		quaternion operator*(const quaternion &rhs) const
		{
			return product(rhs);
		}
		quaternion operator*(T s) const
		{
			return quaternion(complex() * s, real() * s);
		}
		quaternion operator+(const quaternion &rhs) const
		{
			return quaternion(this->x() + rhs.x(), this->y() + rhs.y(), this->z() + rhs.z(), this->w() + rhs.w());
		}
		quaternion operator-(const quaternion &rhs) const
		{
			return quaternion(this->x() - rhs.x(), this->y() - rhs.y(), this->z() - rhs.z(), this->w() - rhs.w());
		}
		quaternion operator-() const
		{
			return quaternion(-this->x(), -this->y(), -this->z(), -this->w());
		}
		quaternion operator/(T s) const
		{
			if (s == 0)
				return quaternion();
			return quaternion(complex() / s, real() / s);
		}

		void scaled_axis(const vec3<T> &w)
		{
			T theta = w.len();
			if (theta > 0.0001)
			{
				T s = (T)sinf((float)((double)theta / 2.0));
				vec3<T> W(w / theta * s);
				this->v[0] = W[0];
				this->v[1] = W[1];
				this->v[2] = W[2];
				this->v[3] = (T)cosf((float)theta / 2.f);
			}
			else
			{
				this->v[0] = this->v[1] = this->v[2] = 0;
				this->v[3] = 1.0;
			}
		}

		vec3<T> rotated_vec(const vec3<T> &v) const
		{
			return (((*this) * quaternion(v, 0)) * conjugate()).complex();
		}

		void euler(const vec3<T> &euler)
		{
			T c1 = (T)cosf((float)euler.z() * .5f);
			T c2 = (T)cosf((float)euler.y() * .5f);
			T c3 = (T)cosf((float)euler.x() * .5f);
			T s1 = (T)sinf((float)euler.z() * .5f);
			T s2 = (T)sinf((float)euler.y() * .5f);
			T s3 = (T)sinf((float)euler.x() * .5f);

			this->v[0] = c1 * c2 * s3 - s1 * s2 * c3; // x : A
			this->v[1] = c1 * s2 * c3 + s1 * c2 * s3; // y : B
			this->v[2] = s1 * c2 * c3 - c1 * s2 * s3; // z : C
			this->v[3] = c1 * c2 * c3 + s1 * s2 * s3; // w : D
		}

		vec3<T> euler() const
		{
			vec3<T> euler;
			const static double PI_OVER_2 = 3.1415 * 0.5;
			const static double EPSILON = 1e-10;
			T sqw, sqx, sqy, sqz;

			sqw = this->v[3] * this->v[3];
			sqx = this->v[0] * this->v[0];
			sqy = this->v[1] * this->v[1];
			sqz = this->v[2] * this->v[2];

			euler[1] = (T)asinf(2.f * (float)(this->v[3] * this->v[1] - this->v[0] * this->v[2]));
			if (PI_OVER_2 - fabs(euler[1]) > EPSILON)
			{
				euler[2] = (T)atan2f(2.f * (this->v[0] * this->v[1] + this->v[3] * this->v[2]),
									 sqx - sqy - sqz + sqw);
				euler[0] = (T)atan2f(2.f * (this->v[3] * this->v[0] + this->v[1] * this->v[2]),
									 sqw - sqx - sqy + sqz);
			}
			else
			{
				euler[2] = (T)atan2f(2 * this->v[1] * this->v[2] - 2 * this->v[0] * this->v[3],
									 2 * this->v[0] * this->v[2] + 2 * this->v[1] * this->v[3]);
				euler[0] = 0;
				if (euler[1] < 0)
					euler[2] = 3.14159265f - euler[2];
			}
			return euler;
		}

		static quaternion axis_angle(const vec3<T> &axis, const T angle)
		{
			return quaternion(axis * (T)sinf((float)angle * .5f), (T)cosf((float)angle * .5f));
		}
		static quaternion from_euler(const vec3<T> &eu)
		{
			quaternion ret;
			ret.euler(eu);
			return ret;
		}
		static quaternion from_euler(T x, T y, T z)
		{
			return from_euler(vec3<T>(x, y, z));
		}
	};

	template <typename T = fp_num>
	struct mat4
	{
		T m[4][4] = {0};

		mat4() {}
		mat4(std::initializer_list<std::initializer_list<T>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 4)
						break;
				}
				c = 0;
				if (++r >= 4)
					break;
			}
		}
		T **to_array()
		{
			T **ret = new T *[4];
			for (size_s i = 0; i < 4; ++i)
			{
				ret[i] = new T[4];
				for (size_s j = 0; j < 4; ++j)
					ret[i][j] = (T)m[i][j];
			}
			return ret;
		}
		static void delete_array(T **arr)
		{
			for (size_s i = 0; i < 4; ++i)
				delete[] arr[i];
			delete[] arr;
		}

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		mat4 transposed()
		{
			mat4 tmp = *this;
			for (size_t y = 0; y < 4; ++y)
				for (size_t x = 0; x < 4; ++x)
					tmp[y][x] = m[x][y];
			return tmp;
		}

		mat4 &transpose()
		{
			m = transposed().m;
			return *this;
		}

		static mat4 identity()
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)1, 0, 0},
				{0, 0, (T)1, 0},
				{0, 0, 0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat4, 4)

		static mat4 perspective(T fovDeg, T aspectRatio, T near, T far)
		{
			auto deg_to_rad = [](T deg)
			{
				return deg * (3.14159265 / 180.000);
			};
			T fovRad = (T)1.0 / (T)tanf((float)deg_to_rad(fovDeg * (T)0.5));
			return mat4{{fovRad * aspectRatio, 0, 0, 0},
						{0, fovRad, 0, 0},
						{0, 0, far / (far - near), 1},
						{0, 0, (-far * near) / (far - near), 0}};
		}
		static mat4 orthographic(T left, T right, T bottom, T top, T near = (T)-1, T far = (T)1)
		{
			return mat4{
				{(T)2.0 / (right - left), 0, 0, 0},
				{0, (T)2.0 / (top - bottom), 0, 0},
				{0, 0, -(T)2.0 / (far - near), 0},
				{-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), (T)1}};
		}
		template <typename E>
		static mat4 look_at_vec_x(const E &pos, const E &target, const E &up)
		{
			E new_forward = (target - pos).normalize();
			E new_up = (up - (new_forward * up.dot(new_forward))).normalize();
			E new_right = new_up.cross(new_forward);
			return mat4{
				{new_right.x(), new_right.y(), new_right.z(), 0},
				{new_up.x(), new_up.y(), new_up.z(), 0},
				{new_forward.x(), new_forward.y(), new_forward.z(), 0},
				{pos.x(), pos.y(), pos.z(), (T)1}};
		}
		FORCE_INLINE static mat4 look_at(const vec3<T> &pos, const vec3<T> &target, const vec3<T> &up)
		{
			return look_at_vec_x<vec3<T>>(pos, target, up);
		}
		FORCE_INLINE static mat4 look_at(const vec4<T> &pos, const vec4<T> &target, const vec4<T> &up)
		{
			return look_at_vec_x<vec3<T>>(pos.xyz(), target.xyz(), up.xyz());
		}
		FORCE_INLINE static mat4 scale(T x, T y, T z)
		{
			return mat4{
				{x, 0, 0, 0},
				{0, y, 0, 0},
				{0, 0, z, 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 scale(vec3<T> s)
		{
			return scale(s.x(), s.y(), s.z());
		}
		FORCE_INLINE static mat4 scale(vec4<T> s)
		{
			return scale(s.x(), s.y(), s.z());
		}
		FORCE_INLINE static mat4 translation(T x, T y, T z)
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)1, 0, 0},
				{0, 0, (T)1, 0},
				{x, y, z, (T)1}};
		}
		FORCE_INLINE static mat4 translation(vec4<T> t)
		{
			return translation(t.x(), t.y(), t.z());
		}
		FORCE_INLINE static mat4 translation(vec3<T> t)
		{
			return translation(t.x(), t.y(), t.z());
		}
		FORCE_INLINE static mat4 x_rotation(T x)
		{
			return mat4{
				{(T)1, 0, 0, 0},
				{0, (T)cosf(x), (T)sinf(x), 0},
				{0, (T)-sinf(x), (T)cosf(x), 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 y_rotation(T y)
		{
			return mat4{
				{(T)cosf(y), 0, (T)sinf(y), 0},
				{0, 1, 0, 0},
				{(T)-sinf(y), 0, (T)cosf(y), 0},
				{0, 0, 0, 1}};
		}
		FORCE_INLINE static mat4 z_rotation(T z)
		{
			return mat4{
				{(T)cosf(z), (T)sinf(z), 0, 0},
				{(T)-sinf(z), (T)cosf(z), 0, 0},
				{0, 0, (T)1, 0},
				{0, 0, 0, (T)1}};
		}
		FORCE_INLINE static mat4 rotation(T x, T y, T z)
		{
			return x_rotation(x) * y_rotation(y) * z_rotation(z);
		}
		FORCE_INLINE static mat4 rotation(vec3<T> r)
		{
			return rotation(r.x(), r.y(), r.z());
		}
		FORCE_INLINE static mat4 rotation(quaternion<T> q)
		{
			q.normalize();
			return mat4{
				{1 - 2 * q.y() * q.y() - 2 * q.z() * q.z(), 2 * q.x() * q.y() - 2 * q.z() * q.w(), 2 * q.x() * q.z() + 2 * q.y() * q.w(), 0},
				{2 * q.x() * q.y() + 2 * q.z() * q.w(), 1 - 2 * q.x() * q.x() - 2 * q.z() * q.z(), 2 * q.y() * q.z() - 2 * q.x() * q.w(), 0},
				{2 * q.x() * q.z() - 2 * q.y() * q.w(), 2 * q.y() * q.z() + 2 * q.x() * q.w(), 1 - 2 * q.x() * q.x() - 2 * q.y() * q.y(), 0},
				{0, 0, 0, 1}};
		}
	};

	template <typename T = fp_num>
	struct mat3
	{
		T m[3][3] = {0};

		mat3() {}
		mat3(std::initializer_list<std::initializer_list<T>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 3)
						break;
				}
				c = 0;
				if (++r >= 3)
					break;
			}
		}

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat3 identity()
		{
			return mat3{
				{(T)1, 0, 0},
				{0, (T)1, 0},
				{0, 0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat3, 3)
	};

	template <typename T = fp_num>
	struct mat2
	{
		T m[2][2] = {0};

		mat2() {}
		mat2(std::initializer_list<std::initializer_list<T>> list)
		{
			size_t r = 0;
			size_t c = 0;
			for (auto row : list)
			{
				for (auto val : row)
				{
					m[r][c] = val;
					if (++c >= 2)
						break;
				}
				c = 0;
				if (++r >= 2)
					break;
			}
		}

		T *operator[](const size_t index)
		{
			return m[index];
		}

		const T *operator[](const size_t index) const
		{
			return m[index];
		}

		static mat2 identity()
		{
			return mat2{
				{(T)1, 0}, {0, (T)1}};
		}

		ALGEBRAIC_MAT_SQUARE(mat2, 2)

		FORCE_INLINE static mat2 rotation(T v)
		{
			return mat2{
				{(T)cosf(v), (T)sinf(v)},
				{(T)-sinf(v), (T)cosf(v)}};
		}

		FORCE_INLINE static mat2 scale(T x, T y)
		{
			return mat2{{x, 0}, {0, y}};
		}
		FORCE_INLINE static mat2 scale(vec2<T> s)
		{
			return scale(s.x(), s.y());
		}
	};
}

#endif