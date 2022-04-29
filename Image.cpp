//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <valarray>
#include "Image.h"
#include "vector"

using namespace std;


bool Image::load(string filename) {
    ifstream ifs;

    ifs.open(filename, ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw ("Can't open input file");
        }
        string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw ("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        int imageSize = w * h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < imageSize; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }

    return true;
}

bool Image::loadRaw(string filename) {
    ifstream in(filename);
    if(in.good())
    {
        in >> w;
        in >> h;

        for(int i = 0; i < w*h; i++)
        {
            float r, g, b;
            in >> r >>g>>b;
            this->pixels[i].r = (unsigned char)(std::max(0.f, min(255.f, powf(r, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].g = (unsigned char)(std::max(0.f, min(255.f, powf(g, 1/2.2) * 255 + 0.5f)));
            this->pixels[i].b = (unsigned char)(std::max(0.f, min(255.f, powf(b, 1/2.2) * 255 + 0.5f)));
        }
        in.close();
        return true;
    }
    return false;
}

bool Image::savePPM(string filename) {
    if (this->w == 0 || this->h == 0) {return false;}
    ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) return false;
        ofs << "P6\n"<< this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            r = static_cast<unsigned char>(min(1.f, (float)this->pixels[i].r)* this->pixels[i].r);
            g = static_cast<unsigned char>(min(1.f, (float)this->pixels[i].g)* this->pixels[i].g);
            b = static_cast<unsigned char>(min(1.f, (float)this->pixels[i].b)* this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
    return false;
}


void Image::filterRed() {
    for (int i = 0; i < w * h; i++) {
        pixels[i].b = 0;
        pixels[i].g = 0;
    }
}

void Image::filterGreen() {
    for (int i = 0; i < w * h; i++) {
        pixels[i].b = 0;
        pixels[i].r = 0;
    }
}

void Image::filterBlue() {
    for (int i = 0; i < w * h; i++) {
        pixels[i].r = 0;
        pixels[i].g = 0;
    }
}

void Image::greyScale() {
    for (int i = 0; i < w * h; ++i) {
        pixels[i].r = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].g = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
        pixels[i].b = (pixels[i].r + pixels[i].g + pixels[i].b) / 3;
    }
}

void Image::flipHorizontal() {
    for (int i = 0; i < h; ++i) {
        for(int j = 0; j < w/2; ++j) {
            int temp = pixels[i * w + j].r;
            pixels[i * w + j].r = pixels[i * w + w - j - 1].r;
            pixels[i * w + w - j - 1].r = temp;
            temp = pixels[i * w + j].g;
            pixels[i * w + j].g = pixels[i * w + w - j - 1].g;
            pixels[i * w + w - j - 1].g = temp;
            temp = pixels[i * w + j].b;
            pixels[i * w + j].b = pixels[i * w + w - j - 1].b;
            pixels[i * w + w - j - 1].b = temp;
        }
    }

}

void Image::flipVertically() {

    for(int k = 0; k < w; ++k) {
        for(int l = 0; l < h / 2; ++l) {
            int temp = pixels[l * w + k].r;
            pixels[l * w + k].r = pixels[(h - l - 1) * w + k].r;
            pixels[(h - l - 1) * w + k].r = temp;
            temp = pixels[l * w + k].g;
            pixels[l * w + k].g = pixels[(h - l - 1) * w + k].g;
            pixels[(h - l - 1) * w + k].g = temp;
            temp = pixels[l * w + k].b;
            pixels[l * w + k].b = pixels[(h - l - 1) * w + k].b;
            pixels[(h - l - 1) * w + k].b = temp;
        }
    }
}
//Rotate image 90 degree clockwise
void Image::rotate90() {
    Image *rot90Image = new Image(h, w);

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            int shift = h * i + j;
            rot90Image->pixels[shift] = this->pixels[w * (h - 1 - j) + i];
        }
    }
    swap(w,h);
    delete[] this->pixels;
    this->pixels = rot90Image->pixels;
    rot90Image = nullptr;
}


//Blur function
void Image::blur() {
    Image *blurImage = new Image(w, h);
    for (int i = 0; i < w * h; i++) {
        blurImage->pixels[i].r = (pixels[i].r + pixels[i + 1].r + pixels[i + w].r + pixels[i + w + 1].r) / 5;
        blurImage->pixels[i].g = (pixels[i].g + pixels[i + 1].g + pixels[i + w].g + pixels[i + w + 1].g) / 5;
        blurImage->pixels[i].b = (pixels[i].b + pixels[i + 1].b + pixels[i + w].b + pixels[i + w + 1].b) / 5;
    }
    swap(w, h);
    swap(w, h);
    delete[] this->pixels;
    this->pixels = blurImage->pixels;
    blurImage = nullptr;
}
//Invert function
void Image::AdditionalFunction2_Invert() {


    for(int i=0; i< w * h;i++){
        this->pixels[i].r =255 -  this->pixels[i].r;
        this->pixels[i].g =255 -  this->pixels[i].g;
        this->pixels[i].b =255 -  this->pixels[i].b;
    }
}
//Noise function
void Image::AdditionalFunction1_Noise() {
    for (int i = 0; i < w * h; ++i) {
        pixels[i].r = (pixels[i].r + pixels[i].g + pixels[i].b)/3 ;
        pixels[i].g = (pixels[i].r + pixels[i].g + pixels[i].b) /3;
        pixels[i].b = (pixels[i].r + pixels[i].g + pixels[i].b)/3;
        int random1 = rand()%100;
        if(random1 <10){
            int random2 = rand()%2;
            if(random2 == 0){
                pixels[i].r =0;
                pixels[i].g =0;
                pixels[i].b =0;

            }else{
                pixels[i].r =255;
                pixels[i].g =255;
                pixels[i].b =255;
            }
        }
    }

}
//Gamma function
void Image::Gamma(){
    for(int i = 0; i < w*h; i++)
    {
        float gamma =2/3.2;

        this->pixels[i].r  = pow(pixels[i].r / 255.0f, gamma) * 255;
        this->pixels[i].g  = pow(pixels[i].g / 255.0f, gamma) * 255;
        this->pixels[i].b  = pow(pixels[i].b / 255.0f, gamma) * 255;
    }
}
/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth() {
    return w;
}

int Image::getHeight() {
    return h;
}

Rgb *Image::getImage() {
    return pixels;
}