#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC256 https://atcoder.jp/contests/abc256

/*
 * コンテスト中に解けず、解説を見て実装した。
 * いもす法で解く。
 * 参考：
 *   https://imoz.jp/algorithms/imos_method.html
 *   https://atcoder.jp/contests/abc256/editorial/4121
 * 
 * 記録のフェーズとシミュレートのフェーズ、そして解の算出のフェーズに分ける。
 * 
 * 記録：
 * 各L,Rについて以下のようにtableを作成する。
 *   L:区間開始のイベントとして、table[L]をインクリメント
 *   R:区間終了のイベントとして、table[R]をデクリメント
 * (Lには↑のイベントが、Rには↓のイベントが立っているイメージ)
 * 
 * シミュレート：
 * 1～2*10^5(Rの最大値)までのそれぞれで、table[i] += table[i-1] を計算する。
 * これにより、table[t]:tを含む区間の個数、となる。
 * 
 * 解の算出：
 * tableが0->1以上になる箇所が、区間の開始となる。
 * tableが1以上->0になる箇所が、区間の終了となる。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N;
	cin >> N;

	// 記録
	int imos_table[200010] = {0};  // 2*10^5より少し大きいサイズ
	for(i = 1; i <= N; i++)
	{
		int l, r;
		cin >> l >> r;
		imos_table[l]++;
		imos_table[r]--;
	}

	// シミュレート
	for(i = 1; i <= 200000; i++)
	{
		imos_table[i] += imos_table[i-1];  // table[0]=0
	}

	// シミュレート結果を元に、解の算出
	vector<pair<int,int>> answer;
	int nowL = 0, nowR = 0;
	for(i = 1; i <= 200000; i++)
	{
		// 0->1以上 のときにLが確定する
		// 同じ値のLが来た場合に 0->2 などになるため、 ==1 判定ではなく !=0 判定にする必要がある点に注意
		if(imos_table[i-1] == 0 && imos_table[i] != 0)
		{
			nowL = i;
		}
		// 1以上->0 のときにRが確定する
		if(imos_table[i-1] != 0 && imos_table[i] == 0)
		{
			nowR = i;
			answer.push_back({nowL, nowR});
		}
	}

	for(auto &e : answer)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
