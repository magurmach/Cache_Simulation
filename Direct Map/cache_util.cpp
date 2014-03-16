#include "cache_util.h"
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

/**
prototyping scope
**/
bool bit_cmp(bool *, bool *, int );
void bit_cpy(bool *, bool *, int );
void tobinary(int , bool *, int);
tri addressParse(int,int ,int,int);


cache::cache(int _size, int _blockSize)
{
    cacheSize=pow(2,_size);
    blockSize=pow(2,_blockSize);
    cacheData=new unsigned char[cacheSize];
}

void cache::set_size(int _size, int _blockSize)
{
    cacheSize=pow(2,_size);
    blockSize=pow(2,_blockSize);
    free(cacheData);
    cacheData=new unsigned char[cacheSize];
}

/**
declaration of dm_block functions
**/

void dm_block::set_size(int tagSize)
{
    tag=0;
    valid=false;
    modified=false;
}

/**
declaration of direct_mappping functions
**/

void direct_mapping::set_size(int _setNumber, int _tagBit,int _indxBit)
{
    setNumber=_setNumber;
    tagBit=_tagBit;
    indxBit=_indxBit;
    block=new dm_block[_setNumber];
    int i;
    for(i=0;i<_setNumber;i++)
    {
        block[i].set_size(tagBit);
    }
}

/**
This function returns if the data of memory address
address is in the cache
**/
bool direct_mapping::isHit(int address)
{
    /*
    get address of cache index

    [tagBit][indxBit][16-indxBit-tagBit]

    */
    parser parse;
    tri ret=parse.addressParse(address,tagBit,indxBit,16-indxBit-tagBit);
    int indx=ret.index;
    if(block[indx].valid && block[indx].tag==ret.tag)
    {
        return true;
    }
    else return false;
}

/**
addressParse function takes in an address and returns three integer
first hading the msb 'first' bits, second having the middle 'second' bits
and third having the lsb 'third' bits
**/
tri parser::addressParse(int address,int first, int second, int third)
{
    unsigned int a;
    a=1<<third;
    a-=1;
    int offset=address & a;
    address=address>>third;
    a=1<<second;
    a-=1;
    int indx=address & a;
    address=address>>second;
    int tag=address;
    return tri(tag,indx,offset);
}
