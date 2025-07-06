#pragma once

#include "Component/JComponent.h"


class AActor;

class JCamera : public JComponent
{
public:
	enum class EProjectionType
	{
		Perspective,
		Orthographic
	};

	static FMatrix GetGpuViewMatrix() { return ViewMatrix; }
	static FMatrix GetGpuProjectionMatrix() { return ProjectionMatrix; }
	static void SetGpuViewMatrix(FMatrix matrix) { ViewMatrix = matrix; }
	static void SetGpuProjectionMatrix(FMatrix matrix) { ProjectionMatrix = matrix; }

	JCamera();
	~JCamera();

	void	Initialize()		override;
	void	Update()			override;
	void	LateUpdate()		override;
	void	Render()			override;

    void	Serialize(json& jsonObject) const	override;
    void	Deserialize(const json& jsonObject) override;

	void	CreateViewMatrix();
	void	CreateProjectionMatrix(EProjectionType type);

	void	SetProjectionType(EProjectionType type) { mProjectionType = type; }
	void	SetSize(float size) { mSize = size; }

private:
	static FMatrix	ViewMatrix;
	static FMatrix	ProjectionMatrix;

	EProjectionType mProjectionType;

	FMatrix			mViewMatrix;
	FMatrix			mProjectionMatrix;
	float			mAspectRatio;
	float			mNear;
	float			mFar;
	float			mSize;

};