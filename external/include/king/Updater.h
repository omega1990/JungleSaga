// (C) king.com Ltd 2014

#pragma once

namespace King {
	struct Input;

	class Updater {
	public:
		// True virtual function
		virtual void Update() = 0;
	protected:
		~Updater() {}
	};
}
