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
bit_cpy is a similar type of function like strcpy,
from source, we copy bitnumber of bits to destination
**/
void bit_cpy(bool *destination, bool *source, int bitnumber)
{
    //tested
    int i;
    for(i=0;i<bitnumber;i++)
    {
        destination[i]=source[i];
    }
}

/**
bit_cmp is also similar to strcmp but with much modifications,
this function compares bitnumber of bits of both source1 and
source2 and both of them are boolean array. Returns true if
first bitnumber of bits of both source matches, else false.
**/

bool bit_cmp(bool *source1, bool *source2, int bitnumber)
{
    //tested
    int i;
    for(i=0;i<bitnumber;i++)
    {
        if(source1[i]!=source2[i])
        {
            return false;
        }
    }
    return true;
}

/**
turn number address into binary of bitnum bits
**/
void tobinary(int address, bool *dest, int bitnumber)//size is bitnum
{
    int i;
    for(i=0;i<bitnumber;i++)
    {
        dest[i]=0;
    }
    i=bitnumber-1;
    while(address)
    {
        dest[i--]=address%2;
        address/=2;
    }
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
