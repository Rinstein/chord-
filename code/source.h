#include<string>
using namespace std;
class word
{
public:
	string key;//使用英语单词作为辨识key
	string mean;
	word * next;

	word():key(NULL), mean(NULL),next(NULL) {}
	word(string id, string means) : key(id), mean(means),next(NULL) {}
	void set(string key, string means);
};
void word::set(string key, string mean)
{
	this->key = key;
	this->mean = mean;
}
