#pragma once
#include <vector>
#include <x264.h>
struct X264Writer {
    x264_param_t param;
    x264_picture_t pic;
    x264_picture_t pic_out;
    x264_t *h;
    int i_frame = 0;
    int i_frame_size;
    x264_nal_t *nal;
    int i_nal;
    int width, height;

    std::vector<uint8_t> outBuffer;

    X264Writer(int width, int height) : width{width}, height{height} {
        x264_param_default_preset(&param, "ultrafast", "zerolatency");



        /* Configure non-default params */
        param.i_bitdepth = 8;
        param.i_csp = X264_CSP_I420;
        param.i_width  = width;
        param.i_height = height;
        param.b_vfr_input = 0;
        param.b_repeat_headers = 1;
        param.b_annexb = 1;
        param.i_log_level = X264_LOG_NONE;

        x264_picture_init(&pic);
        pic.img.i_csp = X264_CSP_I420;
        pic.img.i_plane = 3;
        pic.img.i_stride[0] = width;
        pic.img.i_stride[1] = width / 2;
        pic.img.i_stride[2] = width / 2;

        
        h = x264_encoder_open( &param );

    }

    ~X264Writer() {
       
        x264_encoder_close( h );
    }

    void writeFrame(uint8_t* yuv) {
        pic.img.plane[0] = yuv;
        pic.img.plane[1] = yuv + width * height;
        pic.img.plane[2] = yuv + width * height + width * height / 4;
        pic.i_pts = i_frame++;
        i_frame_size = x264_encoder_encode( h, &nal, &i_nal, &pic, &pic_out );

        outBuffer.insert(outBuffer.end(), nal->p_payload, nal->p_payload + i_frame_size);
    }

    std::vector<uint8_t> flushBuffer() {
        while( x264_encoder_delayed_frames( h ) )
        {
            i_frame_size = x264_encoder_encode( h, &nal, &i_nal, NULL, &pic_out );
            outBuffer.insert(outBuffer.end(), nal->p_payload, nal->p_payload + i_frame_size);
        }
        auto ret = std::move(outBuffer);
        outBuffer.clear();
        return ret;
    }
};
