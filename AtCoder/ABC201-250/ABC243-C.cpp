#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC243 https://atcoder.jp/contests/abc243

/*
 * 同じY座標の人を集めて、その中で
 * 1)左に向かう人のうち、X座標の最大
 * 2)右に向かう人のうち、X座標の最小
 * を求める。
 * (1)>(2)なら衝突が発生する。
 * 
 * multisetを使ったが、vectorでも問題なく解けるかと思う。
 */

struct Person
{
	ll x;
	ll y;
	int dir; // 0:L 1:R
	bool operator<(const Person& a) const {  // multisetで扱うために必要
        return y < a.y;
    }	
};

int main(void)
{
	int i, j;
	int N;
	cin >> N;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
	}
	string s;
	cin >> s;

	multiset<Person> p;
	for(i = 0; i < N; i++)
	{
		Person tmp;
		tmp.x = x[i];
		tmp.y = y[i];
		if(s[i] == 'L')  tmp.dir = 0;
		else  tmp.dir = 1;
		p.insert(tmp);
	}

	// left_x_max  = 左に向かう人のうち、X座標の最大
	// right_x_min = 右に向かう人のうち、X座標の最小
	// left_x_max > right_x_minなら衝突する
	ll nowY;
	ll left_x_max, right_x_min;
	bool bClash = false;  // 衝突するならtrue
	for(auto itr = p.begin(); itr != p.end();)
	{
		left_x_max = -1;  // 判定処理を楽にするため、初期値は範囲外にしておく
		right_x_min = INF64;
		nowY = itr->y;
		for(; itr != p.end(); itr++)  // 同じy座標について探索
		{
			if(nowY != itr->y) break;
			if(itr->dir == 0)  // left
			{
				left_x_max = max(left_x_max, itr->x);
			}
			else  // right
			{
				right_x_min = min(right_x_min, itr->x);
			}
		}
		if(left_x_max > right_x_min)
		{
			bClash = true;
			break;
		}
	}

	if(bClash) cout << "Yes" << endl;
	else cout << "No" << endl;

	return 0;
}
