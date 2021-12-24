#include <iostream>
#include <string>
#include <vector>


using namespace std;

const int filters[8][3][3] = {
        {
                {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}
        },
        {
                {0, 1, 2},
                {-1, 0, 1},
                {-2, -1, 0}
        },
        {
                {1, 2, 1},
                {0,0,0},
                {-1,-2,-1}
        },
        {
                {2, 1, 0},
                {1, 0, -1},
                {0, -1, -2}
        },
        {
            {1, 0, -1},
            {2, 0, -2},
            {1, 0, -1}
        },
        {
                {0, -1, -2},
                {1, 0, -1},
                {2, 1, 0}
        },
        {
                {-1, -2, -1},
                {0,0,0},
                {1,2,1}
        },
        {
                {-2, -1, 0},
                {-1, 0, 1},
                {0, 1, 2}
        }
};



void readImageBitmapHeader(const char info[54]){
    string compressionType [4] = {"BI_RGB", "BI_RLE8", "BI_RLE4", "BI_BITFIELDS"};
    int bitmapSize = *(int*)&info[2];
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
    cout << endl;
    cout << "Rozmiar Bitmapy: " << bitmapSize << endl;
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
    cout << endl;
}

vector< vector< vector<int> > > applyFilter(int filterId, vector< vector< vector<int> > > data){
    return data;
}


void sobelImageProcessing(const char* filename, const char* newFilename){
    cout << "Nazwa pliku: " << filename << endl;
    FILE* f = fopen(filename, "rb");
    char info[54];
    fread(info, sizeof(unsigned char), 54, f);
    readImageBitmapHeader(info);

    int bitmapSize = *(int*)&info[2];
    int height = *(int*)&info[22];
    int width = *(int*)&info[18];

    int bajtyZerowe = ((((bitmapSize/height)/3)) - width) * 3;
    cout << "Bajty zerowe: " << bajtyZerowe << endl;

 ;   int row_padded = (width*3 + 3) & (~3);
    auto* data = new unsigned char[width];

    vector< vector< vector<int> > > pixels(height, vector< vector<int> >(width , vector<int>(3)));

    //get every pixel data
    for(int i = 0; i < height; i++){
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width; j++){
            pixels[i][j][0] = (int)data[j*3];
            pixels[i][j][1] = (int)data[(j*3) + 1];
            pixels[i][j][2] = (int)data[(j*3) + 2];
        }
    }

    //apply every filter
    for (int i = 0; i < 8; i++){
        vector<vector<vector<int>>> postFilterData =  applyFilter(i, pixels);
    }

    //Calculate average R G B values based on all filters

    //Store the  new image in a new file
}

int main(){
    sobelImageProcessing(R"(C:\Users\lukas\Desktop\DevTools\Sobel\sampleImages\RAY.BMP)", "test");
}


