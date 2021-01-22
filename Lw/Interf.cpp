﻿#include "Interf.h"

//----------------------------------Настройки-Settings-Начало------------------------------
int settings::createSettings(int ScreenW, int ScreenH, bool VertS, int TxtS, bool FullS, bool Sound, int SoundV) {
	fullScreen = FullS;
	verticalSync = VertS;
	screenHeight = ScreenH;
	screenWidth = ScreenW;
	textSize = TxtS;
	sound = Sound;
	soundVolume = SoundV;
	return saveSettings();
}

int settings::saveSettings() {
	std::ofstream zap;
	zap.open("settings.cfg");
	zap << "fullScreen = " << fullScreen << ";\n";
	zap << "verticalSync = " << verticalSync << ";\n";
	zap << "screenHeight = " << screenHeight << ";\n";
	zap << "screenWidth = " << screenWidth << ";\n";
	zap << "textSize = " << textSize << ";\n";
	zap << "sound = " << sound << ";\n";
	zap << "soundVolume = " << soundVolume << ";\n";
	zap.close();
	return 0;
}

int settings::loadSettings() {
	std::ifstream read;
	read.open("settings.cfg");
	if (read.is_open()) {
		std::string *st = new std::string;
		std::string *line = new std::string;
		std::string *command = new std::string;

		while (! read.eof()) {
			getline(read, *line);
			*st += *line;
		}

		delete line;

		read.close();
		int line_num = 0;
		for (std::string::iterator it = st->begin(); it != st->end(); it++) {
			if (line_num == 0) {
				if (*it == '=') {
					it++;
					while (*it != ';') { //Получаем строку с информацией о полноэкранном режиме и записываем её в переменную fullScreen
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> fullScreen;
					*command = "";
					line_num++;
				}
			} else if (line_num == 1) { //Получаем строку с информацией о вертикальной синхронизации и записываем её в переменную verticalSync
				if (*it == '=') {
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> verticalSync;
					*command = "";
					line_num++;
				}
			} else if (line_num == 2) { //Получаем строку с информацией о высоте разрешения экрана и записываем её в переменную screenHeight
				if (*it == '=') {
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> screenHeight;
					*command = "";
					line_num++;
				}
			} else if (line_num == 3) { //Получаем строку с информацией о ширине разрешения экрана и записываем её в переменную screenWidth
				if (*it == '=') {
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> screenWidth;
					*command = "";
					line_num++;
				}
			}  else if (line_num == 4) { //Получаем строку с информацией о размере текста и записываем её в переменную textSize
				if (*it == '=') {
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> textSize;
					*command = "";
					line_num++;
				}
			} else if (line_num == 5) { //Получаем строку с информацией о проигрывании звуков и записываем её в переменную sound
				if (*it == '=') { 
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> sound;
					*command = "";
					line_num++;
				}
			} else if (line_num == 6) { //Получаем строку с информацией о громкости звуков и записываем её в переменную soundVolume
				if (*it == '=') {
					it++;
					while (*it != ';') {
						*command += *it;
						it++;
					}
					std::istringstream iss(*command);
					iss >> soundVolume;
				}
			}
		}

		delete st, command;
		return 1; //Возвращаем 1, если все переменные из конфига были записаны успешно
	} else {
		return 0; //Возвращаем 0, если что-то пошло не так
	}
}
//----------------------------------Настройки-Settings-Конец-------------------------------

//----------------------------------Структура-оси-axes-Начало-------------------------------
template<typename type>
axes<type>::axes() {
	x = 0;
	y = 0;
}

template<typename type>
axes<type>::axes(type _x, type _y) : x(_x), y(_y) {}
//----------------------------------Структура-оси-axes-Конец-------------------------------

// ----------------------------------Персанаж-Character-Начало------------------------------
Character::Character(Texture* ptr_texture, float X_POS, float Y_POS, int hp) :
	BaseCharacter(ptr_texture, X_POS, Y_POS, hp)
	{
	sprt->setTextureRect(IntRect(0, 250, 300, 250));
	sprt->setPosition(pos.x, pos.y);
	direction = 0;
	last_direction = direction;
		
	rect_collis = new Collision(IntRect(sprt->getGlobalBounds().left + 140, sprt->getGlobalBounds().top, 165, sprt->getGlobalBounds().height));

	HP = new _interface::min_bar(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height, health, 0, Color::Black, Color::Red);
}

Character::~Character() {
	delete HP;
}

void __fastcall Character::setPosition(float x, float y) noexcept {
	pos.x = x;
	pos.y = y;
	sprt->setPosition(x, y);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x + 140, pos.y);
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
}

void Character::setPosition(const axes_f &xy) noexcept {
	pos = xy;

	sprt->setPosition(pos.x, pos.y);
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x + 140, pos.y);
}

float Character::getPositionX_forCamer() noexcept {
	return sprt->getGlobalBounds().left + (sprt->getGlobalBounds().height / 2);
}

float Character::getPositionY_forCamer() noexcept {
	return sprt->getGlobalBounds().top + (sprt->getGlobalBounds().width / 2);
}

void __fastcall Character::move(float time) noexcept {
	if (!((Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Left)) ||
			(Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Left)))) {
		if (Keyboard::isKeyPressed(Keyboard::Right)) { //-------------Вправо
				direction = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) { //-------------Влево
				direction = 2;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) { //-------------Вверх 
				direction = 3;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) { //-------------Вниз 
				direction = 4;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Right)) { //-------------Вверх и вправо
			direction = 5;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Left)) { //-------------Вверх и влево 
			direction = 6;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Right)) { //-------------Вниз и вправо
			direction = 7;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Left)) { //-------------Вниз и влево
			direction = 8;
	}

	switch (direction) { //Направление движения персонажа
	case 1:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250)); 
		sprt->move(0.5 * time, 0); break;
	case 2:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250)); 
		sprt->move(-0.5 * time, 0); break;
	case 3:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250)); 
		sprt->move(0, -0.5 * time); break;
	case 4:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
		sprt->move(0, 0.5 * time); break;
	case 5:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
		sprt->move(0.5 * time, -0.5 * time); 
		last_direction = 1; break;
	case 6:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250));
		sprt->move(-0.5 * time, -0.5 * time); 
		last_direction = 2; break;
	case 7:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
		sprt->move(0.5 * time, 0.5 * time); 
		last_direction = 1; break;
	case 8:
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250));
		sprt->move(-0.5 * time, 0.5 * time); 
		last_direction = 2; break;
	default: 
		switch (last_direction) {
		case 1:
			//sprt->setTextureRect(IntRect(0, 250, 300, 250)); break;
		case 2:
			//sprt->setTextureRect(IntRect(300, 250, -300, 250)); break;
		default:
			break;
		}
		break;
	}
	last_direction = direction;
	direction = 0;
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2) + 40, sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setBounds(IntRect(sprt->getGlobalBounds().left + 140, sprt->getGlobalBounds().top, 165, sprt->getGlobalBounds().height));
	pos.x = sprt->getGlobalBounds().left;
	pos.y = sprt->getGlobalBounds().top;
}

void __fastcall Character::move(float time, int direct) noexcept {
	direction = direct;
	
	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {		
		frame += 0.04 * time;
		if (frame > 7) {
			frame = 6;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 0, -400, 250));
	} else {
		switch (direction) { //Направление движения персонажа
		case 1:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250)); 
			sprt->move(0.5 * time, 0); break;
		case 2:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250)); 
			sprt->move(-0.5 * time, 0); break;
		case 3:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
			sprt->move(0, -0.5 * time); break;
		case 4:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
			sprt->move(0, 0.5 * time); break;
		case 5:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
			sprt->move(0.5 * time, -0.5 * time);
			last_direction = 1; break;
		case 6:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250));
			sprt->move(-0.5 * time, -0.5 * time);
			last_direction = 2; break;
		case 7:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame), 250, 400, 250));
			sprt->move(0.5 * time, 0.5 * time);
			last_direction = 1; break;
		case 8:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(400 * int(frame) + 400, 250, -400, 250));
			sprt->move(-0.5 * time, 0.5 * time);
			last_direction = 2; break;
		default:
			switch (last_direction) {
			case 1:
				//sprt->setTextureRect(IntRect(0, 250, 300, 250)); break;
			case 2:
				//sprt->setTextureRect(IntRect(300, 250, -300, 250)); break;
			default:
				break;
			}
			break;
		}
	}
	last_direction = direction;
	direction = 0;
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2) + 40, sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setBounds(IntRect(sprt->getGlobalBounds().left + 140, sprt->getGlobalBounds().top, 165, sprt->getGlobalBounds().height));
	pos.x = sprt->getGlobalBounds().left;
	pos.y = sprt->getGlobalBounds().top;
}

void __fastcall Character::attack(float time) {
	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 6;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 0, -400, 250));
	} else {
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame) + 400, 500, -400, 250));
	}
}

bool Character::isCooldown(float time) {
	if (!cooldown) {
		timer_cooldown = 0;
		return false;
	} else {
		timer_cooldown += time;
		if (timer_cooldown >= 3) {
			cooldown = false;
			timer_cooldown = 0;
			return false;
		}
		cooldown = true;
		return true;
	}
}

void Character::render(RenderWindow &wd) noexcept {
	rect_collis->render(wd);
	wd.draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}

void Character::render(RenderWindow *wd) noexcept {
	rect_collis->render(wd);
	wd->draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}
//----------------------------------Персанаж-Character-Конец------------------------------

//---------------------------Статический-объект-ObjectStatic-Начало------------------------------
ObjectStatic::ObjectStatic(Texture* ptr_texture, float X, float Y) {
	pos.x = X;
	pos.y = Y;

	sprt = new Sprite;
	sprt->setTexture(*ptr_texture);
	sprt->setTextureRect(IntRect(0, 0, ptr_texture->getSize().x, ptr_texture->getSize().y));
	sprt->setPosition(pos.x, pos.y);

	rect_collis = new Collision(IntRect(0, 0, 0, 0));
}

ObjectStatic::~ObjectStatic() {
	delete sprt;
	delete rect_collis;
}

axes_i ObjectStatic::getPosition() {
	return pos;
}

FloatRect ObjectStatic::getSize() {
	return sprt->getGlobalBounds();
}

void ObjectStatic::setRect(const IntRect &bound) {
	sprt->setTextureRect(IntRect(bound.left, bound.top, bound.width, bound.height));
}

void __fastcall ObjectStatic::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;

	sprt->setPosition(pos.x, pos.y);
	rect_collis->setPosition(pos.x, pos.y);
}

void ObjectStatic::setPosition(const axes_i &xy) {
	pos = xy;

	sprt->setPosition(pos.x, pos.y);
	rect_collis->setPosition(xy);
}

void ObjectStatic::render(RenderWindow &wd) {
	wd.draw(*sprt);
}

void ObjectStatic::render(RenderWindow *wd) {
	wd->draw(*sprt);
}
//----------------------------------Статический-объект-ObjectStatic-Конец-------------------------------

//-------------------------------Анимированный-объект-ObjectAnimated-Начало------------------------------
ObjectAnimated::ObjectAnimated(Texture* ptr_texture, float X, float Y) :
	frame(0),
	end(false),
	ObjectStatic(ptr_texture, X, Y)
	{	
}

