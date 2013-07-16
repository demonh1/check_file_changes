#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <unistd.h>

typedef std::list<unsigned int> crcData;
typedef std::list<unsigned int>::iterator Index;

crcData data; 
Index index;

unsigned int calculateCrc (unsigned char* buf, unsigned long len) {

unsigned long crcTable[256];
unsigned long crc;

for (int i = 0; i< 256; i++) {
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
unsigned int countCrc (char* fname) {

char buf [4096*64];
std::ifstream is (fname, std::ios::binary);
is.read(buf,4096*64);

return calculateCrc ((unsigned char*) buf, is.gcount() );
}

void threadCalc() {

data.push_back(countCrc("myfile.txt"));
}
void print () {

std::cout <<" list crc: \n" ;
	for ( Index ind = data.begin(); ind != data.end(); ++ind) 
   std::cout << *ind << ' ' << std::endl;
}

void threadCheck() {

Index i, j;

for ( i = data.begin(); i != data.end(); ++i)
{
     if (i == data.begin()) ++i;
     j = i;
     --j;
     if (*i != *j) {
          std:: cout << "File is modified!" << std::endl;
     }
  }
}

void test() { 
int tik = 0;
while (tik < 5*60) {
sleep(1);
std::thread thr(threadCalc);
thr.join();
tik++;
}

}

int main() {

test();
threadCheck();
//print ();

return 0;
}
