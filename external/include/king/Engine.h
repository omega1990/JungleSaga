// (C) king.com Ltd 2014

#pragma once

#include <glm/fwd.hpp>
#include <memory>

namespace King {
	class Updater;
	class Engine {
	public:
		enum Texture {
			TEXTURE_BACKGROUND,
			TEXTURE_BLUE,
			TEXTURE_GREEN,
			TEXTURE_PURPLE,
			TEXTURE_RED,
			TEXTURE_YELLOW,
			TEXTURE_MAX
		};

		// Constructor
		Engine(const char* assetsDirectory);
		// Destructor
		~Engine();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		void Render(Texture texture, const glm::mat4& transform);
		void Render(Texture texture, float x, float y, float rotation = 0.0f);


		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		int GetWidth() const;
		int GetHeight() const;

	private:
		struct EngineImplementation;

		// Smart pointer - http://en.cppreference.com/w/cpp/memory/unique_ptr
		// Object is destroyed automatically on exiting out of the scope
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}