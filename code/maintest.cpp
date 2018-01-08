#include<iostream>
#include"chordnet.h"
#include<stdlib.h>
#include<fstream>
using namespace std;
const int N = 32371;//（素数）网络中最大的id数+1 
const int sizeoffingerdata = n;//15为N的以二为底的对数的下整
int main()
{
	chordnet mynet(N);

	string control;
	cout << endl;
	cout << "————————————————————欢迎使用由Rinstein团队开发的chord网络模拟器——————————————————————" << endl;
	cout << endl;
	cout << "    注：在下面的体验过程中，按下任意字符回车即可进入下一个过程" << endl;
	cin >> control;
	cout << "虚拟IP范围为 ：[" << 0 << "," << N << "]" << endl;
	cin >> control;

	cout << "使用随机函数生成包含20个主机的网络: " << endl << "loading----------------------" << endl;
	cin >> control;
	for (int i = 0; i < 20; i++)
	{
		int id = (rand()/2 + 21957 )*3% N;
		if (!mynet.Join(id))
			i--;
	}

	cout << "初始化主机之后，整个网络的结构如下（第一个加入的为元主机）:" << endl;
	cin >> control;
	mynet.print();
	cin >> control;

	cout << "自动加载单词资源" << endl << "loading----------------" << endl;
	ifstream a20,b20,m10;
	string key, means;
	a20.open("H:\\C++ Sudio project\\chord网络模拟\\a20.txt");
	for (int i = 0; i < 20; i++)
	{
		getline(a20, key);
		getline(a20, means);
		word *w = new word(key, means);
		mynet.Insert(w);
	}
	a20.close();
	b20.open("H:\\C++ Sudio project\\chord网络模拟\\b20.txt");
	for (int i = 0; i < 20; i++)
	{
		getline(b20, key);
		getline(b20, means);
		word *w = new word(key, means);
		mynet.Insert(w);
	}
	b20.close();
	m10.open("H:\\C++ Sudio project\\chord网络模拟\\m10.txt");
	for (int i = 0; i < 10; i++)
	{
		getline(m10, key);
		getline(m10, means);
		word *w = new word(key, means);
		mynet.Insert(w);
	}
	m10.close();
	cout << "添加单词后，当前网络状况如下：" << endl;
	cin >> control;
	mynet.print();
	
	cin >> control;
	cout << "展示路由过程:(设置三条展示)" << endl;
	cin >> control;
	cout << endl;
	mynet.Lookup("abroad");
	mynet.showpath();
	cout << endl;
	mynet.Lookup("absolutely");
	mynet.showpath();
	cout << endl;
	mynet.Lookup("teacher");
	mynet.showpath();
	cout << endl;

	cin >> control;
	cout << "搜索单词面板开启：" << endl;
	cin >> control;
	cout << endl;
	while (1)
	{
		cout << "请输入搜索单词(输入0结束搜索)：";
		string s;
		cin >> s;
		if (s == "0")
			break;
		if (mynet.Lookup(s) != NULL)
		{
			mynet.showpath();
			cout << endl;
		}
	}

	cout << endl;
	cout << "*****************************感谢体验chord网络模拟器********************************" << endl;
	cout << endl;
	return 0;
}
