#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC360 https://atcoder.jp/contests/abc360

/*
 * 公式解説の
 * ＞より一般に、線形漸化式の第N項を求める高速な方法として、行列累乗や Fiduccia のアルゴリズムがあります。
 * を参考に、行列累乗で解いてみた版。
 * 
 * DP遷移式を整理すると
 *   newdp = (N^2-2N)/(N^2)*dp + 2/(N^2)
 * となる。
 * 定数部分をa=(N^2-2N)/(N^2), b=2/(N^2)と置くと、ndp=a*dp+bより
 *   (ndp  = (a 1  (dp
 *      b)    0 1)   b)
 * という行列式が出てくる。これは行列累乗で求めることができる。
 * 
 * K回操作後のDP値から期待値を求める部分は、ABC360-E-2.cppと同じ。
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
	ll N, K; cin >> N >> K;
	Matrix<mint> m(2, 2);
	mint a = ((mint)N*N-2*N) / (N*N);
	mint b = (mint)2 / (N*N);
	m[0][0] = a;
	m[0][1] = 1;
	m[1][0] = 0;
	m[1][1] = 1;
	m = m.pow(K);
	mint dp = m[0][0]*1 + m[0][1]*b;  // 最終のDP値
	mint ans = dp + (1-dp)/2*(N+2);
	cout << ans.val() << endl;

	return 0;
}
