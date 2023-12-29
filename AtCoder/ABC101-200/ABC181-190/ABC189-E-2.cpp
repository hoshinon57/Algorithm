#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC189 https://atcoder.jp/contests/abc189

/*
 * 各種解説を参考に、アフィン変換で解く。
 *   https://atcoder.jp/contests/abc189/editorial/539
 *   https://zenn.dev/ac/articles/9bd08f9012f8ca
 *   https://kyo-pro.hatenablog.com/entry/ABC189E
 *   https://nullkara.jp/2021/01/24/sankakiabc189/#toc6
 *   https://imagingsolution.blog.fc2.com/blog-entry-284.html
 * 
 * 時計回りに90度  反時計回りに90度
 * ( 0 1 0        (0 -1 0
 *  -1 0 0         1  0 0
 *   0 0 1)        0  0 1)
 * のように書ける。また、
 * x=pで反転   y=pで反転
 * (-1 0 2p   (1  0  0
 *   0 1  0    0 -1 2p
 *   0 0  1)   0  0  1)
 * のように書ける。
 * ope[i]を上記とope[i-1]の積で設定していけば、  ※ope[0]は単位行列
 * クエリについてope[a]を使ってO(1)で計算できる。
 * 
 * ※実装たいへんだった
 */

using vvll = vector<vector<ll>>;
vvll func(vvll op1, vvll op2)
{
	int i, j, k;
	vvll ans(3, vector<ll>(3));
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			for(k = 0; k < 3; k++)
			{
				ans[i][j] +=op1[i][k]*op2[k][j];
			}
		}
	}
	return ans;
}

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];
	int M; cin >> M;
	vector ope(M+1, vector(3, vector<ll>(3, 0)));  // ope[M+1][3][3]
	ope[0] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	vector<vector<ll>> f1(3, vector<ll>(3));
	vector<vector<ll>> f2(3, vector<ll>(3));
	vector<vector<ll>> f3(3, vector<ll>(3));
	vector<vector<ll>> f4(3, vector<ll>(3));
	f1 = {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}};
	f2 = {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}};

	for(i = 1; i <= M; i++)
	{
		int kind; cin >> kind;
		if(kind == 1)
		{
			ope[i] = func(f1, ope[i-1]);
		}
		else if(kind == 2)
		{
			ope[i] = func(f2, ope[i-1]);
		}
		else if(kind == 3)
		{
			int p; cin >> p;
			f3 = {{-1, 0, p*2}, {0, 1, 0}, {0, 0, 1}};
			ope[i] = func(f3, ope[i-1]);
		}
		else
		{
			int p; cin >> p;
			f4 = {{1, 0, 0}, {0, -1, p*2}, {0, 0, 1}};
			ope[i] = func(f4, ope[i-1]);
		}
	}

#if 0
	for(i = 0; i <= M; i++)
	{
		cout << "op:" << i << endl;
		for(j = 0; j < 3; j++)
		{
			for(k = 0; k < 3; k++)
			{
				cout << ope[i][j][k] << " ";
			}
			cout << endl;
		}
		cout << "---" << endl;
	}
#endif
	int Q; cin >> Q;
	vector<vector<ll>> p(3, vector<ll>(3));
	while(Q > 0)
	{
		Q--;
		int a, b; cin >> a >> b;
		b--;
		ll tmp1 = ope[a][0][0]*x[b] + ope[a][0][1]*y[b] + ope[a][0][2]*1;
		ll tmp2 = ope[a][1][0]*x[b] + ope[a][1][1]*y[b] + ope[a][1][2]*1;
		cout << tmp1 << " " << tmp2 << endl;
	}

	return 0;
}
