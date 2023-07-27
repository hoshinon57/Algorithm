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

// ABC241 https://atcoder.jp/contests/abc241

/*
 * 周期性を用いて解く。
 * 
 * 最大でもNの回数でループする。
 * よってまずは「ループに入るまでの要素数」と「ループの周期」を求める。
 * 求めたいのはアメの総和なので、以下それぞれのアメの個数を求めて加算したものが答となる。
 * ・ループに入るまで
 * ・1ループぶん
 * ・ループの端数ぶん
 * 
 * ループに入る前に操作が終了するケースに注意。
 * 計算量はO(N).
 */
int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++) cin >> a[i];

	// まずはループを見つける
	vector<bool> used(N, false);  // 探索済みか
	vector<ll> path;  // 探索済みのAの番号を順に格納する
	ll cur = 0;
	// ループするまで先頭から見ていく
	while(!used[cur])
	{
		path.push_back(cur);
		// 探索済みにしてAの次の要素へ
		used[cur] = true;
		cur = (cur + a[cur]) % N;
	}
	// curにはループの最初の要素番号が入っている

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();  // ループに入るまでの要素数
	ll loop_num = path.size() - bef_loop;  // ループ周期
	ll answer = 0;
	// ループに入るまでのアメの個数を求める
	for(i = 0; i < bef_loop; i++)
	{
		answer += a[path[i]];
		if(i+1 == K)  // ループに入るまでに終了
		{
			cout << answer << endl;
			return 0;
		}
	}
	
	ll one_loop_sum = 0;  // 1ループぶんの総和
	for(i = bef_loop; i < bef_loop+loop_num; i++)
	{
		one_loop_sum += a[path[i]];
	}
	answer += ((K - bef_loop) / loop_num) * one_loop_sum;  // ループ数 x 1ループぶんの総和

	// ループの端数ぶんのアメの個数を加算する
	ll rem = (K - bef_loop) % loop_num;  // 残り
	for(i = bef_loop; i < bef_loop+rem; i++)
	{
		answer += a[path[i]];
	}
	cout << answer << endl;

	return 0;
}
