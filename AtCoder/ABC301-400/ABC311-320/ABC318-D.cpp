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

// ABC318 https://atcoder.jp/contests/abc318

/*
 * コンテスト中に解けず、解説を見た。
 * 深さ優先探索で解く。evima氏の解説が近い。
 * https://atcoder.jp/contests/abc318/editorial/7076
 * 
 * 選ぶ頂点をDFSにて全探索し、2X番目と2X+1番目に選んだ頂点を辺で結ぶこととする。
 * このとき、2X番目側は「まだ使っていない頂点のうち、最も若い番号を選べば十分」と考えると、
 * 選び方は 15*13*11*...*3*1=2027025通り であり、十分に間に合う。
 * これはdfs()の実装を参照。
 * 類題：ABC236-D Dance
 * 
 * ここで問題になるのが、Nが奇数であるケース。
 * 自分はNを偶数に補正(切り上げ方向)することとした。
 * このとき、追加された頂点は辺の重みを計算できないので、dummy[]にて判別できるようにした。
 */

int N;
vector<vector<ll>> dist;
vector<bool> dummy;  // dummy[i]:頂点iを使わないときtrue
ll ans;

// 再帰を用いてa[]に頂点番号を列挙していく。
// 全頂点をa[]に積んだら辺の重みの総和を計算する。
// used[i]:頂点iをa[]に積み済ならtrue
void dfs(vector<int> &a, vector<bool> &used)
{
	int i;
	if((int)a.size() == N)  // Nは偶数に補正している
	{
		ll tmp = 0;
		for(i = 0; i < N; i+=2)
		{
			// Nが奇数の場合、偶数に補正している
			// この頂点を使った辺は計算対象外とする
			// ※この処理が無い場合、頂点0が必ず選ばれてしまう
			if(dummy[a[i]] || dummy[a[i+1]]) continue;

			tmp += dist[a[i]][a[i+1]];
		}
		ans = max(ans, tmp);
		return;
	}

	// 次の頂点を積んでいくときに、積んだ数が偶数/奇数で処理を変える
	//   積むのが奇数番目：まだ選んでない頂点のうち、最も若い1つのみを選ぶ
	//   積むのが偶数番目：まだ選んでない頂点を、順に選んでいく
	// 選びたいのは2つの頂点のペアであり、奇数番目は先頭固定で選んで問題ないため
	for(i = 0; i < N; i++)
	{
		if(used[i]) continue;

		used[i] = true;
		a.push_back(i);
		dfs(a, used);
		a.pop_back();
		used[i] = false;

		if((int)a.size()%2 == 0) break;  // 奇数番目なら、まだ選んでない最も若い頂点のみを選んで終了
	}
}

int main(void)
{
	int i, j;
	cin >> N;
	dist.resize(N, vector<ll>(N));
	for(i = 0; i < N-1; i++)
	{
		for(j = i+1; j < N; j++)
		{
			cin >> dist[i][j];
			dist[j][i] = dist[i][j];
		}
	}
	// dfs()での実装を簡易にするため、Nを偶数(切り上げ方向)に補正する
	// 補正した場合、末尾の要素は頂点として使わないため dummy[]=true とする
	dummy.assign((N+1)*2/2, true);  // 2の倍数
	for(i = 0; i < N; i++) dummy[i] = false;
	if(N%2 == 1) N++;

	vector<int> a;
	vector<bool> used(N, false);
	dfs(a, used);
	cout << ans << endl;

	return 0;
}
