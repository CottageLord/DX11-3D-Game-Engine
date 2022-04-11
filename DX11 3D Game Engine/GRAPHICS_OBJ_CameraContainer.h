#pragma once
#include <vector>
#include <memory>

class Camera;
class Graphics;

namespace Rgph
{
	class RenderGraph;
}
class CameraContainer
{
public:
	/**
	  * @brief spawn control window
	  */
	void SpawnWindow(Graphics& gfx);
	void Bind(Graphics& gfx);
	void AddCamera(std::unique_ptr<Camera> pCam);
	/**
	  * @brief get the currently active camera
	  */
	Camera* operator->();
	~CameraContainer();
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit() const;
private:
	Camera& GetControlledCamera();
private:
	/// We want to maintain the fkexibility from pointers
	std::vector<std::unique_ptr<Camera>> cameras;
	/// Index of the selected camera
	int active = 0;
	int controlled = 0;
};