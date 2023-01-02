#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題83
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2015ho/tasks/joi2015ho_a

/*
 * いもす法で解く。
 * 
 * まず、各鉄道に乗る回数をいもす法で求める。
 * このとき、いもす法のシミュレートの処理を行うために、
 * 都市4->2といった「数値の小さい方への移動」でも、2->4への移動として処理する。
 * 
 * 次に運賃計算をする。
 * 各鉄道にて乗る回数が算出できたので、
 *   紙の切符で乗車する場合：A円x乗る回数
 *   ICカードで乗車する場合：B円x乗る回数+C円
 * のうち安い方を採用すればよい。
 * 
 * 計算量はO(M+N).
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> p(M);
	for(i = 0; i < M; i++)
	{
		cin >> p[i];
		p[i]--;  // 0-indexedに変換
	}
	vector<ll> a(N-1), b(N-1), c(N-1);  // 鉄道はN-1本
	for(i = 0; i < N-1; i++)
	{
		cin >> a[i] >> b[i] >> c[i];
	}

	// いもす法 記録フェーズ
	vector<ll> train_imos(N);
	for(i = 0; i+1 < M; i++)  // 各iについて、P(i)とP(i+1)間を調べる
	{
		int t1, t2;  // t1->t2への移動
		// 都市4->2といった移動でも、2->4への移動として処理する
		t1 = min(p[i], p[i+1]);
		t2 = max(p[i], p[i+1]);
		train_imos[t1]++;
		train_imos[t2]--;
	}

	// いもす法 シミュレートのフェーズ
	for(i = 1; i < N; i++)
	{
		train_imos[i] += train_imos[i-1];
	}

	// いもす法 解の算出のフェーズ
	ll answer = 0;
	for(i = 0; i < N-1; i++)
	{
		int n = train_imos[i];
		answer += min(a[i]*n, b[i]*n+c[i]);
	}
	cout << answer << endl;
	
	return 0;
}
