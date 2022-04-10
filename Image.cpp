//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"

using namespace std;


bool Image::load(string filename) {
    ifstream ifs;

    ifs.open(filename, ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw ("Can't open input file");
        }
        std::string header;
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
    ifstream ifs;

    ifs.open(filename, ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw ("Can't open input file");
        }
        std::string header;
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
    return false;
}

bool Image::savePPM(string filename) {
    if (this->w == 0 || this->h == 0) {
        fprintf(stderr, "Can't save an empty image\n");
        return false;
    }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw ("Can't open output file");
        ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
        unsigned char pix[3]; // convert floats to bytes
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            pix[1] = static_cast<unsigned char>( this->pixels[i].r);
            pix[2] = static_cast<unsigned char>(this->pixels[i].g);
            pix[3] = static_cast<unsigned char>(this->pixels[i].b);
            ofs.write(reinterpret_cast<char *>(pix), 3);
        }
        ofs.close();
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

void Image::AdditionalFunction2() {

}

void Image::AdditionalFunction3() {

}

void Image::AdditionalFunction1() {

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