ObjectAnimated::~ObjectAnimated() {}

void __fastcall ObjectAnimated::update(float time) {}
//-------------------------------Анимированный-объект-ObjectAnimated-Конец-------------------------------

//----------------------------------Камера-Camer-Начало------------------------------
Camer::Camer(int x, int y, int screen_width, int screen_height) :
	screen_H(screen_height),
	screen_W(screen_width)
	{
	pos.x = x;
	pos.y = y;
	Vid = new View;
	Vid->reset(FloatRect(0, 0, screen_W, screen_H));
}

Camer::Camer() :
	screen_H(600),
	screen_W(800)
	{
	pos.x = 0;
	pos.y = 0;
	Vid = new View;
	Vid->reset(FloatRect(0, 0, screen_W, screen_H));
}

Camer::~Camer() {
	Vid->~View();
	delete Vid;
}

void __fastcall Camer::move(int x, int y) {
	pos.x = x;
	pos.y = y;
	Vid->setCenter(pos.x, pos.y);
}

void Camer::setZoom(float zoom) {
	Vid->zoom(zoom);
}

axes_i Camer::getPosition() noexcept {
	return pos;
}

int Camer::getScreenWidth() noexcept {
	return screen_W;
}
int Camer::getScreenHeight() noexcept {
	return screen_H;
}

FloatRect Camer::getBounds() {
	return Vid->getViewport();
}

void Camer::setView(RenderWindow &wd) {
	wd.setView(*Vid);
}

void Camer::setView(RenderWindow *wd) {
	wd->setView(*Vid);
}

//----------------------------------Камера-Camer-Конец-------------------------------

//-----------------------------------Мир-World-Начало-------------------------------------
World::World(Image *ptr_on_img, int X_SIZE, int Y_SIZE) :
	size_x(X_SIZE),
	size_y(Y_SIZE)
	{
	worldTexture = new Texture;
	worldTexture->loadFromImage(*ptr_on_img);
	worldSpr = new Sprite;
	worldSpr->setTexture(*worldTexture);
	mass_sp = new sf::String[size_y]; 
	char *mass_ch = new char[size_x];

	for (int i = 0; i < size_x; i++) {
		mass_ch[i] = '0';
	}

	for (int i = 0; i < size_y; i++) {
		mass_sp[i] = String(mass_ch);
	}
	delete[] mass_ch;
}

World::~World() noexcept {
	delete[] mass_sp;
	delete worldTexture, worldSpr;
}

void World::render(RenderWindow& wd) noexcept {
	for (int i = 0; i < size_y; i++) { //Отрисовка мира
		for (int j = 0; j < size_x; j++) {
			if (mass_sp[i][j] == '0') {
				worldSpr->setTextureRect(IntRect(0, 0, worldTexture->getSize().x, worldTexture->getSize().y)); //0, 0, 128, 128
			}
			worldSpr->setPosition(j * worldTexture->getSize().x, i * worldTexture->getSize().y);
			wd.draw(*worldSpr);
		}
	}
}

void World::render(RenderWindow *wd) noexcept {
	for (int i = 0; i < size_y; i++) { //Отрисовка мира
		for (int j = 0; j < size_x; j++) {
			if (mass_sp[i][j] == '0') {
				worldSpr->setTextureRect(IntRect(0, 0, worldTexture->getSize().x, worldTexture->getSize().y)); //0, 0, 128, 128
			}
			worldSpr->setPosition(j * worldTexture->getSize().x, i * worldTexture->getSize().y);
			wd->draw(*worldSpr);
		}
	}
}
//-----------------------------------Мир-World-Конец-------------------------------------

//-----------------------------------Полоса-bar-Начало-------------------------------------
_interface::bar::bar(int x, int y, int br_ma, int br_mi, const std::wstring &name, Color mcol, Color bcol, Color tcol) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	max_bar(br_ma),
	min_bar(br_mi),
	curr_bar(br_ma)
	{
	font_main = new Font;
	font_main->loadFromFile("Img/18094.ttf");
	label = new Text;
	label->setFont(*font_main);
	label->setString(name);
	label->setCharacterSize(small);
	label->setFillColor(tcol);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(float, pos.x, label->getGlobalBounds().left, 5), repoz_y(float, pos.y, label->getGlobalBounds().top, 5));

	main = new RectangleShape;
	main->setSize(Vector2f(label->getGlobalBounds().width + 420, label->getGlobalBounds().height + 10));
	main->setFillColor(mcol);
	main->setPosition(pos.x, pos.y);

	bevel = new RectangleShape;
	bevel->setSize(Vector2f(400, label->getGlobalBounds().height));
	bevel->setFillColor(bcol);
	bevel->setPosition(label->getGlobalBounds().width + 10 + 5, pos.y + 5);

	fl_rect = main->getGlobalBounds();
}

_interface::bar::bar() :
	BaseInerface(0, 0, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	max_bar(100),
	min_bar(0),
	curr_bar(100)
	{
	font_main = new Font;
	label = new Text;
	main = new RectangleShape;
	bevel = new RectangleShape;
}

_interface::bar::~bar() {
	delete font_main, label, main, bevel;
}

void __fastcall _interface::bar::changeBar(int arg) noexcept {
	if (curr_bar != arg) {
		if (arg < 0) {
			arg = 0;
		}
	bevel->setSize(Vector2f((400 * arg) / max_bar, label->getGlobalBounds().height));
	curr_bar = arg;
	}
}

void _interface::bar::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*main);
		wd.draw(*bevel);
		wd.draw(*label);
	}
}

void _interface::bar::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*main);
		wd->draw(*bevel);
		wd->draw(*label);
	}
}

void __fastcall _interface::bar::setPosition(int x, int y) noexcept {
	pos.x = x;
	pos.y = y;
	main->setPosition(pos.x, pos.y);
	bevel->setPosition(pos.x + label->getGlobalBounds().width + 10 + 5, pos.y + 5); 
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(float, pos.x, label->getGlobalBounds().left, 5), repoz_y(float, pos.y, label->getGlobalBounds().top, 5));

	fl_rect = main->getGlobalBounds();
}

void _interface::bar::setPosition(const axes_i &xy) noexcept {
	pos = xy;
	main->setPosition(pos.x, pos.y);
	bevel->setPosition(pos.x + label->getGlobalBounds().width + 10 + 5, pos.y + 5);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(float, pos.x, label->getGlobalBounds().left, 5), repoz_y(float, pos.y, label->getGlobalBounds().top, 5));

	fl_rect = main->getGlobalBounds();
}

void _interface::bar::freeze(Camer *camera, const axes_i &xy) {
	this->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2) + xy.x, camera->getPosition().y - (camera->getScreenHeight() / 2) + xy.y);
}

void _interface::bar::freeze(Camer* camera, int x, int y) {
	this->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2) + x, camera->getPosition().y - (camera->getScreenHeight() / 2) + y);
}

void _interface::bar::resize(int size) noexcept {
	label->setCharacterSize(size);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(float, pos.x, label->getGlobalBounds().left, 5), repoz_y(float, pos.y, label->getGlobalBounds().top, 5));
	main->setSize(Vector2f(label->getGlobalBounds().width + 420, label->getGlobalBounds().height + 10));
	bevel->setSize(Vector2f((400 * curr_bar) / max_bar, label->getGlobalBounds().height));
	bevel->setPosition(label->getGlobalBounds().width + 10 + 5, pos.y + 5);

	fl_rect = main->getGlobalBounds();
}
//-----------------------------------Полоса-bar-Конец--------------------------------------

//-----------------------------------Текст-text-Начало-------------------------------------
_interface::text::text(int x, int y, const std::wstring& txt, Color lbcol, Color bvcol) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	visible_bevel(true)
	{
	font_main = new Font;
	font_main->loadFromFile("Img/18094.ttf");
	label = new Text;
	label->setFont(*font_main);
	label->setString(txt);
	label->setCharacterSize(small);

	label_cl = new Color;  
	*label_cl = lbcol;
	label->setFillColor(*label_cl);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(int, pos.x, label->getGlobalBounds().left, 5), repoz_y(int, pos.y, label->getGlobalBounds().top, 5));

	bevel = new RectangleShape;
	bevel->setSize(Vector2f(label->getGlobalBounds().width + 10, label->getGlobalBounds().height + 10));
	bevel_cl = new Color;
	*bevel_cl = bvcol;
	bevel->setFillColor(*bevel_cl);
	bevel->setPosition(pos.x ,pos.y);

	fl_rect = bevel->getGlobalBounds();
}

_interface::text::~text() {
	delete font_main, label, label_cl, bevel, bevel_cl;
}

void _interface::text::setString(const std::wstring& txt) noexcept {
	label->setString(txt);
	_interface::text::resize(label->getCharacterSize());
}

void _interface::text::setFont(const String& txt) noexcept {
	font_main->loadFromFile(txt);
}

void _interface::text::setPosition(const axes_i &xy) noexcept {
	pos = xy;
	bevel->setPosition(pos.x, pos.y);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(int, pos.x, label->getGlobalBounds().left, 5), repoz_y(int, pos.y, label->getGlobalBounds().top, 5));

	fl_rect = bevel->getGlobalBounds();
}

void __fastcall _interface::text::setPosition(int x, int y) noexcept {
	pos.x = x;
	pos.y = y;
	bevel->setPosition(pos.x, pos.y);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(int, pos.x, label->getGlobalBounds().left, 5), repoz_y(int, pos.y, label->getGlobalBounds().top, 5));

	fl_rect = bevel->getGlobalBounds();
}

void _interface::text::resize(int size) noexcept {
	label->setCharacterSize(size);
	label->setPosition(pos.x, pos.y);
	label->setPosition(repoz_x(int, pos.x, label->getGlobalBounds().left, 5), repoz_y(int, pos.y, label->getGlobalBounds().top, 5));
	bevel->setSize(Vector2f(label->getGlobalBounds().width + 10, label->getGlobalBounds().height + 10));

	fl_rect = bevel->getGlobalBounds();
}

void _interface::text::freeze(Camer* camera, const axes_i &xy) {
	this->setPosition(camera->getPosition().x - (camera->getScreenHeight() / 2) + xy.x, camera->getPosition().y - (camera->getScreenWidth() / 2) + xy.y);
}

void _interface::text::freeze(Camer* camera, int x, int y) {
	this->setPosition(camera->getPosition().x - (camera->getScreenHeight() / 2) + x, camera->getPosition().y - (camera->getScreenWidth() / 2) + y);
}

void _interface::text::render(RenderWindow &wd) noexcept {
	if (visible) {
		if (visible_bevel) {
			wd.draw(*bevel);
		}
		wd.draw(*label);
	}
}

void _interface::text::render(RenderWindow *wd) noexcept {
	if (visible) {
		if (visible_bevel) {
			wd->draw(*bevel);
		}
		wd->draw(*label);
	}
}
//-----------------------------------Текст-text-Конец-------------------------------------

