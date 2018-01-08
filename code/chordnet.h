#include<string>
#include"host.h"
#include"mymath.h"
using namespace std;
class chordnet
{
public:	
	int size;//最大容量
	int numofhost;
	int path[n];
	bool flag = false;
	host * basichost;
	chordnet(int size) :size(size), numofhost(0), basichost(NULL) {}
	~chordnet()
	{
		host * t = basichost;
		for (int i = 0; i < numofhost; i++)
		{
			host * h = t->next;
			delete t;
			t = h;
		}
	}
	
	host* idtohost(int id); //循环遍历链表网络来寻找已经存在的主机
	host* findinfinger(host * t,int id);//作为路由的递归函数
	host* findhost(int id);//通过路由的方式寻找一个虚拟资源id应该保存的主机位置
	void initfingertable(host *h);
	
	bool Join(int id);
	bool Leave(int id);

	void Insert(word*);
	word* Lookup(string key);
	bool Update(word);
	
	void print();
	void showpath();
};

host* chordnet::idtohost(int id) 
{ 
	if (numofhost==0)
	{
		cout << "网络中并无主机，请加入主机后再进行寻找"<<endl;
		return NULL;
	} 
	host *result = basichost;
	if (result->id != id)
	{
		while (result->next != basichost)
		{
			result = result->next;
			if (result->id == id)
				return result;
		}
		if (result->id == id)
			return result;
		else
			return NULL;
	}
	return result;
}

host* chordnet::findinfinger(host * h, int id)
{
	if (h->id == id)
	{
		if(flag)
		{   
			int i = 1;
			for ( ; i < n; i++)
			{
				if (path[i] < 0)
					break;
			}
			path[i] = h->next->id;
		}
		return h->next;
	}
	for (int i = 0; i < n; i++)
	{
		if (id == h->fingertable.virtualid[i])
		{
			if (flag)
			{
				int i = 1;
				for (; i < n; i++)
				{
					if (path[i] < 0)
						break;
				}
				path[i] = h->fingertable.hostid[i];
			}
			return idtohost(h->fingertable.hostid[i]);
		}
		else if (id == h->fingertable.virtualid[(i + 1) % n])
		{
			if (flag)
			{
				int i = 1;
				for (; i < n; i++)
				{
					if (path[i] < 0)
						break;
				}
				path[i] = h->fingertable.hostid[(i + 1) % n];
			}
			return idtohost(h->fingertable.hostid[(i + 1) % n]);
		}
		if (h->fingertable.virtualid[i] < h->fingertable.virtualid[(i + 1) % n])//第一种结构顺序
		{
			if (id > h->fingertable.virtualid[i] && id < h->fingertable.virtualid[(i + 1) % n])
			{
				if (h->fingertable.hostid[i] > id || h->fingertable.hostid[i] == h->fingertable.hostid[(i + 1) % n])
				{
					if (flag)
					{
						int i = 1;
						for (; i < n; i++)
						{
							if (path[i] < 0)
								break;
						}
						path[i] = h->fingertable.hostid[i];
					}
					return idtohost(h->fingertable.hostid[i]);
				}
				if (flag)
				{
					int i = 1;
					for (; i < n; i++)
					{
						if (path[i] < 0)
							break;
					}
					path[i] = h->fingertable.hostid[i];
				}
				return findinfinger(idtohost(h->fingertable.hostid[i]), id);//常规操作
			}
		}
		else//存在模后的情况，首尾相接的时候
		{
			//下面判断是否位于这种结构之间
			if ((id > h->fingertable.virtualid[i] && id > h->fingertable.virtualid[(i + 1) % n]))
			{   //位于较大端
				if (h->fingertable.hostid[i] > id || h->fingertable.hostid[i] < h->fingertable.virtualid[i])
				{
					if (flag)
					{
						int i = 1;
						for (; i < n; i++)
						{
							if (path[i] < 0)
								break;
						}
						path[i] = h->fingertable.hostid[i];
					}
					return idtohost(h->fingertable.hostid[i]);
				}
				if (flag)
				{
					int i = 1;
					for (; i < n; i++)
					{
						if (path[i] < 0)
							break;
					}
					path[i] = h->fingertable.hostid[i];
				}
				return findinfinger(idtohost(h->fingertable.hostid[i]), id);
			}
			else if(id < h->fingertable.virtualid[i] && id < h->fingertable.virtualid[(i + 1) % n])
			{   //位于较小端
				if (h->fingertable.hostid[i] < h->fingertable.virtualid[i] && h->fingertable.hostid[i] > id)
				{
					if (flag)
					{
						int i = 1;
						for (; i < n; i++)
						{
							if (path[i] < 0)
								break;
						}
						path[i] = h->fingertable.hostid[i];
					}
					return idtohost(h->fingertable.hostid[i]);
				}
				if (flag)
				{
					int i = 1;
					for (; i < n; i++)
					{
						if (path[i] < 0)
							break;
					}
					path[i] = h->fingertable.hostid[i];
				}
				return findinfinger(idtohost(h->fingertable.hostid[i]), id);
			}
		}
	}
}
host* chordnet::findhost(int id) 
{
	if (basichost == NULL)
	{
		cout << "此chordnet虚拟网络中并没有主机！" << endl;
		return NULL;
	}
	else if(basichost->next == basichost)
	{
		return basichost;
	}
	else
	{
		host * h = basichost;
		return findinfinger(h, id);
	}
}

