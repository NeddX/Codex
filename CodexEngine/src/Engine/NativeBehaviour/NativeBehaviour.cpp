#include "NativeBehaviour.h"

#include <Engine/Scene/Components.h>

namespace codex {
    TransformComponent& NativeBehaviour::GetTransform() noexcept
    {
        return m_Parent.GetComponent<TransformComponent>();
    }
} // namespace codex