//-----------------------------------Многострочный-текст-multiline_text-Конец-------------------------------------
_interface::multiline_text::multiline_text(float x, float y, Color lbcol, Color bvcol) :
	visible(true),
	visible_bevel(true)
	{
	pos.x = x;
	pos.y = y;
	font_main = new Font;
	font_main->loadFromFile("Img/18094.ttf");
	SIZE_MSTX = 0;
	max_length = 0;

	bevel_cl = new Color; 
	*bevel_cl = bvcol;
	label_cl = new Color; 
	*label_cl = lbcol;

	bevel = new RectangleShape;
}

_interface::multiline_text::~multiline_text() noexcept {
	if (SIZE_MSTX > 0) {
		for (dyn_vector_text::iterator it_ms = mass_string.begin(); it_ms != mass_string.end();) {
			delete* it_ms;
			it_ms = mass_string.erase(it_ms);
		}
		mass_string.shrink_to_fit();
	}
	delete font_main, bevel, bevel_cl, label_cl;
}

void _interface::multiline_text::add(const std::wstring& txt) noexcept {
	float rep_y = 0;
	int height = 0;
	++SIZE_MSTX;
	#undef small
	mass_string.push_back(new Text());
	mass_string.back()->setFont(*font_main);
	mass_string.back()->setString(txt);
	mass_string.back()->setCharacterSize(text_size::small);
	mass_string.back()->setFillColor(*label_cl);

	for (int i = 0; i < SIZE_MSTX; i++) {
		if (i == 0) {
			mass_string[i]->setPosition(pos.x, pos.y);
			max_length = mass_string[i]->getGlobalBounds().width;
			mass_string[i]->setPosition(repoz_x(float, pos.x, mass_string[i]->getGlobalBounds().left, 5), repoz_y(float, pos.y, mass_string[i]->getGlobalBounds().top, 5));
			height += mass_string[i]->getGlobalBounds().height;
		} else {
			mass_string[i]->setPosition(pos.x, pos.y);
			max_length < mass_string[i]->getGlobalBounds().width ? max_length = mass_string[i]->getGlobalBounds().width : max_length;
			rep_y = repoz_y(float, pos.y, mass_string[i]->getGlobalBounds().top, 5);
			rep_y += (height + 5 * i);
			mass_string[i]->setPosition(repoz_x(float, pos.x, mass_string[i]->getGlobalBounds().left, 5), rep_y);
			height += mass_string[i]->getGlobalBounds().height;
		}
	}

	bevel->setSize(Vector2f(max_length + 10, height + (SIZE_MSTX * 5) + 5));
	bevel->setFillColor(*bevel_cl);
	bevel->setPosition(pos.x, pos.y);

	
}

void _interface::multiline_text::resize(int size) noexcept { 
	float rep_y = 0;
	int height = 0;
	for (int i = 0; i < SIZE_MSTX; i++) {
		if (i == 0) {
			mass_string[i]->setCharacterSize(size);
			mass_string[i]->setPosition(pos.x, pos.y);
			max_length = mass_string[i]->getGlobalBounds().width;
			mass_string[i]->setPosition(repoz_x(float, pos.x, mass_string[i]->getGlobalBounds().left, 5), repoz_y(float, pos.y, mass_string[i]->getGlobalBounds().top, 5));
			height += mass_string[i]->getGlobalBounds().height;
		} else {
			mass_string[i]->setCharacterSize(size);
			mass_string[i]->setPosition(pos.x, pos.y);
			max_length < mass_string[i]->getGlobalBounds().width ? max_length = mass_string[i]->getGlobalBounds().width : max_length;
			rep_y = repoz_y(float, pos.y, mass_string[i]->getGlobalBounds().top, 5);
			rep_y += (height + 5 * i);
			mass_string[i]->setPosition(repoz_x(float, pos.x, mass_string[i]->getGlobalBounds().left, 5), rep_y);
			height += mass_string[i]->getGlobalBounds().height;
		}
	}

	bevel->setSize(Vector2f(max_length + 10, height + (SIZE_MSTX * 5) + 5));
	bevel->setFillColor(*bevel_cl);
	bevel->setPosition(pos.x, pos.y);
}

void _interface::multiline_text::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*bevel);
		for (int i = 0; i < SIZE_MSTX; i++) {
			wd.draw(*mass_string[i]);
		}
	}
}

void _interface::multiline_text::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*bevel);
		for (int i = 0; i < SIZE_MSTX; i++) {
			wd->draw(*mass_string[i]);
		}
	}
}
//-----------------------------------Многострочный-текст-multiline_text-Конец-------------------------------------

//-----------------------------------Кнопка-button-Начало-------------------------------------
_interface::button::button(int x, int y, const std::wstring& text, Color maincl, Color textcl, Color activecl) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	active(false)
	{
	pos.x = x;
	pos.y = y;

	font = new Font;
	font->loadFromFile("Img/18094.ttf");

	txt = new Text;
	txt->setFont(*font);
	txt->setString(text);
	txt->setCharacterSize(small);

	txt_cl = new Color;
	*txt_cl = textcl;
	txt->setFillColor(*txt_cl);
	txt->setPosition(pos.x, pos.y);
	txt->setPosition(repoz_x(int, pos.x, txt->getGlobalBounds().left, 5), repoz_y(int, pos.y, txt->getGlobalBounds().top, 5));

	main = new RectangleShape;
	main->setSize(Vector2f(txt->getGlobalBounds().width + 10, txt->getGlobalBounds().height + 10));
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);
	main->setPosition(pos.x, pos.y);

	active_bvl = new RectangleShape;
	active_bvl->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));
	active_cl = new Color;
	*active_cl = activecl;
	active_bvl->setFillColor(*active_cl);
	active_bvl->setPosition(pos.x - 5, pos.y - 5);

	fl_rect = main->getGlobalBounds();
}

_interface::button::~button() {
	delete main, active_bvl, txt, font, active_cl, main_cl, txt_cl;
}

void _interface::button::setPosition(const axes_i &xy) {
	pos = xy;
	txt->setPosition(pos.x, pos.y);
	txt->setPosition(repoz_x(int, pos.x, txt->getGlobalBounds().left, 5), repoz_y(int, pos.y, txt->getGlobalBounds().top, 5));
	main->setPosition(pos.x, pos.y);
	active_bvl->setPosition(pos.x - 5, pos.y - 5);
	fl_rect = main->getGlobalBounds();
}

void __fastcall _interface::button::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
	txt->setPosition(pos.x, pos.y);
	txt->setPosition(repoz_x(int, pos.x, txt->getGlobalBounds().left, 5), repoz_y(int, pos.y, txt->getGlobalBounds().top, 5));
	main->setPosition(pos.x, pos.y);
	active_bvl->setPosition(pos.x - 5, pos.y - 5);
	fl_rect = main->getGlobalBounds();
}

void _interface::button::render(RenderWindow &wd) noexcept {
	if (visible) {
		if (active) {
			wd.draw(*active_bvl);
		}
		wd.draw(*main);
		wd.draw(*txt);
	}
}

void _interface::button::render(RenderWindow *wd) noexcept {
	if (visible) {
		if (active) {
			wd->draw(*active_bvl);
		}
		wd->draw(*main);
		wd->draw(*txt);
	}
}

void _interface::button::resize(int size) {
	txt->setCharacterSize(size);
	txt->setPosition(pos.x, pos.y);
	txt->setPosition(repoz_x(int, pos.x, txt->getGlobalBounds().left, 5), repoz_y(int, pos.y, txt->getGlobalBounds().top, 5));
	main->setSize(Vector2f(txt->getGlobalBounds().width + 10, txt->getGlobalBounds().height + 10));
	active_bvl->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));
	fl_rect = main->getGlobalBounds();
}

void _interface::button::freeze(Camer *camera, const axes_i &xy) {
	this->setPosition(camera->getPosition().x - (camera->getScreenHeight() / 2) + xy.x, camera->getPosition().y - (camera->getScreenWidth() / 2) + xy.y);

}

void _interface::button::freeze(Camer *camera, int x, int y) {
	this->setPosition(camera->getPosition().x - (camera->getScreenHeight() / 2) + x, camera->getPosition().y - (camera->getScreenWidth() / 2) + y);
}

bool __fastcall _interface::button::isAction(int x, int y) {
	fl_rect.contains(x, y) ? active = true : active = false;
	return active;
}

bool _interface::button::isAction(const axes_i &xy) {
	fl_rect.contains(xy.x, xy.y) ? active = true : active = false;
	return active;
}
//-----------------------------------Кнопка-button-Конец--------------------------------------

//-----------------------------------Меню-menu-Начало--------------------------------------
_interface::menu::menu(Camer *camera, Color maincl, Color bordercl) : 
	active(true),
	blackout_visible(true)
	{
	pos = camera->getPosition();
	main = new RectangleShape;
	main->setSize(Vector2f(500, 500));
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);
	main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);

	border = new RectangleShape;
	border->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));
	border_cl = new Color;
	*border_cl = bordercl;
	border->setFillColor(*border_cl);
	border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);

	blackout = new RectangleShape;
	blackout->setSize(Vector2f(camera->getScreenWidth(), camera->getScreenHeight()));
	blackout->setFillColor(Color(0, 0, 0, 255 / 2));
	blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));

	btContinue = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 20,  L"Продолжить", Color::Black, Color::Yellow, Color::Yellow);
	btOptions = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 100, L"Настройки", Color::Black, Color::Yellow, Color::Yellow);
	btExit = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 180, L"Выйти", Color::Black, Color::Yellow, Color::Yellow);

	txMenu = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 450, L"Меню", Color::Yellow, Color::Black);
	txMenu->visible_bevel = false;
	grFirst = new gradient(FloatRect(Vector2f(100, 100), Vector2f(200, 5)), gradient_direction::rightOnLeft, Color::Transparent, Color::Yellow);
	grSecond = new gradient(FloatRect(Vector2f(150, 150), Vector2f(200, 5)), gradient_direction::leftOnRight, Color::Transparent, Color::Yellow);
}

_interface::menu::~menu() {
	delete main, border, blackout, main_cl, border_cl;
	delete btContinue, btOptions, btExit;
	delete txMenu;
	delete grFirst, grSecond;
}

void _interface::menu::render(RenderWindow& wd, Camer *camera) noexcept {

	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		btContinue->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btContinue->getSize().width / 2), main->getGlobalBounds().top + 60);
		btOptions->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btOptions->getSize().width / 2), main->getGlobalBounds().top + 160);
		btExit->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btExit->getSize().width / 2), main->getGlobalBounds().top + 260);
		txMenu->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMenu->getSize().width / 2), main->getGlobalBounds().top + 450);
		grFirst->setPosition(txMenu->getPosition().x - 5 - grFirst->getSize().width, txMenu->getPosition().y + (txMenu->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMenu->getPosition().x + 5 + txMenu->getSize().width, txMenu->getPosition().y + (txMenu->getSize().height / 2) - (grSecond->getSize().height / 2));

		if (blackout_visible) {
			blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));
			wd.draw(*blackout);
		}
		wd.draw(*border);
		wd.draw(*main);

		btContinue->render(wd);
		btOptions->render(wd);
		btExit->render(wd);

		txMenu->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::menu::render(RenderWindow *wd, Camer *camera) noexcept {
	
	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		btContinue->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btContinue->getSize().width / 2), main->getGlobalBounds().top + 60);
		btOptions->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btOptions->getSize().width / 2), main->getGlobalBounds().top + 160);
		btExit->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btExit->getSize().width / 2), main->getGlobalBounds().top + 260);
		txMenu->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMenu->getSize().width / 2), main->getGlobalBounds().top + 450);
		grFirst->setPosition(txMenu->getPosition().x - 5 - grFirst->getSize().width, txMenu->getPosition().y + (txMenu->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMenu->getPosition().x + 5 + txMenu->getSize().width, txMenu->getPosition().y + (txMenu->getSize().height / 2) - (grSecond->getSize().height / 2));

		if (blackout_visible) {
			blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));
			wd->draw(*blackout);
		}
		wd->draw(*border);
		wd->draw(*main);

		btContinue->render(wd);
		btOptions->render(wd);
		btExit->render(wd);

		txMenu->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}
