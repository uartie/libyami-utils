/*
 * Copyright (C) 2016 Intel Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tests/decodeinput.h"

#include <cstring> // std::memset
#include <iostream>
#include <libyami/VideoDecoderHost.h>
#include <string>

namespace YVAC {

    typedef SharedPtr<DecodeInput> VideoInput;
    typedef SharedPtr<YamiMediaCodec::IVideoDecoder> VideoDecoder;
    typedef SharedPtr<NativeDisplay> NativeDisplay;

} // namespace YVAC

int main(int argc, const char** argv)
{
    YVAC::VideoInput input(DecodeInput::create(argv[1]));

    if (!input) {
        std::cerr << "invalid input file" << std::endl;
        return -1;
    }

    if (std::string(input->getMimeType()) != std::string(YAMI_MIME_H264)) {
        std::cerr << "invalid mime type " << input->getMimeType() << std::endl;
        std::cerr << "only " << YAMI_MIME_H264 << " is supported" << std::endl;
        return -1;
    }

    YVAC::VideoDecoder decoder(
        createVideoDecoder(input->getMimeType()), releaseVideoDecoder);

    YVAC::NativeDisplay native(new ::NativeDisplay);
    native->type = NATIVE_DISPLAY_DRM;
    native->handle = -1;

    decoder->setNativeDisplay(native.get());

    VideoConfigBuffer configBuffer;
    std::memset(&configBuffer, 0, sizeof(configBuffer));
    configBuffer.profile = VAProfileNone;

    if (decoder->start(&configBuffer) != DECODE_SUCCESS) {
        std::cerr << "failed to start decoder" << std::endl;
        return -1;
    }

    VideoDecodeBuffer buffer;
    unsigned n(0);
    while (input->getNextDecodeUnit(buffer)) {
        YamiStatus status = decoder->decode(&buffer);
        if (YAMI_DECODE_FORMAT_CHANGE == status) {
            do {
                SharedPtr<VideoFrame> frame = decoder->getOutput();
                if (!frame)
                    break;
                // putsurface
            } while (true);

            // continue decoding the current buffer
            status = decoder->decode(&buffer);
        }

        if (status != YAMI_SUCCESS) {
            std::cerr << "decode failed" << std::endl;
            return -1;
        }

        ++n;

        do {
            SharedPtr<VideoFrame> frame = decoder->getOutput();
            if (!frame)
                break;

            VARectangle crop;
            crop.x = frame->crop.x;
            crop.y = frame->crop.y;
            crop.width = frame->crop.width;
            crop.height = frame->crop.height;

            // putsurface

        } while (true);
    }

    decoder->stop();

    std::cout << "decoded " << n << " frames" << std::endl;

    return 0;
}
