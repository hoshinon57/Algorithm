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

// ABC194 https://atcoder.jp/contests/abc194

/*
 * 尺取り法っぽく解いた。
 * 以下0-indexedで記載する。
 * 
 * 問題文の整数iが1変化したとき、着目する数列の変化は
 *   A(i+M)が追加される
 *   Aiが除去される
 * の2要素のみとなり、変化量は少ないことに着目する。
 * 
 * cnt[i]:今見ているM個の値のうちに、iが何回出てくるか
 * を用意しておく。
 * まず最初、A0～A(M-1)のM要素を探索し、この間のmexを求める。
 * 次に各iについて、
 *   cnt[a[i+M]]を+1
 *   cnt[a[i]]を-1  これが0になれば解の候補
 * としていく。
 * 解の候補にはこれまでの最小値を保持しておけば、最終的にそれが答となる。
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> cnt(N);  // cnt[i]:今見ているM個の値のうちに、iが何回出てくるか

	int ans = INF32;
	// 最初のA0～A(M-1)について計算
	for(i = 0; i < M; i++)
	{
		cnt[a[i]]++;
	}
	for(i = 0; i < N; i++)
	{
		if(cnt[i] == 0)
		{
			ans = i;
			break;
		}
	}
	if(ans == INF32) ans = N;  // N=Mかつ数列Aに0～N-1まで1つずつ含まれるケース(1WAした)

	// 以降、各iについてA(i+M)を追加し、Aiを削除する
	for(i = 0; i+M < N; i++)
	{
		cnt[a[i+M]]++;
		if(--cnt[a[i]] == 0)
		{
			ans = min(ans, a[i]);
		}
	}
	cout << ans << endl;

	return 0;
}
