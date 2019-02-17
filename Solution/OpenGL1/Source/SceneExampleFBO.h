#ifndef SCENE_EXAMPLE_FBO
#define SCENE_EXAMPLE_FBO

#include "Scene.h"
#include "Paintable.h"
#include "Car.h"

class SceneExampleFBO : public Scene
{
public:
	SceneExampleFBO();
	~SceneExampleFBO();
protected:

	GameObject test1, test2;
	Car testCar;

	virtual void InitDerived();
	virtual void RenderDerived();
	virtual void UpdateDerived(double dt);
	virtual void UpdateDerivedBounced(double dt);
};

#endif // !SCENE_EXAMPLE_FBO
