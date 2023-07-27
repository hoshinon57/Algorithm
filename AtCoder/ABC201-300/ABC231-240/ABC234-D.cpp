#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC234 https://atcoder.jp/contests/abc234

/*
 * 1-indexedで解く。
 * 
 * setを使って要素を昇順に保持していく。
 * まずはPの先頭K個を入れておく。
 * 
 * i=Kのとき、
 * 「Pの先頭K個のうち、K番目に大きいもの」＝「先頭K個のうち最小のもの」
 * となる。
 * よってPの先頭K個だけ入れておいたsetについて、
 * begin()が最小値となるのでそれを出力すればよい。
 * 
 * i=K+1, K+2, ..., Nについて、
 * (1)*itr < P[i]のとき
 *    K番目に大きい値、が1つ変化する。
 *    P[i]をsetに挿入し、itrの次の要素を出力すればよい。
 * (2)*itr > P[i]のとき
 *    K番目に大きい値、は変化しない。
 *    よってsetへの挿入は不要、itrの要素を出力すればよい。
 * ※問題文より、*itr=P[i]のケースは存在しない
 */
int main(void)
{
	// 1-indexed
	int i;
	int N, K;
	cin >> N >> K;
	vector<int> P(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> P[i];
	}
	set<int> p_set;  // setを使い、要素を昇順に保持していく
	for(i = 1; i <= K; i++)  // まず、Pの先頭K個を入れる
	{
		p_set.insert(P[i]);
	}

	// i=Kのみ先だって処理する
	// 先頭K個のうち、K番目に大きいもの＝先頭K個のうち最小のもの
	auto itr = p_set.begin();
	cout << *itr << endl;

	// i=K+1, K+2, ... , Nについて処理していく
	for(i = K+1; i <= N; i++)
	{
		// *itr < P[i] なら、p_setに挿入
		if(*itr < P[i])
		{
			p_set.insert(P[i]);
			// K番目に大きいものは、itrの次の要素になる
			itr++;
			cout << *itr << endl;
		}
		// *itr > P[i] なら、上位K番目に入ることは無いため、p_setに挿入しない
		else
		{
			// K番目に大きいものは変わらず
			cout << *itr << endl;
		}
	}

	return 0;
}
