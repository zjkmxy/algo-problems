#include<stdio.h>
#include<stdlib.h>

typedef long long int LL;
LL a[1000000];

LL FastFind(LL *arr,int beg,int en)
{
    int i,j,k,l;
    LL exam,temp;

    if(en - beg <= 1)
        return arr[beg];

    exam = arr[en];
    i = k = beg;
    j = l = en - 1;

    while(i < j)
    {
        while(arr[i]<=exam)
        {
            if(arr[i]==exam)
            {
                arr[i] = arr[k];
                arr[k] = exam;
                k++;
            }
            i++;
            if(i > en)
                break;
        }
        while(arr[j]>=exam)
        {
            if(arr[j]==exam)
            {
                arr[j] = arr[l];
                arr[j] = exam;
                l--;
            }
            j--;
            if(j < beg)
                break;
        }

        if(i >= j)
            break;

        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    i--;
    j++;
    k--;
    l++;

    while(k>=beg)
    {
        temp = arr[i];
        arr[i] = arr[k];
        arr[k] = temp;
        k--;
        i--;
    }
    while(l<=en)
    {
        temp = arr[j];
        arr[j] = arr[l];
        arr[l] = temp;
        l++;
        j++;
    }

    if(j - i <= 2)
        return arr[i+1];

    if(((i - beg + 1)%2) == 1)
    {
        return FastFind(arr,beg,i);
    }else{
        return FastFind(arr,j,en);
    }
}

int main()
{
    int i, n;
    
    scanf("%d",&n);
    for(i=0;i<n;i++) scanf("%llu",a+i);
    //for(i=0;scanf("%d",a+i)!=EOF;i++)
    //{
    //}

    printf("%llu\n",FastFind(a,0,i-1));

    return 0;
}
