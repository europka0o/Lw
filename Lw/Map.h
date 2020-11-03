#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

//#define rend_on_view_Y(j, width_img, screen_width) (j * width_img) > screen_width ? true : false
//#define rend_on_view_X(i, height_img, screen_height) (i * height_img) > screen_height ? true : false

class World {
	private:
		int size_x, size_y;
		sf::String pathFromFile;
		Image *worldimg;
		Texture *worldTexture;
		Sprite *worldSpr;
		sf::String *mass_sp;
	public:
		World(String Path, int X_SIZE, int Y_SIZE);
		~World() noexcept;
		void render(RenderWindow &wd) noexcept;
		void render(RenderWindow *wd) noexcept;
};