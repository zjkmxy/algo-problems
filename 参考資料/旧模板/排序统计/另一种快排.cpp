int Partition(int *A,int st, int ed)  
{  
    int i,j,k;  
  
    i = st - 1;  
    for(j=st;j<ed;j++)  
    {  
        if(A[j]<=A[ed])  
        {  
            i++;  
            k = A[i];  
            A[i] = A[j];  
            A[j] = k;  
        }  
    }  
    //顺次划分  
    k = A[i+1];  
    A[i+1] = A[ed];  
    A[ed] = k;  
    //主元归位  
  
    return i+1;  
}  
  
void QuickSort(int *A,int st, int ed)  
{  
    int q;  
  
    while(st < ed)  
    {  
        q = Partition(A,st,ed);  
        QuickSort(A,st,q-1);  
        st = q + 1;  
    }  
}
