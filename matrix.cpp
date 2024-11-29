#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

/*
 * 行列計算のライブラリ
 * 
 * [ざっくり概要]
 * ・行列同士の演算 +,-,*, +=,-=,*= が可能。
 * ・pow()にて行列累乗が可能。
 * ・要素の型にatcoder/modintを使用可能。
 *   ただし入力時はいったん別変数を仲介必要。また出力時は .val() が必要。
 * 
 * [関連する問題 / verifyした問題]
 * [Library Checker]Matrix Product https://judge.yosupo.jp/problem/matrix_product
 * [Library Checker]Pow of Matrix  https://judge.yosupo.jp/problem/pow_of_matrix
 * AOJ ITP1_7_D https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D
 * アルゴリズムと数学 054 https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_at
 * アルゴリズムと数学 055 https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_au
 * アルゴリズムと数学 056 https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_av
 * ABC293-E
 * 
 * [参考資料]
 *   https://ei1333.github.io/luzhiled/snippets/math/matrix.html
 *   https://qiita.com/gnbrganchan/items/47118d45b3af9d5ae9a4
 *   https://github.com/atcoder/live_library/blob/master/mat.cpp
 */

// #include <cassert> が必要
// https://ei1333.github.io/luzhiled/snippets/math/matrix.html
// Matrix<int> a(H,W), b(N); のように定義する。前者はH行W列, 後者はN行N列。
// (1)各種演算 : a+b, a-b, a*b, a+=b, a-=b, a*=b などに対応
// (2)要素アクセス : m[h][w]で要素に直接アクセス可能
// (3)pow(p) : m.pow(p)でm^pを返す 正方行列であること
// (4)rorate() : m.rotate(deg)で90度単位で回転したものを返す
template <typename T>  // 要素の型
struct Matrix {
	vector<vector<T>> a;  // a[行][列]
	Matrix() {}
	Matrix(size_t h, size_t w) : a(h, vector<T>(w, 0)) {}  // a[h][w]で初期化
	Matrix(size_t n) : a(n, vector<T>(n, 0)) {}  // 正方行列
	size_t height(void) const { return a.size(); }
	size_t width(void) const { return a[0].size(); }
	static Matrix I(size_t n) {  // 単位行列を返す
		Matrix r(n);
		for(size_t i = 0; i < n; i++) r[i][i] = 1;
		return r;
	}

	const vector<T>& operator[](int i) const { return a[i]; }  // Read
	vector<T>& operator[](int i) { return a[i]; }  // write
	Matrix& operator+=(const Matrix &b) {
		size_t h = height(), w = width();
		assert(h == b.height() && w == b.width());
		for(size_t i = 0; i < h; i++) {
			for(size_t j = 0; j < w; j++) {
				a[i][j] += b[i][j];
			}
		}
		return *this;
	}
	Matrix& operator-=(const Matrix &b) {
		size_t h = height(), w = width();
		assert(h == b.height() && w == b.width());
		for(size_t i = 0; i < h; i++) {
			for(size_t j = 0; j < w; j++) {
				a[i][j] -= b[i][j];
			}
		}
		return *this;
	}
	Matrix& operator*=(const Matrix &b) {
		size_t h = height(), w = b.width(), p = width();
		assert(p == b.height());
		Matrix r(h, w);
		for(size_t i = 0; i < h; i++) {
			for(size_t j = 0; j < w; j++) {
				for(size_t k = 0; k < p; k++) {
					r[i][j] += a[i][k] * b[k][j];
				}
			}
		}
		// a.swap(r);
		swap(a, r.a);  // modintを使う場合、こちらでないとビルドエラーになった
		return *this;
	}

	Matrix operator+(const Matrix &b) const {
		return Matrix(*this) += b;
	}
	Matrix operator-(const Matrix &b) const {
		return Matrix(*this) -= b;
	}
	Matrix operator*(const Matrix &b) const {
		return Matrix(*this) *= b;
	}

	// p乗した要素を返す
	Matrix pow(long long p) {
		assert(height() == width());
		Matrix r = Matrix::I(height()), b = (*this);
		while(p > 0) {
			if(p&1) r *= b;
			b *= b;
			p >>= 1LL;
		}
		return r;
	}

	// 時計回りに回転したものを返す (deg=1:90度, 2:180度, 3:270度)
	Matrix rotate(int deg) {
		Matrix<T> r;
		size_t y, x;
		if(deg == 1 || deg == 3) {
			r.a.resize(width(), vector<T>(height()));
			for(y = 0; y < height(); y++)
			{
				for(x = 0; x < width(); x++)
				{
					if(deg == 1) r[x][height()-y-1] = a[y][x];
					if(deg == 3) r[width()-x-1][y] = a[y][x];
				}
			}
		}
		else if(deg == 2) {
			r.a.resize(height(), vector<T>(width()));
			for(y = 0; y < height(); y++)
			{
				for(x = 0; x < width(); x++)
				{
					r[height()-y-1][width()-x-1] = a[y][x];
				}
			}
		}
		return r;
	}
};

int main(void)
{
	Matrix<int> a(2,2), b(2,2);
	a[0] = {1,3};
	a[1] = {5,7};
	b.a = { {10,100},
	        {0,0} };  // この形式も可
	assert((a+b).a == vector<vector<int>>({ {11, 103}, {5, 7} }) );
	assert((a-b).a == vector<vector<int>>({ {-9, -97}, {5, 7} }) );
	assert((a*b).a == vector<vector<int>>({ {10, 100}, {50, 500} }) );
	assert(a.pow(5).a == vector<vector<int>>({ {10816, 17088}, {28480, 44992} }) );

	Matrix<int> c(3,4), d(3,4), r, r2;
	c.a = {{1,2,3,4},
	       {5,6,7,8},
	       {9,10,11,12}};
	d.a = {{12,11,10,9},
	       {8,7,6,5},
	       {4,3,2,1}};
	r = c.rotate(1); r = r.rotate(1);  // 90度を2回
	assert(r.a == d.a);
	r = c.rotate(2);  // 一気に180度
	assert(r.a == d.a);
	r = r.rotate(1);  // 180+90度
	r2 = c.rotate(3);  // 一度に270度
	assert(r.a == r2.a);

	return 0;
}
