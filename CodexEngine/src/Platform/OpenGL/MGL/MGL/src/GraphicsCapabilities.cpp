#include "GraphicsCapabilities.h"

#include "Constants.h"

namespace mgl {
    namespace GraphicsCapabilities {
        int GetMaxTextureSlotCount() noexcept
        {
            int max_slot_count;
            GL_Call(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_slot_count));
            return max_slot_count;
        }
    } // namespace GraphicsCapabilities
} // namespace mgl
