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

// ABC179 https://atcoder.jp/contests/abc179

/*
 * 周期性を用いて解く。
 * 
 * 数列Aは最大でもMでループする。
 * よってまずは「ループに入るまでの要素数」と「ループの周期」を求める。
 * 求めるものは数列Aの総和であるため、
 * ・ループに入るまで
 * ・1ループぶんの総和*ループ回数
 * ・ループの端数ぶん
 * の和が答となる。
 * 
 * 計算量はO(M).
 */
int main(void)
{
	ll i;
	ll N, X, M;
	cin >> N >> X >> M;

	// ループを見つける
	vector<bool> used(M, false);  // 探索済みか
	vector<ll> path;  // 探索済みのAの値を順に格納する
	ll cur = X;
	while(!used[cur])
	{
		path.push_back(cur);
		used[cur] = true;
		cur = (cur*cur) % M;
	}
	// curにはループの先頭の値が入っている

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();  // ループに入る前の要素数
	ll loop_num = path.size() - bef_loop;  // ループ周期
	ll answer = 0;
	// ループに入る前の和を計算
	for(i = 0; i < bef_loop; i++)
	{
		answer += path[i];
		if(i+1 == N)  // ループに入る前に終了
		{
			cout << answer << endl;
			return 0;
		}
	}

	// 1ループの総和
	ll one_loop_sum = 0;
	for(i = bef_loop; i < bef_loop+loop_num; i++)
	{
		one_loop_sum += path[i];
	}

	// ループを何周するか
	answer += ((N-bef_loop)/loop_num) * one_loop_sum;
	// ループの端数
	for(i = bef_loop; i < bef_loop + (N-bef_loop)%loop_num; i++)
	{
		answer += path[i];
	}
	cout << answer << endl;

	return 0;
}
