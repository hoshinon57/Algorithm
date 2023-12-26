#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC213 https://atcoder.jp/contests/abc213

// X,Yそれぞれ座標圧縮して出力する

// 1次元の座標圧縮
// a：座標圧縮したい元データ 処理途中で要素が書き換えられる点に注意(保持したい場合は参照を外す)
// 返り値：圧縮後のデータ
// 計算量はO(NlogN)
template <typename T>
vector<T> compression_one(vector<T> &a)
{
	vector<T> a_comp = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する

	// それぞれの元データが「何番目に小さいか」をlower_bound()で求める
	for(int i = 0; i < (int)a_comp.size(); i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
	}

	return a_comp;
}

int main(void)
{
	int i;
	int H, W, N; cin >> H >> W >> N;
	vector<int> a(N), b(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i] >> b[i];
	}
	auto a_comp = compression_one(a);
	auto b_comp = compression_one(b);
	for(i = 0; i < N; i++)
	{
		cout << a_comp[i]+1 << " " << b_comp[i]+1 << endl;
	}

	return 0;
}
