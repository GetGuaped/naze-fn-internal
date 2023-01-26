#pragma once

namespace elysian
{
	namespace render
	{
		ucanvas* render_canvas = nullptr;

		void line(fvector2d a, fvector2d b, flinearcolor color, float thickness)
		{
			render_canvas->k2_drawline(a, b, thickness, color);
		}

		void text(fstring content, fvector2d screen, flinearcolor color, bool center_x, bool center_y, bool outlined, double scale = 1)
		{
			render_canvas->k2_drawtext(content, screen, color, scale, center_x, center_y, outlined);
		}

		void filled_box(fvector2d position, fvector2d size, flinearcolor render_color)
		{
			for (int i = 0; i < size.y; i++)
			{
				line(fvector2d(position.x, position.y + i), fvector2d(position.x + size.x, position.y + i), render_color, 1.f);
			}
		}

		void box(fvector2d position, fvector2d size, flinearcolor render_color, float thickness)
		{
			line(fvector2d(position.x, position.y), fvector2d(position.x + size.x, position.y), render_color, thickness);
			line(fvector2d(position.x + size.x, position.y), fvector2d(position.x + size.x, position.y + size.y), render_color, thickness);
			line(fvector2d(position.x + size.x, position.y + size.y), fvector2d(position.x, position.y + size.y), render_color, thickness);
			line(fvector2d(position.x, position.y + size.y), fvector2d(position.x, position.y), render_color, thickness);
		}

		void circle(fvector2d position, flinearcolor color, double radius, double segments)
		{
			double m_pi = 3.1415927;

			double step = m_pi * 2.0 / segments;
			int count = 0;
			fvector2d vectors[128];
			for (double deg = 0; deg < m_pi * 2; deg += step)
			{
				double x_1 = radius * libs::math->cos(deg) + position.x;
				double y_1 = radius * libs::math->sin(deg) + position.y;
				double x_2 = radius * libs::math->cos(deg + step) + position.x;
				double y_2 = radius * libs::math->sin(deg + step) + position.y;

				vectors[count].x = x_1;
				vectors[count].y = y_1;
				vectors[count + 1].x = x_2;
				vectors[count + 1].y = y_2;

				line(fvector2d(vectors[count].x, vectors[count].y), fvector2d(x_2, y_2), color, 1.f);
			}
		}

		void filled_circle(fvector2d position, flinearcolor color, double radius)
		{
			double smooth = 0.07;

			int size = 2.0 * 3.14159265359 / smooth + 1;

			for (double angle = 0; angle < 2 * 3.14159265359; angle += smooth)
			{
				render::line(position, fvector2d(position.x + cos(angle) * radius, position.y + sin(angle) * radius), color, 1.0f);
			}
		}
	}
}