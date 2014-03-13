#Cache Simulation
----------
This project is an effort to simulate real-time Cache management with DRAM data read/write request.

##Compile and Run
We have used a CodeBlocks setup. `Cache Simulation.cdp` will directly open the project in CodeBlocks. To compile directly, open `Makefile` and write:

	all:
	g++ main.cpp cache_util.h cache_util.cpp -o hello.exe

For Windows 7, open `Developer Command Prompt` of Visual Studio with command `nmake -f Makefile`

##Policies
###Write Policy:
We will be using [**Write back**](http://en.wikipedia.org/wiki/Cache_%28computing%29#Writing_policies) as write policy. And, for a write request, if data is in the cache, data is modified with data being set as dirty. If the data is not in the cache, data is loaded to cache. And before loading if the data in that cache location is dirty, then propagation is confirmed.

###Read Policy:
For every read request, look up in cache if it's a hit. If hit, fetch data. If miss, checking is done is the data present is dirty. If so, data is propagated to memory. New requested data is loaded in cache.

##Input and Output
###Input
Initially setup the cache size and specify he input and output file.**Cache size is always in power of 2 and block size is also power of two and smaller that cache size.**
 
Input file is read till `EOF`. Each line should have a _16 bit address_, indication the DRAM access address. Then a single `space`. Then `FF` indicates write request and `00` indicates read request. Then a `one byte data`. **All in `HEX`.** For read request, the extra byte data is ignored as read from I/O flag is set off by CPU controller.

###Output
Output will be for read request only. For every read request, the program outputs the 1 byte data in **hex** and then is it a hit outputs 1, else 0. And then the initial condition of dirty bit when the read request is made.

####A test input file and corresponding output file has been included.