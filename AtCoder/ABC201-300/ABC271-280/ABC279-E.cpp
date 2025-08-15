#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 2025/8に再解きしたソースコードに置き換えた

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 自力で解けず、解説を見た。
 * 累積和的な考え方で解く。
 * 
 * 基本はkyopro_friends氏の解説の通り。
 *   https://atcoder.jp/contests/abc279/editorial/5311
 *   https://twitter.com/kyopro_friends/status/1596501785682006016
 * 
 * ＞すなわち、iを除いた1以上M以下の整数kについて、昇順に以下の操作を行う
 * について、「iより前の操作」と「iより後の操作」とで分けて考える。
 * 
 * (1)iより前の操作
 *   "1"の行き先を保持しておき、操作ごとに追従していく。(fr_idx)
 *   詳細は実装を参照。
 * 
 * (2)iより後の操作
 *   b[N]={1,2,3,...,N} という配列を用意しておく。
 *   b[x]=yで、左からx番目が、最終的にy番目に来るということを指す。
 *   入力Aの後ろから操作すると実現できる。
 *   
 * 以上、各操作ごとに(1)と(2)の結果を組み合わせれば答となる。
 */

int main(void)
{
	// 1-indexed
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> a(M+1); for(i = 1; i <= M; i++) {cin >> a[i];}

	vector<ll> fr_idx(M+1);  // i番目の操作まで行った後、"1"がいる位置
	fr_idx[0] = 1;
	for(i = 1; i <= M; i++)
	{
		if(fr_idx[i-1] == a[i])
		{
			fr_idx[i] = fr_idx[i-1]+1;
		}
		else if(fr_idx[i-1] == a[i]+1)
		{
			fr_idx[i] = fr_idx[i-1]-1;
		}
		else
		{
			fr_idx[i] = fr_idx[i-1];  // "1"の位置は変化しない
		}
	}

	vector<ll> b(N+1);  // 後ろから用
	for(i = 0; i <= N; i++) b[i] = i;

	vector<ll> ans(M+1);
	for(i = M; i >= 1; i--)  // i番目の操作を無視する
	{
		ll idx = fr_idx[i-1];  // 前から (i-1)番目までやった結果
		ans[i] = b[idx];

		// 後ろからの操作
		swap(b[a[i]], b[a[i]+1]);
	}

	for(i = 1; i <= M; i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
