#include<stdio.h>
#include<string.h>
#define MAX_SIZE 309//309 digits required for 2^1024-1(1024 bit largest no.),found via (characteristic of log) + 1 
//#define BIT_SIZE 1024

typedef enum {POSITIVE,NEGATIVE} Sign;
typedef struct Bigint_type{
    int digits[MAX_SIZE];//Each element will hold a digit,from 0 to 9
    Sign s;
}BigInt;

//BigInt ULimit,LLimit;


void Initialize(BigInt *num);
void PrintBigInt(BigInt num);
BigInt Read(char *str);
int Compare(BigInt a,BigInt b);
BigInt Add(BigInt a,BigInt b);
BigInt Subtract(BigInt a,BigInt b);
BigInt Multiply(BigInt a,BigInt b);
//BigInt Overflow(BigInt a);
/*Short note:
    The fns written are assumed to have closure,no explicit check has been made...
    Code for overflow has to be added in:
        Add(as well as subtract)
        Multiply
*/

void Initialize(BigInt *num)//Debugged=True
{
    int i;
    num->s=POSITIVE;
    for(i=0;i<MAX_SIZE;i++)
    {
        num->digits[i]=0;
    }
}

void PrintBigInt(BigInt num)//Working=True
{
    int i=0;
    printf("\n");
    if(num.s==NEGATIVE)
    {
        printf("-");
    }
    while(num.digits[i]==0&&i<MAX_SIZE)
    {
        i++;
    }
    if(i==MAX_SIZE)//Number is Zero
    {
        printf("0");
    }
    else
    {    
        while(i<MAX_SIZE)
        {
            printf("%d",num.digits[i]);
            i++;
        }
    }
    printf("\n");
}

BigInt Read(char *str/*What to print while taking input*/)//Working=True
{
    char temp[MAX_SIZE];
    int digit,j,k=0,i=0,scan_continue=1;
    BigInt num;
    Initialize(&num);
    printf("%s",str);
    scanf("%s",temp);
    j=strlen(temp);
    //   printf("%d",j);
    /*while(temp[i]==' ')
    {
        i++;
    }*/
    if(temp[i]=='-')
    {
        num.s=NEGATIVE;//Negative number
        i++;
    }
    j--;
    for(;j>=i&&scan_continue==1;j--)
    {
        digit=temp[j]-'0';//Checking for validity can be made
        
        if(digit>9||digit<0)
        {
            scan_continue=0;
            Initialize(&num);

        }
        else
        {num.digits[MAX_SIZE-1-k]=digit;
        //    printf("\n At this stage, num is");
        //    PrintBigInt(num);
        k++;
        }
    }
    return num;
}
//Compare fn working=True
int Compare(BigInt a,BigInt b)//1 if a is bigger ,-1 if b,0 if equal(only magnitude)
{
    int i,j,retval,flag=0;
    i=0;
    j=0;
    while(a.digits[i]==0&&i<MAX_SIZE)
    {
        i++;
    }
    while(b.digits[j]==0&&i<MAX_SIZE)
    {
        j++;
    }
    if(i>j)
    {
        retval=-1;
    }
    else if(j>i)
    {
        retval=1;
    }
    else
    {
        while(flag==0&&i<MAX_SIZE)
        {
            if(a.digits[i]>b.digits[i])//i=j
            {
                retval=1;
                flag=1;
            }
            else if(a.digits[i]<b.digits[i])
            {
                retval=-1;
                flag=1;
            }
            else
            {
                retval=0;
                flag=0;
            }
            i++;
        }
        if(flag==0)
        {
            retval=0;
        }
    }
    return retval;
}

