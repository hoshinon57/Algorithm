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

// ABC146 https://atcoder.jp/contests/abc146

/*
 * セグメント木で解く。
 * 以下の解説記事を参考にした。
 *   https://betrue12.hateblo.jp/entry/2019/11/25/225556
 *   https://kmjp.hatenablog.jp/entry/2019/11/24/0930
 * 
 * dp[i]:マスiからゴールまでの最小移動回数とすると、
 *   dp[i] = min(dp[i+1] ～ dp[i+M]) + 1
 * となる。
 * よってセグメント木に各マスからの最小移動回数を保持しておき、
 * 上記minの部分を区間最小で求めればよい。
 * まずこれで各マスごとの最小移動回数が求められる。
 * 
 * ※マスNを超えるような出目のときの処理が簡易になるよう、
 * 　セグメント木の要素数をN+M+5とかに設定した。
 * 
 * あとは出目を復元する。
 * これは各マスについて、出目の小さい方から順に見ていって、
 * ゴールまでの回数が初めて減るマスに移動していけばよい。
 */

// Range Minimum Query(RMQ)の実装
// (1)Update(x, val) : 要素xをvalで更新する
// (2)GetMin(a, b) : 区間[a,b)にある要素の最小値を返す
// (3)Set(i, val) & Build() : 木の更新を行わず要素iを更新し(Set), まとめて木の構築を行う(Build)
// (4)Find_Leftmost(a, b, x) : 区間[a,b)の範囲で、x以下となる最も左側の要素番号を返す
// [注意]
//   0-indexed, および半開区間で処理する。
// 以下URLをほぼそのまま持ってきている
// https://tsutaj.hatenablog.com/entry/2017/03/29/204841
// https://algo-logic.info/segment-tree/
template <typename T>
struct SegmentTree_RMQ
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INF = numeric_limits<T>::max();

public:
	// 要素数で初期化
	SegmentTree_RMQ(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INF);
	}

	// 要素xをvalで更新する
	void Update(int x, T val)
	{
		x += n-1;  // node[]の要素番号に変換
		// 最下段のnodeを更新したら、あとは親へ上って更新していく
		node[x] = val;
		while(x > 0)
		{
			x = (x-1)/2;  // 親
			node[x] = min(node[2*x+1], node[2*x+2]);
		}
	}

	// 区間[a,b)にある要素の最小値を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T GetMin(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INF;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = GetMin(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = GetMin(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return min(vl, vr);
	}

	// [a,b)の範囲で、x以下となる最も左側の要素番号を返す
	// 範囲内にx以下が見つからなければ、b(=範囲外)を返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	int Find_Leftmost(int a, int b, T x, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// 自分の値がxより大きい   もしくは
		// クエリ[a,b)と対象[l,r)が交わらない
		if(node[k] > x || (r <= a || b <= l)) return b;  // 自身の右隣を返す

//		if(k >= n-1) return k-(n-1);  // 自分が葉なら、その位置を返す
		if(k >= n-1) return l;  // 自分が葉なら、その位置を返す
		// 葉なので、lが位置を表している

		int vl = Find_Leftmost(a, b, x, 2*k+1, l, (l+r)/2);  // 左側
		if(vl != b)  // 左側に答がある
		{
			return vl;
		}
		else
		{
			return Find_Leftmost(a, b, x, 2*k+2, (l+r)/2, r);  // 右側
		}
	}

	// 要素xをvalで更新する
	// Update()と違い、木全体の更新は行わない。Build()の呼び出しが必要。
	// 用途：初期化時に全要素を設定し、Build()で木を構築する
	void Set(int i, T val)
	{
		i += n-1;  // node[]の要素番号に変換
		node[i] = val;
	}

	// 初期化時にSet()で全要素を設定した後、Build()にて木を構築する
	void Build(void)
	{
		// 最下段を除いた右下の要素から、左上方向へ構築していく
		int i;
		for(i = n-2; i >= 0; i--)
		{
			node[i] = min(node[2*i+1], node[2*i+2]);
		}
	}
};

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	string s; cin >> s;

	// マスiからゴールまでの最小移動回数を記録する
	SegmentTree_RMQ<int> seg(N+M+5);  // マスN-1から出目Mが出てもはみ出ないように
	seg.Update(N, 0);  // マスNからは0回でゴール
	for(i = N-1; i >= 0; i--)  // マスiから移動
	{
		if(s[i] == '1') continue;

		// 「マス[i+1, i+M]のうち、そこからゴールまでの回数が最小のもの」
		// に1を加えたものが、マスiからゴールまでの最小回数
		int cnt = seg.GetMin(i+1, i+M+1);
		// そこから先に進めないようなマスiがあれば、スタートから考えてもゴールにたどり着けない
		// ここで終了させてしまう
		if(cnt > INF32)
		{
			cout << -1 << endl;
			return 0;
		}
		seg.Update(i, cnt+1);
	}

	int cur = 0;
	vector<int> ans;
	while(cur < N)
	{
		int cur_cnt = seg.GetMin(cur, cur+1);
		// 出目が小さい方から順に見ていって、ゴールまでの回数が初めて減るマスに移動する
		for(i = 1; i <= M; i++)
		{
			if(seg.GetMin(cur+i, cur+i+1) < cur_cnt)
			{
				cur += i;
				ans.push_back(i);
				break;
			}
		}
	}
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}
