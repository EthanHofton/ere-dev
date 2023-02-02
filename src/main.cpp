#include <ere/mappings/keyMap.hpp>
#include <ere/core/entryPoint.hpp>
#include <ere/core/layer.hpp>
#include <ere/core/logger.hpp>
#include <ere/ecs/entity.hpp>
#include <imgui.h>

class l1 : public ere::layer {
    
public:

    l1(std::weak_ptr<ere::application> t_app) : m_app(t_app) {}

    virtual bool onKeyPressed(ere::keyPressedEvent& t_e) override {
        std::cout << "layer 1: " << t_e.toString() << std::endl;

        if (t_e.getKeyCode() == ERE_KEY_C) {
            m_app.lock()->setBackgroundColor({0.0f, 1.f, 0.f, 1.f});
        }

        if (t_e.getKeyCode() == ERE_KEY_F) {
            m_app.lock()->setFPS(120);
        }

        return false;
    }

    virtual bool onImguiUpdate(ere::imguiUpdateEvent& t_e) override {
        ImGui::ShowDemoWindow();

        return false;
    }

private:

    std::weak_ptr<ere::application> m_app;
};

class l2 : public ere::scene {
public:

    virtual bool onAttach(ere::attachEvent& t_e) override {

        test_e = createEntity();

        return false;
    }

    virtual bool onKeyPressed(ere::keyPressedEvent& t_e) override {
        if (t_e.getKeyCode() == ERE_KEY_V) {
            ERE_INFO("test_e uuid: {}", test_e.getComponent<ere::uuid>().getUUID());
        } else if (t_e.getKeyCode() == ERE_KEY_R) {
            test_e.getComponent<ere::uuid>().regenerate();
        }

        return false;
    }

private:

    ere::entity test_e;

};

ere::ref<ere::application> ere::createApplication() {
    ere::windowAPI::windowProps props = {"Hello, World", 600, 400};
    ere::ref<ere::application> app = ere::createRef<ere::application>(props);

    app->setWindowSwapInterval(0);
    app->setBackgroundColor({1.0f, 0.0f, 0.0f, 1.0f});

    app->setWindowTitle("Hello again, World!");

    app->pushOverlay(std::make_shared<l1>(app));
    app->pushLayer(std::make_shared<l2>());

    return app;
}
