#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC257 https://atcoder.jp/contests/abc257

/*
 * ABC257-C.cppの少し別の実装。公式解説のコードをヒントにした。
 * {体重, 子供or大人の種別}のpairについて、種別を問題文と同じく 0:子供,1:大人 とする。
 * 
 * 同じ体重の人を全て処理したときにanswerMaxを更新する、というのが変化点。
 * 同じ体重なら子供が先に並ぶため、同じ体重に
 *   子供、子供、大人、大人、大人
 * がいると、最終的にはanswerNowが1減算になるはずが、途中でanswerNow+2がanswerMaxに登録されてしまう。
 * そのため、answerMaxの更新は「同じ体重の人を全て処理したとき」とする。
 */
int main(void)
{
	// 1-indexed
	ll N;
	string S;
	cin >> N;
	cin >> S;
	vector<ll> W(N+1);
	vector<pair<int, int>> person;  // {weight, 種別 0:子供,1:大人}
	ll answerNow = 0;
	ll answerMax = 0;
	for(int i = 1; i <= N; i++)
	{
		cin >> W[i];
		if(S[i-1] == '0')  // 子供
		{
			person.push_back(make_pair(W[i], 0));
		}
		else  // 大人
		{
			person.push_back(make_pair(W[i], 1));
			answerNow++;  // 最初はボーダーを0とするので、大人の数だけ正解になる
		}
	}
	answerMax = max(answerMax, answerNow);
	sort(person.begin(), person.end());

	ll weightNow = person[0].first;
	for(auto &e : person)
	{
		// answerMaxの更新は「同じ体重の人を全て処理したとき」とする。
		if(weightNow != e.first)
		{
			answerMax = max(answerMax, answerNow);
			weightNow = e.first;
		}
		if(e.second == 0) // 子供
		{
			answerNow++;
		}
		else  // 大人
		{
			answerNow--;
		}
	}
	answerMax = max(answerMax, answerNow);  // 最後にもう1回更新
	cout << answerMax <<endl;
	
	return 0;
}
