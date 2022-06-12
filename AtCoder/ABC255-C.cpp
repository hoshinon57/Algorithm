#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC255 https://atcoder.jp/contests/abc255

/*
 * 場合分けして解く。
 * 
 * 初項A, 公差D, 項数Nのとき、
 * 末項L = A+(N-1)*D
 * 
 * 処理を簡易にするため、D<0であればDの符号を反転し、A,Lを交換しておく。
 * つまり負方向の数列を正方向に置き換えるイメージ。
 * 
 * (1)
 * X<=A、つまり数列の範囲外のとき、A-Xが答え。
 * 
 * (2)
 * L<=Xのときは(1)と同じ考え方で、X-Lが答え。
 * 
 * (3)
 * A<X<L、つまり数列の範囲内のとき。
 * 数列をS(i)としたときに、S(i)<=X<=S(i+1)となるiが存在する。
 * XがS(i)とS(i+1)のどちらに近いかを調べれば良い。
 *   (X-A)mod D が、S(i)との距離。
 *   Dから上記の値を引いたものが、S(i+1)との差分。
 * 小さい方が答えとなる。
 * 
 * コンテスト時、(3)にてDによる除算を行うにも関わらず
 * D=0の場合分けを忘れてしまっていた。
 * ただ(1),(2)の判定により、D=0時は(3)に遷移しないので問題なし。（結果オーライ）
 */
int main(void)
{

	ll x, a, d, n, l;
	cin >> x >> a >> d >> n;
	l = a+(n-1)*d;  //末項

	// 後の処理を簡易にするため、D<0であれば反転しておく
	if(d < 0)
	{
		swap(a, l);
		d = -d;
	}

	if(x <= a)
	{
		cout << a-x << endl;
	}
	else if(x >= l)
	{
		cout << x-l << endl;
	}
	else
	{
		ll tmp1, tmp2;
		tmp1 = (x-a)%d;
		tmp2 = d - tmp1;
		cout << min(tmp1, tmp2) << endl;
	}

	return 0;
}
