#ifndef CACHE_UTIL_H
#define CACHE_UTIL_H

struct tri
{
	int tag, index,offset;
	tri()
	{
	}
	tri(int _tag, int _index, int _offset)
	{
		tag=_tag;
		index=_index;
		offset=_offset;
	}
};

struct cache
{
    unsigned char *cacheData;
    int cacheSize,blockSize;
    cache()
    {

    }
    cache(int , int );
    void set_size(int, int);
};

struct dm_block
{
    bool valid,modified; //dirty=modified
    int tag;
    void set_size(int );
};

struct direct_mapping
{
    int setNumber;
    int indxBit,tagBit;
    dm_block *block;
    void set_size(int , int ,int);
    bool isHit(int);
};

struct binary_utility
{

};

struct parser
{
    tri addressParse(int,int,int,int);
};

#endif // CACHE_UTIL_H
