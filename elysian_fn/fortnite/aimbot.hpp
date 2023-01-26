#pragma once

namespace elysian_fn {
	namespace aimbot {
		static void set_aim(uobject* world, controller* handler, fvector aim_location, bool interpolate, bool constant, double speed) {
			auto aim_rotation = math::find_look_at_rotation(variables::camera_location, aim_location);
			auto world_delta_seconds = game_statics::get_world_delta_seconds(world);

			auto result = aim_rotation;
			if (interpolate) {
				if (constant)
					result = math::interpolate_constant(variables::camera_rotation, aim_rotation, world_delta_seconds, speed);
				else
					result = math::interpolate(variables::camera_rotation, aim_rotation, world_delta_seconds, speed);
			}

			result.pitch = (result.pitch - variables::camera_rotation.pitch) / handler->input_pitch_scale();
			result.yaw = (result.yaw - variables::camera_rotation.yaw) / handler->input_yaw_scale();

			handler->add_pitch_input((float)result.pitch);
			handler->add_yaw_input((float)result.yaw);
		}
	}
}