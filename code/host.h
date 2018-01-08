#include<string>
#include"source.h"
#include"finger.h"
using namespace std;
class host
{
public:
	int id;
	int numofsource;
	host * next;
	word * sourcehead;
	finger  fingertable;

	host() {}
	host(int id) :id(id),numofsource(0),next(NULL),sourcehead(NULL) {}
	~host()
	{
		word * t = sourcehead;
		for (int i = 0; i < numofsource; i++)
		{
			word * w = t->next;
			delete t;
			t = w;
		}
		delete sourcehead;
	}
	void sethostid(int id) { this->id = id;}
};
