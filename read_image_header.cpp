#include <iostream>
#include <fstream>

using namespace std;

int main() {
    read_bitmap_header("lena.bmp");
    return 0;
}

int read_bitmap_header(char* filename){
    FILE* bitmap_file = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, bitmap_file);
    cout << "Name: " << filename << endl;
    return 0;
}