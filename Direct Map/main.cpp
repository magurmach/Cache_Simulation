//Bismillahir Rahmanir Raheem

#include<stdio.h>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<math.h>
#include<string.h>
#include<assert.h>
#include<cache_util.h>

#define clr(a) memset(a,0,sizeof(a))
#define FRO freopen("mycache.txt","r",stdin); // file name to read from
//#define FROut freopen("dmtest1000out.txt","w",stdout); //file name to write
#define came "Came"
using namespace std;

/**

16 bit addressing
byte accessable memory and cache

cache size= 2^x bytes
block size= 2^y bytes
setNumber= (2^(x-y))
setbits/Index bit = x-y bits
blockBits/Block offset=y bits
tagbits=(16-(x-y+y))=x bits

FORMAT:
[tag: x bits][index: y bits][offset: z bits]

**/

/**Entity Scope**/

ifstream input;
ofstream output;
int ln;

int setNumber, tagbit,indexbit,offset,address;

unsigned char mainMemory[65536];

cache Cache; /**defined in cache_util.h**/

direct_mapping table;

/**function prototyping scope**/

void selectInputFileName();/**
Function details:
set the file name and prepare file stream
for input
**/

void selectOutputFileName();/**
Function details:
set the file name and prepare file stream
for output
**/

void cacheDetailsInput();/**
Function details:
Input cache details, get cache ready
for work. Get direct mapping table
ready.
**/

void process();/**
Function details:
read from file the read/write request
and simulate the cache and output in
ofstream for read requests.
**/

void readCache(int);/**
Function Details:
Read from Cache with read policy
**/

void writeCache(int,unsigned char);/**
Function Details:
write in cache with write back policy
**/

void writeMemtoCache(int, int);/**
Read from main memory and write to cache;
**/

void writeCachetoMem(int);/**
write from Cache to memory
**/

void showMainMemoryInfo();/**
show DRAM info
**/

/**function Implementation Scope**/

void cacheDetailsInput()
{
    int cachetp,blocktp;
    cout<<"$ Cache Size 2^X (byte), X(<=16): ";
    cin>>cachetp;
    if(cachetp>16)
    {
        cout<<"Cache Size Error. Try Again"<<endl;
        cacheDetailsInput();
        return;
    }
    blocktp=cachetp+1;

    while(blocktp>cachetp)
    {
        cout<<"$ Block Size 2^Y (byte), Y(<=X): ";
        cin>>blocktp;
        if(blocktp>cachetp)
        {
            cout<<"Block Size Error"<<endl;
        }
        else break;
    }
    Cache.set_size(cachetp,blocktp);
    indexbit=cachetp-blocktp;
    offset=blocktp;
    setNumber=pow(2,indexbit);
    tagbit=16 - indexbit - offset;
    table.set_size(setNumber,tagbit,indexbit);
}

void selectInputFileName()
{
    cout<<"$ Specify Input File Name: ";
    string x;
    char a[5];
    gets(a);
    getline(cin,x);
    const string y(x);
    input.open(y.c_str());
    if(input.good())
    {
        cout<<x<<" openned for prossing"<<endl;
    }
    else
    {
        cout<<"Can\'t open file. Please try again."<<endl;
        selectInputFileName();
    }
}

void selectOutputFileName()
{
    cout<<"$ Specify Output File Name: ";
    string x;
    getline(cin,x);
    const string y(x);
    output.open(y.c_str());
    if(output.good())
    {
        cout<<x<<" openned for output"<<endl;
    }
    else
    {
        cout<<"Can\'t open file. Please try again."<<endl;
        selectOutputFileName();
    }
}

