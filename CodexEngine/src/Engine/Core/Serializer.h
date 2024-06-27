#ifndef CODEX_CORE_SERIALIZER_H
#define CODEX_CORE_SERIALIZER_H

#include <sdafx.h>

namespace codex {
    // Forward declerations.
    class Scene;
    class Entity;

    class CODEX_API Serializer
    {
        friend class Scene;
        friend class Entity;

    public:
        static void SerializeScene(const std::filesystem::path path, const Scene& scene);
        static void DeserializeScene(const std::filesystem::path path, Scene& scene);
    };
} // namespace codex
#endif // CODEX_CORE_SERIALIZER_H
