#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC271 https://atcoder.jp/contests/abc271

/*
 * 二分探索で解く。
 * コンテスト中はシミュレーションで解いたが、解説を見て二分探索の練習として実装。
 * 
 * 「1～x巻まで読むことができるか」の判定問題を考えると、
 *   1～x巻まで読める
 *   1～(x+1)巻までは読めない
 * という境界が存在する。
 * このxが答であり、二分探索で求める。
 * 
 * 上記の判定問題は、
 * (1)売っても良い冊数
 * (2)持っていない（ので買いたい）冊数
 * をそれぞれ計算し、(1)を売ることで(2)を全て買えるか判定すればよい。
 * 
 * 計算量はO(NlogN)になると思われる。
 * 二分探索の部分がO(logN)で、判定関数がO(N).
 * ※unordered_setによる操作は平均的にO(1)
 * 
 * unordered_setではなくsetを使っても正しい答が得られる。
 * ただしsetの操作はO(logN)であり、計算量はO(NlogN*logN)となる。
 * 実際、提出したら実行時間はTLEギリギリだった(3933ms)。
 */

// x巻まで読めるならtrue
bool check(int x, vector<int> &a)
{
	int i;

	// まず、売っても良い冊数を計算する。
	// 「x巻よりあとの巻」「ダブリ」が対象となる。
	unordered_set<int> s;  // 1～x巻で持っている本の集合（重複除く）
	// set でも可 ただし計算量がO(N*logN*logN)になり、TLEギリギリだった
	int sell = 0;
	int N = (int)a.size();
	for(i = 0; i < N; i++)
	{
		if(a[i] > x)  // x巻より後の巻は売却対象
		{
			sell++;
			continue;
		}
		if(s.count(a[i]) > 0)  // 重複しているなら売却対象
		{
			sell++;
			continue;
		}

		s.insert(a[i]);
	}

	// 次に1～x巻までのうち、持っていない（ので買いたい）冊数を計算する
	int buy = 0;
	for(i = 1; i <= x; i++)
	{
		if(s.count(i) == 0) buy++;
	}

	// 買いたい冊数を全て買えるか判定
	if(sell/2 >= buy)  // 2冊売って1冊買う
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main(void)
{
	int N;
	cin >> N;
	vector<int> a(N);
	for(int i = 0; i < N; i++) cin >> a[i];

	int l = 0;  // 「0巻まで読める」は必ずtrue
	int r = N+1;  // 「N+1巻まで読める」は必ずfalse
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid, a))
		{
			l = mid;
		}
		else
		{
			r = mid;
		}
	}
	cout << l << endl;

	return 0;
}