BigInt Add(BigInt a,BigInt b)//Working=True
{
    /*Currently,closure of this operation is assumed.
    Not written any code for overflowing.*/
    BigInt c;
    int i,carry=0,borrow=0;
    Initialize(&c);
    if((a.s==POSITIVE&&b.s==POSITIVE)||(a.s==NEGATIVE&&b.s==NEGATIVE))
    {
        for(i=MAX_SIZE-1;i>=0;i--)//Have to do something about overflow,add some code
        {
            c.digits[i]=a.digits[i]+b.digits[i]+carry;
            carry=0;
            while(c.digits[i]>9)
            {
                c.digits[i]=c.digits[i]-10;
                carry++;
            }
            /*if(carry>0)//overflown number
            {

            }*/
            c.s=a.s;
        }
    }//Debugged till here(for all sign same cases)
    else
    {
        if(Compare(a,b)==1)//a is bigger
        {
            for(i=MAX_SIZE-1;i>=0;i--)
            {
                borrow=0;
                if(a.digits[i]<b.digits[i])
                {
                    borrow++;
                    a.digits[i-1]--;
                }
                c.digits[i]=a.digits[i]-b.digits[i]+borrow*10;
            }
            c.s=a.s;
        }
        else if(Compare(a,b)==-1)//b is bigger
        {
            for(i=MAX_SIZE-1;i>=0;i--)
            {
                borrow=0;
                if(b.digits[i]<a.digits[i])
                {
                    borrow++;
                    b.digits[i-1]--;
                }
                c.digits[i]=b.digits[i]-a.digits[i]+borrow*10;
            }
            c.s=b.s;
        }
        else//both equal and opposite magnitude
        {
            Initialize(&c);
        }    
    }
    return c;
}
//Subtract=Working
BigInt Subtract(BigInt a ,BigInt b)//a-b is ans ,calculated as a +(-b)
{
    BigInt c;
    if(b.s==POSITIVE)
    {
        b.s=NEGATIVE;
    }
    else
    {
        b.s=POSITIVE;
    }
    c=Add(a,b);
    return c;
}
//Multipy=Working
BigInt Multiply(BigInt a,BigInt b)//c=a*b
{
    /*Assuming Closure*/
    int a_start,a_traverse,b_start,b_traverse,temp_traverse,carry=0;
    BigInt temp,c;
    Initialize(&temp);
    Initialize(&c);
    a_start=0;
    b_start=0;
    while(a_start<MAX_SIZE&&a.digits[a_start]==0)
    {
        a_start++;
    }
    while(b_start<MAX_SIZE&&b.digits[b_start]==0)
    {
        b_start++;
    }
    temp_traverse=MAX_SIZE-1;
    for(a_traverse=MAX_SIZE-1;a_traverse>=a_start;a_traverse--)
    {
        temp_traverse=a_traverse;//Update temp_traverse
        Initialize(&temp);
        for(b_traverse=MAX_SIZE-1;b_traverse>=b_start;b_traverse--)
        {
            temp.digits[temp_traverse]+=a.digits[a_traverse]*b.digits[b_traverse];
            carry=0;
            while(temp.digits[temp_traverse]>9)
            {
                temp.digits[temp_traverse]-=10;
                carry++;
            }
            temp.digits[temp_traverse-1]+=carry;
            temp_traverse--;
        }
        c=Add(c,temp);
    }
    
    if(a.s==NEGATIVE)
    {
        if(b.s==NEGATIVE)
        {
            c.s=POSITIVE;
        }
        else
        {
            c.s=NEGATIVE;
        }  
    }
    else
    {
        if(b.s==NEGATIVE)
        {
            c.s=NEGATIVE;
        }
        else
        {
            c.s=POSITIVE;
        }
        
    }
    
    return c;
}

/*BigInt ULimitDefiner(int BitSize)//Use files to store this,To Be Tested
{
    int i;
    FILE *fptr;
    BigInt two,two_power,ans;
    Initialize(&two_power);
    Initialize(&two);
    two_power.digits[MAX_SIZE-1]=1;
    two.digits[MAX_SIZE-1]=2;
    Initialize(&ans);
    for(i=0;i<BitSize;i++)
    {
        ans=Add(ans,two_power);
        two_power=Multiply(two_power,two);
    }
    fptr=fopen("ULimit.txt","w");
    for(i=0;i<MAX_SIZE;i++)
    {
        fprintf(fptr,"%d",ans.digits[i]);
    }
    return ans;
}*/

