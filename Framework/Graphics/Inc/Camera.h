#pragma once


namespace FlowerEngine::Graphics
{
	class Camera
	{
	public:

		enum class ProjectionMode
		{
			Perpective,
			Orthographic
		};

		void SetMode(ProjectionMode mode);

		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void SetLookAt(const Math::Vector3& target);

		//Perpective params
		void SetFov(float fov);
		void SetAspectRatio(float ratio);
		
		//Orthographic params
		void SetSice(float width, float height);

		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);


		//==============TODO movement calls

		//Return values
		const Math::Vector3& GetPosition()const;
		const Math::Vector3& GetDirection()const;

		Math::Matrix4 GetViewMatrix()const;
		Math::Matrix4 GetProjectionMatrix()const;

		//ProjetionMode transform
		Math::Matrix4 GetPerpectiveMatrix()const;
		Math::Matrix4 GetOrthographicMatrix()const;

		//Camera movement
		//3 degrees of freedom for translation
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		//2 degrees of freedom for rotation
		void Yaw(float radians);
		void Pitch(float radians);

	private:
		ProjectionMode mProjectionMode = ProjectionMode::Perpective;

		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		float mFov = 60 * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;

		float mWidth = 0.0f;
		float mHeight = 0.0f;

		float mNearPlane = 0.01f;
		float mFarPlane = 10000.0f;
	};
}