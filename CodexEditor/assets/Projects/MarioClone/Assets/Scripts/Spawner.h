#include <Codex.h>

using namespace codex;

RF_CLASS()
class Spawner : public NativeBehaviour
{
    RF_GENERATE_BODY()

public:
    void OnInit() override {}
    void OnUpdate(const f32 deltaTime) override
    {
        static auto count = 0.0f;
        static auto ent   = 0;
        if (count >= 1.0f)
        {
            auto  entity = CreateEntity("bruh_" + std::to_string(++ent));
            auto& c      = entity.AddComponent<SpriteRendererComponent>();
            c.GetSprite().SetTexture(
                Resources::GetResource<gfx::Texture2D>("Assets/Super Mario Bros - Codex/tilesheet_crt.png"));
            c.GetSprite().SetSize({ 64.0f, 64.0f });
            entity.GetTransform().position =
                Vector3f{ static_cast<f32>((std::rand() % (500 + -500 + 1)) - -500),
                          static_cast<f32>((std::rand() % (500 + -500 + 1)) - -500), 0.0f };
            count = 0.0f;
        }
        count += deltaTime;
    }
};
