#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// ABC247 https://atcoder.jp/contests/abc247

int main(void)
{
	int Q;
	cin >> Q;
	
	vector<long long> a, b;  // 数2が3個なら、a[i]=2, b[i]=3
	int nowB = 0;  // 今b[]の何番目から残っているか
	for(int numQ = 0; numQ < Q; numQ++)
	{
		int kind;  // 1か2
		cin >> kind;
		long long x, c;
		long long sum = 0;
		if( kind == 1 )
		{
			cin >> x >> c;
			a.push_back(x);
			b.push_back(c);
		}
		else  // kind==1
		{
			cin >> c;  // c個出す必要がある
			int i;
			for(i = nowB; ; i++)  // 上限チェックは不要
			{
				if(b[i] >= c)  // b[i]で全部出せる
				{
					// 数a[i]を、b[i]から出す
					sum += a[i] * c;
					b[i] -= c;
					c = 0;
				}
				else  // b[i]で全部出せないので、次
				{
					sum += a[i] * b[i];  // c個は出せない、b[i]個まで
					c -= b[i];
					b[i] = 0;
				}
				if( c == 0 )
				{
					cout << sum << endl;
					break;  // 次のQueueへ
				}
			}
			nowB = i;  // 次はここから
		}
	}

	return 0;
}