#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC243 https://atcoder.jp/contests/abc243

/*
 * 愚直に移動すると数値が大きくなりすぎるため、移動を削減する。
 * 制約：高橋君が N 回の移動後にいる頂点の番号は 10^18 以下である
 * 
 * R->U または L->U という移動は、移動しないことと同じ。
 * つまりUが来たとき、手前にL/Rがあれば相殺できる。
 * 
 * よってまずmove[]とnextを用いて移動を削減したものを作る。
 * 次にmove[]を使ってXから移動していけばよい。
 */
int main(void)
{
	ll i;
	ll N, X;
	string S;
	cin >> N >> X >> S;

	// U:0, L:1, R:2
	vector<ll> move(N);
	ll next = 0;  // 次はmove[next]に入れる
	for(i = 0; i < N; i++)
	{
		switch(S[i])
		{
		case 'U':
			// 手前にL/Rがあれば相殺
			if(next == 0 || move[next-1] == 0)  // next-1の範囲外アクセスは、next==0によりガードされる(はず)
			{
				// いま先頭、または1つ前がUなので、相殺しない
				move[next] = 0;
				next++;
			}
			else
			{
				// 相殺
				next--;				
			}
			break;
		case 'L':
			move[next] = 1;
			next++;
			break;
		case 'R':
			move[next] = 2;
			next++;
			break;
		}
	}

	// 作り直した操作で移動する
	for(i = 0; i < next; i++)
	{
		switch(move[i])
		{
		case 0:  // U
			X /= 2;
			break;
		case 1:  // L
			X *= 2;
			break;
		case 2:  // R
			X = X*2+1;
			break;
		}
	}
	cout << X << endl;

	return 0;
}
