#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Primitives/ComputeShaderConfiguration.h"
#include "BlackBirdBox/Primitives/Point.h"
#include "Buffers/VertexArray.h"

#include <glm/glm.hpp>

namespace BlackBirdBox {

class RendererAPI {
public:
    enum class API { None = 0, OpenGL = 1 };

    enum class DepthFunction { NEVER = 0, LESS = 1, EQUAL = 2, LEQUAL = 3, GREATER = 4, NOTEQUAL = 5, GEQUAL = 6, ALWAYS = 7 };

    enum class PolygonMode { POINT = 0, LINE = 1, FILL = 2 };

    enum class RenderCapabilities {
        ALPHA_TEST,
        AUTO_NORMAL,
        BLEND,
        CLIP_PLANE_I,
        COLOR_LOGIC_OP,
        COLOR_MATERIAL,
        COLOR_SUM,
        COLOR_TABLE,
        CONVOLUTION_1D,
        CONVOLUTION_2D,
        CULL_FACE,
        DEPTH_TEST,
        DITHER,
        FOG,
        HISTOGRAM,
        INDEX_LOGIC_OP,
        LIGHT_I,
        LIGHTING,
        LINE_SMOOTH,
        LINE_STIPPLE,
        MAP1_COLOR_4,
        MAP1_INDEX,
        MAP1_NORMAL,
        MAP1_TEXTURE_COORD_1,
        MAP1_TEXTURE_COORD_2,
        MAP1_TEXTURE_COORD_3,
        MAP1_TEXTURE_COORD_4,
        MAP1_VERTEX_3,
        MAP1_VERTEX_4,
        MAP2_COLOR_4,
        MAP2_INDEX,
        MAP2_NORMAL,
        MAP2_TEXTURE_COORD_1,
        MAP2_TEXTURE_COORD_2,
        MAP2_TEXTURE_COORD_3,
        MAP2_TEXTURE_COORD_4,
        MAP2_VERTEX_3,
        MAP2_VERTEX_4,
        MINMAX,
        MULTISAMPLE,
        NORMALIZE,
        POINT_SMOOTH,
        POINT_SPRITE,
        POLYGON_OFFSET_FILL,
        POLYGON_OFFSET_LINE,
        POLYGON_OFFSET_POINT,
        POLYGON_SMOOTH,
        POLYGON_STIPPLE,
        POST_COLOR_MATRIX_COLOR_TABLE,
        POST_CONVOLUTION_COLOR_TABLE,
        RESCALE_NORMAL,
        SAMPLE_ALPHA_TO_COVERAGE,
        SAMPLE_ALPHA_TO_ONE,
        SAMPLE_COVERAGE,
        SEPARABLE_2D,
        SCISSOR_TEST,
        STENCIL_TEST,
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBE_MAP,
        TEXTURE_GEN_Q,
        TEXTURE_GEN_R,
        TEXTURE_GEN_S,
        TEXTURE_GEN_T,
        VERTEX_PROGRAM_POINT_SIZE,
        VERTEX_PROGRAM_TWO_SIDE,
    };

public:
    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
    virtual void DrawPoints(const std::vector<Ref<Point>>& points) = 0;
    virtual void DispatchCompute(const ComputeShaderConfiguration& compute_shader_configuration) = 0;
    virtual void WaitMemoryBarrier() = 0;
    virtual void SetDepthFunction(DepthFunction depth_function) = 0;
    virtual void SetPolygonMode(PolygonMode mode) = 0;
    virtual void Enable(RenderCapabilities capability) = 0;
    virtual void Disable(RenderCapabilities capability) = 0;

    inline static API GetAPI() { return s_API; }
    static Scope<RendererAPI> Create();

private:
    static API s_API;
};

}
