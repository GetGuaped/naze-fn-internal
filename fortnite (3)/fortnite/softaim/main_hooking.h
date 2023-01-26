#pragma once

namespace main_hooking {
	using draw_transition_sim = void(*)(ugameviewportclient* viewport, ucanvas* canvas);
	draw_transition_sim draw_transition_o = 0;

	void draw_transition(ugameviewportclient* viewport, ucanvas* canvas) {
		elysian_main(viewport, canvas);

		return draw_transition_o(viewport, canvas);
	}

	void initialize() {

		game = elysian::get_module(L"FortniteClient-Win64-Shipping.exe");
		//game = *(std::uintptr_t*)__readgsqword(0x60);

		if ( !game )
			return;

		defines::init();

		auto world = defines::get_world();
		
		auto game_instance = defines::game_statics->get_game_instance(world);
		auto local_player = game_instance->get_local_players()[0];
		
		auto controller = defines::game_statics->get_player_controller(world, std::int32_t(0));
		auto viewport = local_player->get_viewport();

		auto engine = (uengine*)defines::system->get_outer_object(game_instance);
		render::font = engine->get_font();

		draw_transition_o = vmt<decltype(draw_transition_o)>(std::uintptr_t(viewport), std::uintptr_t(draw_transition), 0x6E);
	}
}