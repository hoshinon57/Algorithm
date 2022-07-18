#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC257 https://atcoder.jp/contests/abc257

/*
 * 二分探索と幅優先探索(BFS)にて解く。
 * 
 * まず、x回訓練をしてS=xとしたときに、
 *   目的を達成できる   -> S=x+1も目的を達成できる
 *   目的を達成できない -> S=x-1も目的を達成できない
 * となる。よって二分探索で解ける。
 * 
 * また、S=xのときに以下を計算する。
 *   各ジャンプ台間を行き来できるか、をグラフで保持する
 *   i番目(i:1～N)のジャンプ台をスタート地点とし、
 *   全てのジャンプ台に行けるかを、全N通りBFSにて計算する
 */
int N;
vector<ll> x, y, power;

// 引数で指定したジャンプ力で、目的を達成できるか
// s:ジャンプ力
bool check(ll s)
{
	int i, j;

	// 各ジャンプ台からどのジャンプ台へ行けるかを調べる
	// graph[i]:i番目のジャンプ台から到達できるジャンプ台を列挙
	// 双方向のグラフではないことに注意。パワーがジャンプ台ごとに設定されているため。
	vector<vector<int>> graph(N);
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(i==j) continue;
			// i->j方向
			if(s * power[i] >= (ll)abs(x[i]-x[j])+(ll)abs(y[i]-y[j]))
			{
				graph[i].push_back(j);  // i->j方向 OK
			}
		}
	}

	for(i = 0; i < N; i++)  // iスタートで全部たどれるか？
	{
		vector<bool> canMove(N, false);  // [i]:i番目のジャンプ台に到達したらtrue
		queue<int> que;
		que.push(i);
		while(!que.empty())
		{
			int v = que.front();
			que.pop();
			canMove[v] = true;
			for(auto &e : graph[v])
			{
				if(canMove[e]) continue;  // すでに来ていたらスルー
				que.push(e);
			}
		}
		// 全てのジャンプ台に来れるか判定
		bool now_ok = true;
		for(j = 0; j < N; j++)
		{
			if(!canMove[j])
			{
				now_ok = false;
				break;
			}
		}
		if(now_ok)  // i番目のジャンプ台をスタート地点として全部たどれる
		{
			return true;
		}
	}

	// どのジャンプ台からスタートしても、目的は達成できない
	return false;
}

int main(void)
{
	// 0-indexed
	cin >> N;
	x.resize(N);
	y.resize(N);
	power.resize(N);
	for(int i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i] >> power[i];
	}

	// lはNG, rはOK
	ll l = 0;
	ll r = (ll)4*1000000000 + 10;  // 距離の最大が4*10^9のため、それより少し大きい値
	while(r - l > 1)
	{
		ll mid = (r+l)/2;
		if(check(mid))
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}

	cout << r << endl;

	return 0;
}
