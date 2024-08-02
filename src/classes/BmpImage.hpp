#ifndef BMPIMAGE_HPP
#define BMPIMAGE_HPP

#include <string>
#include <vector>

class BmpImage {
public:
    BmpImage(const std::string& filepath);
    ~BmpImage();

    bool isValid() const;
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    unsigned char* getData() const;

    static void setFlipVerticallyOnLoad(bool enable);

private:
    int width, height, channels;
    unsigned char* data;
    bool valid;

    void loadBMP(const std::string& filepath);
    void flipVertically();

    static bool flipVerticallyOnLoad;
};

#endif // BMPIMAGE_HPP