//-----------------------------------Меню-menu-Конец---------------------------------------

//------------------------------Чекбокс-check_box-Начало--------------------------------------
_interface::check_box::check_box(int x, int y, Color maincl, Color bordercl, Color checkcl) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	isCheck(false)
	{

	main_cl = new Color;
	*main_cl = maincl;
	main = new RectangleShape;
	main->setSize(Vector2f(24, 24));
	main->setFillColor(*main_cl);
	main->setPosition(pos.x + 2, pos.y + 2);

	border_cl = new Color;
	*border_cl = bordercl;
	border = new RectangleShape;
	border->setSize(Vector2f(28, 28));
	border->setFillColor(*border_cl);
	border->setPosition(pos.x, pos.y);

	check_cl = new Color;
	*check_cl = checkcl;
	check = new RectangleShape;
	check->setSize(Vector2f(20, 20));
	check->setFillColor(*check_cl);
	check->setPosition(pos.x + 4, pos.y + 4);

	fl_rect = border->getGlobalBounds();
}

_interface::check_box::~check_box() {
	delete main, border, check;
	delete main_cl, border_cl, check_cl;
}

void _interface::check_box::setPosition(const axes_i &xy) {
	pos = xy;

	border->setPosition(pos.x, pos.y);
	main->setPosition(pos.x + 2, pos.y + 2);
	check->setPosition(pos.x + 4, pos.y + 4);
	fl_rect = border->getGlobalBounds();
}

void __fastcall _interface::check_box::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
	
	border->setPosition(pos.x, pos.y);
	main->setPosition(pos.x + 2, pos.y + 2);
	check->setPosition(pos.x + 4, pos.y + 4);
	fl_rect = border->getGlobalBounds();
}

void _interface::check_box::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*border);
		wd.draw(*main);
		if (isCheck) {
			wd.draw(*check);
		}
	}
}

void _interface::check_box::invers(bool operation) {
	isCheck = operation;
}

void _interface::check_box::invers() {
	isCheck = isCheck ? false : true;
}

void _interface::check_box::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*border);
		wd->draw(*main);
		if (isCheck) {
			wd->draw(*check);
		}
	}
}
//------------------------------Чекбокс-check_box-Конец---------------------------------------

//------------------------Меню-настроек-settings_menu-Начало--------------------------------------
_interface::settings_menu::settings_menu(configuration *cf, Camer *camera, Color maincl, Color bordercl) :
	active(false), 
	blackout_visible(false)
	{
		pos = camera->getPosition();
		main = new RectangleShape;
		main->setSize(Vector2f(550, 600));
		main_cl = new Color;
		*main_cl = maincl;
		main->setFillColor(*main_cl);
		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);

		border = new RectangleShape;
		border->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));
		border_cl = new Color;
		*border_cl = bordercl;
		border->setFillColor(*border_cl);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);

		blackout = new RectangleShape;
		blackout->setSize(Vector2f(camera->getScreenWidth(), camera->getScreenHeight()));
		blackout->setFillColor(Color(0, 0, 0, 255 / 2));
		blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));

		btBack = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 410, L"Назад", Color::Black, Color::Yellow, Color::Yellow);
		btSave = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 480, L"Сохранить", Color::Black, Color::Yellow, Color::Yellow); 

		std::string *t = new std::string(intToStr(std::string, cf->screenWidth));
		*t += " x ";
		*t += intToStr(std::string, cf->screenHeight);

		txVertS = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 20, L"Вертикальная синхронизация:", Color::Yellow, Color::Black);
		txVertS->visible_bevel = false;
		txFullS = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 100, L"На весь экран:", Color::Yellow, Color::Black);
		txFullS->visible_bevel = false;
		txScreen = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 180, L"Разрешение экрана:", Color::Yellow, Color::Black);
		txScreen->visible_bevel = false;
		txSound = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 260, L"Звук:", Color::Yellow, Color::Black);
		txSound->visible_bevel = false;
		txSoundV = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 340, L"Громкость звука:", Color::Yellow, Color::Black);
		txSoundV->visible_bevel = false;

		combScreen = new combo_box(txScreen->getSize().left + txScreen->getSize().width + 5, txScreen->getSize().top, Color::Yellow, Color::Yellow);
		combScreen->add(L"1280 x 720", 0);
		combScreen->add(L"1600 x 900", 1);
		combScreen->add(L"1600 x 1200", 2);
		combScreen->add(L"1920 x 1080", 3);
		combScreen->add(L"2560 x 1440", 4);

		combSoundV = new combo_box(txSoundV->getSize().left + txSoundV->getSize().width + 5, txSoundV->getSize().top, Color::Yellow, Color::Yellow);
		combSoundV->add(L"0", 0);
		combSoundV->add(L"10", 10);
		combSoundV->add(L"20", 20);
		combSoundV->add(L"30", 30);
		combSoundV->add(L"40", 40);
		combSoundV->add(L"50", 50);
		combSoundV->add(L"60", 60);
		combSoundV->add(L"70", 70);
		combSoundV->add(L"80", 80);
		combSoundV->add(L"90", 90);
		combSoundV->add(L"100", 100);

		while (std::wstring(t->begin(), t->end()) != combScreen->getText()) {
			combScreen->back();
		}

		*t = intToStr(std::string, cf->soundVolume);

		while (std::wstring(t->begin(), t->end()) != combSoundV->getText()) {
			combSoundV->back();
		}

		cbVertS = new check_box((main->getGlobalBounds().width / 2) + 0 + (txVertS->getSize().width / 2) + 5, main->getGlobalBounds().top + 20, Color::Black, Color::Yellow, Color::Yellow);
		cbVertS->isCheck = cf->verticalSync;
		cbFullS = new check_box((main->getGlobalBounds().width / 2) + 0 + (txFullS->getSize().width / 2) + 5, main->getGlobalBounds().top + 100, Color::Black, Color::Yellow, Color::Yellow);
		cbFullS->isCheck = cf->fullScreen;
		cbSound = new check_box((main->getGlobalBounds().width / 2) + 0 + (txVertS->getSize().width / 2) + 5, main->getGlobalBounds().top + 260, Color::Black, Color::Yellow, Color::Yellow);
		cbSound->isCheck = cf->sound;

		txMenuSettings = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 550, L"Настройки", Color::Yellow, Color::Black);
		txMenuSettings->visible_bevel = false;
		grFirst = new gradient(FloatRect(Vector2f(100, 100), Vector2f(180, 5)), gradient_direction::rightOnLeft, Color::Transparent, Color::Yellow);
		grSecond = new gradient(FloatRect(Vector2f(150, 150), Vector2f(180, 5)), gradient_direction::leftOnRight, Color::Transparent, Color::Yellow);

		delete t;
}

_interface::settings_menu::settings_menu(configuration *cf, Color maincl, Color bordercl) :
	active(false),
	blackout_visible(true)
	{
	pos.x = cf->screenWidth / 2;
	pos.y = cf->screenHeight / 2;
	main = new RectangleShape;
	main->setSize(Vector2f(550, 600));
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);
	main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);

	border = new RectangleShape;
	border->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));
	border_cl = new Color;
	*border_cl = bordercl;
	border->setFillColor(*border_cl);
	border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);

	blackout = new RectangleShape;
	blackout->setSize(Vector2f(cf->screenWidth, cf->screenWidth));
	blackout->setFillColor(Color(0, 0, 0, 255 / 2));
	blackout->setPosition(0,0);

	btBack = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 410, L"Назад", Color::Black, Color::Yellow, Color::Yellow);
	btSave = new button((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 480, L"Сохранить", Color::Black, Color::Yellow, Color::Yellow);
	btBack->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btBack->getSize().width / 2), main->getGlobalBounds().top + 410);
	btSave->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btSave->getSize().width / 2), main->getGlobalBounds().top + 480);


	auto *t = new std::string(intToStr(std::string, cf->screenWidth));
	*t += " x ";
	*t += intToStr(std::string, cf->screenHeight);

	txVertS = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 20, L"Вертикальная синхронизация:", Color::Yellow, Color::Black);
	txVertS->visible_bevel = false;
	txFullS = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 100, L"На весь экран:", Color::Yellow, Color::Black);
	txFullS->visible_bevel = false;
	txScreen = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 180, L"Разрешение экрана:", Color::Yellow, Color::Black);
	txScreen->visible_bevel = false;
	txSound = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 260, L"Звук:", Color::Yellow, Color::Black);
	txSound->visible_bevel = false;
	txSoundV = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 340, L"Громкость звука:", Color::Yellow, Color::Black);
	txSoundV->visible_bevel = false;

	txVertS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txVertS->getSize().width / 2), main->getGlobalBounds().top + 20);
	txFullS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txFullS->getSize().width / 2), main->getGlobalBounds().top + 100);
	txScreen->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txScreen->getSize().width / 2) - 50, main->getGlobalBounds().top + 180); 
	txSound->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSound->getSize().width / 2), main->getGlobalBounds().top + 260);
	txSoundV->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSoundV->getSize().width / 2) - 50, main->getGlobalBounds().top + 340);


	combScreen = new combo_box(txScreen->getSize().left + txScreen->getSize().width + 5, txScreen->getSize().top, Color::Yellow, Color::Yellow);
	combScreen->add(L"1280 x 720", 0);
	combScreen->add(L"1600 x 900", 1);
	combScreen->add(L"1600 x 1200", 2);
	combScreen->add(L"1920 x 1080", 3);
	combScreen->add(L"2560 x 1440", 4);

	combSoundV = new combo_box(txSoundV->getSize().left + txSoundV->getSize().width + 5, txSoundV->getSize().top, Color::Yellow, Color::Yellow);
	combSoundV->add(L"0", 0);
	combSoundV->add(L"10", 10);
	combSoundV->add(L"20", 20);
	combSoundV->add(L"30", 30);
	combSoundV->add(L"40", 40);
	combSoundV->add(L"50", 50);
	combSoundV->add(L"60", 60);
	combSoundV->add(L"70", 70);
	combSoundV->add(L"80", 80);
	combSoundV->add(L"90", 90);
	combSoundV->add(L"100", 100);
	
	while (std::wstring(t->begin(), t->end()) != combScreen->getText()) {
		combScreen->back();
	}

	*t = intToStr(std::string, cf->soundVolume);

	while (std::wstring(t->begin(), t->end()) != combSoundV->getText()) {
		combSoundV->back();
	}

	cbVertS = new check_box((main->getGlobalBounds().width / 2) + 0 + (txVertS->getSize().width / 2) + 5, main->getGlobalBounds().top + 20, Color::Black, Color::Yellow, Color::Yellow);
	cbVertS->isCheck = cf->verticalSync;
	cbFullS = new check_box((main->getGlobalBounds().width / 2) + 0 + (txFullS->getSize().width / 2) + 5, main->getGlobalBounds().top + 100, Color::Black, Color::Yellow, Color::Yellow);
	cbFullS->isCheck = cf->fullScreen;
	cbSound = new check_box((main->getGlobalBounds().width / 2) + 0 + (txVertS->getSize().width / 2) + 5, main->getGlobalBounds().top + 260, Color::Black, Color::Yellow, Color::Yellow);
	cbSound->isCheck = cf->sound;

	cbVertS->setPosition(txVertS->getSize().left + txVertS->getSize().width + 5, main->getGlobalBounds().top + 20);
	cbFullS->setPosition(txFullS->getSize().left + txFullS->getSize().width + 5, main->getGlobalBounds().top + 100);
	cbSound->setPosition(txSound->getSize().left + txSound->getSize().width + 5, main->getGlobalBounds().top + 260);

	txMenuSettings = new text((main->getGlobalBounds().width / 2) + 0, main->getGlobalBounds().top + 550, L"Настройки", Color::Yellow, Color::Black);
	txMenuSettings->visible_bevel = false;
	grFirst = new gradient(FloatRect(Vector2f(100, 100), Vector2f(180, 5)), gradient_direction::rightOnLeft, Color::Transparent, Color::Yellow);
	grSecond = new gradient(FloatRect(Vector2f(150, 150), Vector2f(180, 5)), gradient_direction::leftOnRight, Color::Transparent, Color::Yellow);
	txMenuSettings->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMenuSettings->getSize().width / 2), main->getGlobalBounds().top + 550);
	grFirst->setPosition(txMenuSettings->getPosition().x - 5 - grFirst->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));
	grSecond->setPosition(txMenuSettings->getPosition().x + 5 + txMenuSettings->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));

	delete t;
}