/*BigInt LLimitDefiner()
{
    BigInt ans,n_one;//To keep consistency with the representation at lower level,just to give a feel of that.
    Initialize(&ans);
    Initialize(&n_one);
    n_one.digits[MAX_SIZE-1]=1;
    n_one.s=NEGATIVE;
    ans=ULimit;
    ans.s=NEGATIVE;
    ans=Add(n_one,ans);
    return ans;
}*/

/*BigInt ULimitReader()//To be tested
{
    BigInt num;
    int i;
    FILE *fptr;
    fptr=fopen("ULimit.txt","r");
    for(i=0;i<MAX_SIZE;i++)
    {
        fscanf(fptr,"%d",&num.digits[i]);
    }
    return num;
}*/
//Define the overflow limit in a variable,say BigInt limit
/*2 approaches:Take 1 extra bit and use it for overflow subtraction or 
in the same bit keep limit such that the extra bit is not needed(the limit would then correspond to
the 1024 bit requirement as the largest number a to be made available)*/
/*
BigInt Overflow(BigInt a)
{
    //BigInt ULimit,LLimit;//Assumed to be defined
    BigInt temp,one;
    //    Initialize(&ULimit);
    //    Initialize(&LLimit);
    //    ULimit.digits[MAX_SIZE-1]=9;  Full initialisation of Ulimit and Llimt has to be manually done
    //    LLimit.digits[MAX_SIZE-1]=9;
    //    LLimit.s=NEGATIVE;
    Initialize(&temp);
    if(a.s==POSITIVE)//Overflow from positive to negative
    {    
        if(Compare(ULimit,a)==-1)//a exceeds limit
        {
            temp=Subtract(a,ULimit);
            Initialize(&one);
            one.digits[MAX_SIZE-1]=1;
            temp=Subtract(temp,one);
            a=Add(temp,LLimit);
        }
    }
    else
    {
        if(Compare(LLimit,a)==-1)//even below lower limit
        {
            temp=Subtract(a,LLimit);
            Initialize(&one);
            one.digits[MAX_SIZE-1]=1;
            temp=Add(temp,one);
            a=Add(temp,ULimit);
        }
    }
    return a;
}
*/
void main()
{
    BigInt a,b,c;
    int operation,exit=0;
    //Initialize(&ULimit);
    //Initialize(&LLimit);
    //ULimit=ULimitDefiner(3);
    //LLimit=LLimitDefiner();
    //Initialize(&a);

    //Initialize(&b);
    //a=Read("Enter 'a':");
    //b=Read("\nEnter 'b':");
    //PrintBigInt(a);
    
    //PrintBigInt(n);
    //printf("Compare fn op:%d",Compare(a,b));
    //c=Multiply(a,b);
    //c=Overflow(c);
   // PrintBigInt(c);

    while(exit==0)
    {
        Initialize(&a);
        Initialize(&b);
        //Initialize(&c);
        printf("Enter the opcode you want to perform:");
        printf("\n1.ADD(a+b)");
        printf("\n2.SUBTRACT(a-b)");
        printf("\n3.MULTIPLY:(a*b)");
        printf("\nAnything else to exit:\n");
        scanf("%d",&operation);
        switch(operation)
        {
            case 1: a=Read("Enter 'a':");
                    b=Read("\nEnter 'b':");
                    c=Add(a,b);
                    PrintBigInt(c);
                    break;

            case 2: a=Read("Enter 'a':");
                    b=Read("\nEnter 'b':");
                    c=Subtract(a,b);
                    PrintBigInt(c);
                    break;  

            case 3: 
                    a=Read("Enter 'a':");
                    b=Read("\nEnter 'b':");
                    c=Multiply(a,b);
                    PrintBigInt(c);
                    break;  
            
            default: exit=1;
                     break;

        }
    }

}
