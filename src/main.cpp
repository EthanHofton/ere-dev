#include <ere/mappings/keyMap.hpp>
#include <ere/core/entryPoint.hpp>
#include <ere/core/layer.hpp>
#include <imgui.h>
#include <glad/gl.h>

class l1 : public ere::layer {
    
public:

    l1(std::weak_ptr<ere::application> t_app) : m_app(t_app) {}

    virtual bool onAttach(ere::attachEvent& t_e) override {

        m_vertices = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        const char* vSource = "#version 330 core\n"
            "layout (location=0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        
        unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vShader, 1, &vSource, NULL);
        glCompileShader(vShader);
        int  success;
        char infoLog[512];
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        const char* fSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
        
        unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fShader, 1, &fSource, NULL);
        glCompileShader(fShader);

        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMET::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        shader = glCreateProgram();
        glAttachShader(shader, vShader);
        glAttachShader(shader, fShader);
        glLinkProgram(shader);

        glDeleteShader(vShader);
        glDeleteShader(fShader);

        return false;
    }

    virtual bool onDetach(ere::detachEvent& t_e) override {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteProgram(shader);

        return false;
    }

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

    virtual bool onUpdate(ere::updateEvent& t_e) override {

        glBindVertexArray(VAO);
        glUseProgram(shader);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        return false;
    }

    virtual bool onImguiUpdate(ere::imguiUpdateEvent& t_e) override {
        ImGui::ShowDemoWindow();

        return false;
    }

private:

    std::weak_ptr<ere::application> m_app;
    std::vector<float> m_vertices;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int shader;

};

class l2 : public ere::layer {
    
    virtual bool onKeyPressed(ere::keyPressedEvent& t_e) override {
        std::cout << "layer 2: " << t_e.toString() << std::endl;

        return true;
    }
};

std::shared_ptr<ere::application> ere::createApplication() {
    ere::windowAPI::windowProps props = {"Hello, World", 600, 400};
    std::shared_ptr<ere::application> app = std::make_shared<ere::application>(props);

    app->setWindowSwapInterval(0);
    app->setBackgroundColor({1.0f, 0.0f, 0.0f, 1.0f});

    app->setWindowTitle("Hello again, World!");

    app->pushOverlay(std::make_shared<l1>(app));
    app->pushLayer(std::make_shared<l2>());

    return app;
}
