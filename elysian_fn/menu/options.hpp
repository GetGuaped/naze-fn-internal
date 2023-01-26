#pragma once

namespace elysian_fn {
	namespace options {
		bool show_menu = true;
		double r, g, b;

		namespace aimbot {
			bool enable = false;
			bool interpolate = false;
			bool constant = false;
			bool visible_only = false;
			bool show_fov = false;
			float speed = 5;
			float fov = 120;
		}

		namespace player {
			bool enable = false;
			bool skeleton = false;
			bool box = false;
			bool snapline = false;
			bool view_angle = false;
			bool distance = false;
			bool active_item = false;
			bool username = false;
			bool platform = false;
			float max_distance = 225;
		}

		namespace environment {
			//bool enable = false;
			bool show_distance = false;
			float max_distance = 225;
		}

		namespace pickup {
			bool enable = false;
			bool common = false;
			bool uncommon = false;
			bool rare = false;
			bool epic = false;
			bool legendary = false;
			bool mythic = false;
		}

		bool disableUAC = false;
	}
}