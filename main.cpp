
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include "Utils.hpp"
#include "x264writer.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

X264Writer* writer = nullptr;
cv::Mat srcImage;
cv::Mat dstImage;

extern "C" void initWriter(int width, int height) {
    if (writer) delete writer;
    writer = new X264Writer(width, height);
}

extern "C" void finalize() {

    auto out = writer->flushBuffer();
    FILE* file = fopen("output.h264", "wb");
    fwrite(out.data(), 1, out.size(), file);
    fclose(file);
    delete writer;
    writer = nullptr;
}

extern "C" void appendImage(uint8_t* binaryJPEG, int length)
{
    const int width = writer->width;
    const int height = writer->height;
    {
        cv::Mat flatArray(1, length, CV_8U, binaryJPEG);
        srcImage = cv::imdecode(flatArray, cv::IMREAD_COLOR);
        dstImage = cv::Mat(height, width, CV_8UC3);
    }
    cv::Mat YUV;
    RGBA src(srcImage.data, srcImage.cols, srcImage.rows);
    RGBA dst(dstImage.data, dstImage.cols, dstImage.rows);

    for(int i = 0; i <= 100; i++) {
        viewerQuery(src, dst, width, height, pi / 2, i / 255.0);
        cv::cvtColor(dstImage, YUV, cv::COLOR_BGR2YUV_I420);
        writer->writeFrame(YUV.data);
    }
    // const int transition_frames = 10;
    // for (int i = 0; i <= transition_frames; ++i) {
    //     cv::Mat img(height, width, CV_8UC3);
    //     const float t = i / (float)transition_frames;
    //     for (int j = 0; j < 3 * width * height; ++j)
    //         img.data[j] = 255.0f * t + (1 - t) * dstImage.data[j];
    //     cv::cvtColor(img, YUV, cv::COLOR_BGR2YUV_I420);
    //     writer->writeFrame(YUV.data);
    // }
   

}
