#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

// ABC147 https://atcoder.jp/contests/abc147

int bitCount(int n)
{
	int count = 0;
	while(n > 0)
	{
		if((n&1)==1) count++;
		n >>= 1;
	}

	return count;
}

int main(void)
{
	/*
	 * 以下、0-indexedで配列に値を入れていく。
	 * N人についてそれぞれ正直/不親切の全パターンをbit全探索で調べる。
	 * 証言talkは、talk[a-1][b-1]に対して以下とする。
	 *   人aが人bに対する証言  0 or 1
	 *   ただし-1は証言無し
	 */
	ll i, j, k;
	ll N;
	cin >> N;
	vector<vector<ll>> talk(N, vector<ll>(N, -1));  // talk[a][b]  人aが人bに対する証言  0or1 -1は証言無し
	for(i = 0; i < N; i++)
	{
		int tmp;
		cin >> tmp;
		for(j = 0; j < tmp; j++)
		{
			int x, y;
			cin >> x >> y;
			talk[i][x-1] = y;
		}
	}

	/*
	for(i = 0; i < N; i++)
	{
		cout << i << ":";
		for(j = 0; j < N; j++)
		{
			cout << talk[i][j] << " ";
		}
		cout << endl;
	}
	cout << "---" << endl;
	*/

	int answer = 0;
	for(i = 0; i < (1<<N); i++)  // N人についてそれぞれ正直/不親切の全パターン探索
	{
		bool bOK = true;
		for(j = 0; j < N; j++)
		{
			if( ((i>>j)&1) == 0 ) continue;  // 人jは不親切なのでパス
			for(k = 0; k < N; k++)
			{
				if(talk[j][k] == -1) continue;  // 証言無し
				if( (talk[j][k] == 1) && ((i>>k)&1) == 1 )
				{
					;  // とりあえず矛盾は無い
				}
				else if( (talk[j][k] == 0) && ((i>>k)&1) == 0 )
				{
					;  // とりあえず矛盾は無い
				}
				else  // 矛盾が発生した
				{
					bOK = false;
					break;
				}
			}
			if(!bOK) break;
		}
		if(bOK)
		{
			answer = max(answer, bitCount(i));
		}
	}

	cout << answer << endl;

	return 0;
}
