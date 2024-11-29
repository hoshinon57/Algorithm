#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_7_D

// #include <cassert> が必要
// https://ei1333.github.io/luzhiled/snippets/math/matrix.html
// Matrix<int> a(H,W), b(N); のように定義する。前者はH行W列, 後者はN行N列。
// (1)各種演算 : a+b, a-b, a*b, a+=b, a-=b, a*=b などに対応
// (2)要素アクセス : m[h][w]で要素に直接アクセス可能
// (3)pow(p) : m.pow(p)でm^pを返す 正方行列であること
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

	Matrix pow(long long p) {  // p乗した要素を返す
		assert(height() == width());
		Matrix r = Matrix::I(height()), b = (*this);
		while(p > 0) {
			if(p&1) r *= b;
			b *= b;
			p >>= 1LL;
		}
		return r;
	}
};

int main(void)
{
	int i, j;
	int n, m, l; cin >> n >> m >> l;
	Matrix<ll> a(n, m), b(m, l), r;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < m; j++) cin >> a[i][j];
	}
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < l; j++) cin >> b[i][j];
	}
	r = a*b;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < l; j++)
		{
			cout << r[i][j];
			if(j != l-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
