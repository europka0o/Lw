#pragma once
#include <SFML/Graphics.hpp>

//using namespace sf;

//#define rend_on_view_Y(j, width_img, screen_width) (j * width_img) > screen_width ? true : false
//#define rend_on_view_X(i, height_img, screen_height) (i * height_img) > screen_height ? true : false

using sf::Texture;
using sf::Sprite;
using sf::Image;
using sf::RenderWindow;

class World {
	private:
		int size_x, size_y;
		Texture *worldTexture;
		Sprite *worldSpr;
		sf::String *mass_sp;
	public:
		World(Image *ptr_on_img, int X_SIZE, int Y_SIZE);
		~World() noexcept;
		void render(RenderWindow &wd) noexcept;
};