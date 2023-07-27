#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC248 https://atcoder.jp/contests/abc248

/*
 * まず、K=1ならInfを返して終了。
 * K>1であれば、答は有限となる。
 * 
 * 2点を全探索することで、全ての直線について探索する。(2点が決まれば直線が決まる)
 * 直線上にある頂点をsetで管理する。
 * 選んだ2点と、全ての点からもう1つ選び、それら3点が同一直線上にあればsetに追加。
 * setの要素数がK以上であれば、その直線を答に追加する。
 * 
 * 重複の排除は、前述のsetの先頭2つを回答用のsetに入れていくことで実現した。
 * 計算量はO(N^3*logN)となるが、C++では十分に間に合った。(627msほど)
 * 
 * もしくは、N*Nの2次元配列(bool型)checkedを用意し、
 * 2頂点a,bを通る直線について判定済みなら checked[a][b]=true とする手法もある。(公式解説がこれ)
 * この場合O(N^3)であり、49msほどであった。
 */

// 3点が同一直線上にあるか調べる
// 3点A,B,CのA->Bの変化量を(dx1,dy1), A->Cの変化量を(dx2,dy2)とする
// 参考：https://qiita.com/tydesign/items/ab8a5ae52eb9c50ad26a
bool IsColliniar(int dx1, int dy1, int dx2, int dy2)
{
	// dy1/dx1 = dy2/dx2
	// となればよいので、dx1*dx2を両辺にかける
	return dx1*dy2 == dx2*dy1;
}

int main(void)
{
	int i, j, k;
	int N, K; cin >> N >> K;
	vector<int> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	if(K == 1)
	{
		cout << "Infinity" << endl;
		return 0;
	}

	set<pair<int,int>> ans_list;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // i,jを通る直線
		{
			set<int> p;  // 選んだ直線上にある頂点を管理
			p.insert(i);
			p.insert(j);
			for(k = 0; k < N; k++)  // スタートはj+1ではなく0
			{
				if(k == i || k == j) continue;
				if(IsColliniar(x[j]-x[i], y[j]-y[i], x[k]-x[i], y[k]-y[i]))
				{
					p.insert(k);
				}
			}
			if((int)p.size() >= K)
			{
				// 重複排除のため、setの先頭2つを回答用のsetに追加
				auto itr = p.begin();
				int p1, p2;
				p1 = *itr;
				itr++;
				p2 = *itr;
				ans_list.insert({p1, p2});
			}
		}
	}
	cout << ans_list.size() << endl;

	return 0;
}
