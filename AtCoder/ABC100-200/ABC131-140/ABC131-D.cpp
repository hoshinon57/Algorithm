#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC131 https://atcoder.jp/contests/abc131

/*
 * 貪欲法で、締め切りの早いものから順に処理していく。
 * 
 * まず、入力を締め切りが早いものから順にソートする。
 * その後、仕事の順番ごとに
 *   「現在時刻＋仕事にかかる時間 <= 仕事の締め切り」
 * を満たせるかを調べればよい。
 * 全て満たせるならば、問題の条件を満たす。
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<pair<int, int>> time(N);  // {締め切り時刻B, かかる時間A} の順
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		time[i] = make_pair(b, a);
	}
	// 締め切りが早いものから順にソート(pairは第1要素でまずソートする)
	sort(time.begin(), time.end());

	int nowTime = 0;
	bool ok = true;
	for(i = 0; i < N; i++)
	{
		// この仕事は締め切り内に終わるか？
		if(nowTime + time[i].second <= time[i].first)
		{
			// 締め切り内に終わるので、現在時刻を更新
			nowTime += time[i].second;
		}
		else
		{
			ok = false;
			break;
		}
	}

	if(ok) cout << "Yes" << endl;
	else cout << "No" << endl;

	return 0;
}
