#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC332 https://atcoder.jp/contests/abc332

/*
 * 全探索で解く。
 * 
 * arr_h[i]を、Aのi行目がBの何行目に相当するかとして定義する。
 * arr_wも同様。
 * next_permutation()を用いてこの組み合わせを全探索する。
 * H,Wは最大5であり、5!*5!=14,400通りなので十分に間に合う。
 * 
 * 現在のarr_h,arr_wを使い、AとBを一致できるかの判定関数を考える。
 * a[i][j]=b[arr_h[i]][arr_w[j]] が全要素について成立すれば、一致できる。
 * 後は操作回数の計算が必要だが、これはarr_hを {0,1,2,3,4} へとバブルソートで並び替える際の交換回数となり、
 * これは転倒数と一致する。
 * 実際にバブルソートを行っても良いが(最初はそうした)、以下では転倒数を計算している。
 * 
 * [ACまでの思考の流れ]
 * ・H,Wの制約より、全探索っぽい。
 * ・「Aのi行目をBの何行目に割り当てるか」を全探索するのに、next_permutation()が使えそう。
 * ・操作回数は、これやってることはバブルソートと一緒だ。
 */

int H, W;
vector<vector<int>> a, b;

// arr_h,wを元に操作したとき、A,Bが一致するか
// 一致するなら操作回数を返す
// 一致しないならINFを返す
int check(vector<int> arr_h, vector<int> arr_w)
{
	int i, j;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			if(a[i][j] != b[arr_h[i]][arr_w[j]]) return INF32;  // 不一致要素あり
		}
	}

	// 一致させることができる
	int sum = 0;
	auto inv = [&](vector<int> &arr) -> int  // 転倒数を返す
	{
		int ans = 0;
		for(i = 0; i < (int)arr.size(); i++)
		{
			for(j = 0; j < i; j++)
			{
				if(arr[j] > arr[i]) ans++;
			}
		}
		return ans;
	};
	sum += inv(arr_h);
	sum += inv(arr_w);
	return sum;
}

int main(void)
{
	int i, j;
	cin >> H >> W;
	a.resize(H, vector<int>(W));
	b.resize(H, vector<int>(W));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> a[i][j];
	}
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> b[i][j];
	}
	vector<int> arr_h(H), arr_w(W);
	iota(arr_h.begin(), arr_h.end(), 0);
	iota(arr_w.begin(), arr_w.end(), 0);

	int ans = INF32;
	do {
		do {
			chmin(ans, check(arr_h, arr_w));
		} while(next_permutation(arr_w.begin(), arr_w.end()));
	} while(next_permutation(arr_h.begin(), arr_h.end()));
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
