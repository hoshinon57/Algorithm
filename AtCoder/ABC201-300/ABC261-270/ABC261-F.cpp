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

// ABC261 https://atcoder.jp/contests/abc261

/*
 * 転倒数で解く。
 * kyopro_friends氏のものが、実装含め近かった。
 *   https://twitter.com/kyopro_friends/status/1550839892972019712
 *   https://atcoder.jp/contests/abc261/submissions/33422725
 * 
 * 整数Xのみで考えると、単なる転倒数を求める問題。
 * Xと連動する色Cが同じ場合は転倒数にカウントしない、というのが厄介。
 * 
 * 考え方を少し変えて、
 *   (Xだけで考えたときの転倒数) - (同じ色CでまとめたXごとの転倒数)
 * とすると、求めたい値が計算できる。
 * ball[1～N][*] という二次元配列を用意しておき、
 *   ball[i]={色C=iとなるXの一覧}
 * とすれば、同じ色ごとの転倒数を計算できる。
 * 
 * ただし初回提出でTLE.
 *   https://atcoder.jp/contests/abc261/submissions/49259422
 * 1～Nの各色ごとに BIT<int> bit(N); として初期化しており、
 * 自作のライブラリでは初期化にO(N)かかるので、全体でO(N^2)となっていたのが原因かなと。
 * 配列ごとに座標圧縮を行い、max_elementで配列の最大値を求めるようにしたところ、ACとなった。
 * ※もしくは、BITの初期化を1回だけにし、
 *   前回bit.Add()した部分をAdd(-1)して戻してから次のを計算することで、
 *   bit(N)で初期化しても間に合ったかも。
 * 
 * [ACまでの思考の流れ]
 * ・転倒数っぽい。色Cの存在が厄介。
 *   色Cを加味しながら全体の転倒数を計算するのは…難しそう。
 * ・「色Cが同じなら転倒数にカウントしない」ではなく、
 *   「色Cが同じものは、その転倒数を後で減算させる」という考え方でできるのでは。
 */

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
		a_comp[i]++;  // 自作のBITは1-indexedなので+1
	}

	return a_comp;
}

// a[]の転倒数を返す
ll Inversion(vector<ll> &a)
{
	ll i;
	ll ans = 0;
	a = compression_one(a);  // 1を最小値として座標圧縮
	ll m = *max_element(a.begin(), a.end());  // BITの要素数は最大値まで
	BIT<int> bit(m);
	for(i = 0; i < (int)a.size(); i++)
	{
		ans += i - bit.Sum(a[i]);
		bit.Add(a[i], 1);
	}
	return ans;
}

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<vector<ll>> ball(N+1);  // ball[N+1][]
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	vector<ll> x(N); for(i = 0; i < N; i++)
	{
		cin >> x[i];
		ball[c[i]].push_back(x[i]);
	}

	ll ans = 0;
	ans = Inversion(x);
	for(i = 1; i <= N; i++)
	{
		if(ball[i].size() == 0) continue;
		ans -= Inversion(ball[i]);
	}
	cout << ans << endl;

	return 0;
}
