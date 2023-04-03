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
 * 周期性を用いて解くのは ABC258-E-2.cpp と同じ。
 * potato_num[]を計算する部分を、尺取り法で求める。
 * 
 * potato_num:i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数
 * どのじゃがいもから箱詰めするとしても、
 *   X / (W[0]～W[N-1]の総和)
 * の数だけループさせる必要がある。
 * よって上記の端数
 *   X % (W[0]～W[N-1]の総和)
 * について、それぞれのpotato_num[i]ごとに尺取り法で求める。
 */
int main(void)
{
	ll i;
	ll N, Q, X;
	cin >> N >> Q >> X;
	vector<ll> w(N*2);  // 2周させる(尺取り法の部分で楽するため)
	ll w_total = 0;  // w[0]～w[N-1]の総和
	for(i = 0; i < N; i++)
	{
		cin >> w[i];
		w[i+N] = w[i];
		w_total += w[i];
	}

	vector<ll> potato_num(N);  // [i]:i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数

	// 尺取り法でpotato_numを事前計算する
	auto shaku = [&](void) -> void
	{
		ll loop_num = X / w_total;  // 何周必要か
		ll rem = X % w_total;  // その端数  これを尺取り法で求める
		ll sum = 0;
		ll r;
		for(i = 0, r = 0; i < N; i++)
		{
			while(sum < rem)  // 求める重さ以上になるまで
			{
				sum += w[r];
				r++;
			}
			potato_num[i] = N*loop_num + (r-i);
			sum -= w[i];  // 次はpotato_num[i+1]を計算するので、先頭のiを減らしておく
		}
	};
	shaku();

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
