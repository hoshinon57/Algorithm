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

// ABC136 https://atcoder.jp/contests/abc136

/*
 * ダブリングで解く。
 * 
 * 以下の方向で考えていった。
 * ・いずれ"RL"の部分で周期2でループしそう。
 * ・Sは最大10^5なので、それ以下でループが始まりそう。
 * ・ダブリングで解くとして、
 *   10^100は偶数なので、偶奇が同じで10^5より大きい2^20程度で良さそう。
 * 
 * ダブリング用の変数db[k][i]を以下のように定める。
 *   iマス目から2^k回移動したときにいるマス
 * db[0][i]は入力Sから求まる。
 * db[k][i]は、iマス目から2^(k-1)回移動したマスから、さらに2^(k-1)回移動したマスとなり、db[k-1][db[k-1][i]]となる。
 * 
 * 上記の事前計算を元に、各マスを起点に2^20回移動した先を計算してカウントしたものが答となる。
 */
int main(void)
{
	// 0-indexed
	int i, k;
	string S;
	cin >> S;
	const int LOG_S = 20;  // 10^5 < 2^20
	int N = S.size();
	vector<vector<int>> db(LOG_S+1, vector<int>(N));
	
	// db[0][*]の計算
	for(i = 0; i < N; i++)
	{
		db[0][i] = (S[i]=='R') ? (i+1) : (i-1);
	}
	// db[k][*]の計算
	for(k = 1; k <= LOG_S; k++)
	{
		for(i = 0; i < N; i++)
		{
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

	vector<int> count(N);
	// iマス目を起点に 2^LOG_S 回移動した後の位置を求める
	for(i = 0; i < N; i++)
	{
		int d = db[LOG_S][i];
		count[d]++;
	}
	for(i = 0; i < N; i++)
	{
		cout << count[i];
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
