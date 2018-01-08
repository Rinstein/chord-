#include<string>
using namespace std;
int hashkey(string key)
{
	int result = 0;
	for (int i = 0; i<key.length(); i++)
	{
		result += key[i];
	}
	result = (result * 11 + 11197) * 17 +(result/3+11)*523;
	return result;
}
