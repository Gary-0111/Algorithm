//Splay Tree


struct Node
{
    int m,s;
    Node *pre, *ch[2];
    Node()
    {
	pre = NULL;
	ch[0] = ch[1] = NULL;
    }
    Node(int _m): m(_m)
    {
	pre = NULL;
	ch[0] = ch[1] = NULL;
    }
    bool operator < (const Node &a) const 
    {
	return m<a.m;
    }
    void update()
    {
	s = 1;
	if(ch[0]) s += ch[0]->s;
	if(ch[1]) s += ch[1]->s;
    }
};

struct SplayTree
{
    Node *Root;

    //--------debug--------
    void Travel(Node *rt)
    {
	if(rt)
	{
	    Travel(rt->ch[0]);
	    printf("rt: %d  lson: %d  rson: %d\n",rt->m,(rt->ch[0]?rt->ch[0]->m:-1),(rt->ch[1]?rt->ch[1]->m:-1));
	    Travel(rt->ch[1]);
	}
    }
    //------debug-end------

    void Init()
    {
	if(Root) Release(Root);
	Root = NULL;
    }
    
    //c=0 Left_Rotate; c=1 Right_Rotate
    void Rotate(Node *x, int c)
    {
	Node *y = x->pre;
	Node *z = y->pre;
	y->ch[!c] = x->ch[c];
	if(x->ch[c])
	    x->ch[c]->pre = y;
	x->pre = z;
	if(z) z->ch[z->ch[1]==y] = x;
	y->pre = x;
	y->update();
	x->ch[c] = y;
    }

    void Splay(Node *x,Node *anc)
    {
	while(x->pre!=anc)
	{
	    Node *y = x->pre;
	    Node *z = y->pre;
	    if(z==anc) Rotate(x,y->ch[0]==x);
	    else
	    {
		if(z->ch[0] == y)
		{
		    if(y->ch[0] == x) Rotate(y,1);
		    else Rotate(x,0);
		    Rotate(x,1);
		}
		else
		{
		    if(y->ch[1] == x) Rotate(y,0);
		    else Rotate(x,1);
		    Rotate(x,0);
		}
	    }
	}
	x->update();
	if(anc == NULL) Root = x;
    }

    //将第k个节点伸展到anc下面
    void *Select(int k, Node *anc)
    {
	int tmp = 0;
	Node *t;
	for(t = Root;;)
	{
	    tmp = (t->ch[0]?t->ch[0]->s:0);
	    if(k == tmp+1) break;	    //当前节点即为第k个节点
	    if(k <= tmp) t = t->ch[0];
	    else k -= tmp+1, t = t->ch[1];
	}
	Splay(t,anc);
    }

    void Insert(Node *x)
    {
	if(Root == NULL)
	{
	    Root = x;
	    Root->update();
	}
	else
	{
	    Node *y = Root;
	    while(y->ch[(*y<*x)])
		y = y->ch[(*y<*x)];
	    y->ch[(*y<*x)] = x;
	    x->pre = y;
	    Splay(x,NULL);
	}
    }
    /*Node *Find(int id)
    {
	Node *x = Root;
	while(x&&x->id!=id)
	    x = x->ch[(x->id)<id];
	if(x) Splay(x,NULL);
	return x;
    }*/
    //获得x的前驱
    Node * GetPre(Node *x)
    {
	Node *Pre = x->ch[0];
	while(Pre&&Pre->ch[1])
	    Pre = Pre->ch[1];
	return Pre;
    }
    //获得x的后继
    Node * GetSuc(Node *x)
    {
	Node *Suc = x->ch[1];
	while(Suc&&Suc->ch[0])
	    Suc = Suc->ch[0];
	return Suc;
    }
    void Delete(Node *x)
    {
	Splay(x,NULL);		    //将x伸展到根节点，即要删除根节点
	if(x->ch[0]&&x->ch[1])	    //如果左右儿子都存在
	{
	    Node *suc = GetSuc(x);  //获取当前节点的后继
	    Splay(suc, x);	    //将后继节点suc伸展到x下面，保证suc的左儿子为空
	    Root = suc;
	    x->ch[0]->pre = Root;
	    Root->ch[0] = x->ch[0]; 
	}
	else if(x->ch[0]) Root = x->ch[0];
	else if(x->ch[1]) Root = x->ch[1];
	else Root = NULL;

	if(Root) Root->pre = NULL;
	delete x;
	x = NULL;
    }

    void Release(Node *rt)
    {
	if(rt->ch[0]) Release(rt->ch[0]);
	if(rt->ch[1]) Release(rt->ch[1]);
	delete rt;
    }
}T;
