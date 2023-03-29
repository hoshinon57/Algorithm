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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題113より
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://atcoder.jp/contests/arc060/tasks/arc060_c

/*
 * ダブリングで解く。
 * 
 * 各クエリに対して、lower_bound()を用いながら1日ずつ進んでいくのは、
 * 最悪ケースではクエリごとにO(N)かかってしまい、TLEとなる。
 * 
 * まず、a<bとしてよい。
 * ダブリング用のテーブルを以下のように定める。
 *   db[k][i]:i番目のホテルから2^k日移動したときに、到達できる最も遠い(右側の)ホテル
 * db[0][i]は二分探索で求められる。(尺取り法でも良いかも)
 * db[k][i]は、
 *   2^(k-1)日移動し、そこからさらに2^(k-1)日移動した地点
 * なので、db[k-1][db[k-1][i]] で求められる。
 * 
 * クエリに対しては、上位側から順に「地点bに到達しない範囲」で進んでいく。
 * これを各kに対して行うと、bに到達しないギリギリの地点にいるので、+1したものが答。
 * 
 * 計算量は、
 *   事前計算：O(NlogN)
 *   クエリ：O(QlogN)
 * より、全体で O((N+Q)logN) となる。
 */
int main(void)
{
	int i, k;
	int N;
	cin >> N;
	vector<int> x(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i];
	}
	int L;
	cin >> L;

	// db[k][i]:i番目のホテルから2^k日移動したときに、到達できる最も遠いホテル
	const int LOG_N = 20;  // Nの最大は10^5 < 2^20
	vector<vector<int>> db(LOG_N+1, vector<int>(N));
	// db[0][*]を求める
	for(i = 0; i < N; i++)
	{
		db[0][i] = upper_bound(x.begin(), x.end(), x[i]+L) - x.begin() - 1;
	}
	// db[k][*]を求める
	for(k = 1; k <= LOG_N; k++)
	{
		for(i = 0; i < N; i++)
		{
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		int a, b;
		cin >> a >> b;
		a--; b--;
		if(a > b) swap(a, b);  // a < bで考える

		int answer = 0;
		for(k = LOG_N; k >= 0; k--)
		{
			if(db[k][a] < b)  // 地点bに到達しない範囲で進む
			{
				a = db[k][a];
				answer += (1<<k);
			}
		}
		answer++;  // bに到達しないギリギリなので、+1すれば到達する
		cout << answer << endl;

		Q--;
	}

	return 0;
}
