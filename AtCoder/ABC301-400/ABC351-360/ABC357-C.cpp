#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC357 https://atcoder.jp/contests/abc357

// 定義に基づいて、レベル0から順にカーペットを構築していく。

// p^0～p^(n-1)を事前計算して返す
// mが0以外の場合、mで割った余りを設定する
template<typename T>
vector<T> cal_pow_inadv(T p, int n, T m = 0)
{
	vector<T> ret(n);
	ret[0] = (T)1;
	for(int i = 1; i < n; i++) {
		ret[i] = ret[i-1] * p;
		if(m != 0) ret[i] %= m;
	}
	return ret;
}

int main(void)
{
	vector<int> p3 = cal_pow_inadv(3, 7);
	int i, y, x;
	int N; cin >> N;
	vector<vector<vector<int>>> c(6+1);  // 1:#
	c[0].resize(1, vector<int>(1, 1));

	// (yy,xx)を起点に、c[lv]にc[lv-1]をコピー
	auto cp = [&](int yy, int xx, int lv) -> void
	{
		int a, b;
		for(a = 0; a < p3[lv-1]; a++)
		{
			for(b = 0; b < p3[lv-1]; b++)
			{
				c[lv][yy+a][xx+b] = c[lv-1][a][b];
			}
		}
	};

	for(i = 1; i <= 6; i++)
	{
		c[i].resize(p3[i], vector<int>(p3[i], 0));
		for(y = 0; y < 3; y++)
		{
			for(x = 0; x < 3; x++)
			{
				if(y == 1 && x == 1) continue;
				cp(p3[i-1]*y, p3[i-1]*x, i);
			}
		}
	}

	for(y = 0; y < p3[N]; y++)
	{
		for(x = 0; x < p3[N]; x++)
		{
			string s = ".";
			if(c[N][y][x] == 1) s = "#";
			cout << s;
		}
		cout << endl;
	}

	return 0;
}
