#include "Renderer.h"
#include "MatrixStack.h"
#include <iostream>


class Game : public oglr::Renderer {
private:
    float totalTime = 0;
    oglr::primitive::Plane plane;
    oglr::ShaderProgram mandelbrotShaderProgram;
    oglr::ShaderProgram juliaShaderProgram;
    oglr::ShaderProgram defaultShaderProgram;
    double scale;
    float juliaScale;
    glm::dvec2 centre;
    glm::dvec2 juliaCentre;
    glm::vec2 screenSize;
    glutil::MatrixStack modelViewMatrixStack;
    int iter;
public:
    Game() {
    
    }

    // Called when engine is run once.
    void OnCreate() override {
        SetWindowTitle("Le Official Pog Rendering Engine");
        mandelbrotShaderProgram = oglr::MANDELBROT_DOUBLE_SHADER_PROGRAM;
        juliaShaderProgram = oglr::JULIA_SHADER_PROGRAM;
        defaultShaderProgram = oglr::DEFAULT_SHADER_PROGRAM;
        plane.Create();
        scale = 3.0;
        juliaScale = 3.12;
        centre = glm::dvec2(0.5, 0.0);
        juliaCentre = glm::dvec2(0.5, 1.5);
        screenSize = glm::vec2(GetScreenWidth(), GetScreenHeight());
        iter = 10;
    }

    //Called every update loop
    void OnUpdate(float fDeltaTimeSeconds) override{
        Clear(oglr::DARK_ELECTRIC_BLUE);
        double scaleModifier = 0.01f;
        totalTime += fDeltaTimeSeconds;
        if (oglr::Mouse::GetScrollY() > 0) {
            scale -= scaleModifier*10.0 * (scale);
        } else if (oglr::Mouse::GetScrollY() < 0) {
            scale += scaleModifier*10.0 * (scale);
        }
        if (oglr::Key::W.IsPressed || oglr::Key::W.IsHeld) {
            centre.y -= scaleModifier * (scale);
        }
        if (oglr::Key::A.IsPressed || oglr::Key::A.IsHeld) {
            centre.x += scaleModifier * (scale);
        }
        if (oglr::Key::S.IsPressed || oglr::Key::S.IsHeld) {
            centre.y += scaleModifier * (scale);
        }
        if (oglr::Key::D.IsPressed || oglr::Key::D.IsHeld) {
            centre.x -= scaleModifier * (scale);
        }
        if (oglr::Key::E.IsPressed || oglr::Key::E.IsHeld) {
            iter++;
            std::cout << iter << "\n";
        }
        if (oglr::Key::Q.IsPressed || oglr::Key::Q.IsHeld) {
            iter==1?iter=1:iter--;
            std::cout << iter << "\n";
        }
        glDisable(GL_CULL_FACE);
        //Matrix Stuff

        /*
        defaultShaderProgram.UseProgram();
        modelViewMatrixStack.SetIdentity();
        //modelViewMatrixStack.LookAt(m_Camera.GetPosition(), m_Camera.GetView(), m_Camera.GetUp());
        modelViewMatrixStack.Push();
            modelViewMatrixStack.Translate(glm::vec3(-0.5f, 0.0f, 0.0f));
            defaultShaderProgram.SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            defaultShaderProgram.SetUniform("matrices.projectionMatrix", glm::mat4(1));
            plane.Render();
        modelViewMatrixStack.Pop();
        */
        //mandelbrot
        
        RenderMandelbrot();
     
        
    }

    void RenderMandelbrot() {
        modelViewMatrixStack.Push();
            mandelbrotShaderProgram.UseProgram();
            mandelbrotShaderProgram.SetUniform("time", (int)(totalTime * 20));
            mandelbrotShaderProgram.SetUniform("scale", scale);
            mandelbrotShaderProgram.SetUniform("screenSize", glm::dvec2(GetScreenWidth(), GetScreenHeight()));
            mandelbrotShaderProgram.SetUniform("centre", centre);
            mandelbrotShaderProgram.SetUniform("iter", iter);
            mandelbrotShaderProgram.SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            mandelbrotShaderProgram.SetUniform("matrices.projectionMatrix", glm::mat4(1));
            plane.Render();
        modelViewMatrixStack.Pop();

        
        
        modelViewMatrixStack.Push();
            juliaShaderProgram.UseProgram();
            juliaShaderProgram.SetUniform("scale", juliaScale);
            juliaShaderProgram.SetUniform("screenSize", glm::vec2(GetScreenWidth(), GetScreenHeight()));
            juliaShaderProgram.SetUniform("centre", juliaCentre);
            // glm::vec2 c = glm::vec2((oglr::Mouse::GetX() / GetScreenWidth() - 0.5f) * juliaScale - centre.x, oglr::Mouse::GetY() / (GetScreenHeight() + 0.5f) * juliaScale - centre.y - 1.5f);
            glm::vec2 c = glm::vec2((oglr::Mouse::GetX() / GetScreenWidth() - 0.5) * scale - centre.x, ((GetScreenHeight() - oglr::Mouse::GetY()) / (GetScreenHeight()) - 0.5)*scale - centre.y);
           // std::cout << glm::to_string(c) << ", scale: " << juliaScale << "\n";
            juliaShaderProgram.SetUniform("c", c);
            modelViewMatrixStack.Translate(glm::vec3(-0.75f, 0.75f, -0.1f));
            juliaShaderProgram.SetUniform("translate", glm::vec2(-0.75f, 0.75f));
            modelViewMatrixStack.Scale(0.25f);
            juliaShaderProgram.SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
            juliaShaderProgram.SetUniform("matrices.projectionMatrix", glm::mat4(1));
            plane.Render();
        modelViewMatrixStack.Pop();
        
    }
};


int main() {
    
    Game game;
    game.Create();
    game.Run();

    return 0;
}