void chordnet::initfingertable(host *h)
{
	for (int i = 0; i < n; i++)
	{
		int myid = (h->id+(1<<i)) % size;
		h->fingertable.virtualid[i] = myid;
		//h->fingertable.hostid[i] = findhost(h->fingertable.virtualid[i])->id;  
		//不能通过路由的方式直接访问主机，因为可能会路由到正在初始化的主机，产生错误，只能遍历网络
	    //加入主机的频率远远低于查找资源的频率，因此可以通过循环遍历来初始化新加入的主机
		host * t = basichost;
		while (true)
		{
			if (myid == t->id)
			{
				h->fingertable.hostid[i] = t->next->id;
				break;
			}
			else if (myid == t->next->id)
			{
				h->fingertable.hostid[i] = t->next->next->id;
				break;
			}
			else
			{
				if (myid > t->id && myid < t->next->id)
				{
					h->fingertable.hostid[i] = t->next->id;
					break;
				}
				else if(t->id > t->next->id)
				{
					if ((myid > t->id && myid >t->next->id )||(myid < t->id && myid <t->next->id))
					{
						h->fingertable.hostid[i] = t->next->id;
						break;
					}
				}
			}
			t = t->next;
		}
	}
}

bool chordnet::Join(int id) 
{ 
	if (id > size || id < 0)
	{
		cout << "所添加主机id过大或者过小，请给主机分配id范围为:[0," << size << "]" << endl;
		return false;
	}
	if (basichost == NULL)
	{
		host *h1 = new host(id);
		basichost = h1;
		basichost->next = basichost;
		numofhost++;
		for (int i = 0; i < n; i++)
		{
			basichost->fingertable.virtualid[i] = (id + (1<<i)) % size;
			basichost->fingertable.hostid[i] = id;
		}
		return true;
	}
	else 
	{
		if (idtohost(id) != NULL)
		{
			cout << "该主机id已存在，不允许再加入相同id的主机！！" << endl;
			return false;
		}
		host * myhost = new host(id);
		//1.主机位置定位,插入
		host * later = findhost(id);
		host * before = idtohost(later->fingertable.hostid[n - 1]);
		while (before->next != later)
			before = before->next;

		before->next = myhost;
		myhost->next = later;
		
		numofhost++;

		//2.资源再分配
		word* wh = later->sourcehead;
		while (wh != NULL)
		{
			if (hashkey(wh->key)%size < myhost->id)
			{
				later->sourcehead = wh->next;
				wh->next = myhost->sourcehead;
				myhost->sourcehead = wh;
			}
		}
		//3.更新finger表
		host * h = basichost;
		while (true)
		{
			if (h == myhost)
			{
				h = h->next;
				if (h == basichost)
					break;
				continue;
			}
			for (int  i = 0; i < n; i++)
			{
				
				if (h->fingertable.hostid[i] == later->id)
				{
					if (h->fingertable.virtualid[i] == before->id)//位于起点处，于before.id相同
						h->fingertable.hostid[i] = myhost->id;
					else//before 到 myhost之间 //不包括端点
					{
						if (myhost->id > before->id)
						{
							if (h->fingertable.virtualid[i] > before->id && h->fingertable.virtualid[i] < myhost->id)
							{
								h->fingertable.hostid[i] = myhost->id;
							}
						}
						else
						{
							if (h->fingertable.virtualid[i] > before->id && h->fingertable.virtualid[i] > myhost->id)
							{
								h->fingertable.hostid[i] = myhost->id;
							}
							else
							{
								if (h->fingertable.virtualid[i] < before->id && h->fingertable.virtualid[i] < myhost->id)
								{
									h->fingertable.hostid[i] = myhost->id;
								}
							}
						}
					}
				}
			}
			h = h->next;
			if (h == basichost)
			{
				break;
			}
		}
		//更新自身fingertable
		initfingertable(myhost);
	}
	return true;
}
bool chordnet::Leave(int id) 
{ 
	host * aimhost = idtohost(id);
	if ( aimhost == NULL)
	{
		cout << "该主机不存在！" << endl;
		return false;
	}
	host * later = aimhost->next;
	//资源后挪
	word * wh = later->sourcehead;
	while (wh != NULL)
	{
		later->sourcehead->next = wh->next;
		wh->next = aimhost->sourcehead;
		aimhost->sourcehead = wh;
	}
	//删除目标主机
	host * before = idtohost(aimhost->fingertable.hostid[n - 1]);
	while (before->next != aimhost)
		before = before->next;
	before->next = later;
	numofhost--;
	//更新finger表
	host * h = basichost;
	int i = numofhost;
	while (i--)
	{
		for (int i = 0; i < n; i++)
		{
			if (h->fingertable.hostid[i] == aimhost->id)
				h->fingertable.hostid[i] = later ->id;
		}
		h = h->next;
	}
	return true; 
}

