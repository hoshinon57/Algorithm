#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC237 https://atcoder.jp/contests/abc237

/*
 * std::listを使って解く。
 * 参考：http://vivi.dyndns.org/tech/cpp/list0.html
 * 
 * insert(itr, num)にて挿入される場所がitrの手前になる以外は、
 * ABC237-D.cppにて作った自作の双方向リストと同じ。
 */
int main(void)
{
	int N;
	string S;
	cin >> N >> S;

	list<int> lrList;
	auto itr = lrList.begin();
	itr = lrList.insert(itr, 0);

	int count = 1;
	for(auto &e : S)
	{
		// std::listのinsert(itr, num) は、itrの指す要素の前に挿入する
		if(e == 'L')
		{
			itr = lrList.insert(itr, count);  // itrの手前に挿入される
		}
		else  // 'R'
		{
			itr++;
			itr = lrList.insert(itr, count);  // itrの次の手前、つまりitrの次に挿入される
		}
		count++;
	}

	for(itr = lrList.begin(); itr != lrList.end(); itr++)
	{
		if(itr != lrList.begin()) cout << " ";  // 最初の要素以外なら、数値の手前にスペース出力
		cout << *itr;
	}
	cout << endl;

	return 0;
}
