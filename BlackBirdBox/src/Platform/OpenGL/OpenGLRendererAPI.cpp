#include "bbbpch.h"
#include "OpenGLRendererAPI.h"

namespace Platform {

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        LOG_CRITICAL(message);
        return;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LOG_ERROR(message);
        return;
    case GL_DEBUG_SEVERITY_LOW:
        LOG_WARN(message);
        return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LOG_TRACE(message);
        return;
    }

    CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLRendererAPI::Init()
{
    PROFILE_FUNCTION();

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    PROFILE_FUNCTION();

    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
    PROFILE_FUNCTION();

    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear()
{
    PROFILE_FUNCTION();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const BlackBirdBox::Ref<BlackBirdBox::VertexArray>& vertexArray, uint32_t indexCount /*= 0*/)
{
    PROFILE_FUNCTION();

    uint32_t count
        = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount; // TODO(Rok Kos): Validate on more cases that this is correct fix
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRendererAPI::DrawPoints(const std::vector<BlackBirdBox::Ref<BlackBirdBox::Point>>& points)
{
    PROFILE_FUNCTION();

    glEnable(GL_POINT_SMOOTH);
    // TODO(Rok Kos): Figure out how to get size and color for all points without having to do multiple glBegin and glEnd
    glPointSize(10.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
    for (auto point : points) {
        glm::vec3 position = point->GetPosition();
        glVertex3f(position.x, position.y, position.z);
    }
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void OpenGLRendererAPI::DispatchCompute(const BlackBirdBox::ComputeShaderConfiguration& compute_shader_configuration)
{
    PROFILE_FUNCTION();

    glDispatchComputeGroupSizeARB(compute_shader_configuration.GetWorkGroupSize()[0], compute_shader_configuration.GetWorkGroupSize()[1],
        compute_shader_configuration.GetWorkGroupSize()[2], compute_shader_configuration.GetLocalGroupSize()[0],
        compute_shader_configuration.GetLocalGroupSize()[1], compute_shader_configuration.GetLocalGroupSize()[2]);
}

void OpenGLRendererAPI::WaitMemoryBarrier()
{
    PROFILE_FUNCTION();

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void OpenGLRendererAPI::SetDepthFunction(BlackBirdBox::RendererAPI::DepthFunction depth_function)
{
    PROFILE_FUNCTION();

    switch (depth_function) {
    case BlackBirdBox::RendererAPI::DepthFunction::NEVER:
        glDepthFunc(GL_NEVER);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::LESS:
        glDepthFunc(GL_LESS);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::EQUAL:
        glDepthFunc(GL_EQUAL);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::LEQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::GREATER:
        glDepthFunc(GL_GREATER);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::NOTEQUAL:
        glDepthFunc(GL_NOTEQUAL);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::GEQUAL:
        glDepthFunc(GL_GEQUAL);
        break;
    case BlackBirdBox::RendererAPI::DepthFunction::ALWAYS:
        glDepthFunc(GL_ALWAYS);
        break;
    default:
        CORE_ASSERT(false, "Depth function unknown");
        break;
    }
}

void OpenGLRendererAPI::SetPolygonMode(BlackBirdBox::RendererAPI::PolygonMode mode)
{
    PROFILE_FUNCTION();

    switch (mode) {
    case BlackBirdBox::RendererAPI::PolygonMode::POINT:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case BlackBirdBox::RendererAPI::PolygonMode::LINE:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case BlackBirdBox::RendererAPI::PolygonMode::FILL:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    default:
        CORE_ASSERT(false, "PolygonMode unknown");
        break;
    }
}

void OpenGLRendererAPI::Enable(RenderCapabilities capability) { glEnable(OpenGLRenderCapabilities(capability)); }

void OpenGLRendererAPI::Disable(RenderCapabilities capability) { glDisable(OpenGLRenderCapabilities(capability)); }

GLenum OpenGLRendererAPI::OpenGLRenderCapabilities(BlackBirdBox::RendererAPI::RenderCapabilities capability) const
{
    switch (capability) {
    case BlackBirdBox::RendererAPI::RenderCapabilities::ALPHA_TEST:
        return GL_ALPHA_TEST;
    case BlackBirdBox::RendererAPI::RenderCapabilities::AUTO_NORMAL:
        return GL_AUTO_NORMAL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::BLEND:
        return GL_BLEND;
    case BlackBirdBox::RendererAPI::RenderCapabilities::CLIP_PLANE_I:
        return GL_CLIP_PLANE0; // TODO(Rok Kos): Do proper implementation when you will use this
    case BlackBirdBox::RendererAPI::RenderCapabilities::COLOR_LOGIC_OP:
        return GL_COLOR_LOGIC_OP;
    case BlackBirdBox::RendererAPI::RenderCapabilities::COLOR_MATERIAL:
        return GL_COLOR_MATERIAL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::COLOR_SUM:
        return GL_COLOR_SUM;
    case BlackBirdBox::RendererAPI::RenderCapabilities::COLOR_TABLE:
        return GL_COLOR_TABLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::CONVOLUTION_1D:
        return GL_CONVOLUTION_1D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::CONVOLUTION_2D:
        return GL_CONVOLUTION_2D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::CULL_FACE:
        return GL_CULL_FACE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::DEPTH_TEST:
        return GL_DEPTH_TEST;
    case BlackBirdBox::RendererAPI::RenderCapabilities::DITHER:
        return GL_DITHER;
    case BlackBirdBox::RendererAPI::RenderCapabilities::FOG:
        return GL_FOG;
    case BlackBirdBox::RendererAPI::RenderCapabilities::HISTOGRAM:
        return GL_HISTOGRAM;
    case BlackBirdBox::RendererAPI::RenderCapabilities::INDEX_LOGIC_OP:
        return GL_INDEX_LOGIC_OP;
    case BlackBirdBox::RendererAPI::RenderCapabilities::LIGHT_I:
        return GL_LIGHT0; // TODO(Rok Kos): Do proper implementation when you will use this
    case BlackBirdBox::RendererAPI::RenderCapabilities::LIGHTING:
        return GL_LIGHTING;
    case BlackBirdBox::RendererAPI::RenderCapabilities::LINE_SMOOTH:
        return GL_LINE_SMOOTH;
    case BlackBirdBox::RendererAPI::RenderCapabilities::LINE_STIPPLE:
        return GL_LINE_STIPPLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_COLOR_4:
        return GL_MAP1_COLOR_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_INDEX:
        return GL_MAP1_INDEX;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_NORMAL:
        return GL_MAP1_NORMAL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_TEXTURE_COORD_1:
        return GL_MAP1_TEXTURE_COORD_1;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_TEXTURE_COORD_2:
        return GL_MAP1_TEXTURE_COORD_2;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_TEXTURE_COORD_3:
        return GL_MAP1_TEXTURE_COORD_3;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_TEXTURE_COORD_4:
        return GL_MAP1_TEXTURE_COORD_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_VERTEX_3:
        return GL_MAP1_VERTEX_3;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP1_VERTEX_4:
        return GL_MAP1_VERTEX_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_COLOR_4:
        return GL_MAP2_COLOR_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_INDEX:
        return GL_MAP2_INDEX;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_NORMAL:
        return GL_MAP2_NORMAL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_TEXTURE_COORD_1:
        return GL_MAP2_TEXTURE_COORD_1;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_TEXTURE_COORD_2:
        return GL_MAP2_TEXTURE_COORD_2;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_TEXTURE_COORD_3:
        return GL_MAP2_TEXTURE_COORD_3;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_TEXTURE_COORD_4:
        return GL_MAP2_TEXTURE_COORD_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_VERTEX_3:
        return GL_MAP2_VERTEX_3;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MAP2_VERTEX_4:
        return GL_MAP2_VERTEX_4;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MINMAX:
        return GL_MINMAX;
    case BlackBirdBox::RendererAPI::RenderCapabilities::MULTISAMPLE:
        return GL_MULTISAMPLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::NORMALIZE:
        return GL_NORMALIZE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POINT_SMOOTH:
        return GL_POINT_SMOOTH;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POINT_SPRITE:
        return GL_POINT_SPRITE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POLYGON_OFFSET_FILL:
        return GL_POLYGON_OFFSET_FILL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POLYGON_OFFSET_LINE:
        return GL_POLYGON_OFFSET_LINE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POLYGON_OFFSET_POINT:
        return GL_POLYGON_OFFSET_POINT;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POLYGON_SMOOTH:
        return GL_POLYGON_SMOOTH;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POLYGON_STIPPLE:
        return GL_POLYGON_STIPPLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POST_COLOR_MATRIX_COLOR_TABLE:
        return GL_POST_COLOR_MATRIX_COLOR_TABLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::POST_CONVOLUTION_COLOR_TABLE:
        return GL_POST_CONVOLUTION_COLOR_TABLE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::RESCALE_NORMAL:
        return GL_RESCALE_NORMAL;
    case BlackBirdBox::RendererAPI::RenderCapabilities::SAMPLE_ALPHA_TO_COVERAGE:
        return GL_SAMPLE_ALPHA_TO_COVERAGE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::SAMPLE_ALPHA_TO_ONE:
        return GL_SAMPLE_ALPHA_TO_ONE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::SAMPLE_COVERAGE:
        return GL_SAMPLE_COVERAGE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::SEPARABLE_2D:
        return GL_SEPARABLE_2D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::SCISSOR_TEST:
        return GL_SCISSOR_TEST;
    case BlackBirdBox::RendererAPI::RenderCapabilities::STENCIL_TEST:
        return GL_STENCIL_TEST;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_1D:
        return GL_TEXTURE_1D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_2D:
        return GL_TEXTURE_2D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_3D:
        return GL_TEXTURE_3D;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_CUBE_MAP:
        return GL_TEXTURE_CUBE_MAP;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_GEN_Q:
        return GL_TEXTURE_GEN_Q;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_GEN_R:
        return GL_TEXTURE_GEN_R;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_GEN_S:
        return GL_TEXTURE_GEN_S;
    case BlackBirdBox::RendererAPI::RenderCapabilities::TEXTURE_GEN_T:
        return GL_TEXTURE_GEN_T;
    case BlackBirdBox::RendererAPI::RenderCapabilities::VERTEX_PROGRAM_POINT_SIZE:
        return GL_VERTEX_PROGRAM_POINT_SIZE;
    case BlackBirdBox::RendererAPI::RenderCapabilities::VERTEX_PROGRAM_TWO_SIDE:
        return GL_VERTEX_PROGRAM_TWO_SIDE;
    }

    CORE_ASSERT(false, "RenderCapabilities unknown");
    return 0;
}

}
