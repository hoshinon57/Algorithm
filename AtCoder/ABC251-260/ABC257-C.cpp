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
 * 子供と大人をまとめて体重でソートし、軽い方から調べていく。
 * (個人的な感覚としては、ABC256-Dがきっかけで学んだイベントソートに近い)
 * 
 * {体重, 子供or大人の種別}のpairを用意し、これに子供と大人をまとめて登録していく。
 * このとき種別は、問題文とは逆に 0:大人,1:子供 とする。
 * その後、体重で昇順ソート。
 * 同じ体重なら種別が小さい大人が先になる。
 * 
 * ボーダーである実数Xを、最初は0に、以降はソートされた各人の体重ごとに動かしていく。
 * ★「未満 / 以上」という条件より、体重が50であれば50.001といった数を設定するイメージ。
 * 最初はXを0にすると、f(X)は大人の人数になる。
 * 以降、体重ごとにXを設定すると
 *   子供なら判定が正しいので、f(X)は1増加
 *   大人なら判定が誤りなので、f(X)は1減少
 * となる。
 * この最大値を求めればよい。
 * 
 * 種別を「0:大人,1:子供」としたことで、同じ体重なら大人が先、つまり減少が先になる。
 * 子供を先にしてしまうと、最大値の判定が(単純な実装では)うまくいかなくなる。
 */
int main(void)
{
	// 1-indexed
	ll N;
	string S;
	cin >> N;
	cin >> S;
	vector<ll> W(N+1);
	vector<pair<int, int>> person;  // {weight, 種別 0:大人,1:子供}  問題文と0/1が逆であることに注意
	ll answerNow = 0;
	ll answerMax = 0;
	for(int i = 1; i <= N; i++)
	{
		cin >> W[i];
		if(S[i-1] == '0')  // 子供
		{
			person.push_back(make_pair(W[i], 1));  // 種別は1とする
		}
		else  // 大人
		{
			person.push_back(make_pair(W[i], 0));  // 種別は0とする
			answerNow++;  // 最初はボーダーを0とするので、大人の数だけ正解になる
		}
	}
	answerMax = max(answerMax, answerNow);
	sort(person.begin(), person.end());

	for(auto &e : person)
	{
		// 体重が同じ場合、大人が先に配置されている(＝減少の処理が先になる)
		if(e.second == 1) // 子供
		{
			answerNow++;
		}
		else  // 大人
		{
			answerNow--;
		}
		answerMax = max(answerMax, answerNow);
	}
	cout << answerMax <<endl;
	
	return 0;
}
