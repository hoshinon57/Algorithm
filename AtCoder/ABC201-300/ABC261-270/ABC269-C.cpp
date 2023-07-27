#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC269 https://atcoder.jp/contests/abc269

/*
 * bit全探索で解く。
 * 
 * まず、Nにてビットが立っている箇所をsub_n[]にて保持する。
 * 例：N=11(0b1011)なら、[0]=0, [1]=1, [2]=3 (0-indexed)
 * 
 * iを 0～2^(sub_n.size())-1 の範囲でループさせて、
 * iのビットが立っている箇所に対応するsub_n[]を、xに加算する。
 * 
 * 「昇順に全て出力してください」とあるが、xは小さい順に生成されるためソートの必要は無い。
 */

// Nにてビットが立っている箇所を順に入れていく
// N=11(0b1011)なら、[0]=0, [1]=1, [2]=3 (0-indexed)
vector<int> sub_n;

// Nからsub_nの生成
void make_bitpos(ll N)
{
	int i = 0;
	while(N > 0)
	{
		if((N&1) == 1)
		{
			sub_n.push_back(i);
		}
		i++;
		N = N >> 1;
	}
}

int main(void)
{
	ll i, j;
	ll N;
	cin >> N;
	make_bitpos(N);

	// bit全探索
	ll sub_n_size = sub_n.size();
	for(i = 0; i < 1<<sub_n_size; i++)  // iについて、bitが1になっているものを選ぶ
	{
		ll sum = 0;
		for(j = 0; j < sub_n_size; j++)
		{
			if(((i >> j)&1) == 1)  // iのjビット目が立っているなら、sub_n[j]ビット目を使う(0-indexed)
			{
				sum += (ll)1<<sub_n[j];
			}
		}
		cout << sum << endl;
	}

	return 0;
}
