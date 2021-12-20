#include <iostream>
#include <string>

using namespace std;

void read_bitmap_header(const char* filename){
    FILE* f = fopen(filename, "rb");
    char info[54];
    string compressionType [4] = {"BI_RGB", "BI_RLE8", "BI_RLE4", "BI_BITFIELDS"};
    fread(info, sizeof(unsigned char), 54, f);
    int headerSize = *(int*)&info[14];
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    int planes = *(int*)&info[26];
    int bitsPerPixel = *(int*)&info[28];
    int compression = *(int*)&info[30];
    int sizeImage = *(int*)&info[34];
    int horizontalRes = *(int*)&info[38];
    int verticalRes = *(int*)&info[42];
    int colors = *(int*)&info[46];
    int importantColors = *(int*)&info[50];
    cout << "Nazwa Pliku: " << filename << endl;
    cout << "Rozmiar naglowka: " << headerSize << endl;
    cout << "Szerokosc: " << width << "px" << endl;
    cout << "Wysokosc: " << height << "px" << endl;
    cout << "Liczba platow: " <<  planes << endl;
    cout << "Liczba bitow na pixel: " <<  bitsPerPixel << endl;
    cout << "Rozmiar rysunku: " <<  sizeImage << endl;
    cout << "Kompresja: " <<  compressionType[compression] <<  endl;
    cout << "Rozdzielczosc pozioma: " <<  horizontalRes << endl;
    cout << "Rozdzielczosc pionowa: " <<  verticalRes << endl;
    cout << "Liczba kolorow w palecie: " <<  colors << endl;
    cout << "Liczba waznych kolorow w palecie: " <<  importantColors << endl;
}

