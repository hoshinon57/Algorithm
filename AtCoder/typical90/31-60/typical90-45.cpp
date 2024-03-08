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

// 競プロ典型90問:45 https://atcoder.jp/contests/typical90/tasks/typical90_as

/*
 * bitDPで解く。
 * 
 * dp[S][k]を以下のように定義する。
 *   Sの各ビットをそれぞれの点に割り当てて、
 *   1になっている点を使用してグループをk個作るときの、距離の最大値の最小値 (ただし2乗)
 *   未計算であれば-1
 * dp[(1<<N)-1][K]が答。
 * 
 * dp[S][k]を求めるには、S=S1∪S2となるようなSの部分集合S1,S2について
 *   max( dp[S1][k-1], dp[S2][1] )
 * を求めていく。
 * グループを1つだけ取り出すイメージ。
 * S1,S2の全組み合わせについて上記を計算し、その最小値がdp[S][k]となる。
 * ⇒Sにて立っている全bitを元に、bit全探索で求めていく。
 * 
 * ※実装方針は合っていたが、テストケースの1つ(random00.txt)にてTLEとなり苦戦した。
 *   bitDP()の先頭でSとkの関係から枝刈りする処理を追加し、ACとなった。
 *   参考：https://seekworser.hatenablog.com/entry/2022/09/23/225908
 * 
 * [ACまでの思考の流れ]
 * ・bitDPで dp[S][k] として値を持つとして、どう遷移させたらよいだろうか。
 *   ⇒再帰で実装したいので、dp[S][k]からグループを分割していくイメージで行けそう。
 * ・TLEに苦戦した。
 *   ⇒枝刈りできる箇所を検討、Sとkの関係性で枝刈りができそう。
 * 
 * [参考]
 * ・部分集合の部分集合はO(3^N)
 *   https://naoyat.hatenablog.jp/entry/order-pow-3-n
 *   本問ではbitDP(S,k)において、S1,S2への再帰を考えると、各ビットについて
 *     SでもS1でも立っている
 *     Sでのみ立っている
 *     SでもS1でも立っていない
 *   の3通りより、こうなる…のかな。
 */

const int MAX_N = 15;  // 点の数の最大
const int MAX_K = 15;  // グループ数の最大
// dp[S][k]:
//   Sの各ビットをそれぞれの点に割り当てて、
//   1になっている点を使用してグループをk個作るときの、距離の最大値の最小値 (ただし2乗)
//   未計算であれば-1
ll dp[1<<MAX_N][MAX_K];
int N, K;
vector<ll> x, y;

ll bitDP(int S, int k)
{
	ll &d = dp[S][k];
	if(S == 0) return INF64;  // 点が属さないグループはNG
	if(d != -1) return d;  // メモ化済み

	int i, j;
	int scnt = __builtin_popcount(S);

	// 以下2つの枝刈りでACした 枝刈り無しはTLEだった
	// (1つ目のみの対応で1700ms, 2つ両方の対応で1100ms)
	if(scnt < k)  { return (d = INF64); }  // 残りの点＜残りのグループ であれば条件を満たさない
	if(scnt == k) { return (d = 0); }  // 残りの点＝残りのグループ であれば1グループ1点になるため、距離は0
	
	vector<int> sid;  // Sに1が立っているbitの番号 つまりSに属する点の一覧
	for(i = 0; i < N; i++)
	{
		if((S>>i)&1) sid.push_back(i);
	}
	if(k == 1)  // ベースケース
	{
		ll ret = 0;
		for(i = 0; i < scnt; i++)
		{
			for(j = i+1; j < scnt; j++)
			{
				ll dx = x[sid[i]] - x[sid[j]];
				ll dy = y[sid[i]] - y[sid[j]];
				chmax(ret, dx*dx + dy*dy);
			}
		}
		d = ret;
		return d;
	}

	d = INF64;
	for(i = 1; i < (1<<scnt); i++)  // bit全探索
	{
		int S1 = 0, S2 = 0;
		for(j = 0; j < scnt; j++)
		{
			if( ((i>>j)&1) == 0)
			{
				S1 |= (1<<sid[j]);
			}
			else
			{
				S2 |= (1<<sid[j]);
			}
		}
		ll nxtd = max(bitDP(S1, k-1), bitDP(S2, 1));
		chmin(d, nxtd);
	}

	return d;
}
int main(void)
{
	int i, j;
	cin >> N >> K;
	x.resize(N);
	y.resize(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];
	for(i = 0; i < 1<<N; i++)
	{
		for(j = 0; j <= K; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}
	ll d = bitDP((1<<N)-1, K);
	cout << d << endl;

	return 0;
}
