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

// ABC396 https://atcoder.jp/contests/abc396

/*
 * 転倒数の気持ちになって解く。公式解説の方針はピンと来ず、kyopro_friend氏およびsnuke氏の解法が似ていた。
 *   https://x.com/kyopro_friends/status/1898368997663654095
 *   https://atcoder.jp/contests/abc396/submissions/63572075
 * 
 * 転倒数が変化するのはどういうときか、を考えてみる。
 * 仮にAi+kをMで「割った余りにしない」としたとき、各kにて転倒数は変化しない。
 * 変化するのは「Mで割った余りにすることで、値が(M-1)から0になるタイミング」となる。
 * 
 * このとき、その要素の位置から見て
 *   左にある要素を見ると：全て自分より大きいので、その要素数ぶんだけ転倒数が増える
 *   右にある要素を見ると：全て自分より小さいので、その要素数ぶんだけ転倒数が減る
 * となる。
 * ※厳密には自分と同じ値の考慮が必要だが、実は相殺されるので気にしなくてよい。
 * 位置をpos, 要素数をNとすると
 *   左にはpos個、
 *   右には(N-pos-1)個
 * あることになる。
 * 
 * 事前にpos[i]として「値iの登場位置」を計算しておく。
 * 各kについて
 *   k=0のとき：ここは愚直に転倒数を計算する。
 *   k>0のとき：前述のように差分計算する。
 * となる。
**/

// Binary Indexed Treeを用いて以下(1)(2)(4)をO(logN)で計算する
//   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
//   (2)Add : iとxが与えられたとき、Aiにxを加算する
//   (4)lower_bound : a1+a2+...+ax>=w となるような最小のxを返す (各aiが0以上であることが条件)
// [注意]
//   1-indexed
//   Sum()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
// https://algo-logic.info/binary-indexed-tree/
template <typename T>
struct BIT
{
	vector<T> array;  
	const int n;  // 1-indexedでA1～Anまでの数列を扱う

	BIT(int n_) : array(n_+1, 0), n(n_) {}  // 1-indxedのため配列の要素数は+1して確保
	
	// 1番目からi番目までの累積和を求める
	T Sum(int i)
	{
		T s = 0;
		while(i > 0)
		{
			s += array[i];
			i -= i & (-i);  // LSBを減算
		}
		return s;
	}

	// [i,j]の要素の累積和を求める
	T Sum(int i, int j)
	{
		T s1 = Sum(i-1);
		T s2 = Sum(j);
		return s2 - s1;
	}

	// i番目の要素にxを加算
	void Add(int i, T x)
	{
		while(i <= n)
		{
			array[i] += x;
			i += i & (-i);  // LSBを加算
		}
	}

	// a1 + a2 + ... + ax >= w となるような最小のxを返す
	// 二分探索を用いるので、
	//   a1
	//   a1+a2
	//   a1+a2+a3 ...
	// が狭義単調増加になっている必要がある
	// ⇒各aiが0以上であること、が条件
	int lower_bound(T w)
	{
		// a[i]>=0 の条件があるので、wが0以下なら"a1"と打ち切ってよい
		if(w <= 0) return 1;

		int l = 0, r = 1;
		while(r <= n) r <<= 1;  // n以下の、最小の2のべき乗
		for(int len = r; len > 0; len >>= 1)  // len:長さ 1段下るごとに1/2になっていく
		{
			// w以上にならないギリギリで探索していく
			if(l+len <= n && array[l+len] < w)  // [l+1, len]を採用するとき
			{
				w -= array[l+len];
				l += len;  // 右側の子に行くイメージ
			}
		}
		return l+1;  // 「w以上にならないギリギリ」なので、+1したものが答
	}
};

int main(void)
{
	ll i, k;
	ll N, M; cin >> N >> M;
	vector<vector<ll>> pos(M);  // pos[i]:値iの登場位置
	vector<ll> a(N);
	for(i = 0; i < N; i++) {
		cin >> a[i];
		pos[a[i]].push_back(i);
	}

	ll ans = 0;
	for(k = 0; k < M; k++)
	{
		if(k == 0)  // 愚直に転倒数計算
		{
			BIT<ll> bi(M+5);
			for(i = 0; i < N; i++)
			{
				ans += i-bi.Sum(a[i]+1);  // BITは1-indexedのため
				bi.Add(a[i]+1, 1);
			}
		}
		else
		{
			ll x = M-k;  // a[i]=xを調べる
			for(auto &e : pos[x])
			{
				ans += e;  // 左側の要素
				ans -= (N-e-1);  // 右側
			}
		}
		cout << ans << endl;
	}

	return 0;
}
