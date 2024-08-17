#include "BmpImage.hpp"
#include <fstream>
#include <iostream> 
#include <cstring>

bool BmpImage::flipVerticallyOnLoad = false;

BmpImage::BmpImage(const std::string& filepath) : data(nullptr), valid(false) {
	loadBMP(filepath);
	if (!valid)
		return ;
	flipVertically();
}

BmpImage::~BmpImage() {
	if (data)
		delete[] data;
}

bool BmpImage::isValid() const {
	return valid;
}

int BmpImage::getWidth() const {
	return width;
}

int BmpImage::getHeight() const {
	return height;
}

int BmpImage::getChannels() const {
	return channels;
}

unsigned char* BmpImage::getData() const {
	return data;
}

void BmpImage::setFlipVerticallyOnLoad(bool enable) {
	flipVerticallyOnLoad = enable;
}

void BmpImage::loadBMP(const std::string& filepath) {
	std::ifstream file(filepath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to open BMP file: " << filepath << std::endl;
		return;
	}
	// Read header
	file.seekg(18);
	file.read(reinterpret_cast<char*>(&width), 4);
	file.read(reinterpret_cast<char*>(&height), 4);
	file.seekg(2, std::ios::cur); // Skip planes
	file.read(reinterpret_cast<char*>(&channels), 2);
	channels /= 8; // Convert from bit depth to bytes (assuming 24-bit BMP)
	if (channels != 3) {
		std::cerr << "Unsupported BMP format (only 24-bit supported)" << std::endl;
		return;
	}
	int rowStride = ((width * 3 + 3) & ~3); // Row size with padding
	data = new unsigned char[rowStride * height];
	file.seekg(54); // Skip header and color table
	file.read(reinterpret_cast<char*>(data), rowStride * height);
	// Convert BGR to RGB
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			unsigned char* pixel = data + (i * rowStride) + (j * 3);
			std::swap(pixel[0], pixel[2]); // Swap blue and red
		}
	}
	valid = true;
}

void BmpImage::flipVertically() {
	int rowStride = ((width * 3 + 3) & ~3); // Row size with padding
	std::vector<unsigned char> rowBuffer(rowStride);
	for (int i = 0; i < height / 2; ++i) {
		unsigned char* row1 = data + i * rowStride;
		unsigned char* row2 = data + (height - i - 1) * rowStride;
		std::memcpy(rowBuffer.data(), row1, rowStride);
		std::memcpy(row1, row2, rowStride);
		std::memcpy(row2, rowBuffer.data(), rowStride);
	}
}
