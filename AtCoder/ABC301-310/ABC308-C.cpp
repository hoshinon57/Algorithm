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

// ABC308 https://atcoder.jp/contests/abc308

/*
 * 成功率をdoubleで管理すると、誤差でWAとなるテストケースが存在する。
 * 整数で管理する。
 * 
 * X1/Y1とX2/Y2の比較は、分母を通分して分子の大小を見る。
 * X1*Y2/Y1*Y2 と X2*Y1/Y1*Y2 で通分すると、
 * X1*Y2 と X2*Y1の大小関係を見ればよい。
 * 
 * 上記の比較関数を作り、それをsort()の引数に渡してソートする。
 */

struct person
{
	ll a, b;
	int no;
	person(ll a_, ll b_, int no_) : a(a_), b(b_), no(no_) {}
};

bool comp(const person &a, const person &b)
{
	// 分母を揃えて分子を比較(降順)
	// 分子が同じ場合、番号で比較(昇順)
	ll n1 = a.a*(b.a+b.b);
	ll n2 = b.a*(a.a+a.b);
	if(n1 != n2)
	{
		return n1 > n2;
	}
	return a.no < b.no;
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<person> p;
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		p.push_back({a, b, i+1});
	}
	sort(p.begin(), p.end(), comp);

	for(auto &e : p)
	{
		cout << e.no << " ";
	}
	cout << endl;

	return 0;
}
