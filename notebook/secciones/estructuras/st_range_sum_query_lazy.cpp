const int MAXN = ...;
const int MAXST = (int) (2*exp2(ceil(log2(MAXN)))+1);

#define OP(a,b) ((a) + (b))
#define NEUTRO 0LL

#define left(x) ((x)<<1)
#define right(x) (((x)<<1)+1)
#define middle(a,b) (((a)+(b))>>1)

ll st[MAXST], lazy[MAXST]; int arr[MAXN+1];

void build(int node, int a, int b)
{
    if(a>b) return;
    if(a==b){st[node] = arr[a]; return;}
    int l=left(node), r=right(node), m=middle(a,b);
    build(l,a,m);
    build(r,m+1,b);
    st[node]=OP(st[l],st[r]);
}

void update(int node, int a, int b, int i, int j, ll value)
{
    int l=left(node), r=right(node), m=middle(a,b);

    if(lazy[node]!=0LL)
    {
        st[node] = (b-a+1)*lazy[node]; // SET
        //      += (b-a+1)*lazy[node]; // ADD
        if(a!=b) lazy[l] = lazy[r] = lazy[node]; // SET
        //       lazy[l]+=lazy[node],lazy[r]+=lazy[node]; // ADD
        lazy[node] = 0LL;
    }

    if(a>b || a>j || b<i) return;

    if(a>=i && b<=j)
    {
        st[node] = (b-a+1)*value; // SET
        //      += (b-a+1)*value; // ADD
        if(a!=b) lazy[l] = lazy[r] = value;
        //       lazy[l]+=value,lazy[r]+=value; // ADD
        return;
    }

    update(l,a,m,i,j,value);
    update(r,m+1,b,i,j,value);

    st[node] = OP(st[l],st[r]);
}

ll query(int node, int a, int b, int i, int j)
{
    if(a>b || a>j || b<i) return NEUTRO;

    int l=left(node), r=right(node), m=middle(a,b);

    if(lazy[node]!=0)
    {
        st[node] = (b-a+1)*lazy[node];
        if(a!=b) lazy[l] = lazy[r] = lazy[node];
        lazy[node] = 0;
    }
    if(a>=i && b<=j) return st[node];

    int ql = query(l,a,m,i,j);
    int qr = query(r,m+1,b,i,j);
    return OP(ql,qr);
}