#ifndef CODEX_PHYSICS_MATERIAL_2D_H
#define CODEX_PHYSICS_MATERIAL_2D_H

namespace codex::phys {
    struct PhysicsMaterial2D
    {
        f32 density              = 1.0f;
        f32 friction             = 0.5f;
        f32 restitution          = 0.5f;
        f32 restitutionThreshold = 0.5f;
    };
} // namespace codex::phys

namespace nlohmann {
    template <>
    struct adl_serializer<codex::phys::PhysicsMaterial2D>
    {
        static void to_json(ordered_json& j, const codex::phys::PhysicsMaterial2D& mat)
        {
            j = ordered_json{ { "Density", mat.density },
                              { "Friction", mat.friction },
                              { "Restitution", mat.restitution },
                              { "RestitutionThreshold", mat.restitutionThreshold } };
        }
        static void from_json(const ordered_json& j, codex::phys::PhysicsMaterial2D& mat)
        {
            j.at("Density").get_to(mat.density);
            j.at("Friction").get_to(mat.friction);
            j.at("Restitution").get_to(mat.restitution);
            j.at("RestitutionThreshold").get_to(mat.restitutionThreshold);
        }
    };
} // namespace nlohmann

#endif // CODEX_PHYSICS_MATERIAL_2D_H
