#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <iterator>
#include <vector>
#include <map>

#include <thread>
#include <unistd.h>

typedef std::list<unsigned int> crcData;


typedef std::map<unsigned int, std::string> mMap;
typedef std::map<unsigned int, std::string>::iterator itMap;


std::ifstream dataFile("dataCheckFiles.txt");

std::istream_iterator<std::string>dataBegin (dataFile);
std::istream_iterator<std::string>dataEnd;

std::vector<std::string> in (dataBegin,dataEnd);
std::vector<std::string>::iterator it;

crcData data; 

mMap m;
itMap iter;


unsigned int calculateCrc (unsigned char* buf, unsigned long len) {

unsigned long crcTable[256];
unsigned long crc;

for (int i = 0; i < 256; i++) {
crc = i;

for (int j = 0; j < 8; j++)
crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

crcTable[i] = crc;
}
crc = 0xFFFFFFFFUL;
while (len--) crc = crcTable[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
return crc ^ 0xFFFFFFFFUL;
}

//возвращание конечного CRC32
unsigned int countCrc (const char* fname) {

char buf [4096*64];
std::ifstream is (fname, std::ios::binary);
is.read(buf,4096*64);

return calculateCrc ((unsigned char*) buf, is.gcount() );
}

void threadCalc() {

unsigned int ii;

for (it = in.begin(); it != in.end(); ++it) {

data.push_back(countCrc((*it).c_str()));
ii = countCrc(((*it).c_str()));
m.insert( std::make_pair(ii, *it));
   }

}

void print () {

std::cout << "Map: " << std::endl;
for (itMap i = m.begin(); i != m.end(); ++i)
std::cout <<  i -> second << " " << i-> first << "\n";

}


void test() { 
	
int tik = 0;
while (tik < 5*60) { // 5 min 
sleep(1);
std::thread thr(threadCalc);
thr.join();
tik++;
	}

}

void check(mMap m) {
itMap i,j;
for ( i = m.begin(); i != m.end(); ++i) {

     j = i;
     --j;
if( i -> second == j -> second) {
          std::cout << "File " << i -> second <<  " is modified!" << std::endl;
	}
     }
}

int main() {

test();
check(m);

print ();

return 0;
}
