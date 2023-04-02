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

// ABC258 https://atcoder.jp/contests/abc258

/*
 * 周期性を用いて解く。
 * 箱詰めするジャガイモの先頭の要素番号は、N回以下の操作で必ずループする。
 * 
 * potato_num:i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数
 * を求める部分はABC258-E.cppと同様。
 * 
 * 次にpotato_numを用いて、ループを見つけるまで先頭から探索する。
 * ⇒箱詰めの先頭のジャガイモの要素番号を、path[]に格納していく。
 * ループを見つけたら、各クエリKにおいてループ部分を除去し、
 * 何番目のジャガイモから箱詰めするかをpath[]を用いて求めればよい。
 * 
 * 計算量はO(NlogN+Q).
 */
int main(void)
{
	ll i;
	ll N, Q, X;
	cin >> N >> Q >> X;
	vector<ll> w(N*2);  // 2周ぶん確保
	vector<ll> w_sum(N*2+1);  // 累積和
	for(i = 0; i < N; i++)
	{
		cin >> w[i];
		w[i+N] = w[i];
	}
	for(i = 0; i < N*2; i++)
	{
		w_sum[i+1] = w_sum[i] + w[i];
	}

	vector<ll> potato_num(N);  // [i]:i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数
	for(i = 0; i < N; i++)
	{
		// 重さがXに到達するまでにW(0)～W(N-1)を何周するかと、その端数
		ll loop_num = X / w_sum[N];
		ll rem = X % w_sum[N];
		// ループ分
		potato_num[i] = loop_num * N;
		// 端数は累積和＆二分探索で求める
		// 種類iから数えて"rem"以上になるまでなので、w_sum[i]+remが境界
		auto e = lower_bound(w_sum.begin(), w_sum.end(), w_sum[i]+rem) - w_sum.begin();
		potato_num[i] += e - i;
	}

	// ループを見つけるまで先頭から探索
	vector<bool> used(N, false);  // used[i]:箱詰めをi番目のジャガイモから始めたことがあるか
	vector<ll> path;  // 箱詰めの先頭のジャガイモの番号を順に格納していく
	ll cur = 0;
	while(!used[cur])
	{
		path.push_back(cur);
		used[cur] = true;
		cur = (cur+potato_num[cur]) % N;
	}
	// curにはループの先頭の要素番号が入っている

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();
	ll loop_num = path.size() - bef_loop;

	while(Q > 0)
	{
		ll answer;
		ll K;
		cin >> K;
		K--;  // 0-indexed
		if(K < bef_loop)  // ループに入る前
		{
			answer = potato_num[path[K]];
		}
		else
		{
			ll tmp = ((K - bef_loop) % loop_num) + bef_loop;
			answer = potato_num[path[tmp]];
		}
		cout << answer << endl;

		Q--;
	}

	return 0;
}
