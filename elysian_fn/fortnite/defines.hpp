#pragma once

namespace elysian_fn {
	namespace variables {
		char local_team;
		fvector camera_location;
		frotator camera_rotation;
		float camera_fov;
		fvector2d screen_size, screen_center;
		ucanvas* canvas;
	}

	namespace defines {
		void(__fastcall* draw_transition_o)(game_viewport*, ucanvas*);
	}
}