_interface::settings_menu::~settings_menu() {
	delete main, border, blackout, main_cl, border_cl;
	delete btBack, btSave;
	delete txVertS, txFullS, txScreen, txSound, txSoundV;
	delete cbFullS, cbVertS, cbSound;
	delete txMenuSettings;
	delete grFirst, grSecond;
	delete combScreen, combSoundV;
}

void _interface::settings_menu::backSettings(configuration *cf) {
	cbFullS->isCheck = cf->fullScreen;
	cbVertS->isCheck = cf->verticalSync;
	cbSound->isCheck = cf->sound;

	std::string* t = new std::string(intToStr(std::string, cf->screenWidth));
	*t += " x ";
	*t += intToStr(std::string, cf->screenHeight);

	while (std::wstring(t->begin(), t->end()) != combScreen->getText()) {
		combScreen->back();
	}

	*t = intToStr(std::string, cf->soundVolume);

	while (std::wstring(t->begin(), t->end()) != combSoundV->getText()) {
		combSoundV->back();
	}

	delete t;
}

int _interface::settings_menu::saveSettings(configuration *cf) {

	int H = 0, W = 0;

	switch (combScreen->getValue()) {
		case 0: W = 1280; H = 720; break;
		case 1: W = 1600; H = 900; break;
		case 2: W = 1600; H = 1200; break;
		case 3: W = 1920; H = 1080; break;
		case 4: W = 2560; H = 1440; break;
		default: break;
	}


	if (cf->fullScreen != cbFullS->isCheck || (cf->screenWidth != W && cf->screenHeight != H)) {
		cf->fullScreen = cbFullS->isCheck;
		cf->verticalSync = cbVertS->isCheck;
		cf->sound = cbSound->isCheck;
		cf->soundVolume = combSoundV->getValue();
		cf->screenWidth = W;
		cf->screenHeight = H;
		cf->saveSettings();
		return static_cast<int>(settings_save_code::other);
	} else {
		cf->verticalSync = cbVertS->isCheck;
		cf->sound = cbSound->isCheck;
		cf->soundVolume = combSoundV->getValue();
		cf->saveSettings();
		return static_cast<int>(settings_save_code::sound_or_sound_volume_or_verticalS);
	}
}

void _interface::settings_menu::render(RenderWindow &wd, Camer *camera) noexcept {
	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		txVertS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txVertS->getSize().width / 2), main->getGlobalBounds().top + 20);
		txFullS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txFullS->getSize().width / 2), main->getGlobalBounds().top + 100);
		txScreen->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txScreen->getSize().width / 2) - 50, main->getGlobalBounds().top + 180);
		
		txSound->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSound->getSize().width / 2), main->getGlobalBounds().top + 260);
		txSoundV->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSoundV->getSize().width / 2), main->getGlobalBounds().top + 340);
		combSoundV->setPosition(txSoundV->getSize().left + txSoundV->getSize().width + 5, txSoundV->getSize().top);
		cbSound->setPosition(txSound->getSize().left + txSound->getSize().width + 5, main->getGlobalBounds().top + 260);

		combScreen->setPosition(txScreen->getSize().left + txScreen->getSize().width + 5, txScreen->getSize().top);
		btBack->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btBack->getSize().width / 2), main->getGlobalBounds().top + 410);
		btSave->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btSave->getSize().width / 2), main->getGlobalBounds().top + 480);
		cbVertS->setPosition(txVertS->getSize().left + txVertS->getSize().width + 5, main->getGlobalBounds().top + 20);
		cbFullS->setPosition(txFullS->getSize().left + txFullS->getSize().width + 5, main->getGlobalBounds().top + 100);
		txMenuSettings->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMenuSettings->getSize().width / 2), main->getGlobalBounds().top + 550);
		grFirst->setPosition(txMenuSettings->getPosition().x - 5 - grFirst->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMenuSettings->getPosition().x + 5 + txMenuSettings->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));
		
		if (blackout_visible) {
			blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));
			wd.draw(*blackout);
		}
		wd.draw(*border);
		wd.draw(*main);

		txVertS->render(wd);
		txScreen->render(wd);
		txFullS->render(wd);
		txSound->render(wd);
		txSoundV->render(wd);

		combScreen->render(wd);
		combSoundV->render(wd);

		cbVertS->render(wd);
		cbFullS->render(wd);
		cbSound->render(wd);

		btBack->render(wd);
		btSave->render(wd);

		txMenuSettings->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::settings_menu::render(RenderWindow *wd, Camer *camera) noexcept {
	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		txVertS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txVertS->getSize().width / 2), main->getGlobalBounds().top + 20);
		txFullS->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txFullS->getSize().width / 2), main->getGlobalBounds().top + 100);
		txScreen->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txScreen->getSize().width / 2) - 50, main->getGlobalBounds().top + 180);

		txSound->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSound->getSize().width / 2), main->getGlobalBounds().top + 260);
		txSoundV->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txSoundV->getSize().width / 2), main->getGlobalBounds().top + 340);
		combSoundV->setPosition(txSoundV->getSize().left + txSoundV->getSize().width + 5, txSoundV->getSize().top);
		cbSound->setPosition(txSound->getSize().left + txSound->getSize().width + 5, main->getGlobalBounds().top + 260);

		combScreen->setPosition(txScreen->getSize().left + txScreen->getSize().width + 5, txScreen->getSize().top);
		btBack->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btBack->getSize().width / 2), main->getGlobalBounds().top + 410);
		btSave->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btSave->getSize().width / 2), main->getGlobalBounds().top + 480);
		cbVertS->setPosition(txVertS->getSize().left + txVertS->getSize().width + 5, main->getGlobalBounds().top + 20);
		cbFullS->setPosition(txFullS->getSize().left + txFullS->getSize().width + 5, main->getGlobalBounds().top + 100);
		txMenuSettings->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMenuSettings->getSize().width / 2), main->getGlobalBounds().top + 550);
		grFirst->setPosition(txMenuSettings->getPosition().x - 5 - grFirst->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMenuSettings->getPosition().x + 5 + txMenuSettings->getSize().width, txMenuSettings->getPosition().y + (txMenuSettings->getSize().height / 2) - (grSecond->getSize().height / 2));

		if (blackout_visible) {
			blackout->setPosition(camera->getPosition().x - (camera->getScreenWidth() / 2), camera->getPosition().y - (camera->getScreenHeight() / 2));
			wd->draw(*blackout);
		}
		wd->draw(*border);
		wd->draw(*main);

		txVertS->render(wd);
		txScreen->render(wd);
		txFullS->render(wd);
		txSound->render(wd);
		txSoundV->render(wd);

		combScreen->render(wd);
		combSoundV->render(wd);

		cbVertS->render(wd);
		cbFullS->render(wd);
		cbSound->render(wd);

		btBack->render(wd);
		btSave->render(wd);

		txMenuSettings->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::settings_menu::render(RenderWindow &wd) noexcept {
	if (active) {
		if (blackout_visible) {
			wd.draw(*blackout);
		}
		wd.draw(*border);
		wd.draw(*main);

		txVertS->render(wd);
		txScreen->render(wd);
		txFullS->render(wd);
		txSound->render(wd);
		txSoundV->render(wd);
		
		combScreen->render(wd);
		combSoundV->render(wd);

		cbVertS->render(wd);
		cbFullS->render(wd);
		cbSound->render(wd);

		btBack->render(wd);
		btSave->render(wd);

		txMenuSettings->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::settings_menu::render(RenderWindow *wd) noexcept {
	if (active) {
		if (blackout_visible) {
			wd->draw(*blackout);
		}
		wd->draw(*border);
		wd->draw(*main);

		txVertS->render(wd);
		txScreen->render(wd);
		txFullS->render(wd);
		txSound->render(wd);
		txSoundV->render(wd);

		combScreen->render(wd);
		combSoundV->render(wd);

		cbVertS->render(wd);
		cbFullS->render(wd);
		cbSound->render(wd);

		btBack->render(wd);
		btSave->render(wd);

		txMenuSettings->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}
//------------------------Меню-настроек-settings_menu-Конец---------------------------------------

//------------------------Главное-меню-main_menu-Начало---------------------------------------
_interface::main_menu::main_menu(configuration *cf, Color maincl) {
	main = new RectangleShape;
	main->setSize(Vector2f(500, cf->screenHeight));
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);
	main->setPosition(0,0);

	btStart = new button(0, 0, L"Новая игра", Color::Black, Color::Yellow, Color::Yellow);
	btStartTren = new button(0, 0, L"Пройти обучение", Color::Black, Color::Yellow, Color::Yellow);
	btOptions = new button(0, 0, L"Настройки", Color::Black, Color::Yellow, Color::Yellow);
	btExit = new button(0, 0, L"Выйти", Color::Black, Color::Yellow, Color::Yellow);

	btStart->setPosition((main->getGlobalBounds().width / 2) - (btStart->getSize().width / 2), main->getGlobalBounds().top + 340);
	btStartTren->setPosition((main->getGlobalBounds().width / 2) - (btStartTren->getSize().width / 2), main->getGlobalBounds().top + 440);
	btOptions->setPosition((main->getGlobalBounds().width / 2) - (btOptions->getSize().width / 2), main->getGlobalBounds().top + 540);
	btExit->setPosition((main->getGlobalBounds().width / 2) - (btExit->getSize().width / 2), main->getGlobalBounds().top + 640);


	txMainMenu = new text(0, 0, L"Главное меню", Color::Yellow, Color::Black);
	txMainMenu->visible_bevel = false;
	grFirst = new gradient(FloatRect(Vector2f(0, 0), Vector2f(100, 5)), gradient_direction::rightOnLeft, Color::Transparent, Color::Yellow);
	grSecond = new gradient(FloatRect(Vector2f(0, 0), Vector2f(100, 5)), gradient_direction::leftOnRight, Color::Transparent, Color::Yellow);

	txMainMenu->setPosition((main->getGlobalBounds().width / 2) - (txMainMenu->getSize().width / 2),  main->getGlobalBounds().height - 100);
	grFirst->setPosition(txMainMenu->getPosition().x - 5 - grFirst->getSize().width, txMainMenu->getPosition().y + (txMainMenu->getSize().height / 2) - (grFirst->getSize().height / 2));
	grSecond->setPosition(txMainMenu->getPosition().x + 5 + txMainMenu->getSize().width, txMainMenu->getPosition().y + (txMainMenu->getSize().height / 2) - (grSecond->getSize().height / 2));
}

_interface::main_menu::~main_menu() {
	delete main, main_cl;
	delete btStart, btOptions, btExit, btStartTren;
	delete txMainMenu;
	delete grFirst, grSecond;
}

void _interface::main_menu::render(RenderWindow &wd) noexcept {
	wd.draw(*main);
	btStart->render(wd);
	btStartTren->render(wd);
	btOptions->render(wd);
	btExit->render(wd);

	txMainMenu->render(wd);
	grFirst->render(wd);
	grSecond->render(wd);
}

void _interface::main_menu::render(RenderWindow *wd) noexcept {
	wd->draw(*main);
	btStart->render(wd);
	btStartTren->render(wd);
	btOptions->render(wd);
	btExit->render(wd);

	txMainMenu->render(wd);
	grFirst->render(wd);
	grSecond->render(wd);
}
//------------------------Главное-меню-main_menu-Конец----------------------------------------

//------------------------------Градиент-gradient-Начало---------------------------------------
_interface::gradient::gradient(const FloatRect &rt, gradient_direction gd, Color first, Color second) :
	BaseInerface(0, 0, rt)
	{
	rect = new sf::VertexArray;

	pos.x = fl_rect.left; 
	pos.y = fl_rect.top;

	rect->setPrimitiveType(sf::Quads);
	
	switch (gd) {
	case _interface::leftOnRight:
		for (int i = 0; i < 2; i++) {
			colors.push_back(second);
			colors.push_back(first);
		}
		break;
	case _interface::rightOnLeft:
		for (int i = 0; i < 2; i++) {
			colors.push_back(first);
			colors.push_back(second);
		}
		break;
	case _interface::topOnBottom:
		colors.push_back(first);
		colors.push_back(first);
		colors.push_back(second);
		colors.push_back(second);
		break;
	case _interface::bottomOnTop:
		colors.push_back(second);
		colors.push_back(second);
		colors.push_back(first);
		colors.push_back(first);
		break;
	default:
		break;
	}

	rect->append(sf::Vertex(sf::Vector2f(fl_rect.left, fl_rect.top), colors[0]));
	rect->append(sf::Vertex(sf::Vector2f(fl_rect.left + fl_rect.width, fl_rect.top), colors[1]));
	rect->append(sf::Vertex(sf::Vector2f(fl_rect.left + fl_rect.width, fl_rect.top + fl_rect.height), colors[3]));
	rect->append(sf::Vertex(sf::Vector2f(fl_rect.left, fl_rect.top + fl_rect.height), colors[2]));
}

_interface::gradient::~gradient() {
	delete rect;
}

void _interface::gradient::setPosition(const axes_i &xy) {
	pos = xy;

	rect->clear();

	rect->append(sf::Vertex(sf::Vector2f(pos.x, pos.y), colors[0]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x + fl_rect.width, pos.y), colors[1]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x + fl_rect.width, pos.y + fl_rect.height), colors[3]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x, pos.y + fl_rect.height), colors[2]));
}

