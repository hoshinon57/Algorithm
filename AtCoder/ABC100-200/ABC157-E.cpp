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

// ABC157 https://atcoder.jp/contests/abc157

/*
 * セグメント木で解く。
 * 
 * 「区間に存在する文字の種類をbitで表す」セグメント木を用意する。
 * 各要素について、その要素が表す区間に
 *   'a'が存在するならbit0を1
 *   'b'が存在するならbit1を1
 *   'z'が存在するならbit25を1
 * という値を設定する。
 * 
 * この場合、親ノードは子ノードのORで表現できる。
 * 
 * type2のクエリについては、__builtin_popcount()を使って
 * bitが1になっている数を出力すればよい。
 */

template <typename T>
struct SegmentTree
{
private:
	int n;   // 木の最下段の要素数 (コンストラクタで指定したsize以上の、2のべき乗)
	vector<T> node;
	const T INITIAL = (T)0;  // 初期値

public:
	// 要素数で初期化
	SegmentTree(int size)
	{
		// 最下段のノード数は、size以上の2のべき乗 -> nとする
		// するとセグメント木全体で必要なノード数は 2*n-1 となる
		n = 1;
		while(n < size) n *= 2;
		node.resize(2*n-1, INITIAL);
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
			node[x] = node[2*x+1] | node[2*x+2];
		}
	}

	// 区間[a,b)にある文字の種類をbitで表現して返す
	// k:自分がいるnodeのindex
	// nodeの[l,r)を対象とする
	T Get(int a, int b, int k = 0, int l = 0, int r = -1)
	{
		// r=-1 なら最初の呼び出し
		if(r < 0) r = n;  // [0,n)を対象とする

		// クエリ[a,b)と対象[l,r)が交わらないので、答に影響しない値を返す
		if(r <= a || b <= l) return INITIAL;

		// クエリが対象を完全に被覆する
		if(a <= l && r <= b) return node[k];

		// 左右の子について再帰的に探索
		T vl = Get(a, b, 2*k+1, l, (l+r)/2);  // 左側
		T vr = Get(a, b, 2*k+2, (l+r)/2, r);  // 右側
		return vl | vr;
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
			node[i] = node[2*i+1] | node[2*i+2];
		}
	}
};

int main(void)
{
	int i;
	int N;
	cin >> N;
	
	// 区間に存在する文字の種類をbitで表す
	// 各要素について、その要素が表す区間に
	//   'a'が存在するならbit0を1
	//   'b'が存在するならbit1を1
	//   'z'が存在するならbit25を1
	SegmentTree<int> seg(N);

	string str;
	cin >> str;
	for(i = 0; i < N; i++)  // 要素i
	{
		int d = (int)(str[i] - 'a');
		seg.Set(i, 1<<d);  // i番目の要素に、i番目の文字のbitが1になった値を設定する
	}
	seg.Build();

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		int type;
		cin >> type;
		if(type == 1)
		{
			char c;
			cin >> i >> c;
			i--;  // 0-indexedへの変換
			int d = (int)(c - 'a');
			seg.Update(i, 1<<d);
		}
		else
		{
			int l, r;
			cin >> l >> r;
			l--; r--;  // 0-indexedへの変換
			r++;  // 半開区間への変換
			auto x = seg.Get(l, r);
			cout << __builtin_popcount(x) << endl;
		}
		Q--;
	}

	return 0;
}
