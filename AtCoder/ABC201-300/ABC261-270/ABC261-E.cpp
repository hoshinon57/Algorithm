#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC261 https://atcoder.jp/contests/abc261

/*
 * コンテスト中に解けず、解説を見て実装した。
 * and/or/xorはbitごとに独立のため、各bitごとに計算する。
 * 
 * 愚直に全操作を行うと操作回数が 1+2+3+...+N となり、計算量がO(N^2)となってTLE.
 * そこで、操作1,2,...,k の次は操作1,2,...,k,k+1であることから、
 * 操作1,2,...k に0および1を入力したときの結果をfunc(k)として保持しておくことで、
 * 次のフェーズはfunc(k)と操作k+1を行えば出力を得られる。
 * 
 * func[n][bit][in]を以下のように定める(0-indexedに置き換えている、注意)
 *   操作0,1,...,n までを順に行う場合、
 *   Xの該当bitがin(0 or 1)のときに、
 *   操作によってXの該当bitが何になるか、を保持する
 * 
 * func[0][*][0] = 0, func[0][*][1] = 1 として、また
 * func[i][*][0] = func[i-1][*][0],
 * func[i][*][1] = func[i-1][*][1]
 * として初期化する。
 * それぞれのfuncに"操作"によるbit変化を計算すれば、
 * 後は入力としてXの該当ビットを入れてやればよい。
 */

int func[200000][30][2] = {0};

int main(void)
{
	// 0-indexed
	int i, j;
	int N, C;
	cin >> N >> C;
	vector<int> t(N), a(N);
	for(i = 0; i < N; i++)
	{
		cin >> t[i] >> a[i];
	}

	vector<int> answer(N);
	for(i = 0; i < 30; i++)
	{
		int num_c = (C>>i)&1;  // Cのiビット目
		for(j = 0; j < N; j++)
		{
			if(j == 0)
			{
				func[0][i][0] = 0;
				func[0][i][1] = 1;
			}
			else
			{
				// 前のフェーズの操作を元にする
				func[j][i][0] = func[j-1][i][0];
				func[j][i][1] = func[j-1][i][1];
			}

			int num_a = (a[j]>>i)&1;
			if(t[j] == 1)
			{
				func[j][i][0] &= num_a;
				func[j][i][1] &= num_a;
			}
			else if(t[j] == 2)
			{
				func[j][i][0] |= num_a;
				func[j][i][1] |= num_a;
			}
			else
			{
				func[j][i][0] ^= num_a;
				func[j][i][1] ^= num_a;
			}
			// 操作1,2,...,(j+1)までを行ったときのXの値(iビット目)
			// 次はこの値を元に操作を行っていく
			num_c = func[j][i][num_c];

			answer[j] |= (num_c<<i);
		}
	}

	for(i = 0; i < N; i++)
	{
		cout << answer[i] << endl;
	}

	return 0;
}
