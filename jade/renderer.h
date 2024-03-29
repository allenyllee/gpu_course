#ifndef RENDERER_H
#define RENDERER_H

namespace jade
{
    class Scene;
    class Camera;

    class Renderer
    {
    public:
        virtual ~Renderer() = 0 {}
        virtual void Render(const Camera* camera, const Scene* scene) = 0;
    };

}

#endif