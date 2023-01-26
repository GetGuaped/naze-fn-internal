#pragma once

namespace elysian_fn {
	namespace menu {
		fvector2d pos = { 25, 25 };

		static void run() {
			if ( ZeroGUI::Window( "Elysian Internals", &pos, fvector2d { 500.0f, 350.0f }, menu_open ) )
			{
				//Simple Tabs
				static int tab = 0;
				if ( ZeroGUI::ButtonTab( "Aimbot", fvector2d { 110, 25 }, tab == 0 ) ) tab = 0;
				if ( ZeroGUI::ButtonTab( "Player", fvector2d { 110, 25 }, tab == 1 ) ) tab = 1;
				if ( ZeroGUI::ButtonTab( "Items", fvector2d { 110, 25 }, tab == 2 ) ) tab = 2;
				if ( ZeroGUI::ButtonTab( "Memes", fvector2d { 110, 25 }, tab == 3 ) ) tab = 3;


				if ( tab == 0 )
				{
					ZeroGUI::NextColumn( 130.0f );
					ZeroGUI::Checkbox( "Enable", &options::aimbot::enable);

					if ( options::player::enable )
					{
						ZeroGUI::Checkbox( "Interoplate", &options::aimbot::interpolate );
						ZeroGUI::Checkbox( "Constant", &options::aimbot::constant );
						ZeroGUI::Checkbox( "Show FOV", &options::aimbot::show_fov );
						ZeroGUI::SliderFloat( "Speed", &options::aimbot::speed, 1, 20 );
						ZeroGUI::SliderFloat( "FOV", &options::aimbot::fov, 30, 950 );
					}
				}

				if ( tab == 1 )
				{
					ZeroGUI::NextColumn( 130.0f );
					ZeroGUI::Checkbox( "Enable", &options::player::enable);

					if ( options::player::enable )
					{
						ZeroGUI::Checkbox( "Skeleton", &options::player::skeleton );
						ZeroGUI::Checkbox( "Box", &options::player::box );
						ZeroGUI::Checkbox( "Snapline", &options::player::snapline );
						ZeroGUI::Checkbox( "Viewangle", &options::player::view_angle );
						ZeroGUI::Checkbox( "Distance", &options::player::distance );
						ZeroGUI::Checkbox( "Active Item", &options::player::active_item );
						ZeroGUI::Checkbox( "Username", &options::player::username );
						ZeroGUI::SliderFloat( "Render Distance", &options::player::max_distance, 5, 900 );
					}
				}

				if ( tab == 2 )
				{
					ZeroGUI::NextColumn( 130.0f );
					ZeroGUI::Checkbox( "Enable", &options::pickup::enable );

					if ( options::pickup::enable )
					{
						ZeroGUI::Checkbox( "Distance", &options::environment::show_distance );
						ZeroGUI::SliderFloat( "Render Distance", &options::environment::max_distance, 5, 900 );
						ZeroGUI::Checkbox( "Common", &options::pickup::common );
						ZeroGUI::Checkbox( "Uncommon", &options::pickup::uncommon );
						ZeroGUI::Checkbox( "Rare", &options::pickup::rare );
						ZeroGUI::Checkbox( "Epic", &options::pickup::epic );
						ZeroGUI::Checkbox( "Legendary", &options::pickup::legendary );
						ZeroGUI::Checkbox( "Mythic", &options::pickup::mythic );


					}
				}
				
				if ( tab == 3 )
				{
					ZeroGUI::NextColumn( 130.0f );
					ZeroGUI::Checkbox( "Disable UserAccountControl Flag (UAC)", &options::disableUAC );
				}


			}

		}
	}
}