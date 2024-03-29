#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC153 https://atcoder.jp/contests/abc153

/*
 * 区間加算が可能なBITで解く。
 * 
 * 考えたこと：
 * ・左側のモンスターから倒していく貪欲で良さそう。
 *   (入力は座標でソートされていないので、自分でソート必要)
 * ・あるモンスターiを狙うとき、なるべく多くのモンスターを爆弾に含めたい。
 *   Xi+Dに爆弾を設置すれば良さそう。
 *   これは区間加算ができるBITを使えば良さそう。
 * ・ただし座標は最大10^9のため、
 *     BITの要素i：座標iにいるモンスターのHP
 *   と定義すると時間＆メモリ制限を超過しそう。なので
 *     BITの要素i：i番目のモンスターのHP
 *   と定義する必要がある。
 * ・上記の定義とした場合、
 *   「i番目のモンスターを倒したいとき、なるべく右側で爆弾を使うとして、
 *   何番目のモンスターまでダメージを与えられるか？」を考える必要がある。
 *   これはモンスターがいる座標(ソート済み)に対して二分探索すれば良さそう。
 * 
 * 以上を実装した。
 */

// Binary Indexed Treeを用いて以下(1)～(3)をO(logN)で計算する
//   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
//            [i,j]の区間の累積和を求めるSumもある
//   (2)Add : iとxが与えられたとき、Aiにxを加算する
//   (3)Add_Range : l,rが与えられたとき、Al～Arにxを加算する (Range Add Query)
// [注意]
//   1-indexed
//   Sum(),Add_Range()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
// https://algo-logic.info/binary-indexed-tree/
template <typename T>
struct BIT_RAQ
{
private:
	vector<T> array[2];
	const int n;  // 1-indexedでA1～Anまでの数列を扱う

	// array[idx]に対し、1番目からi番目までの累積和を求める
	T Sum_sub(int idx, int i)
	{
		T s = 0;
		while(i > 0)
		{
			s += array[idx][i];
			i -= i & (-i);  // LSBを減算
		}
		return s;
	}

	// array[idx]に対し、i番目の要素にxを加算
	void Add_sub(int idx, int i, T x)
	{
		while(i <= n)
		{
			array[idx][i] += x;
			i += i & (-i);  // LSBを加算
		}
	}

public:
	// 引数n_に対し、A1～A(n_)まで使えるよう初期化(1-indexed)
	BIT_RAQ(int n_) : n(n_)
	{
		array[0].assign(n+1, 0);  // 1-indxedのため配列の要素数は+1して確保
		array[1].assign(n+1, 0);
	}

	// 1番目からi番目までの累積和を求める
	T Sum(int i)
	{
		return Sum_sub(1, i)*i + Sum_sub(0, i);
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
		Add_sub(0, i, x);
	}

	// [l,r]の要素にxを加算
	void Add_Range(int l, int r, T x)
	{
		/*
		 * BIT0 の l 番目に -x(l−1)を加算
		 * BIT0 の r+1 番目に xr を加算
		 * BIT1 の l 番目に x を加算
		 * BIT1 の r+1 番目に -x を加算
		 */
		Add_sub(0, l	, -x * (l - 1));
		Add_sub(0, r+1	,  x * r);
		Add_sub(1, l	,  x);
		Add_sub(1, r+1	, -x);
	}
};

int main(void)
{
	// 1-indexed
	ll i;
	ll N, D, A;
	cin >> N >> D >> A;
	BIT_RAQ<ll> bit(N);  // 要素iのモンスターのHP 座標は別配列x[]にて管理
	vector<pair<ll, ll>> mons = {{-1, -1}};  // {座標X, HP} モンスターを座標順にソートする 1-indexedのため適当な初期値を追加
	vector<ll> x(N+1);
	for(i = 1; i <= N; i++)
	{
		ll x, h;
		cin >> x >> h;
		mons.push_back({x, h});
	}
	sort(mons.begin(), mons.end());  // モンスターの情報を座標順にソート
	for(i = 1; i <= N; i++)
	{
		bit.Add(i, mons[i].second);
		x[i] = mons[i].first;
	}
	// 以降、座標x[]は昇順ソート済み

	// モンスター1から順に、HPが残っている敵iを見つける。
	// なるべく右側に爆弾をなげるため、 x[敵i]+2*D <= x[敵idx] となる最大のidxを見つける。
	// 爆弾を投げる回数は、 HP[i]/A の切り上げとなる。
	ll answer = 0;
	for(i = 1; i <= N; i++)  // モンスター:i
	{
		if(bit.Sum(i, i) <= 0) continue;  // 撃破済み

		// x[i]*2*Dのupper_bound(), の1つ手前
		auto idx = upper_bound(x.begin(), x.end(), x[i] + D*2) - x.begin();
		idx--;

		ll count = (bit.Sum(i,i)+(A-1)) / A;  // モンスターiを撃破するために使う爆弾の数 HP/Aを切り上げ
		bit.Add_Range(i, idx, -(A*count));
		answer += count;
	}
	cout << answer << endl;

	return 0;
}
