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

// ABC125 https://atcoder.jp/contests/abc125

/*
 * セグメント木を用いて解く。
 * 
 * 区間最小や区間和を求める関数を、「区間の最大公約数を求める」ものに置き換えたセグメント木を実装する。
 * 初期値を0にしておくと、メンバ関数のgcd()の実装では gcd(a,0)=a になるので、処理が簡易になる。
 * 
 * これを使う側では、
 *   A1を除いたN-1個の最大公約数
 *   A2を除いたN-1個の最大公約数
 *   ...
 * の最大を求めることになる。
 * A1～ANを2周ぶん並べて登録すると、判定が簡易になる。
 */

// セグメント木を用いた、区間の最大公倍数を返す構造体
// (1)Update(i, val) : 要素iをvalで更新する
// (2)GetGCD(a, b) : 区間[a,b)にある要素全体のGCDを返す
// [注意]
//   0-indexed, および半開区間で処理する。
template <typename T>
struct SegmentTree_GCD
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INITIAL = (T)0;  // 初期値  実装したgcd()ではgcd(a,0)=aになることを利用する

	T gcd(T a, T b)
	{
		if(b == 0) return a;
		else return gcd(b, a%b);
	}

public:
	// 要素数で初期化
	SegmentTree_GCD(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
	}

	// 要素iをvalで更新する
	void Update(int i, T val)
	{
		i += n-1;  // node[]の要素番号に変換
		// 最下段のnodeを更新したら、あとは親へ上って更新していく
		node[i] = val;
		while(i > 0)
		{
			i = (i-1)/2;  // 親
			node[i] = gcd(node[2*i+1], node[2*i+2]);
		}
	}

	// 区間[a,b)にある要素全体のGCDを返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T GetGCD(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		// INITIAL=0であり、メンバ関数のgcd()ではgcd(a,0)=aになることを利用する
		if(r <= a || b <= l) return INITIAL;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetGCD(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetGCD(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return gcd(vl, vr);
	}
};

int main(void)
{
	int i;
	int N;
	cin >> N;
	SegmentTree_GCD<int> seg(N*2);  // 実装を簡易にするために2周
	for(i = 0; i < N; i++)
	{
		int a;
		cin >> a;
		seg.Update(i, a);
		seg.Update(i+N, a);  // 2周目
	}

	int answer = 1;
	for(i = 0; i < N; i++)  // 各Aiを除いたN-1個の要素の、最大公約数を求める
	{
		int g = seg.GetGCD(i, i+(N-1));  // iからN-1個
		answer = max(answer, g);
	}
	cout << answer << endl;

	return 0;
}
