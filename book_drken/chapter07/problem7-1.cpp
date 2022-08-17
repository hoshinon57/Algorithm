#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題7.1

/*
 * a,bをそれぞれ昇順ソートしておく。
 * 
 * aの小さい順に、ペアを作れるbを探索していく。ペアを作れたら次のaへ。
 * 終端まで来たら、aの参照番号が答となる。
 */
int main(void)
{
	int i, j;
	int N;
	cin >> N;
	vector<int> a(N), b(N);
	for(i = 0; i < N; i++) cin >> a[i];
	sort(a.begin(), a.end());
	for(i = 0; i < N; i++) cin >> b[i];
	sort(b.begin(), b.end());

	i = j = 0;
	while(i < N && j < N)
	{
		if(a[i] < b[j])
		{
			i++;  // ペア作成OK aを次へ
		}
		j++;  // ペア作成OK/NGに関係なく、bを次へ
	}
	cout << i << endl;  // iがペア作成数

	return 0;
}
