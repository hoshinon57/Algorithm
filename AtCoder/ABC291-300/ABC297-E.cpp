#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC297 https://atcoder.jp/contests/abc297

/*
 * コンテスト中に解けず、解説を見た。
 * 優先度付きキューを用いて解く。
 * 
 * まず例として、A1～Anがそれぞれキューに入った状態を考える。
 * A1が最小値と仮定すると、A1+A1,A1+A2, ..., A1+Anをそれぞれキューに加える。
 * すると、A1の次に小さい値はキューの中に必ず存在するので、キューの先頭アクセスにて取得できる。
 * 
 * つまり、あるキューの状態において
 *   キューの先頭を取得 -> vとする
 *   v+A1, v+A2, ..., v+An をそれぞれキューに追加
 * を繰り返すことで、安い方から順に取得できる。
 * ただし異なる買い方で同じ金額になるケースがあるので、上記vが前回vと同じであれば、スルーする。
 *   
 * 計算量は、キューへのアクセスがNK回なので、O(NKlog(NK)).
 */

int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}

	priority_queue<ll, vector<ll>, greater<ll>> que;
	ll q, q_prev = -1;
	que.push(0);
	K++;  // 初回の0円のぶん
	while(K > 0)
	{
		q = que.top();
		que.pop();
		if(q == q_prev) continue;  // 前回と同じ値はスルー

		// キューの最小値に、各Aiを加算したものをキューに積む
		// すると、次の最小値は必ずキューの中に存在する
		for(i = 0; i < N; i++)
		{
			que.push(q + a[i]);
		}

		q_prev = q;
		K--;
	}
	cout << q << endl;

	return 0;
}
