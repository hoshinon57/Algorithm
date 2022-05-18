#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;

// ABC233 https://atcoder.jp/contests/abc233

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * 袋1から順に見ていく。
 * 袋1の各ボールについて、Xを割り切ることができれば、Xをその分割ってから再帰。
 * 割り切れなければそこで打ち切り。
 * 
 * 袋Nまで割ったときにXが1になっていれば成功。
 */

vector<vector<ll>> a;
ll N, X;
ll answer = 0;

// nowX:ボールに書かれた数で割った現状のX
// nextBox:次にどの袋について割るか
void func(ll nowX, ll nextBox)
{
	ll i, j;
	// 袋Nまで確認したら終了 Xが1なら成功
	if(nextBox == N+1)
	{
		if(nowX == 1) answer++;
		return;
	}

	for(i = 1; i < a[nextBox].size(); i++)
	{
		if(nowX % a[nextBox][i] != 0) continue;  // Xがボールの値で割り切れなければ探索終了
		func(nowX/a[nextBox][i], nextBox+1);
	}

	return;
}

int main(void)
{
	// 1-indexed
	ll i, j;
	cin >> N >> X;

	// a[1～N][1～Lx]
	a.resize(N+1);
	for(i = 1; i <= N; i++)
	{
		ll tmp;
		cin >> tmp;
		a[i].resize(tmp+1);
		for(j = 1; j <= tmp; j++)
		{
			cin >> a[i][j];
		}
	}
	/*
	for(i = 1; i <= N; i++)
	{
		for(j = 1; j < a[i].size(); j++)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	*/

	func(X, 1);
	cout << answer << endl;

	return 0;
}
