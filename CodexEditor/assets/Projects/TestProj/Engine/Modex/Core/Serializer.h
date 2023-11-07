#ifndef CODEX_CORE_SERIALIZER_H
#define CODEX_CORE_SERIALIZER_H

#include <sdafx.h>

namespace codex {
    // Forward declerations.
    class Scene;
    class Entity;

    class Serializer
    {
        friend class Scene;
        friend class Entity;

    public:
        static void SerializeScene(const char* path, const Scene& scene);
        static void DeserializeScene(const char* path, Scene& scene);
    };
} // namespace codex
#endif // CODEX_CORE_SERIALIZER_H
