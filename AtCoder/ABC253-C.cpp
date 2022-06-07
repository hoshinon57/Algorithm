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

// ABC253 https://atcoder.jp/contests/abc253

/*
 * multisetを用いて処理していけば良い。
 * 
 * また、解説にはmultiset使用時の注意があり勉強になる。
 * erase()で1つだけ消したい場合はイテレータを使う、count()やfind()の計算量について、など。
 * https://atcoder.jp/contests/abc253/editorial/4019
 */
int main(void)
{
	multiset<int> s;
	int i, kind, x, c;
	int Q;
	cin >> Q;
	for(i = 0; i < Q; i++)
	{
		cin >> kind;
		if(kind == 1)
		{
			cin >> x;
			s.insert(x);
//			for(auto &e : s) {cout << e << ", ";}  cout << endl;
		}
		else if(kind == 2)
		{
			cin >> x >> c;
			auto itr = s.lower_bound(x);
			while(*itr == x)
			{
				itr = s.erase(itr);
//				for(auto &e : s) {cout << e << ", ";}  cout << endl;
				c--;
				if(c==0) break;
			}
		}
		else
		{
			auto itrMin = s.begin();
			auto itrMax = s.end();
			itrMax--;
//			cout << *itrMin << " " << *itrMax << endl;
			cout << *itrMax - *itrMin << endl;
		}
	}

	return 0;
}
