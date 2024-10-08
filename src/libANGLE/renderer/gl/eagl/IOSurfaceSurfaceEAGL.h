//
// Copyright 2020 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef LIBANGLE_RENDERER_GL_EAGL_IOSURFACESURFACEEAGL_H_
#define LIBANGLE_RENDERER_GL_EAGL_IOSURFACESURFACEEAGL_H_

#include "libANGLE/renderer/gl/SurfaceGL.h"
#include "libANGLE/renderer/gl/eagl/DisplayEAGL.h"

#include <IOSurface/IOSurfaceRef.h>

namespace egl
{
class AttributeMap;
}  // namespace egl

namespace rx
{

class DisplayEAGL;
class FunctionsGL;
class StateManagerGL;

class IOSurfaceSurfaceEAGL : public SurfaceGL
{
  public:
    IOSurfaceSurfaceEAGL(const egl::SurfaceState &state,
                         RendererGL *renderer,
                         EAGLContextObj cglContext,
                         EGLClientBuffer buffer,
                         const egl::AttributeMap &attribs);
    ~IOSurfaceSurfaceEAGL() override;

    egl::Error initialize(const egl::Display *display) override;
    egl::Error makeCurrent(const gl::Context *context) override;
    egl::Error unMakeCurrent(const gl::Context *context) override;

    egl::Error swap(const gl::Context *context) override;
    egl::Error postSubBuffer(const gl::Context *context,
                             EGLint x,
                             EGLint y,
                             EGLint width,
                             EGLint height) override;
    egl::Error querySurfacePointerANGLE(EGLint attribute, void **value) override;
    egl::Error bindTexImage(const gl::Context *context,
                            gl::Texture *texture,
                            EGLint buffer) override;
    egl::Error releaseTexImage(const gl::Context *context, EGLint buffer) override;
    void setSwapInterval(const egl::Display *display, EGLint interval) override;

    EGLint getWidth() const override;
    EGLint getHeight() const override;

    EGLint isPostSubBufferSupported() const override;
    EGLint getSwapBehavior() const override;

    static bool validateAttributes(EGLClientBuffer buffer, const egl::AttributeMap &attribs);

    bool hasEmulatedAlphaChannel() const override;

    egl::Error attachToFramebuffer(const gl::Context *context,
                                   gl::Framebuffer *framebuffer) override;
    egl::Error detachFromFramebuffer(const gl::Context *context,
                                     gl::Framebuffer *framebuffer) override;

  private:
    angle::Result initializeAlphaChannel(const gl::Context *context, GLuint texture);

#if ANGLE_PLATFORM_IOS_FAMILY_SIMULATOR
    IOSurfaceLockOptions getIOSurfaceLockOptions() const;
#endif
    // TODO(geofflang): Don't store these, they are potentially specific to a single GL context.
    // http://anglebug.com/40096492
    const FunctionsGL *mFunctions;
    StateManagerGL *mStateManager;

    EAGLContextObj mEAGLContext;
    IOSurfaceRef mIOSurface;
    GLuint mFramebufferID;
    GLuint mTextureID;
    int mWidth;
    int mHeight;
    int mPlane;
    int mFormatIndex;
    int mRowStrideInPixels;

    bool mAlphaInitialized;
#if ANGLE_PLATFORM_IOS_FAMILY_SIMULATOR
    GLuint mBoundTextureID;
    bool mUploadFromIOSurface;
    bool mReadbackToIOSurface;
#endif
};

}  // namespace rx

#endif  // LIBANGLE_RENDERER_GL_EAGL_IOSURFACESURFACEEAGL_H_