void __fastcall _interface::gradient::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;

	rect->clear();

	rect->append(sf::Vertex(sf::Vector2f(pos.x, pos.y), colors[0]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x + fl_rect.width, pos.y), colors[1]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x + fl_rect.width, pos.y + fl_rect.height), colors[3]));
	rect->append(sf::Vertex(sf::Vector2f(pos.x, pos.y + fl_rect.height), colors[2]));
}

void _interface::gradient::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*rect);
	}
}

void _interface::gradient::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*rect);
	}
}
//-------------------------------Градиент-gradient-Конец---------------------------------------

//----------------------------Комбо-бокс-combo_box-Начало---------------------------------------
_interface::combo_box::cell::cell(Text txt, int val) :
	text(txt),
	value(val)
	{
	
}

_interface::combo_box::combo_box(int x, int y, Color maincl, Color textcl) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	visible_main(false),
	active(false)
	{
	font = new Font;
	font->loadFromFile("Img/18094.ttf");
	
	text_cl = new Color;
	*text_cl = textcl;

	main = new RectangleShape;
	main->setSize(Vector2f(0, 0));
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);
	main->setPosition(pos.x, pos.y);
	
	fl_rect = main->getGlobalBounds();

	it = mass_text.begin();
}

_interface::combo_box::~combo_box() {
	delete main;
	delete text_cl, main_cl;

	for (it = mass_text.begin(); it != mass_text.end(); it++) {
		delete *it;
	}

	mass_text.clear();
}

void _interface::combo_box::setPosition(const axes_i &xy) {
	pos = xy;

	main->setPosition(pos.x, pos.y);
	(*it)->text.setPosition(pos.x, pos.y);
	(*it)->text.setPosition(repoz_x(int, pos.x, (*it)->text.getGlobalBounds().left, 5), repoz_y(int, pos.y, (*it)->text.getGlobalBounds().top, 5));

	fl_rect = main->getGlobalBounds();
}

void __fastcall _interface::combo_box::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;

	main->setPosition(pos.x, pos.y);
	(*it)->text.setPosition(pos.x, pos.y);
	(*it)->text.setPosition(repoz_x(int, pos.x, (*it)->text.getGlobalBounds().left, 5), repoz_y(int, pos.y, (*it)->text.getGlobalBounds().top, 5));

	fl_rect = main->getGlobalBounds();
}

void _interface::combo_box::add(const std::wstring& st, int val) {
	mass_text.push_back(new cell(Text(st, *font, _interface::text_size::small), val));
	it = mass_text.end();
	it--;
	(*it)->text.setFillColor(*text_cl);
	(*it)->text.setPosition(pos.x, pos.y);
	(*it)->text.setPosition(repoz_x(int, pos.x, (*it)->text.getGlobalBounds().left, 5), repoz_y(int, pos.y, (*it)->text.getGlobalBounds().top, 5));
	main->setSize(Vector2f((*it)->text.getGlobalBounds().width + 10, (*it)->text.getGlobalBounds().height + 10));
	fl_rect = main->getGlobalBounds();
}

void _interface::combo_box::next() {
	it++;
	if (it == mass_text.end()) {
		it = mass_text.begin();
	}

	if (active) {
		(*it)->text.setFillColor(Color::Black);
	} else {
		(*it)->text.setFillColor(Color::Yellow);
	}

	main->setSize(Vector2f((*it)->text.getGlobalBounds().width + 10, (*it)->text.getGlobalBounds().height + 10));
	fl_rect = main->getGlobalBounds();
}

void _interface::combo_box::back() {
	if (it != mass_text.begin()) {
		it--;
	} else {
		it = mass_text.end();
		it--;
	}

	if (active) {
		(*it)->text.setFillColor(Color::Black);
	} else {
		(*it)->text.setFillColor(Color::Yellow);
	}

	main->setSize(Vector2f((*it)->text.getGlobalBounds().width + 10, (*it)->text.getGlobalBounds().height + 10));
	fl_rect = main->getGlobalBounds();
}

std::wstring _interface::combo_box::getText() {
	return std::wstring((*it)->text.getString().begin(), (*it)->text.getString().end());
}

int _interface::combo_box::getValue() {
	return (*it)->value;
}

bool __fastcall _interface::combo_box::isAction(int x, int y) {
	fl_rect.contains(x, y) ? active = true : active = false;
	if (active) {
		(*it)->text.setFillColor(Color::Black);
		visible_main = true;
	} else {
		(*it)->text.setFillColor(Color::Yellow);
		visible_main = false;
	}
	return active;
}

bool _interface::combo_box::isAction(const axes_i &xy) {
	fl_rect.contains(xy.x, xy.y) ? active = true : active = false;
	if (active) {
		(*it)->text.setFillColor(Color::Black);
		visible_main = true;
	} else {
		(*it)->text.setFillColor(Color::Yellow);
		visible_main = false;
	}
	return active;
}

void _interface::combo_box::render(RenderWindow &wd) {
	if (visible) {
		if (visible_main) {
			wd.draw(*main);
		}
		wd.draw((*it)->text);
	}
}

void _interface::combo_box::render(RenderWindow *wd) {
	if (visible) {
		if (visible_main) {
			wd->draw(*main);
		}
		wd->draw((*it)->text);
	}
}
//-----------------------------Комбо-бокс-combo_box-Конец---------------------------------------

//-----------------------------Сообщение-message-Начало---------------------------------------
_interface::message::message(int x, int y, const std::wstring &txt, Color maincl, Color bordercl, Color textcl) :
	active(false)
	{
	pos.x = x;
	pos.y = y;

	main = new RectangleShape;
	main_cl = new Color;
	*main_cl = maincl;
	main->setFillColor(*main_cl);

	border = new RectangleShape;
	border_cl = new Color;
	*border_cl = bordercl;
	border->setFillColor(*border_cl);

	txInfo = new text(0, 0, txt, textcl);
	txInfo->visible_bevel = false;
	txMess = new text(0, 0, L"Сообщение", Color::Yellow);
	txMess->visible_bevel = false;
	
	main->setSize(Vector2f(txInfo->getSize().width + 10, 150));
	border->setSize(Vector2f(main->getGlobalBounds().width + 10, main->getGlobalBounds().height + 10));

	main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
	border->setPosition(main->getGlobalBounds().left - 5, main->getGlobalBounds().top - 5);

	txInfo->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txInfo->getSize().width / 2), main->getGlobalBounds().top + 10);
	txMess->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMess->getSize().width / 2), main->getGlobalBounds().top + main->getGlobalBounds().height - 50);
	grFirst = new gradient(FloatRect(Vector2f(0, 0), Vector2f(150, 5)), gradient_direction::rightOnLeft, Color::Transparent, Color::Yellow);
	grSecond = new gradient(FloatRect(Vector2f(0, 0), Vector2f(150, 5)), gradient_direction::leftOnRight, Color::Transparent, Color::Yellow);

	grFirst->setPosition(txMess->getPosition().x - 5 - grFirst->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grFirst->getSize().height / 2));
	grSecond->setPosition(txMess->getPosition().x + 5 + txMess->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grSecond->getSize().height / 2));

	btOk = new button(0, 0, L"Ок", Color::Black, Color::Yellow, Color::Yellow);
	btOk->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btOk->getSize().width / 2), txMess->getSize().top - 35);
}