void writeMemtoCache(int mainMemAddress, int rindx)
{
    /*
    get the blocks to be loaded from memory
    copy them in raw_cache
    update table for the indx

    when?

    hit miss
    read miss
    */
    int r=mainMemAddress;
    parser parse;
    tri x=parse.addressParse(mainMemAddress,tagbit,indexbit,offset);
    int tag=x.tag;
    int i;
    r=r>>offset;
    r<<=offset;
    int lim=pow(2,offset);
    for(i=0;i<lim;i++)
    {
        Cache.cacheData[(rindx<<offset)+i]=mainMemory[r+i];
    }
    table.block[rindx].valid=1;
    table.block[rindx].tag=tag;
    table.block[rindx].modified=0;
}
/**
Address FORMAT:
[tag: tagbits     ][index: indexbits       ][offset: offset     ]
[ 0 - (tagbits-1) ][tagbits - (indexbits-1)][ tagbits+offset -15]
**/

void writeCachetoMem(int rindx)
{
    /*
    miss and dirty bit 1
    */
    int tag=table.block[rindx].tag;
    int address;
    address=tag;
    address=(address<<indexbit)+rindx;
    address=(address<<offset);
    int lim=1<<offset,i;
    for(i=0;i<lim;i++)
    {
        mainMemory[address+i]=Cache.cacheData[((rindx)<<offset) +i];
    }
    table.block[rindx].modified=false;
}

/**
FORMAT:
[tag: tagbit][index: indexbit][offset: offset]
**/
void readCache(int address)
{
    int rindx=0,roffset;
    parser parse;
    tri x=parse.addressParse(address,tagbit,indexbit,offset);
    rindx=x.index;
    roffset=x.offset;
    //assert(table.block[rindx].valid);

    if(table.isHit(address))
    {
        //hit
        if((int)Cache.cacheData[(rindx<<offset)+roffset]<=(0xF))
        {
            output<<0;
        }
        output<<hex<<(int)Cache.cacheData[(rindx<<offset)+roffset]<<" "<<1<<" "<<table.block[rindx].modified<<endl;
    }
    else
    {
        bool wasdirty=false;
        table.block[rindx].valid=true;
        //miss n load from Mem
        if(table.block[rindx].valid && table.block[rindx].modified)
        {
            wasdirty=true;
            writeCachetoMem(rindx);
            table.block[rindx].modified=0;
        }

        table.block[rindx].valid=1;

        writeMemtoCache(address,rindx);

        if((int)Cache.cacheData[(rindx<<offset)+roffset]<=(0xF))
        {
            output<<0;
        }
        output<<hex<<(int)Cache.cacheData[(rindx<<offset)+roffset]<<" "<<0<<" "<<wasdirty<<endl;
    }
}

void writeCache(int address, unsigned char data) // write back policy
{
    //write to cache and set dirty/modified bit 1
    int rindx=0;
    parser parse;
    tri x=parse.addressParse(address,tagbit,indexbit,offset);
    rindx=x.index;
    if(table.block[rindx].valid && !table.isHit(address) && table.block[rindx].modified)
    {
        writeCachetoMem(rindx);
    }
    if(!table.isHit(address))
    {
        writeMemtoCache(address,rindx);
    }

    table.block[rindx].modified=1;
    table.block[rindx].valid=1;
    table.block[rindx].tag=x.tag;
    int roffset=x.offset;
    Cache.cacheData[(rindx<<offset)+roffset]=data;
}

void process()
{
    output<<uppercase;
    char action[10];
    unsigned int x;
    unsigned char data;
    while(input>>hex>>address)
    {
        input>>action;
        if(action[0] =='0')
        {
            input>>hex>>x;
            data=(unsigned char)x;
            readCache(address);
            ///cnt++;
        }
        else
        {
            input>>hex>>x;
            data=(unsigned char)x;
            writeCache(address, data);
        }
    }
}

void showMainMemoryInfo()
{
    cout<<"$ DRAM details:"<<endl;
    cout<<"\t****************************"<<endl;
    cout<<"\t*Size       : 65536 byte(s)*"<<endl;
    cout<<"\t*Addressing :    16  bit(s)*"<<endl;
    cout<<"\t*Access Unit:     1 byte(s)*"<<endl;
    cout<<"\t****************************"<<endl;
}

int main()
{
    //FRO
    showMainMemoryInfo();
    cacheDetailsInput();
    selectInputFileName();
    selectOutputFileName();
    process();
    input.close();
    output.close();
    return 0;
}
