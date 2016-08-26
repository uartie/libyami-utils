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

#include <dlfcn.h>
#include <iostream>
#include <tr1/functional>
#include <va/va.h>

template<typename T>
class VAFunction : public std::tr1::function<T>
{
public:
    bool loadv(const std::string& name, const std::string& version)
    {
        dlerror();
        void * const fn = dlvsym(RTLD_NEXT, name.c_str(), version.c_str());
        char * const error = dlerror();
        if (error or not fn) {
            std::cerr << "can't load symbol '"
                << name << "' version '" << version << "'";
            if (error)
                std::cerr << ": " << error;
            std::cerr << std::endl;
            return false;
        }
        std::tr1::function<T> f = reinterpret_cast<T*>(fn);
        this->swap(f);
        return bool(*this);
    }

    bool load(const std::string& name)
    {
        dlerror();
        void * const fn = dlsym(RTLD_NEXT, name.c_str());
        char * const error = dlerror();
        if (error or not fn) {
            std::cerr << "can't load symbol '" << name << "'";
            if (error)
                std::cerr << ": " << error;
            std::cerr << std::endl;
            return false;
        }
        std::tr1::function<T> f = reinterpret_cast<T*>(fn);
        this->swap(f);
        return bool(*this);
    }
};

class VAVTable
{
public:
    static const VAVTable& Instance()
    {
        static const VAVTable vtable;
        return vtable;
    }

    VAFunction<VAStatus(
        VADisplay, VAProfile, VAEntrypoint,
        VAConfigAttrib*, int, VAConfigID*)> createConfig;

    VAFunction<VAStatus(
        VADisplay, VAConfigID)> destroyConfig;

    VAFunction<VAStatus(
        VADisplay, VAConfigID, int, int, int, VASurfaceID *, int,
        VAContextID *)> createContext;

    VAFunction<VAStatus(
        VADisplay, VAContextID)> destroyContext;

    VAFunction<VAStatus(
        VADisplay, unsigned, unsigned, unsigned, VASurfaceID *, unsigned,
        VASurfaceAttrib *, unsigned)> createSurfaces;

    VAFunction<VAStatus(
        VADisplay, VASurfaceID *, int)> destroySurfaces;

    VAFunction<VAStatus(
        VADisplay, VAContextID, VABufferType, unsigned, unsigned, void *,
        VABufferID *)> createBuffer;

    VAFunction<VAStatus(
        VADisplay, VABufferID)> destroyBuffer;

    VAFunction<VAStatus(
        VADisplay, VABufferID, void **)> mapBuffer;

    VAFunction<VAStatus(
        VADisplay, VABufferID)> unmapBuffer;

    VAFunction<VAStatus(
        VADisplay, VAContextID, VASurfaceID)> beginPicture;

    VAFunction<VAStatus(
        VADisplay, VAContextID, VABufferID *, int)> renderPicture;

    VAFunction<VAStatus(
        VADisplay, VAContextID)> endPicture;

private:
    VAVTable()
    {
        createConfig.load("vaCreateConfig");
        destroyConfig.load("vaDestroyConfig");

        createContext.load("vaCreateContext");
        destroyContext.load("vaDestroyContext");

        createSurfaces.loadv("vaCreateSurfaces", "VA_API_0.33.0");
        destroySurfaces.load("vaDestroySurfaces");

        createBuffer.load("vaCreateBuffer");
        destroyBuffer.load("vaDestroyBuffer");

        mapBuffer.load("vaMapBuffer");
        unmapBuffer.load("vaUnmapBuffer");

        beginPicture.load("vaBeginPicture");
        renderPicture.load("vaRenderPicture");
        endPicture.load("vaEndPicture");
    }
};

extern "C" {
    VAStatus vaCreateConfig(VADisplay display, VAProfile profile,
        VAEntrypoint entrypoint, VAConfigAttrib *attributes, int nattribs,
        VAConfigID *id)
    {
        std::cout << "vaCreateConfig!!!!" << std::endl;
        return VAVTable::Instance().createConfig(display, profile, entrypoint,
            attributes, nattribs, id);
    }

    VAStatus vaDestroyConfig(VADisplay display, VAConfigID id)
    {
        std::cout << "vaDestroyConfig!!!!" << std::endl;
        return VAVTable::Instance().destroyConfig(display, id);
    }

    VAStatus vaCreateContext(VADisplay display, VAConfigID configId,
        int width, int height, int flag, VASurfaceID *renderTargets,
        int numRenderTargets, VAContextID *id)
    {
        std::cout << "vaCreateContext!!!!" <<  std::endl;
        return VAVTable::Instance().createContext(
            display, configId, width, height, flag, renderTargets,
            numRenderTargets, id);
    }

    VAStatus vaDestroyContext(VADisplay display, VAContextID id)
    {
        std::cout << "vaDestroyContext!!!!" << std::endl;
        return VAVTable::Instance().destroyContext(display, id);
    }

    VAStatus vaCreateSurfaces(VADisplay display, unsigned format,
        unsigned width, unsigned height, VASurfaceID *surfaces,
        unsigned numSurfaces, VASurfaceAttrib *attributes, unsigned numAttribs)
    {
        std::cout << "vaCreateSurfaces!!!!" << std::endl;
        return VAVTable::Instance().createSurfaces(display, format, width,
            height, surfaces, numSurfaces, attributes, numAttribs);
    }

    VAStatus vaDestroySurfaces(VADisplay display, VASurfaceID *surfaces,
        int numSurfaces)
    {
        std::cout << "vaDestroySurfaces!!!!" << std::endl;
        return VAVTable::Instance().destroySurfaces(display, surfaces,
            numSurfaces);
    }

    VAStatus vaCreateBuffer(VADisplay display, VAContextID context,
        VABufferType type, unsigned size, unsigned numElements, void *data,
        VABufferID *id)
    {
        std::cout << "vaCreateBuffer!!!!" << std::endl;
        return VAVTable::Instance().createBuffer(display, context, type, size,
            numElements, data, id);
    }

    VAStatus vaDestroyBuffer(VADisplay display, VABufferID id)
    {
        std::cout << "vaDestroyBuffer!!!!" << std::endl;
        return VAVTable::Instance().destroyBuffer(display, id);
    }


    VAStatus vaMapBuffer(VADisplay display, VABufferID id, void **buffer)
    {
        std::cout << "vaMapBuffer!!!!" << std::endl;
        return VAVTable::Instance().mapBuffer(display, id, buffer);
    }

    VAStatus vaUnmapBuffer(VADisplay display, VABufferID id)
    {
        std::cout << "vaUnmapBuffer!!!!" << std::endl;
        return VAVTable::Instance().unmapBuffer(display, id);
    }

    VAStatus vaBeginPicture(VADisplay display, VAContextID id, VASurfaceID target)
    {
        std::cout << "vaBeginPicture!!!!" << std::endl;
        return VAVTable::Instance().beginPicture(display, id, target);
    }

    VAStatus vaRenderPicture(VADisplay display, VAContextID id,
        VABufferID *buffers, int numBuffers)
    {
        std::cout << "vaRenderPicture!!!!" << std::endl;
        return VAVTable::Instance().renderPicture(display, id, buffers,
            numBuffers);
    }

    VAStatus vaEndPicture(VADisplay display, VAContextID context)
    {
        std::cout << "vaEndPicture!!!!" << std::endl;
        return VAVTable::Instance().endPicture(display, context);
    }
} // extern "C"
