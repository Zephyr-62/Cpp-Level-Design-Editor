#pragma once

class Framebuffer {
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    void resize(int width, int height); 
    void bind() const;
    static void unbind();

    unsigned int colorTexture() const { return m_colorTexture; }
    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    void create();
    void destroy();

    int m_width;
    int m_height;
    unsigned int m_fbo = 0;
    unsigned int m_colorTexture = 0;
    unsigned int m_rbo = 0;
};