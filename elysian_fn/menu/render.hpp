#pragma once

namespace elysian_fn {
	namespace render {
		double angle = 0;
		double orounding = 1, orounding_step = 1;

		void text(fstring text, fvector2d position, flinearcolor color, bool center_x, bool center_y, bool outline = true) {
			variables::canvas->k2_drawtext(classes::defines::font, text, position, fvector2d(0.8f, 0.8f), color, 0.8f, flinearcolor(), fvector2d(), center_x, center_y, true, flinearcolor(0.f, 0.f, 0.f, 1.f));
		}

		void line(fvector2d from, fvector2d to, flinearcolor color, float thickness) {
			variables::canvas->k2_drawline(from, to, thickness, color);
		}

		void polygon(fvector2d points[], flinearcolor color, float thickness) {
			int previous = 0;
			
			for (int i = 1; i < sizeof(points); i++) {
				line(points[previous], points[i], color, thickness);
				previous = i;
			}
		}

		double deg2rad(double deg) {
			return deg * 3.1415927 / 180.0;
		}

		void box_filled_rounded(fvector2d position, fvector2d size, flinearcolor render_color, double rounding, double rounding_step) {
			//top
			fvector2d rotate_point = fvector2d(position.x + rounding, position.y + rounding);
			fvector2d previous = fvector2d();
			for (int i = 180; i < 270; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				double a = result.x - position.x;
				if (previous)
					line(result, fvector2d(position.x + size.x - a, result.y), render_color, 1.f);
				previous = result;
			}

			//middle
			for (int i = rounding; i < size.y - rounding + 1; i++) {
				line(fvector2d(position.x, position.y + i), fvector2d(position.x + size.x, position.y + i), render_color, 1.f);
			}

			//bottom
			rotate_point = fvector2d(position.x + rounding, position.y + size.y - rounding);
			previous = fvector2d();
			for (int i = 90; i < 180; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				double a = result.x - position.x;
				if (previous)
					line(result, fvector2d(position.x + size.x - a, result.y), render_color, 1.f);
				previous = result;
			}
		}

		void box_rounded(fvector2d position, fvector2d size, flinearcolor render_color, float thickness, double rounding, double rounding_step) {
			
			//top left corner
			fvector2d rotate_point = fvector2d(position.x + rounding, position.y + rounding);
			fvector2d previous = fvector2d();
			for (int i = 180; i < 270; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				if (previous)
					line(previous, result, render_color, thickness);
				previous = result;
			}

			//bottom left corner
			rotate_point = fvector2d(position.x + rounding, position.y + size.y - rounding);
			previous = fvector2d();
			for (int i = 90; i < 180; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				if (previous)
					line(previous, result, render_color, thickness);
				previous = result;
			}

			//bottom right corner
			rotate_point = fvector2d(position.x + size.x - rounding, position.y + size.y - rounding);
			previous = fvector2d();
			for (int i = 0; i < 90; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				if (previous)
					line(previous, result, render_color, thickness);
				previous = result;
			}

			//top right corner
			rotate_point = fvector2d(position.x + size.x - rounding, position.y + rounding);
			previous = fvector2d();
			for (int i = 270; i < 360; i += rounding_step) {
				double radians = deg2rad(i);
				fvector2d result = fvector2d(rounding * math::cos(radians) + rotate_point.x, rounding * math::sin(radians) + rotate_point.y);
				if (previous)
					line(previous, result, render_color, thickness);
				previous = result;
			}

			//left line
			line(fvector2d(position.x, position.y + rounding), fvector2d(position.x, position.y + size.y - rounding), render_color, thickness);

			//right line
			line(fvector2d(position.x + size.x, position.y + rounding), fvector2d(position.x + size.x, position.y + size.y - rounding), render_color, thickness);

			//bottom line
			line(fvector2d(position.x + rounding, position.y + size.y), fvector2d(position.x + size.x - rounding, position.y + size.y), render_color, thickness);

			//top line
			line(fvector2d(position.x + rounding, position.y), fvector2d(position.x + size.x - rounding, position.y), render_color, thickness);
		}

		void box(fvector2d position, fvector2d size, flinearcolor render_color, float thickness) {
			line(fvector2d(position.x, position.y), fvector2d(position.x + size.x, position.y), render_color, thickness);
			line(fvector2d(position.x + size.x, position.y), fvector2d(position.x + size.x, position.y + size.y), render_color, thickness);
			line(fvector2d(position.x + size.x, position.y + size.y), fvector2d(position.x, position.y + size.y), render_color, thickness);
			line(fvector2d(position.x, position.y + size.y), fvector2d(position.x, position.y), render_color, thickness);
		}

		void filled_box_round(fvector2d position, fvector2d size, flinearcolor render_color) {
			int round_size = 5;
			
			for (int i = 0; i < size.y; i++) {

				if (i < round_size) { //top rounding
					line(fvector2d(position.x - i, position.y + i), fvector2d(position.x + size.x + i, position.y + i), render_color, 1.f);
				}
				else {
					line(fvector2d(position.x, position.y + i), fvector2d(position.x + size.x, position.y + i), render_color, 1.f);
				}
			}
		}

		void filled_box(fvector2d position, fvector2d size, flinearcolor render_color) {
			for (int i = 0; i < size.y; i++) {
				line(fvector2d(position.x, position.y + i), fvector2d(position.x + size.x, position.y + i), render_color, 1.f);
			}
		}

		void circle(fvector2d position, flinearcolor color, int radius, int segments) {
			double m_pi = 3.1415927;

			double step = m_pi * 2.0 / segments;
			int count = 0;
			fvector2d vectors[128];
			for (double deg = 0; deg < m_pi * 2; deg += step) {
				double x_1 = radius * math::cos(deg) + position.x;
				double y_1 = radius * math::sin(deg) + position.y;
				double x_2 = radius * math::cos(deg + step) + position.x;
				double y_2 = radius * math::sin(deg + step) + position.y;

				vectors[count].x = x_1;
				vectors[count].y = y_1;
				vectors[count + 1].x = x_2;
				vectors[count + 1].y = y_2;

				line(fvector2d(vectors[count].x, vectors[count].y), fvector2d(x_2, y_2), color, 1.f);
			}
		}
	}
}