void chordnet::Insert(word *a) 
{
	int nid = hashkey(a->key)%size;
	host *p = findhost(nid);
	//cout << nid << "," << p->id << endl;
	word *q = p->sourcehead;
	if (q == NULL)
	{
		p->sourcehead = a;
		return;
	}
	a->next = q;
	p->sourcehead = a;
}
word* chordnet::Lookup(string key)
{
	memset(path, -1, sizeof(path));
	path[0] = basichost->id;//必然从元主机开始搜索
	flag = true;
	int nid = hashkey(key) % size;
	host *p = findhost(nid);
	int j = 1;
	for (; j < n; j++)
	{
		if (path[j] < 0)
			break;
	}
	path[j] = p->id;
	word*q = p->sourcehead;
	while (q)
	{
		if (nid == hashkey(q->key) % size)
		{
			cout << q->key << endl << q->mean << endl;
			flag = false;
			return q;
		}
		else
			q = q->next;
	}
	cout << "don't have that word\n";
	flag = false;
	return NULL;
}
bool chordnet::Update(word a) 
{
	int nid = hashkey(a.key)%size;
	host *p = findhost(nid);
	word*q = p->sourcehead;
	while (q)
	{
		if (nid == hashkey(q->key))
		{
			q->mean = a.mean;
			return true;
		}
		else
			q = q->next;
	}
	return false;
}

void chordnet::print()
{
	host*p = basichost;
	for (int i = 1; i <= numofhost; i++)
	{
		cout << "主机编号为：" << i << "   主机id为：" << p->id << endl;
		word*q = p->sourcehead;
		if (q == NULL)
			cout << "该主机资源为空！" << endl;
		else 
			cout << i << "号主机资源列表如下：" << endl;
		while (q != NULL)
		{
			cout << q->key << "(key_id:"<< hashkey(q->key) % size <<") " << q->mean << endl;
			q = q->next;
		}
		p = p->next;
		cout << endl;
	}
}
void chordnet::showpath()
{   
	cout << "路由路径为: ";
	int i = 0;
	for (; i <= n; i++)
	{
		if (path[i + 2] < 0)
			break;
		cout << path[i] << "-->";
	}
	if (path[i] == path[i + 1])
		cout << path[i];
	else
		cout << path[i] << "-->" << path[i+1] << endl;
}
