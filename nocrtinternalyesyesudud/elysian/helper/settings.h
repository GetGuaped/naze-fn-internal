#pragma once

namespace elysian
{
	namespace settings
	{
		namespace cheat_vars
		{
			ulocalplayer* old_player;
			aplayercontroller* old_controller;

			fvector target_location = fvector();

			fvector original_location = fvector();
			frotator original_rotation = frotator();

			bool fly = false;
		}

		namespace aimbot
		{
			bool enable = true;

			//tested around with these
			bool interpolate = false;
			bool constant = false;
			bool only_visible = false;
			bool full_deg_360 = true;
			double speed = 0;
			double fov = 250;
		}

		namespace player
		{
			bool enable = true;
			bool local = false;
			bool wounded = true;
			bool teammate = true;
			int box_mode = 1;
			bool skeleton = true;
			int line_mode = 0;
			bool username = true;
			bool distance = true;
			bool weapon = true;
			double distance_limit = 300;
		}

		namespace environment
		{
			bool enable = true;
			bool unknown_items = false;
			double distance_limit = 200;

			bool pickups = true;
			int pickup_min_tier_mode = 3;

			bool containers = true;

			bool vehicle = true;
		}

		namespace exploits
		{
			//vehicle shit
			bool vehicle_teleport = true;
			bool vehicle_flight = false;
			bool boat_speedhack = true;
			double boat_speed = 10;
			double flight_speed = 10;

			//todo - i made them using .text patches, currently looking for another thing
			bool bullet_teleport = true;
			bool no_spread = false;
			bool no_bloom = false;
			bool instant_reload = false;
			bool bullet_no_clip = false; //removed for now - it worked with minhook
			bool rapid_fire = false; //maybe use from current weapon - but ill find something better for this

			bool camera_fov_changer = true; //either calling controller->fov or changing it in getviewpoint
			double camera_fov = 110;
		}
	}
}