_interface::message::~message() {
	delete main_cl, border_cl;
	delete main, border;
	delete txInfo, txMess;
	delete grFirst, grSecond;
	delete btOk;
}

void _interface::message::render(RenderWindow &wd, Camer *camera) noexcept {
	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		txInfo->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txInfo->getSize().width / 2), main->getGlobalBounds().top + 10);
		txMess->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMess->getSize().width / 2), main->getGlobalBounds().top + main->getGlobalBounds().height - 50);
		btOk->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btOk->getSize().width / 2), txInfo->getSize().top + txInfo->getSize().height + 20);
		grFirst->setPosition(txMess->getPosition().x - 5 - grFirst->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMess->getPosition().x + 5 + txMess->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grSecond->getSize().height / 2));

		wd.draw(*border);
		wd.draw(*main);

		txInfo->render(wd);
		
		btOk->render(wd);

		txMess->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::message::render(RenderWindow *wd, Camer *camera) noexcept {
	if (active) {
		pos = camera->getPosition();

		main->setPosition(pos.x - main->getGlobalBounds().width / 2, pos.y - main->getGlobalBounds().height / 2);
		border->setPosition(pos.x - 5 - main->getGlobalBounds().width / 2, pos.y - 5 - main->getGlobalBounds().height / 2);
		txInfo->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txInfo->getSize().width / 2), main->getGlobalBounds().top + 10);
		txMess->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (txMess->getSize().width / 2), main->getGlobalBounds().top + main->getGlobalBounds().height - 50);
		btOk->setPosition(main->getGlobalBounds().left + (main->getGlobalBounds().width / 2) - (btOk->getSize().width / 2), txInfo->getSize().top + txInfo->getSize().height + 20);
		grFirst->setPosition(txMess->getPosition().x - 5 - grFirst->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grSecond->getSize().height / 2));
		grSecond->setPosition(txMess->getPosition().x + 5 + txMess->getSize().width, txMess->getPosition().y + (txMess->getSize().height / 2) - (grSecond->getSize().height / 2));

		wd->draw(*border);
		wd->draw(*main);

		txInfo->render(wd);

		btOk->render(wd);

		txMess->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::message::render(RenderWindow &wd) noexcept {
	if (active) {
		wd.draw(*border);
		wd.draw(*main);

		txInfo->render(wd);
		btOk->render(wd);

		txMess->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}

void _interface::message::render(RenderWindow *wd) noexcept {
	if (active) {
		wd->draw(*border);
		wd->draw(*main);

		txInfo->render(wd);

		btOk->render(wd);

		txMess->render(wd);
		grFirst->render(wd);
		grSecond->render(wd);
	}
}
//-----------------------------Сообщение-message-Конец---------------------------------------

//-----------------------------Коллизия-Collision-Начало---------------------------------------
Collision::Collision(const IntRect &rect) :
	active(true),
	visible_deb(false)
	{
	pos.x = rect.left;
	pos.y = rect.top;
	rect_collis = rect;

	main = new RectangleShape;
	main->setPosition(Vector2f(rect_collis.left, rect_collis.top));
	main->setSize(Vector2f(rect_collis.width, rect_collis.height));
	main->setFillColor(Color::Red);
}

Collision::~Collision() {
	delete main;
}

void __fastcall Collision::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
	
	rect_collis.left = pos.x;
	rect_collis.top = pos.y;
	main->setPosition(Vector2f(pos.x, pos.y));
}

void Collision::setPosition(const axes_i &xy) {
	pos = xy;

	rect_collis.left = pos.x;
	rect_collis.top = pos.y;
	main->setPosition(Vector2f(pos.x, pos.y));
}

axes_i Collision::getPosition() {
	return pos;
}

IntRect Collision::getBounds() {
	return rect_collis;
}

void Collision::setBounds(const IntRect &rect) {
	pos.x = rect.left;
	pos.y = rect.top;
	rect_collis = rect;
	main->setPosition(Vector2f(pos.x, pos.y));
	main->setSize(Vector2f(rect_collis.width, rect_collis.height));
}

void Collision::render(RenderWindow &wd) {
	if (visible_deb) {
		wd.draw(*main);
	}
}

void Collision::render(RenderWindow *wd) {
	if (visible_deb) {
		wd->draw(*main);
	}
}
//-----------------------------Коллизия-Collision-Конец----------------------------------------

//-----------------------------Мини-полоса-min_bar-Начало----------------------------------------
_interface::min_bar::min_bar(int x, int y, int br_ma, int br_mi, Color mcol, Color bcol) :
	BaseInerface(x, y, FloatRect(Vector2f(0, 0), Vector2f(0, 0))),
	max_br(br_ma),
	min_br(br_mi),
	curr_br(br_ma)
	{
	main = new RectangleShape;
	main->setSize(Vector2f(150, 9));
	main->setFillColor(mcol);
	main->setPosition(pos.x, pos.y);

	bevel = new RectangleShape;
	bevel->setSize(Vector2f(main->getGlobalBounds().width - 6, main->getGlobalBounds().height - 6));
	bevel->setFillColor(bcol);
	bevel->setPosition(pos.x + 3, pos.y + 3);

	fl_rect = main->getGlobalBounds();
}

_interface::min_bar::~min_bar() {
	delete main, bevel;
}

void __fastcall _interface::min_bar::setPosition(int x, int y) noexcept {
	pos.x = x;
	pos.y = y;

	main->setPosition(pos.x, pos.y);
	bevel->setPosition(pos.x + 3, pos.y + 3);
	fl_rect = main->getGlobalBounds();
}

void _interface::min_bar::setPosition(const axes_i &xy) noexcept {
	pos = xy;

	main->setPosition(pos.x, pos.y);
	bevel->setPosition(pos.x + 3, pos.y + 3);
	fl_rect = main->getGlobalBounds();
}

void  _interface::min_bar::changeBar(int arg) noexcept {
	if (curr_br != arg) {
		if (arg < 0) {
			arg = 0;
		}
		bevel->setSize(Vector2f((144 * arg) / max_br, 3));
		curr_br = arg;
	}
}

void _interface::min_bar::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*main);
		wd.draw(*bevel);
	}
}

void _interface::min_bar::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*main);
		wd->draw(*bevel);
	}
}
//-----------------------------Мини-полоса-min_bar-Конец-----------------------------------------

//-----------------------------------Разрушитель-замков-DestroerCastle-Начало-----------------------------------------
DestroerCastle::DestroerCastle(Texture* ptr_texture, float X_POS, float Y_POS, int hp) :
	BaseCharacter(ptr_texture, X_POS, Y_POS, hp)
	{
	sprt->setTextureRect(IntRect(0, 0, 600, 350));
	sprt->setPosition(pos.x, pos.y);
	direction = 0;
	last_direction = direction;

	HP = new _interface::min_bar(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height, health, 0, Color::Black, Color::Red);
}

DestroerCastle::~DestroerCastle() {
	delete HP;
}

void __fastcall DestroerCastle::setPosition(float x, float y) noexcept {
	pos.x = x;
	pos.y = y;

	sprt->setPosition(pos.x, pos.y);
}

void DestroerCastle::setPosition(const axes_f &xy) noexcept {
	pos = xy;

	sprt->setPosition(pos.x, pos.y);
}

void __fastcall DestroerCastle::move(float time, int direct) noexcept {
	direction = direct;

	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 6) {
			frame = 5;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(600 * int(frame), 700, 600, 350));
	} else {
		switch (direction) { //Направление движения
		case direcrion8::right:
			frame += 0.023 * time;
			if (frame > 4) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(600 * int(frame) + 600, 0, -600, 350)); 
			sprt->move(0.5 * time, 0); break;
		case direcrion8::left:
			frame += 0.023 * time;
			if (frame > 4) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(600 * int(frame), 0, 600, 350));
			sprt->move(-0.5 * time, 0); break;
		default:
			break;
		}
	}
	last_direction = direction;
	direction = 0;
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
	fl_rect = sprt->getGlobalBounds();
	pos.x = sprt->getGlobalBounds().left;
	pos.y = sprt->getGlobalBounds().top;
}

void __fastcall DestroerCastle::attack(float time) {
	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 6) {
			frame = 5;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(600 * int(frame), 700, 600, 350));
	} else {
		frame += 0.023 * time;
		if (frame > 6) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(600 * int(frame), 350, 600, 350));
	}
}

bool DestroerCastle::isCooldown(float time) {
	if (!cooldown) {
		timer_cooldown = 0;
		return false;
	} else {
		timer_cooldown += time;
		if (timer_cooldown >= 3) {
			cooldown = false;
			timer_cooldown = 0;
			return false;
		}
		cooldown = true;
		return true;
	}
}

void DestroerCastle::render(RenderWindow &wd) noexcept {
	wd.draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}

void DestroerCastle::render(RenderWindow *wd) noexcept {
	wd->draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}
//-----------------------------------Разрушитель-замков-DestroerCastle-Конец------------------------------------------

//-----------------------------------------Копейщик-Spearman-Начало------------------------------------------
Spearman::Spearman(Texture* ptr_texture, float X_POS, float Y_POS, int hp) :
	BaseCharacter(ptr_texture, X_POS, Y_POS, hp)
	{
	sprt->setTextureRect(IntRect(0, 0, 600, 350));
	sprt->setPosition(pos.x, pos.y);
	direction = 0;
	last_direction = direction;

	fl_rect = sprt->getGlobalBounds();
	rect_collis = new Collision(IntRect(sprt->getGlobalBounds().left + 120, sprt->getGlobalBounds().top, 115, sprt->getGlobalBounds().height / 2));

	HP = new _interface::min_bar(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2), sprt->getGlobalBounds().top + sprt->getGlobalBounds().height, health, 0, Color::Black, Color::Red);
}

Spearman::~Spearman() {
	delete rect_collis, HP;
}

void __fastcall Spearman::setPosition(float x, float y) noexcept {
	pos.x = x;
	pos.y = y;

	sprt->setPosition(pos.x, pos.y);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x + 120, pos.y);
}

void Spearman::setPosition(const axes_f &xy) noexcept {
	pos = xy;

	sprt->setPosition(pos.x, pos.y);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x + 120, pos.y);
}

void __fastcall Spearman::move(float time, int direct) noexcept {
	direction = direct;

	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 6;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(300 * int(frame) + 300, 360, -300, 180));
	} else {
		switch (direction) { //Направление движения
		case direcrion8::right:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(300 * int(frame), 0, 300, 180));
			sprt->move(0.5 * time, 0); break;
		case direcrion8::left:
			frame += 0.023 * time;
			if (frame > 7) {
				frame = 0;
			}
			sprt->setTextureRect(IntRect(300 * int(frame) + 300, 0, -300, 180));
			sprt->move(-0.5 * time, 0); break;
		default:
			break;
		}
	}
	last_direction = direction;
	direction = 0;
	HP->setPosition(sprt->getGlobalBounds().left + (sprt->getGlobalBounds().width / 2) - (HP->getSize().width / 2) + 30, sprt->getGlobalBounds().top + sprt->getGlobalBounds().height);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setBounds(IntRect(sprt->getGlobalBounds().left + 120, sprt->getGlobalBounds().top, 115, sprt->getGlobalBounds().height / 2));
	pos.x = sprt->getGlobalBounds().left;
	pos.y = sprt->getGlobalBounds().top;
}

