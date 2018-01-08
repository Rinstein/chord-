#include<string>
using namespace std;
const int n = 15;
const int size = 32371;
class finger
{
public:
	int virtualid[15];//size<2的15次方
	int hostid[15];
	finger() 
	{
		memset(virtualid, 0, sizeof(virtualid)); 
		memset(hostid, 0, sizeof(hostid));
	}
};
