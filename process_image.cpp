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

vector< vector< vector<int> > > applyFilter(int filterId, vector< vector< vector<int> > > data, int height, int width){
    vector< vector< vector<int> > > result(height, vector< vector<int> >(width , vector<int>(3)));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            //If it's a border pixel set all values of that pixel to 0
            if (i == 0 || j == 0 || i == height-1 || j == width-1){
                result[i][j][0] = 0;
                result[i][j][1] = 0;
                result[i][j][2] = 0;
            }
            else{
                for (int z = 0; z < 3; z++){
                    int m0 = data[i-1][j-1][z] * filters[filterId][0][0];
                    int m1 = data[i-1][j][z] * filters[filterId][0][1];
                    int m2 = data[i-1][j+1][z] * filters[filterId][0][2];
                    int m3 = data[i][j-1][z] * filters[filterId][1][0];
                    int m4 = data[i][j][z] * filters[filterId][1][1];
                    int m5 = data[i][j+1][z] * filters[filterId][1][2];
                    int m6 = data[i+1][j-1][z] * filters[filterId][2][0];
                    int m7 = data[i+1][j][z] * filters[filterId][2][1];
                    int m8 = data[i+1][j+1][z] * filters[filterId][2][2];
                    result[i][j][z] = m0 + m1 + m2 + m3 + m4 + m5 + m6 + m7 + m8;
                }
            }
        }
    }
    return result;
}

int calculateAverageValue(vector<vector<vector< vector<int>>>> data, int i, int j, int z){
    int avg = 0;
    for (int k = 0; k < 8; k++){
        avg += data[k][i][j][z];
    }
    return (avg/8);
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

    int row_padded = (width*3 + 3) & (~3);
    auto* data = new unsigned char[width];

    vector< vector< vector<int> > > pixels(height, vector< vector<int> >
                                          (width , vector<int>
                                          (3)));

    //get every pixel data
    for(int i = 0; i < height; i++){
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width; j++){
            pixels[i][j][0] = (int)data[j*3];
            pixels[i][j][1] = (int)data[(j*3) + 1];
            pixels[i][j][2] = (int)data[(j*3) + 2];
        }
    }
    cout << "get every pixel" << endl;
    vector<vector<vector<vector<int>>>> allFilters(8, vector<vector<vector<int>>>
                                                  (height, vector<vector<int>>
                                                  (width, vector<int>
                                                  (3))));
    //apply every filter
    for (int i = 0; i < 8; i++){
        vector<vector<vector<int>>> postFilterData =  applyFilter(i, pixels, height, width);
        allFilters[i] = postFilterData;
    }
    cout << "done filters" << endl;
    vector< vector< vector<int> > > result(height, vector< vector<int> >(width , vector<int>(3)));
    //Calculate average R G B values based on all filters
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            for (int z = 0; z < 3; z++){
                result[i][j][z] = calculateAverageValue(allFilters, i, j, z);
            }
        }
    }
    cout << "done average" << endl;

    //Store the  new image in a new file
}

int main(){
    sobelImageProcessing(R"(C:\Users\lukas\Desktop\DevTools\Sobel\sampleImages\RAY.BMP)", "test");
}


