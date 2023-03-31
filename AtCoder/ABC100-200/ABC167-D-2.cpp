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

// ABC167 https://atcoder.jp/contests/abc167

/*
 * 周期性を用いて解く。
 * 参考：https://algo-logic.info/abc167d/
 * 
 * テレポーターをN回使う間に、必ずループする。
 * 先頭から順に進んでいき、ループの先頭を見つけつつ、それまでの町をpath[]に保存しておく。
 *   bef_loop:ループに入るまでに辿る町の数
 *   loop_num:ループの周期
 * とすると、
 *   K回目に到着する町
 * 　"(K-bef_loop)%loop_num + bef_loop" 回目に到着する町 (<N)
 * は一致するので、後者を出力すればよい。
 * 
 * 計算量はO(N).
 */
int main(void)
{
	// 0-indexed
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i]--;
	}
	vector<bool> used(N, false);  // 探索済みか
	vector<int> path;  // スタートから辿る町を順に格納する
	int cur = 0;  // 町1スタート
	while(!used[cur])
	{
		path.push_back(cur);
		// 探索済みにして次の町へ
		used[cur] = true;
		cur = a[cur];
	}
	// curにはループの最初の町が入っている

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();  // ループに入るまでに辿る町の数
	ll loop_num = path.size() - bef_loop;  // ループの周期
	ll answer;
	if(K < bef_loop)  // ループに入る前
	{
		answer = path[K];
	}
	else
	{
		ll tmp = ((K - bef_loop) % loop_num) + bef_loop;
		answer = path[tmp];
	}
	cout << answer+1 << endl;  // 1-indexedに戻すために+1

	return 0;
}
