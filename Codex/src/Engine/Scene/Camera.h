#ifndef CODEX_CORE_CAMERA_H
#define CODEX_CORE_CAMERA_H

#include <sdafx.h>

#include "../Core/Geomtryd.h"

namespace codex::scene {
    class Camera
    {
    public:
        enum class ProjectionType
        {
            Orthographic,
            Perspective
        };

    private:
        i32            m_Width;
        i32            m_Height;
        f32            m_Fov;
        f32            m_NearClip;
        f32            m_FarClip;
        ProjectionType m_ProjectionType;
        Matrix4f       m_Projection;
        Matrix4f       m_View;

    public:
        Vector3f position;
        Vector3f target;

    public:
        Camera(const i32 width, const i32 height, Vector3f position = Vector3f(0.0f, 0.0f, 30.0f),
               const f32 nearClip = 0.0f, const f32 farClip = 100.0f,
               const ProjectionType projectionType = ProjectionType::Orthographic, const f32 fov = 90.0f,
               Vector3f target = Vector3f()) noexcept
            : m_Width(width)
            , m_Height(height)
            , m_ProjectionType(projectionType)
            , m_Fov(fov)
            , m_NearClip(nearClip)
            , m_FarClip(farClip)
            , m_Projection(Matrix4f(1.0f))
            , position(std::move(position))
            , target(std::move(target))
        {
            UpdateProjectionMatrix();
        }

    public:
        inline void UpdateProjectionMatrix() noexcept
        {
            if (m_ProjectionType == ProjectionType::Orthographic)
                m_Projection = glm::ortho(0.0f, (f32)m_Width, 0.0f, (f32)m_Height, m_NearClip, m_FarClip);
            else
                m_Projection = glm::perspective(m_Fov, (f32)m_Width / (f32)m_Height, m_NearClip, m_FarClip);
        }
        inline Matrix4f GetViewMatrix() noexcept
        {
            const auto camera_front = Vector3f(0.0f, 0.0f, -1.0f);
            const auto camera_up    = Vector3f(0.0f, 1.0f, 0.0f);
            const auto camera_dir   = glm::normalize(position - target);
            m_View                      = glm::lookAt(position, position + camera_front, camera_up);
            return m_View;
        }
        inline Matrix4f GetProjectionMatrix() const noexcept { return m_Projection; }
        inline i32      GetWidth() const noexcept { return m_Width; }
        inline i32      GetHeight() const noexcept { return m_Height; }
        inline void     SetProjectionType(const ProjectionType projectionType) noexcept
        {
            m_ProjectionType = projectionType;
            UpdateProjectionMatrix();
        }
        inline void SetFOV(const f32 fov) noexcept
        {
            m_Fov = fov;
            UpdateProjectionMatrix();
        }
        inline void SetWidth(const i32 newWidth) noexcept
        {
            m_Width = newWidth;
            UpdateProjectionMatrix();
        }
        inline void SetHeight(const i32 newHeight) noexcept
        {
            m_Height = newHeight;
            UpdateProjectionMatrix();
        }
    };
} // namespace codex

#endif // CODEX_CORE_CAMERA_H
