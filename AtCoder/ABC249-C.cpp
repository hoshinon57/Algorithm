#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

int main(void)
{
	int i, j, k;
	int N, K;
	cin >> N >> K;
	vector<string> str(N);
	for(i=0; i<N; i++)
	{
		cin >> str[i];
	}

	int maxCount = 0;
	for( i = 0; i < (1<<N); i++ )  // bitが1になっているものを選ぶ
	{
		int count = 0;
		int num[26] = {0};
		for( j = 0; j < N; j++ )
		{
			if( ((i>>j)&1) == 0 ) continue;  // str[j]を選ばない
//			cout << i << " " << j << endl;
			for(k = 0; k < (int)str[j].length(); k++)
			{
				num[str[j][k]-'a']++;
			}
		}

		for( j = 0; j < 26; j++ )
		{
			if(num[j] == K) count++;
		}
		maxCount = max(maxCount, count);
	}

	cout << maxCount << endl;

	return 0;
}
