#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC271 https://atcoder.jp/contests/abc271

/*
 * コンテスト時に解けず、解説を見た。
 * 半分全列挙で解く。
 * 
 * N*N行列のとき、(0,0)から(N-1,N-1)への移動回数は(N-1)+(N-1)となる。
 * うち(N-1)回を縦方向への移動とすると、 (2N-2)C(N-1) が移動方法の数となる。
 * これはN=20のとき3*10^10通りとかになり、愚直に探索してもTLE.
 * 
 * ここで半分全列挙を用いる。
 * x+y=N-1 となる対角線のマスは必ず通ることから、
 *   (1,1)からのxor
 *   (N,N)からのxor
 * を各対角線のマスについて組み合わせればよい。
 * これはmapを用いて、 mp[i][xor]:上からi行目のゴールについて、そこまでのxor値の組み合わせ と定義した。
 * (N,N)からは、盤面を反転させると同じ処理を使いまわせて楽。
 * ※反転は、90度回転のライブラリを持ってたのでそれを2回行った
 * 
 * 対角線のマスは2回計算されることになる点に注意。
 * (N,N)からの探索時は計算から除外するようにした。
 * これにより、(1,1)からの探索と(N,N)の探索とで、値が一致するものが組み合わせ可能となる。
 * 
 * 組み合わせは、mp[i]とmp[N-i-1]を比較する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・移動回数は2(N-1)回。つまりN=20のとき38回となる。
 *   メタだが、40に近いので半分全列挙を考えてみる。
 */

// 0-indexed
int N;
vector<vector<int>> a;
map<int,ll> mp[20], mp_rv[20];  // mp[i][xor]:上からi行目のゴールについて、そこまでのxor値の組み合わせ

void dfs(int y, int x, int num, int k)
{
	num ^= a[y][x];
	if(y+x == N-1)  // 終点
	{
		// 終点は2回計算されてしまうので、k=1のときは無かったことにする
		if(k == 1) num ^= a[y][x];  // もう1回xorすると取り消しになる

		if(k == 0) mp[y][num]++;  // 上からy行目のxor値(num)を+1
		else mp_rv[y][num]++;
		return;
	}
	// 終点ではないので、右および下は必ず行ける
	dfs(y+1, x, num, k);
	dfs(y, x+1, num, k);
}

// 要素がN*Nであるaについて、右に90度回転させる
void rotate(vector<vector<int>> &a_)
{
	int n = a_.size();
	auto a_bk = a_;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			a_[i][j] = a_bk[n-1-j][i];
		}
	}
}

int main(void)
{
	int i, j;
	cin >> N;
	a.resize(N, vector<int>(N, 0));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}

	dfs(0, 0, 0, 0);
	// 盤面を反転
	rotate(a);
	rotate(a);
	dfs(0, 0, 0, 1);

	// mp[i]とmp_rv[N-i-1]を比較
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		for(auto &[key, val] : mp[i])
		{
			if(mp_rv[N-i-1].count(key) == 0) continue;
			int val2 = mp_rv[N-i-1][key];
			ans += (ll)val * val2;
		}
	}
	std::cout << ans << endl;

	return 0;
}
