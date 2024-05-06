#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC091 https://atcoder.jp/contests/abc091

/*
 * 貪欲法で解く。
 * 参考：https://drken1215.hatenablog.com/entry/2021/07/05/183800
 * 
 * 赤い点をx座標で昇順ソートしておく。(xが同じならyが小さい順に)
 * 赤い点の終端、つまりxが大きいものから順に探索する。
 * 着目している赤い点と、各青い点について仲良しペアになれるかを判定し、
 * ペアになれるもののうち青い点のy座標が最小のものを、最終的にペアとする。
 * 
 * 後はペアの作成数を保持しておき、出力すればよい。
 */
int main(void)
{
	int i, j;
	int N;
	cin >> N;
	vector<pair<int,int>> red(N), blue(N);  // pairはx,yの順
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		red[i].first = a;
		red[i].second = b;
	}
	sort(red.begin(), red.end());  // redはx座標でソート xが同じならyが小さい順
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		blue[i].first = a;
		blue[i].second = b;
	}

	int answer = 0;
	for(i = N-1; i >= 0; i--)  // red[]でxが大きいものから調査
	{
		int min_y = INF32;  // red[i]とペアになれるblue[]のうち、y座標が最小のもの
		int blue_index;
		for(j = 0; j < N; j++)
		{
			if(red[i].first < blue[j].first && red[i].second < blue[j].second)  // ペアにできるか
			{
				if(min_y > blue[j].second)
				{
					min_y = blue[j].second;
					blue_index = j;
				}
			}
		}
		if(min_y != INF32)  // ペアにできるものがあった
		{
			answer++;
			blue[blue_index].second = -1;  // 以降ペアに使われないよう、下方向の範囲外に
		}
	}

	cout << answer << endl;

	return 0;
}