void __fastcall Spearman::attack(float time) {
	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 7) {
			frame = 6;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(300 * int(frame) + 300, 360, -300, 180));
	} else {
		frame += 0.023 * time;
		if (frame > 6) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(300 * int(frame) + 300, 180, -300, 180));
	}
}

bool Spearman::isCooldown(float time) {
	if (!cooldown) {
		timer_cooldown = 0;
		return false;
	} else {
		timer_cooldown += time;
		if (timer_cooldown >= 2) {
			cooldown = false;
			timer_cooldown = 0;
			return false;
		}
		cooldown = true;
		return true;
	}
}

void Spearman::render(RenderWindow &wd) noexcept {
	rect_collis->render(wd);
	wd.draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}

void Spearman::render(RenderWindow *wd) noexcept {
	rect_collis->render(wd);
	wd->draw(*sprt);
	HP->changeBar(health);
	HP->render(wd);
}
//------------------------------------------Копейщик-Spearman-Конец------------------------------------------

//-------------------------------------Задний-фон-background_color-Начало------------------------------------------
_interface::background_color::background_color(int x, int y, Color cl, configuration *cf) :
	visible(false)
	{
	pos.x = x;
	pos.y = y;

	background = new RectangleShape;
	background->setSize(Vector2f(cf->screenWidth, cf->screenHeight));
	background->setFillColor(cl);
	background->setPosition(pos.x, pos.y);
}

_interface::background_color::~background_color() {
	delete background;
}

void _interface::background_color::setColor(Color cl) {
	background->setFillColor(cl);
}

void _interface::background_color::render(RenderWindow &wd) noexcept {
	if (visible) {
		wd.draw(*background);
	}
}

void _interface::background_color::render(RenderWindow *wd) noexcept {
	if (visible) {
		wd->draw(*background);
	}
}
//-------------------------------------Задний-фон-background_color-Конец------------------------------------------

//---------------------------------------Ледяной-шар-IceBall-Начало------------------------------------------
IceBall::IceBall(Texture* ptr_texture, float X_POS, float Y_POS, int hp) :
	BaseCharacter(ptr_texture, X_POS, Y_POS, hp)
	{
	visible = true;
	sprt->setTextureRect(IntRect(0, 0, 400, 120));
	sprt->setPosition(pos.x, pos.y);

	fl_rect = sprt->getGlobalBounds();
	rect_collis = new Collision(IntRect(sprt->getGlobalBounds().left, sprt->getGlobalBounds().top, 120, sprt->getGlobalBounds().height));

	HP = new _interface::min_bar(rect_collis->getBounds().left + (rect_collis->getBounds().width / 2) - 70, sprt->getGlobalBounds().top + sprt->getGlobalBounds().height, health, 0, Color::Black, Color::Green);
}

IceBall::~IceBall() {
	delete rect_collis, HP;
}

void __fastcall IceBall::setPosition(float x, float y) noexcept {
	pos.x = x;
	pos.y = y;

	sprt->setPosition(pos.x, pos.y);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x, pos.y);
}

void IceBall::setPosition(const axes_f &xy) noexcept {
	pos = xy;

	sprt->setPosition(pos.x, pos.y);
	fl_rect = sprt->getGlobalBounds();
	rect_collis->setPosition(pos.x, pos.y);
}

void __fastcall IceBall::update(float time) noexcept {
	if (health <= 0 && !zeroing) {
		zeroing = true;
		frame = 0;
	}

	if (health <= 0) {
		frame += 0.023 * time;
		if (frame > 5) {
			frame = 4;
			isDead = true;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 120, 400, 120));
	} else {
		frame += 0.023 * time;
		if (frame > 5) {
			frame = 0;
		}
		sprt->setTextureRect(IntRect(400 * int(frame), 0, 400, 120));
	}	
}

bool IceBall::isCooldown(float time) {
	if (!cooldown) {
		timer_cooldown = 0;
		return false;
	} else {
		timer_cooldown += time;
		if (timer_cooldown >= 1) {
			cooldown = false;
			timer_cooldown = 0;
			return false;
		}
		cooldown = true;
		return true;
	}
}

void IceBall::render(RenderWindow &wd) noexcept {
	if (visible) {
		rect_collis->render(wd);
		wd.draw(*sprt);
		HP->changeBar(health);
		HP->render(wd);
	}
}

void IceBall::render(RenderWindow *wd) noexcept {
	if (visible) {
		rect_collis->render(wd);
		wd->draw(*sprt);
		HP->changeBar(health);
		HP->render(wd);
	}
}
//--------------------------------------Ледяной-шар-IceBall-Конец------------------------------------------

//--------------------------------База-Характера-BaseCharacter-Начало------------------------------------------
BaseCharacter::BaseCharacter() :
	pos(axes_f(0, 0)),
	health(0),
	visible(true),
	cooldown(false),
	isDead(false),
	zeroing(false),
	timer_cooldown(0),
	frame(0)
	{
	sprt = new Sprite;
}

BaseCharacter::BaseCharacter(Texture* ptr_texture, float x, float y, int _hp) :
	pos(x, y),
	health(_hp),
	visible(true),
	cooldown(false),
	isDead(false),
	zeroing(false),
	timer_cooldown(0),
	frame(0)
	{
	sprt = new Sprite;
	sprt->setTexture(*ptr_texture);
}

BaseCharacter::BaseCharacter(Texture* ptr_texture, const axes_f &xy, int _hp) :
	pos(xy),
	health(_hp),
	visible(true),
	cooldown(false),
	isDead(false),
	zeroing(false),
	timer_cooldown(0),
	frame(0)
	{
	sprt = new Sprite;
	sprt->setTexture(*ptr_texture);
}

BaseCharacter::~BaseCharacter() {
	delete sprt;
}

axes_f BaseCharacter::getPosition() {
	return pos;
}

FloatRect BaseCharacter::getSize() {
	return fl_rect;
}

void BaseCharacter::setPosition(float x, float y) {
	pos.x = x;
	pos.y = y;
}

void BaseCharacter::setPosition(const axes_f &xy) {
	pos = xy;
}

void BaseCharacter::render(RenderWindow &wd) {
	if (visible) {
		wd.draw(*sprt);
	}
}

void BaseCharacter::render(RenderWindow *wd) {
	if (visible) {
		wd->draw(*sprt);
	}
}
//--------------------------------База-Характера-BaseCharacter-Конец------------------------------------------

//--------------------------------База-Интерфейса-BaseInterface-Начало------------------------------------------
_interface::BaseInerface::BaseInerface() : 
	pos(axes_i(0, 0)),
	visible(true)
	{

}

_interface::BaseInerface::BaseInerface(int x, int y, const FloatRect &rect) :
	pos(axes_i(x, y)),
	fl_rect(rect),
	visible(true)
	{

}

_interface::BaseInerface::BaseInerface(const axes_i &xy, const FloatRect &rect) :
	pos(xy),
	fl_rect(rect),
	visible(true)
	{

}

_interface::BaseInerface::~BaseInerface() {

}

axes_i _interface::BaseInerface::getPosition() {
	return pos;
}

void _interface::BaseInerface::setPosition(const axes_i &xy) {
	pos = xy;
}

void _interface::BaseInerface::setPosition(int x, int y) {
	pos.x = x;
	pos.y = y;
}

FloatRect _interface::BaseInerface::getSize() {
	return fl_rect;
}

void _interface::BaseInerface::render(RenderWindow &wd) {

}
void _interface::BaseInerface::render(RenderWindow *wd) {

}
//--------------------------------База-Интерфейса-BaseInterface-Конец------------------------------------------

//--------------------------------------Метеорит-Meteor-Начало------------------------------------
Meteor::Meteor(Texture* ptr_texture, Texture* ptr_texture2, float X, float Y) :
	cooldown(false),
	reached_point(false),
	is_sound_play(false),
	current_len(0),
	ObjectAnimated(ptr_texture, X, Y)
{
	sprt_meteor = new Sprite;
	sprt_meteor->setTexture(*ptr_texture2);

	sprt->setTextureRect(IntRect(400, 0, 400, 400));
	sprt->setPosition(pos.x, pos.y);

	mouse_point.x = X + 200;
	mouse_point.y = Y + 200;

	start_point.x = mouse_point.x - 270;
	start_point.y = mouse_point.y - 1440;

	full_len = sqrt(pow(mouse_point.x - start_point.x, 2) + pow(mouse_point.y - start_point.y, 2));
}

Meteor::~Meteor() {
	delete sprt_meteor;
}

void __fastcall Meteor::update(float time) {

	isReachedPoint(time);

	if (reached_point) {
		frame += 0.022 * time;
		if (frame > 24) {
			end = true;
		}

		if (frame >= 0 && frame <= 5) {
			sprt->setTextureRect(IntRect(400 * int(frame), 0, 400, 400));
		} else if (frame >= 6 && frame <= 11) {
			sprt->setTextureRect(IntRect(400 * (int(frame) - 6), 400, 400, 400));
		} else if (frame >= 12 && frame <= 17) {
			sprt->setTextureRect(IntRect(400 * (int(frame) - 12), 800, 400, 400));
		} else if (frame >= 18 && frame <= 23) {
			sprt->setTextureRect(IntRect(400 * (int(frame) - 18), 1200, 400, 400));
		}
		rect_collis->setBounds(IntRect(pos.x, pos.y, 400, 400));
	}
}

void Meteor::isReachedPoint(float time) {
	Vector2f C;

	if (current_len < full_len) {
		C = (mouse_point - start_point);
		C.x = C.x * (current_len / full_len);
		C.y = C.y * (current_len / full_len);
		C = start_point + C;
		sprt_meteor->setPosition(C.x - (sprt_meteor->getGlobalBounds().width / 2), C.y - (sprt_meteor->getGlobalBounds().height / 2));
		current_len += 3.6 * time;
	} else {
		reached_point = true;
	}
}

void Meteor::render(RenderWindow& wd) {
	if (reached_point) {
		wd.draw(*sprt);
	} else {
		wd.draw(*sprt_meteor);
	}
}

void Meteor::render(RenderWindow* wd) {
	if (reached_point) {
		wd->draw(*sprt);
	} else {
		wd->draw(*sprt_meteor);
	}
}
//--------------------------------------Метеорит-Meteor-Конец------------------------------------

//---------------------------------------Вспомогательные-функции-----------------------------------------------

template<typename arg>
arg IntToStr(int input) {
	std::ostringstream output;
	output << input;
	return output.str();
}

template<typename arg>
arg _interface::repoz_X(arg X, arg size, arg indent) {
	arg A = 0;
	A = size - X;
	X = X - A;
	return X + indent;
}

template<typename arg>
arg _interface::repoz_Y(arg Y, arg size, arg indent) {
	arg A = 0;
	A = size - Y;
	Y = Y - A;
	return Y + indent;
}
//-------------------------------------------------------------------------------------------------------------
