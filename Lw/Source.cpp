#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include "Interf.h"
#include <iostream>
#include "MemoryPool.h"
#include <list>
#include <random>

#if defined _WIN32 || defined _WIN64 //Для windows

#include <windows.h>

unsigned int screen_width = GetSystemMetrics(SM_CXSCREEN);
unsigned int screen_height = GetSystemMetrics(SM_CYSCREEN);

#endif
#pragma hdrstop
using namespace sf;
using namespace std;

enum lvlnum {
	lvlRun = 1,
	lvlTraining,
	menuGame,
	exitGame = 0
};

class Game {
	private:
		configuration *config;
		RenderWindow *window;

		uint32_t block_memory_sprite;
		uint32_t block_memory_font;
		uint32_t block_memory_texture;

		void* ptr_global_memory;

		sf::Vector2i pos;
		sf::Vector2f realPos;
		uint32_t CENTER_SCREEN_X;
		uint32_t CENTER_SCREEN_Y;
		int volume; //Громкость звука
		float barhp; //Здоровье
		float barmp; //Текущее значение маны
		float barmp_max; //Максимальное значение маны, нужно для восстановления маны
		int mp_need_cast_expl; //Нужное кол-во маны для взрыва
		int mp_need_cast_ice; //Нужное кол-во маны для призыва кристалла
		
	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Game() {

			config = new configuration;
			if (!config->loadSettings()) {
				config->createSettings(screen_width, screen_height, 8, true, 30, true, true, 50);
			}

			if (config->sound) {
				volume = config->soundVolume;
			} else {
				volume = 0;
			}

			sf::ContextSettings settings_sf;
			settings_sf.antialiasingLevel = config->anisFilt;

			window = new RenderWindow(VideoMode(config->screenWidth, config->screenHeight), "Little World", config->fullScreen ? 8 : 7, settings_sf);
			window->setVerticalSyncEnabled(config->verticalSync);

			ptr_global_memory = memory_block_allocation_void(sizeof(Image) + (sizeof(Texture) * 11) + (sizeof(Sprite) * 11) + sizeof(Font));

			block_memory_texture = sizeof(Image);
			block_memory_sprite = block_memory_texture + (sizeof(Texture) * 11);
			block_memory_font = block_memory_sprite + (sizeof(Sprite) * 11);

			memory_initialization(pointer_cast(ptr_global_memory, Image), Image);

			for (int i = 0; i < 11; i++) {
				memory_initialization(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + i, Texture);
				memory_initialization(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + i, Sprite);
			}

			memory_initialization(pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Font);

			(pointer_cast(ptr_global_memory, Image))->loadFromFile("Img/ground.png");

			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 0)->loadFromFile("Img/exp.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 1)->loadFromFile("Img/meteor.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 2)->loadFromFile("Img/castle.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 3)->loadFromFile("Img/sprman.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 4)->loadFromFile("Img/CastlDestr.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 5)->loadFromFile("Img/gg.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 6)->loadFromFile("Img/ice.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 7)->loadFromFile("Img/World.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 8)->loadFromFile("Img/castd.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 9)->loadFromFile("Img/d1.png");
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + 10)->loadFromFile("Img/castd2.png");

			for (int i = 0; i < 11; i++) {
				(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + i)->setTexture(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + i));
			}

			pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font)->loadFromFile("Img/18094.ttf");

			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->setOrigin((pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->getGlobalBounds().width / 2.f,
																									  (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->getGlobalBounds().height / 2.f);
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->setPosition((config->screenWidth / 2 + 100), config->screenHeight / 2);

			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 8)->setOrigin(((pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 8)->getGlobalBounds().width / 2) + 30,
																									   (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->getOrigin().y + 137);
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 8)->setPosition((config->screenWidth / 2 + 100), config->screenHeight / 2);

			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 10)->setOrigin(((pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 10)->getGlobalBounds().width / 2) + 30,
																									   (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->getOrigin().y + 137);
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 10)->setPosition((config->screenWidth / 2 + 100), config->screenHeight / 2);

			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 9)->setOrigin(((pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 9)->getGlobalBounds().width / 2) - 20,
																									   (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->getOrigin().y + 110);
			(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 9)->setPosition((config->screenWidth / 2 + 100), config->screenHeight / 2);

			CENTER_SCREEN_X = config->screenWidth / 2;
			CENTER_SCREEN_Y = config->screenHeight / 2;

			barhp = 200; 
			barmp_max = 150;
			barmp = barmp_max;
			mp_need_cast_expl = 20;
			mp_need_cast_ice = 100; 

		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		~Game() {
			delete config, window; 

			destructor(pointer_cast(ptr_global_memory, Image), Image);

			for (int i = 0; i < 11; i++) {
				destructor(pointer_cast(((char*)(ptr_global_memory) + block_memory_texture), Texture) + i, Texture);
				destructor(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + i, Sprite);
			}

			destructor(pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Font);

			free_block_memory(ptr_global_memory);
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		int Run() {
			Clock clock;
			float timer = 0;

			_interface::main_menu *main_men = new _interface::main_menu(config, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color(38, 38, 38, 255));
			_interface::settings_menu *st_men = new _interface::settings_menu(config, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color::Color(38, 38, 38, 255), Color::Yellow);
			st_men->blackout_visible = true;

			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			auto random {
				[](__int32 _from, __int32 _do)->__int32 {
					std::random_device rd;
					std::mt19937_64 gen(rd());
					std::uniform_int_distribution<int> dist(_from, _do);
					return dist(gen);
				}
			};

			int rand_i = random(8, 9);

			if (rand_i == 8) {
				(pointer_cast(((char*)(ptr_global_memory)+block_memory_sprite), Sprite) + rand_i)->setRotation((pointer_cast(((char*)(ptr_global_memory)+block_memory_sprite), Sprite) + 9)->getRotation() + 100);
			} else {
				rand_i = 10;
				(pointer_cast(((char*)(ptr_global_memory)+block_memory_sprite), Sprite) + rand_i)->setRotation((pointer_cast(((char*)(ptr_global_memory)+block_memory_sprite), Sprite) + 9)->getRotation() - 100);
			}

			while (window->isOpen()) {

				timer = clock.getElapsedTime().asMicroseconds();
				timer /= 1000;

				pos = sf::Mouse::getPosition(*window);
				realPos = window->mapPixelToCoords(pos);
				Event event;

				while (window->pollEvent(event)) {
					if (event.type == Event::Closed) {
						window->close();
					}

					if (!st_men->active && !message_settings->active) {
						if (main_men->btExit->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								window->close();
							}
						}

						if (main_men->btStart->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								delete main_men, st_men, message_settings;
								return lvlnum::lvlRun;
							}
						}

						if (main_men->btStartTren->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								delete main_men, st_men, message_settings;
								return lvlnum::lvlTraining;
							}
						}

						if (main_men->btOptions->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->active = true;
							}
						}

					} else if (!message_settings->active) {
						if (st_men->btBack->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->active = false;
								st_men->backSettings(config);
							}
						}

						if (st_men->cbFullS->getSize().contains(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->cbFullS->invers();
							}
						}

						if (st_men->cbVertS->getSize().contains(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->cbVertS->invers();
							}
						}

						if (st_men->cbSound->getSize().contains(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->cbSound->invers();
							}
						}

						if (st_men->btSave->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								if (st_men->saveSettings(config) == static_cast<int>(settings_save_code::other)) {
									message_settings->active = true;
								} else {
									st_men->active = false;
								}

								window->setVerticalSyncEnabled(config->verticalSync);

								if (config->sound) {
									volume = config->soundVolume;
								} else {
									volume = 0;
								}
							}
						}

						if (st_men->combAnisF->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->combAnisF->next();
							} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
								st_men->combAnisF->back();
							}
						}

						if (st_men->combScreen->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->combScreen->next();
							} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
								st_men->combScreen->back();
							}
						}

						if (st_men->combSoundV->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->combSoundV->next();
							} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
								st_men->combSoundV->back();
							}
						}

					} else {
						if (message_settings->btOk->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								message_settings->active = false;
								st_men->active = false;
							}
						}
					}
				}

				if (timer >= 1.f) {
					(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7)->rotate(0.01); //Planet->sprt->rotate(0.01);
					(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + rand_i)->rotate(0.17); //D1->sprt->rotate(0.17);
					(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 9)->rotate(0.17); //Castd->sprt->rotate(0.17);
					clock.restart();
				}

				window->clear();
				main_men->render(*window);
				window->draw(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + rand_i));
				window->draw(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 9));
				window->draw(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 7));
				st_men->render(*window);
				message_settings->render(*window);
				window->display();
			}
			
			delete main_men, st_men, message_settings;
			return lvlnum::exitGame;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		int LvlRun() {
			Clock clock;
			World *MainWrd;
			_interface::bar *HP;
			_interface::bar *MP;
			_interface::multiline_text *mlt;
			_interface::text *lvlInfo;
			Camer *Camera;

			ObjectStatic *Castle;
			_interface::menu *men;
			list<Meteor*> *Expl_list = new list<Meteor*>;

			list<Character*> *Pers = new list<Character*>;
			list<DestroerCastle*> *DC = new list<DestroerCastle*>;
			list<Spearman*> *Spman = new list<Spearman*>;
			list<IceBall*> *Ice = new list<IceBall*>;

			MainWrd = new World((pointer_cast(ptr_global_memory, Image)), 40, 60);
			mlt = new _interface::multiline_text(100, 200, Color::Black, Color::Yellow);
			mlt->resize(_interface::text_size::normal);
			Camera = new Camer(CENTER_SCREEN_X, CENTER_SCREEN_Y, config->screenWidth, config->screenHeight);

			Castle = new ObjectStatic(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 2), 0, 0); //Замок
			Castle->sprite_rect = IntRect(0, 0, 400, 1500);
			Castle->rect_collis->setBounds(IntRect(0, 0, 400, 1500));
			int numlvl = 0; //Номер волны
			bool change_numlvl = false; //Смена волны
			bool cooldown_expl = false; //Кулдаун для метеорита

			float timer; //Глобальное время

			HP = new _interface::bar(r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), 5, 5, barhp, 0, L"HP:", Color::White, Color::Red, Color::Black); //Полоса здоровья
			MP = new _interface::bar(r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), 5, HP->getSize().left + HP->getSize().width + 5, barmp_max, 0, L"MP:", Color::White, Color::Blue, Color::Black); //Полоса маны

			_interface::message *message_end = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Захватчики разрушили ваш замок, вы проиграли", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_vic = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Вы победили всех нападающих противников! Поздравляем!", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			_interface::settings_menu *st_men = new _interface::settings_menu(config, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color::Color(38, 38, 38, 255), Color::Yellow); //Меню настроек
			men = new _interface::menu(Camera, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color::Color(38, 38, 38, 255), Color::Yellow); //Главное меню
			men->active = false;
			st_men->blackout_visible = false;
			
			lvlInfo = new _interface::text(r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), 0, 0, L"Волна 1", Color::Black, Color::Yellow); //Надпись с номером волны
			lvlInfo->resize(_interface::text_size::medium);
			lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);
			

			mlt->add(L"Тревога!");
			mlt->add(L"На наш замок нападают");

			float Second = 0; //Накапливает время в секундах
			float NacopSec = 0; //Накапливает время в секундах
			float NacopSec2 = 0; //Накапливает время в секундах

//------------------------------------------------------------------------------------------------------------------------Звуки и музыка
			SoundBuffer shootBuffer; //Создаём буфер для звука																		
			shootBuffer.loadFromFile("Sounds/expls.ogg"); //Загружаем в него звук													
			Sound expl_sound(shootBuffer); //Создаем звук и загружаем в него звук из буфера												
			
			expl_sound.setVolume(volume);

			SoundBuffer game_overBuffer;
			game_overBuffer.loadFromFile("Sounds/game_over.ogg");
			Sound game_over_sound(game_overBuffer);

			game_over_sound.setVolume(volume);

			SoundBuffer victoryBuffer;
			victoryBuffer.loadFromFile("Sounds/victory.ogg");
			Sound victory_sound(victoryBuffer);

			victory_sound.setVolume(volume);

			SoundBuffer attackBuffer;
			attackBuffer.loadFromFile("Sounds/attack.ogg");
			list<Sound*> *attack_sound = new list<Sound*>;
			
			SoundBuffer attackIceBuffer;
			attackIceBuffer.loadFromFile("Sounds/attack_ice.ogg");
			list<Sound*> *attack_ice_sound = new list<Sound*>;
		
			Music *music_main_theme = new Music; //Создаем объект музыки																
			music_main_theme->openFromFile("Sounds/main_theme.ogg"); //Загружаем файл		
			music_main_theme->setVolume(volume);
			music_main_theme->play();																								
//------------------------------------------------------------------------------------------------------------------------------------
			
			while (window->isOpen()) {
				
				music_main_theme->setLoop(true);

				timer = clock.getElapsedTime().asMicroseconds(); //Получаем время в микросек
				Second = clock.getElapsedTime().asSeconds(); //Получаем время в секундах
				clock.restart();
				timer /= 1000;
				//timer -= 10;
				pos = sf::Mouse::getPosition(*window);
				realPos = window->mapPixelToCoords(pos);
				Event event;
				while (window->pollEvent(event)) {
					if (event.type == Event::Closed) {
						window->close();
					}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Меню и его обработчики событий
					if (!message_end->active && !message_vic->active) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) { //Если нажата левая кнопка мыши
							if (barmp >= mp_need_cast_expl) {
								Expl_list->push_back(new Meteor(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 0), realPos.x - 200, realPos.y - 200));
								barmp -= mp_need_cast_expl;
							}
						}

						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right && !men->active) { //Если нажата правая кнопка мыши
							if (barmp >= mp_need_cast_ice) {
								if (Ice->empty()) {
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 200));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 200));
								}
								barmp -= mp_need_cast_ice;
							}
						}

						if (men->active && !st_men->active) { //Меню паузы
							if (men->btContinue->isAction(realPos.x, realPos.y)) { //Если нажата кнопка продолжить 
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									men->active = false;
								}
							}

							if (men->btOptions->isAction(realPos.x, realPos.y)) { //Если нажата кнопка настройки
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = true;
								}
							}

							if (men->btExit->isAction(realPos.x, realPos.y)) { //Если нажата кнопка выйти
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
										delete* it_expl;
										it_expl = Expl_list->erase(it_expl);
									}
									delete Expl_list;
									delete men, st_men;
									delete HP, MP;
									delete mlt;
									delete lvlInfo;
									for (list<Sound*>::iterator it_sd_ice = attack_ice_sound->begin(); it_sd_ice != attack_ice_sound->end();) {
										delete* it_sd_ice;
										it_sd_ice = attack_ice_sound->erase(it_sd_ice);
									}
									for (list<Sound*>::iterator it_sd = attack_sound->begin(); it_sd != attack_sound->end();) {
										delete* it_sd;
										it_sd = attack_sound->erase(it_sd);
									}
									for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
										delete* it_sp;
										it_sp = Spman->erase(it_sp);
									}
									for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
										delete* it_p;
										it_p = Pers->erase(it_p);
									}
									for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
										delete* it_dc;
										it_dc = DC->erase(it_dc);
									}
									for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
										delete* it_ic;
										it_ic = Ice->erase(it_ic);
									}
									delete Pers, DC, Spman, Ice;
									delete MainWrd;
									delete message_settings, message_end, message_vic;
									delete Camera;
									delete music_main_theme, attack_sound, attack_ice_sound;
									return lvlnum::menuGame;
								}
							}
						} else if (st_men->active && !message_settings->active) { //Меню настроек
							if (st_men->btBack->isAction(realPos.x, realPos.y)) { //Если нажата кнопка назад
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = false;
									st_men->backSettings(config);
								}
							}

							if (st_men->cbFullS->getSize().contains(realPos.x, realPos.y)) { //Если нажат чекбокс на весь экран
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbFullS->invers();
								}
							}

							if (st_men->cbVertS->getSize().contains(realPos.x, realPos.y)) { //Если нажат чекбокс верикальная синхронизация
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbVertS->invers();
								}
							}

							if (st_men->cbSound->getSize().contains(realPos.x, realPos.y)) { //Если нажат чекбокс звука
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbSound->invers();
								}
							}

							if (st_men->btSave->isAction(realPos.x, realPos.y)) { //Если нажата кнопка сохранить
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									if (st_men->saveSettings(config) == static_cast<int>(settings_save_code::other)) {
										message_settings->active = true;
									} else {
										st_men->active = false;
									}	

									window->setVerticalSyncEnabled(config->verticalSync);

									if (config->sound) {
										volume = config->soundVolume;
									} else {
										volume = 0;
									}
									music_main_theme->setVolume(volume);
								}
							}

							if (st_men->combAnisF->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->combAnisF->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
									st_men->combAnisF->back();
								}
							}

							if (st_men->combScreen->isAction(realPos.x, realPos.y)) { 
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) { //Если нажата левая кнопка мыши на комбобоксе разрешение экрана
									st_men->combScreen->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) { //Если нажата правая кнопка мыши на комбобоксе разрешение экрана
									st_men->combScreen->back();
								}
							}
						
							if (st_men->combSoundV->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) { //Если нажата левая кнопка мыши на громкости звука
									st_men->combSoundV->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) { //Если нажата правая кнопка мыши на громкости звука
									st_men->combSoundV->back();
								}
							}

						} else {
							if (message_settings->btOk->isAction(realPos.x, realPos.y)) { //Если нажата кнопка ок в сообщении
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									message_settings->active = false;
									st_men->active = false;
								}
							}
						}
					} else {
						if (message_end->btOk->isAction(realPos.x, realPos.y) && message_end->active) { //Если нажата кнопка ок в сообщении о проигрыше
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
									delete* it_expl;
									it_expl = Expl_list->erase(it_expl);
								}
								delete Expl_list;
								delete men, st_men;
								delete HP, MP;
								delete mlt;
								delete lvlInfo;
								for (list<Sound*>::iterator it_sd_ice = attack_ice_sound->begin(); it_sd_ice != attack_ice_sound->end();) {
									delete* it_sd_ice;
									it_sd_ice = attack_ice_sound->erase(it_sd_ice);
								}
								for (list<Sound*>::iterator it_sd = attack_sound->begin(); it_sd != attack_sound->end();) {
									delete* it_sd;
									it_sd = attack_sound->erase(it_sd);
								}
								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									delete* it_sp;
									it_sp = Spman->erase(it_sp);
								}
								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									delete* it_p;
									it_p = Pers->erase(it_p);
								}
								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									delete* it_dc;
									it_dc = DC->erase(it_dc);
								}
								for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
									delete* it_ic;
									it_ic = Ice->erase(it_ic);
								}
								delete Pers, DC, Spman, Ice;
								delete MainWrd;
								delete message_settings, message_end, message_vic;
								delete Camera;
								delete music_main_theme, attack_sound, attack_ice_sound;
								return lvlnum::menuGame;
							}
						}

						if (message_vic->btOk->isAction(realPos.x, realPos.y) && message_vic->active) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
									delete* it_expl;
									it_expl = Expl_list->erase(it_expl);
								}
								delete Expl_list;
								delete men, st_men;
								delete HP, MP;
								delete mlt;
								delete lvlInfo;
								for (list<Sound*>::iterator it_sd_ice = attack_ice_sound->begin(); it_sd_ice != attack_ice_sound->end();) {
									delete* it_sd_ice;
									it_sd_ice = attack_ice_sound->erase(it_sd_ice);
								}
								for (list<Sound*>::iterator it_sd = attack_sound->begin(); it_sd != attack_sound->end();) {
									delete* it_sd;
									it_sd = attack_sound->erase(it_sd);
								}
								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									delete* it_sp;
									it_sp = Spman->erase(it_sp);
								}
								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									delete* it_p;
									it_p = Pers->erase(it_p);
								}
								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									delete* it_dc;
									it_dc = DC->erase(it_dc);
								}
								for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
									delete* it_ic;
									it_ic = Ice->erase(it_ic);
								}
								delete Pers, DC, Spman, Ice;
								delete MainWrd;
								delete message_settings, message_end, message_vic;
								delete Camera;
								delete music_main_theme, attack_sound, attack_ice_sound;
								return lvlnum::menuGame;
							}
						}
					}

					if (window->isOpen()) { break; }
				}

				if (!message_end->active) {
					if (Keyboard::isKeyPressed(Keyboard::Escape)) { //Выход
						men->active = true;
					}

					if (change_numlvl) {
						switch (numlvl) {
						case 1: //Первая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 2");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 920, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 950, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 740, 990, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 920, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 905, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 950, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 740, 400, 45));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 180, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 260, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 340, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 420, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 580, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 660, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 740, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 920, 190, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 905, 280, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 900, 370, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 460, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 950, 550, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 740, 640, 45));
							}

							change_numlvl = false;
							break;
						case 2: //Вторая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 3");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 4; //mp_need_cast_expl = 16

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 1200, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 920, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 20, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 70, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 120, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 220, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 270, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 320, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 370, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 420, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 470, 45));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 180, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 260, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 340, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 420, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 580, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 660, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 180, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 260, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 340, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 420, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 580, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 660, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 170, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 240, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 310, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 380, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 520, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 590, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 660, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 730, 45));
							}

							change_numlvl = false;
							break;
						case 3: //Третья волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 4");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 4; //mp_need_cast_expl = 12

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 1000, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 920, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 90, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 130, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 210, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 290, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 330, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 370, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 410, 45));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 500, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 550, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1910, 550, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 600, 45));
							}

							change_numlvl = false;
							break;
						case 4: //Четвертая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 5");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 2; //mp_need_cast_expl = 10

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 1200, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 120, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 240, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 410, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 550, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 810, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 930, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 970, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 1100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 110, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 220, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 405, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 575, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 809, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 915, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1890, 970, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 2000, 1005, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1950, 1130, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 2100, 600, 650));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 400, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 90, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 130, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 210, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1890, 390, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 2000, 430, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1950, 450, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 2100, 400, 650));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1905, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1900, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1908, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1920, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1890, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 2000, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1950, 500, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 2100, 450, 650));
							}
		
							change_numlvl = false;
							break;
						case 5: //Пятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 6");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 85

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 57, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 95, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 230, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 1200, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 230, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 560, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 920, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 990, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 1000, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 750, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 1050, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 1200, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1870, 700, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 57, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 95, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 230, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 400, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 450, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1870, 450, 500));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 57, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 95, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 230, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 600, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1300, 500, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1870, 500, 500));
							}

							change_numlvl = false;
							break;
						case 6: //Шестая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 7");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 1; //mp_need_cast_expl = 9
							
							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 750, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 1050, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 1200, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1500, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1700, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 750, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 1050, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1500, 1200, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 600, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1500, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1700, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 450, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 300, 500));
							} else { //Если любое другое
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1000, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1050, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1100, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1150, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1200, 550, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1500, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1700, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1650, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1600, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1550, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 1500, 500, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1900, 400, 500));
							}

							change_numlvl = false;
							break;
						case 7: //Седьмая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 8");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 70

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 750, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 1050, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 1200, 45));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 1000, 700, 1000));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 450, 45));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 1000, 250, 1000));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 110, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 115, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 120, 500, 45));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 1000, 400, 1000));
							}

							change_numlvl = false;
							break;
						case 8: //Восьмая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 9");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);
							
							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 650, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 850, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 650, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 850, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 1050, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 300, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 350, 600, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 800, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 300, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 350, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 300, 200, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 350, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 400, 500));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 105, 550, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 305, 550, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 300, 100, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 350, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 500, 500));
							}

							change_numlvl = false;
							break;
						case 9: //Девятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Последняя волна");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 100, 700, 1000));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 500, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 500, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 570, 900, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 100, 200, 1000));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 500, 150, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 200, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 570, 250, 500));
							} else { //Если любое другое
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 100, 300, 1000));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 500, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 700, 400, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 570, 600, 500));
							}

							change_numlvl = false;
							break;
						case 10: //Десятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Победа!");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 55
							mp_need_cast_expl -= 1; //mp_need_cast_expl = 8

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //Если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 400, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 400, 700, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 1000, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 400, 1100, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 800, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 500, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 600, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 800, 700, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 800, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 900, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 1000, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 800, 1100, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1300, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1400, 800, 500));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2000, 100, 1000));
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2700, 700, 1000));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //Если разрешение 1280 х 720
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 400, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 400, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 400, 450, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 800, 50, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 800, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 800, 450, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1300, 100, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1400, 400, 500));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2000, 50, 1000));
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2700, 300, 1000));
							} else { //Если любое другое
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140, 500, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 400, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 400, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 400, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 400, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 400, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 400, 500, 45));

								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 130 + 800, 100, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 150, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 200, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 250, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 10 + 800, 300, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 40 + 800, 350, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 70 + 800, 400, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 100 + 800, 450, 45));
								Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 140 + 800, 500, 45));

								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1300, 300, 500));
								Pers->push_back(new Character(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5), config->screenWidth + 1400, 500, 500));

								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2000, 100, 1000));
								DC->push_back(new DestroerCastle(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4), config->screenWidth + 2700, 560, 1000));
							}

							change_numlvl = false;
							break;
						default:
							lvlInfo->visible = false;
							message_vic->active = true;
							change_numlvl = false;
							break;
						}
					}

					if (Spman->empty() && Pers->empty() && DC->empty()) {
						NacopSec2 += Second;
						lvlInfo->visible = true;
						if (NacopSec2 >= 3) {
							numlvl++;
							change_numlvl = true;
							barmp = barmp_max;
						}

						if (numlvl == 11) {
							music_main_theme->stop();
							victory_sound.setVolume(volume);
							victory_sound.play();
							numlvl++;
						}
					} else {
						NacopSec2 = 0;
					}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Регистрация попаданий
					if (!men->active) {

						for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
							cooldown_expl = false;
							if ((*it_expl)->end) {
								delete* it_expl;
								it_expl = Expl_list->erase(it_expl);
							} else {

								if ((*it_expl)->reached_point && !(*it_expl)->is_sound_play) {
									expl_sound.setVolume(volume);
									expl_sound.play();
									(*it_expl)->is_sound_play = true;
								}

								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects((*it_sp)->rect_collis->getBounds()) && !(*it_expl)->cooldown) {
										cooldown_expl = true;
										(*it_sp)->health -= 30;
										if ((*it_sp)->health <= 0) { (*it_sp)->health = 0; }
									}
									it_sp++;
								}

								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects((*it_dc)->rect_collis->getBounds()) && !(*it_expl)->cooldown) {
										cooldown_expl = true;
										(*it_dc)->health -= 25;
										if ((*it_dc)->health <= 0) { (*it_dc)->health = 0; }
									}
									it_dc++;
								}

								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects((*it_p)->rect_collis->getBounds()) && !(*it_expl)->cooldown) {
										cooldown_expl = true;
										(*it_p)->health -= 25;
										if ((*it_p)->health <= 0) { (*it_p)->health = 0; }
									}
									it_p++;
								}

								(*it_expl)->update(timer);

								if (cooldown_expl) {
									(*it_expl)->cooldown = true;
								}
								it_expl++;
							}

						}

						bool Ice_ball_is_cooldown = false;
						for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
							if ((*it_ic)->isDead) {
								delete* it_ic;
								it_ic = Ice->erase(it_ic);
							} else {

								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									if ((*it_ic)->rect_collis->getBounds().intersects((*it_sp)->rect_collis->getBounds())) {
										if (!(*it_ic)->cooldown) {
											(*it_sp)->health -= 5;
											Ice_ball_is_cooldown = true;
											//(*it_ic)->cooldown = true;
										}
									}
									it_sp++;
								}

								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									if ((*it_ic)->rect_collis->getBounds().intersects((*it_dc)->rect_collis->getBounds())) {
										if (!(*it_ic)->cooldown) {
											(*it_dc)->health -= 5;
											Ice_ball_is_cooldown = true;
											//(*it_ic)->cooldown = true;
										}
									}
									it_dc++;
								}

								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									if ((*it_ic)->rect_collis->getBounds().intersects((*it_p)->rect_collis->getBounds())) {
										if (!(*it_ic)->cooldown) {
											(*it_p)->health -= 5;
											Ice_ball_is_cooldown = true;
											//(*it_ic)->cooldown = true;
										}
									}
									it_p++;
								}

								if (Ice_ball_is_cooldown) {
									(*it_ic)->cooldown = true;
								}

								if (!(*it_ic)->isCooldown(Second)) {
									Ice_ball_is_cooldown = false;
									(*it_ic)->cooldown = false;
								}

								(*it_ic)->update(timer / 4.5);
								it_ic++;
							}
						}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Логика поведения врагов

						for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
							if ((*it_sp)->isDead) {
								delete* it_sp;
								it_sp = Spman->erase(it_sp);
							} else {

								if ((*it_sp)->rect_collis->getBounds().intersects(Castle->rect_collis->getBounds()) && (*it_sp)->health != 0) {
									(*it_sp)->attack(timer / 2);
									if (!(*it_sp)->isCooldown(Second)) {
										barhp -= 3;
										(*it_sp)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->setVolume(volume);
										attack_sound->back()->play();
									}
								} else {

									if (Ice->empty()) {
										(*it_sp)->move(timer / 2, direcrion8::left);
									} else {
										if ((*it_sp)->rect_collis->getBounds().intersects(Ice->back()->rect_collis->getBounds()) && Ice->back()->health != 0) {
											(*it_sp)->attack(timer / 2);
											if (!(*it_sp)->isCooldown(Second)) {
												Ice->back()->health -= 3;
												(*it_sp)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
												attack_ice_sound->back()->setVolume(volume);
												attack_ice_sound->back()->play();
											}
										} else {
											(*it_sp)->move(timer / 2, direcrion8::left);
										}
									}	
								}
								it_sp++;
							}
						}

						for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
							if ((*it_p)->isDead) {
								delete* it_p;
								it_p = Pers->erase(it_p);
							} else {

								if ((*it_p)->rect_collis->getBounds().intersects(Castle->rect_collis->getBounds()) && (*it_p)->health != 0) {
									(*it_p)->attack(timer / 1.5);
									if (!(*it_p)->isCooldown(Second)) {
										barhp -= 10;
										(*it_p)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->setVolume(volume);
										attack_sound->back()->play();
									}
								} else {
									if (Ice->empty()) {
										(*it_p)->move(timer / 3.5, direcrion8::left);
									} else {
										if ((*it_p)->rect_collis->getBounds().intersects(Ice->back()->rect_collis->getBounds()) && Ice->back()->health != 0) {
											(*it_p)->attack(timer / 2);
											if (!(*it_p)->isCooldown(Second)) {
												Ice->back()->health -= 10;
												(*it_p)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
												attack_ice_sound->back()->setVolume(volume);
												attack_ice_sound->back()->play();
											}
										} else {
											(*it_p)->move(timer / 3.5, direcrion8::left);
										}
									}
									
								}

								it_p++;
							}
						}

						for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
							if ((*it_dc)->isDead) {
								delete* it_dc;
								it_dc = DC->erase(it_dc);
							} else {
								if ((*it_dc)->rect_collis->getBounds().intersects(Castle->rect_collis->getBounds()) && (*it_dc)->health != 0) {
									(*it_dc)->attack(timer / 4.2);
									if (!(*it_dc)->isCooldown(Second)) {
										barhp -= 20;
										(*it_dc)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->setVolume(volume);
										attack_sound->back()->play();
									}
								} else {
									if (Ice->empty()) {
										(*it_dc)->move(timer / 3.5, direcrion8::left);
									} else {
										if ((*it_dc)->rect_collis->getBounds().intersects(Ice->back()->rect_collis->getBounds()) && Ice->back()->health != 0) {
											(*it_dc)->attack(timer / 2);
											if (!(*it_dc)->isCooldown(Second)) {
												Ice->back()->health -= 20;
												(*it_dc)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
												attack_ice_sound->back()->setVolume(volume);
												attack_ice_sound->back()->play();
											}
										} else {
											(*it_dc)->move(timer / 3.5, direcrion8::left);
										}
									}
								}
								it_dc++;
							}
						}

						for (list<Sound*>::iterator it_sd = attack_sound->begin(); it_sd != attack_sound->end();) {
							if (!(*it_sd)->getStatus()) {
								delete* it_sd;
								it_sd = attack_sound->erase(it_sd);
							} else {
								it_sd++;
							}
						}

						if (NacopSec2 >= 3) {
							mlt->visible = false;
						}

						HP->changeBar(barhp);
						HP->freeze(Camera, 5, 5);
						if (barhp <= 125) {
							barmp += timer / 100;
							Castle->sprite_rect = IntRect(400, 0, 400, 1500);
						} if (barhp <= 75) {
							Castle->sprite_rect = IntRect(400 * 2, 0, 400, 1500);
						} if (barhp <= 0) {
							Castle->sprite_rect = IntRect(400 * 3, 0, 400, 1500);
							barhp = 0;
							music_main_theme->stop();
							game_over_sound.play();
							message_end->active = true;
						}
						barmp >= barmp_max ? barmp = barmp_max : barmp += timer / 100;
						MP->changeBar(barmp);
						MP->freeze(Camera, 5, 40);
					}
				}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Отрисовка
				window->clear();
				MainWrd->render(*window);
				Camera->setView(*window);
				for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
					(*it_sp)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3));
					it_sp++;
				}

				for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
					(*it_dc)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 4));
					it_dc++;
				}
				for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
					(*it_p)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 5));
					it_p++;
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6));
					it_ic++;
				}
				for (list<Meteor*>::iterator it_met = Expl_list->begin(); it_met != Expl_list->end();) {
					(*it_met)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 0), (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 1));
					it_met++;
				}
				Castle->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 2));
				HP->render(*window);
				MP->render(*window);
				mlt->render(*window);
				lvlInfo->render(*window);
				men->render(*window, Camera);
				st_men->render(*window, Camera);
				message_settings->render(*window, Camera); //Camer
				message_end->render(*window, Camera); //Camer
				message_vic->render(*window, Camera);
				window->display();
			}

			for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
				delete* it_expl;
				it_expl = Expl_list->erase(it_expl);
			}
			delete Expl_list;
			delete men, st_men;
			delete HP, MP;
			delete mlt;
			delete lvlInfo;
			for (list<Sound*>::iterator it_sd_ice = attack_ice_sound->begin(); it_sd_ice != attack_ice_sound->end();) {
				delete* it_sd_ice;
				it_sd_ice = attack_ice_sound->erase(it_sd_ice);
			}
			for (list<Sound*>::iterator it_sd = attack_sound->begin(); it_sd != attack_sound->end();) {
				delete* it_sd;
				it_sd = attack_sound->erase(it_sd);
			}
			for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
				delete *it_sp;
				it_sp = Spman->erase(it_sp);
			}
			for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
				delete *it_p;
				it_p = Pers->erase(it_p);
			}
			for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
				delete *it_dc;
				it_dc = DC->erase(it_dc);
			}
			for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
				delete* it_ic;
				it_ic = Ice->erase(it_ic);
			}
			delete Pers, Castle, DC, Spman, Ice;
			delete MainWrd;
			delete message_settings, message_end, message_vic;
			delete Camera;
			delete music_main_theme, attack_sound;

			return lvlnum::exitGame;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		int lvlTraining() {
			Clock clock;
			World *MainWrd;
			_interface::bar *HP;
			_interface::bar *MP;
			Camer *Camera;
			bool EnemyActive = false;

			auto *backgroundcl = new _interface::background_color(0, 0, Color(0, 0, 0, 200), config);
			backgroundcl->visible = true;

			auto *mlt = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mlt->add(L"                           Добро пожаловать!");
			mlt->add(L"Это тренировочный уровень, здесь вам расскажут");
			mlt->add(L"и покажут основные принципы и механику игры.");
			mlt->add(L"Если вы хотите его пропустить, то нажмите ENTER, а если");
			mlt->add(L"хотите продолжить нажмите левую кнопку мыши.");
			mlt->resize(_interface::text_size::normal);

			auto *mltAboutHP = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutHP->add(L"HP - health points(с англ. очки здоровья)");
			mltAboutHP->add(L"Этот идикатор отображает количество ваших жизней,");
			mltAboutHP->add(L"когда полоса индикатора дойдет до конца, ваш замок");
			mltAboutHP->add(L"будет разрушен, и, следовательно, вы проиграете.");
			mltAboutHP->add(L"Старайтесь этого не допускать.");
			mltAboutHP->add(L"Нажмите левую кнопку мыши чтобы продолжить");
			mltAboutHP->visible = false;
			mltAboutHP->resize(_interface::text_size::normal);

			auto *mltAboutMP = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutMP->add(L"MP - mana points(с англ. очки маны)");
			mltAboutMP->add(L"Мана нужна для того чтобы вы могли призывать различные");
			mltAboutMP->add(L"разрушительные заклинания, способные навредить противникам.");
			mltAboutMP->add(L"В отличии от HP мана сама со временем восстанавливается.");
			mltAboutMP->add(L"Если индикатор маны дойдет до конца, то вы утратите способность");
			mltAboutMP->add(L"призывать заклинания, пока мана не восстановится до необходимого");
			mltAboutMP->add(L"заклинанию количества очков.");
			mltAboutMP->add(L"Нажмите левую кнопку мыши чтобы продолжить");
			mltAboutMP->visible = false;
			mltAboutMP->resize(_interface::text_size::normal);

			auto *mltAboutEnemy = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutEnemy->add(L"А вот и наш первый противник!");
			mltAboutEnemy->add(L"Под противником находится полоса отображающая его здоровье.");
			mltAboutEnemy->add(L"Старайся уничтожить его до того как он доберется до замка.");
			mltAboutEnemy->add(L"Наведите курсор на противника и нажмите на левую кнопку мыши");
			mltAboutEnemy->visible = false;
			mltAboutEnemy->resize(_interface::text_size::normal);

			auto *mltAboutIce = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutIce->add(L"                                    Хорошо!");
			mltAboutIce->add(L"Но в настоящем бою одним только метеором не обойдешься,");
			mltAboutIce->add(L"нажмите на правую кнопку мыши в любом месте на карте.");
			mltAboutIce->visible = false;
			mltAboutIce->resize(_interface::text_size::normal);

			auto *mltAboutIce2 = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutIce2->add(L"Это ледяной кристалл");
			mltAboutIce2->add(L"Он будет принимать удар противников, подошедших слишком близко,");
			mltAboutIce2->add(L"на себя, при этом нанося небольшое количество урона в ответ.");
			mltAboutIce2->add(L"Кристалл стоит очень много очков маны, и, к тому же, вы можете");
			mltAboutIce2->add(L"поставить только один кристалл за раз, если предыдущий не был");
			mltAboutIce2->add(L"разрущен, то при создании нового, старый разрушется.");
			mltAboutIce2->add(L"Попробуйте поставить ещё один кристалл.");
			mltAboutIce2->visible = false;
			mltAboutIce2->resize(_interface::text_size::normal);

			auto *mltAboutIce3 = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltAboutIce3->add(L"Как и говорилось первый кристалл разрушился");
			mltAboutIce3->add(L"и больше не способен ни на что.");
			mltAboutIce3->add(L"Старайтесь ставить кристалл напротив сильных противников");
			mltAboutIce3->add(L"и если есть такая возможность, то дальше от замка.");
			mltAboutIce3->add(L"Нажмите левую кнопку мыши чтобы продолжить");
			mltAboutIce3->visible = false;
			mltAboutIce3->resize(_interface::text_size::normal);

			auto *mltEnd = new _interface::multiline_text(700, 340, Color::Black, Color::Yellow);
			mltEnd->add(L"                                   Отлично!");
			mltEnd->add(L"Вы прошли обучение и теперь можете перейти в меню!");
			mltEnd->add(L"Нажмите правую кнопку мыши чтобы продолжить");
			mltEnd->visible = false;
			mltEnd->resize(_interface::text_size::normal);

			ObjectStatic *Castle;
			_interface::menu *men;
			list<Meteor*> *Expl_list;
			list<Spearman*> *Spman = new list<Spearman*>;

			list<IceBall*> *Ice = new list<IceBall*>;

			Trigger *triggerForEnemy = new Trigger(IntRect(config->screenWidth - 500, 0, 20, config->screenHeight));

			MainWrd = new World(pointer_cast(ptr_global_memory, Image), 60, 60);
			HP = new _interface::bar(r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), 5, 5, 100, 0, L"HP:", Color::White, Color::Red, Color::Black);
			MP = new _interface::bar(r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), 5, HP->getSize().left + HP->getSize().width + 5, 100, 0, L"MP:", Color::White, Color::Blue, Color::Black);
			Camera = new Camer(CENTER_SCREEN_X, CENTER_SCREEN_Y, config->screenWidth, config->screenHeight);

			Castle = new ObjectStatic(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 2), 0, 0);
			Castle->sprite_rect = IntRect(0, 0, 400, 1500);
			Castle->rect_collis->setBounds(IntRect(0, 0, 400, 1500));
			//int sd = 0;
			float timer;
			//float barhp = 100;
			//flbarmp = 100;

			_interface::message *message_end = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Замок разрушен, вы проиграли", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			_interface::settings_menu *st_men = new _interface::settings_menu(config, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color::Color(38, 38, 38, 255), Color::Yellow);
			men = new _interface::menu(Camera, r_pointer_cast(((char*)(ptr_global_memory) + block_memory_font), Font), Color::Color(38, 38, 38, 255), Color::Yellow);
			men->active = false;
			st_men->blackout_visible = false;
			Expl_list = new list<Meteor*>;

			float Second = 0;

			SoundBuffer shootBuffer; //Создаём буфер для звука
			shootBuffer.loadFromFile("Sounds/expls.ogg"); //Загружаем в него звук
			Sound expl_sound(shootBuffer); //Создаем звук и загружаем в него звук из буфера
			expl_sound.setVolume(volume);

			while (window->isOpen()) {

				timer = clock.getElapsedTime().asMicroseconds(); //Получаем время в микросек
				Second = clock.getElapsedTime().asSeconds();
				clock.restart();
				timer /= 1000;
				pos = sf::Mouse::getPosition(*window);
				realPos = window->mapPixelToCoords(pos);
				Event event;
				while (window->pollEvent(event)) {
					if (event.type == Event::Closed) {
						window->close();
					}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Меню и его обработчики событий

					if (!mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible && !mltAboutEnemy->visible && !mltAboutIce->visible && !mltAboutIce2->visible && !mltAboutIce3->visible && mltEnd->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
								delete* it_expl;
								it_expl = Expl_list->erase(it_expl);
							}
							delete Expl_list;
							delete men, st_men;
							delete HP, MP;
							delete mlt, mltAboutHP, mltAboutMP, mltAboutEnemy, mltAboutEnemy, mltAboutIce, mltAboutIce2, mltAboutIce3, mltEnd, backgroundcl;
							for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
								delete* it_sp;
								it_sp = Spman->erase(it_sp);
							}
							for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
								delete* it_ic;
								it_ic = Ice->erase(it_ic);
							}
							delete Spman, Castle;
							delete triggerForEnemy;
							delete MainWrd;
							delete message_settings, message_end;
							delete Camera;
							return lvlnum::menuGame;
						}
					}
					
					if (!mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible && !mltAboutEnemy->visible && !mltAboutIce->visible && !mltAboutIce2->visible && mltAboutIce3->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							mltAboutIce3->visible = false;
							mltEnd->visible = true;	
						}
					}

					if (!mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible && !mltAboutEnemy->visible && !mltAboutIce->visible && mltAboutIce2->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right && !men->active) {
							if (barmp >= 100) {
								if (Ice->empty()) {
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 60));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 60));
								}
								barmp -= 100;
							}
							mltAboutIce2->visible = false;
							mltAboutIce3->visible = true;
						}
					}

					if (!mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible && !mltAboutEnemy->visible && mltAboutIce->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right && !men->active) {
							barmp = 100;
							if (barmp >= 100) {
								if (Ice->empty()) {
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 60));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6), realPos.x - 55, realPos.y - 65, 60));
								}
							}
							mltAboutIce->visible = false;
							mltAboutIce2->visible = true;
						}
					}
					
					if (!mlt->visible && !mltAboutHP->visible && mltAboutMP->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							mltAboutMP->visible = false;
							backgroundcl->visible = false;
							EnemyActive = true;
						}
					}

					if (!mlt->visible && mltAboutHP->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							mltAboutHP->visible = false;
							mltAboutMP->visible = true;
						}
					}

					if (mlt->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							mlt->visible = false;
							mltAboutHP->visible = true;
						}
					}

					if (!message_end->active && !mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) {
							if (barmp >= 20 && mltAboutEnemy->visible) {
								Expl_list->push_back(new Meteor(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 0), realPos.x - 200, realPos.y - 200));
							}
						}

						if (men->active && !st_men->active) {
							if (men->btContinue->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									men->active = false;
								}
							}

							if (men->btOptions->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = true;
								}
							}

							if (men->btExit->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									window->close();
								}
							}
						} else if (st_men->active && !message_settings->active) {
							if (st_men->btBack->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = false;
									st_men->backSettings(config);
								}
							}

							if (st_men->cbFullS->getSize().contains(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbFullS->invers();
								}
							}

							if (st_men->cbVertS->getSize().contains(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbVertS->invers();
								}
							}

							if (st_men->cbSound->getSize().contains(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbSound->invers();
								}
							}

							if (st_men->btSave->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									if (st_men->saveSettings(config) == static_cast<int>(settings_save_code::other)) {
										message_settings->active = true;
									} else {
										st_men->active = false;
									}

									window->setVerticalSyncEnabled(config->verticalSync);

									if (config->sound) {
										volume = config->soundVolume;
									} else {
										volume = 0;
									}
								}
							}

							if (st_men->combScreen->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->combScreen->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
									st_men->combScreen->back();
								}
							}

							if (st_men->combSoundV->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->combSoundV->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
									st_men->combSoundV->back();
								}
							}

						} else {
							if (message_settings->btOk->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									message_settings->active = false;
									st_men->active = false;
								}
							}
						}
					} else {
						if (message_end->btOk->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
									delete* it_expl;
									it_expl = Expl_list->erase(it_expl);
								}
								delete Expl_list;
								delete men, st_men;
								delete HP, MP;
								delete mlt, mltAboutHP, mltAboutMP, mltAboutEnemy, mltAboutEnemy, mltAboutIce, mltAboutIce2, mltAboutIce3, mltEnd, backgroundcl;
								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									delete* it_sp;
									it_sp = Spman->erase(it_sp);
								}
								for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
									delete* it_ic;
									it_ic = Ice->erase(it_ic);
								}
								delete Spman, Castle;
								delete triggerForEnemy;
								delete MainWrd;
								delete message_settings, message_end;
								delete Camera;
								return lvlnum::menuGame;
							}
						}
					}

					if (window->isOpen()) { break; }
				}

				if (Keyboard::isKeyPressed(Keyboard::Enter)) {
					for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
						delete* it_expl;
						it_expl = Expl_list->erase(it_expl);
					}
					delete Expl_list;
					delete men, st_men;
					delete HP, MP;
					delete mlt, mltAboutHP, mltAboutMP, mltAboutEnemy, mltAboutEnemy, mltAboutIce, mltAboutIce2, mltAboutIce3, mltEnd, backgroundcl;
					for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
						delete* it_sp;
						it_sp = Spman->erase(it_sp);
					}
					for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
						delete* it_ic;
						it_ic = Ice->erase(it_ic);
					}
					delete Spman, Castle;
					delete triggerForEnemy;
					delete MainWrd;
					delete message_settings, message_end;
					delete Camera;
					return lvlnum::menuGame;
				}

				if (mltAboutEnemy->visible) {

					for (list<Meteor*>::iterator it = Expl_list->begin(); it != Expl_list->end();) {
						
						if ((*it)->reached_point && !(*it)->is_sound_play) {
							expl_sound.setVolume(volume);
							expl_sound.play();
							(*it)->is_sound_play = true;
						}
						
						for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
							if ((*it)->rect_collis->getBounds().intersects((*it_sp)->rect_collis->getBounds()) && !(*it)->cooldown) {
								expl_sound.play();
								(*it_sp)->health -= 30;
								if ((*it_sp)->health <= 0) { (*it_sp)->health = 0; }
								(*it)->cooldown = true;
							}
							it_sp++;
						}
						it++;
					}

					for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
						(*it_sp)->move(timer / 2, direcrion8::non);
						if ((*it_sp)->isDead) {
							delete* it_sp;
							it_sp = Spman->erase(it_sp);
							mltAboutEnemy->visible = false;
							mltAboutIce->visible = true;
						}
						it_sp++;
					}
				}

				if (!message_end->active && !mltAboutEnemy->visible) {
					if (Keyboard::isKeyPressed(Keyboard::Escape) && !mlt->visible && !mltAboutHP->visible && !mltAboutMP->visible) { //Выход
						men->active = true;
					}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Регистрация попаданий
					if (!men->active) {
						
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Логика поведения врагов
						for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
							if ((*it_sp)->isDead) {
								delete* it_sp;
								it_sp = Spman->erase(it_sp);
							} else {
								if ((*it_sp)->rect_collis->getBounds().intersects(triggerForEnemy->getBounds())) {
									mltAboutEnemy->visible = true;
									backgroundcl->visible = true;
								}
								if ((*it_sp)->rect_collis->getBounds().intersects(Castle->rect_collis->getBounds()) && (*it_sp)->health != 0) {
									(*it_sp)->attack(timer / 2);
									if (!(*it_sp)->isCooldown(Second)) {
										barhp -= 5;
										(*it_sp)->cooldown = true;
									}
								} else {
									(*it_sp)->move(timer / 2, direcrion8::left);
								}
								it_sp++;
							}
						}

						HP->changeBar(barhp);
						HP->freeze(Camera, 5, 5);
						if (barhp <= 50) {
							barmp += timer / 100;
							Castle->sprite_rect = IntRect(400, 0, 400, 1500);
						} if (barhp <= 25) {
							Castle->sprite_rect = IntRect(400 * 2, 0, 400, 1500);
						} if (barhp <= 0) {
							Castle->sprite_rect = IntRect(400 * 3, 0, 400, 1500);
							barhp = 0;
							message_end->active = true;
						}
						barmp >= 100 ? barmp = 100 : barmp += timer / 100;
						MP->changeBar(barmp);
						MP->freeze(Camera, 5, 40);

						if (EnemyActive) { //----------------------------------------------Скрипт с врагом
							Spman->push_back(new Spearman(*(pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3), config->screenWidth + 20, config->screenHeight / 2, 45));
							EnemyActive = false;
						}	
					}
				}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Отрисовка
				window->clear();
				MainWrd->render(*window);
				
				for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
					(*it_sp)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3));
					it_sp++;
				}
				Castle->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 2));
				if (!mltAboutHP->visible) {
					#undef small
					HP->resize(_interface::text_size::small);
					HP->render(*window);
					#define small char
				}
				if (!mltAboutMP->visible) {
					#undef small
					MP->resize(_interface::text_size::small);
					MP->render(*window);
					#define small char
				}
				backgroundcl->render(*window);
				if (mltAboutHP->visible) {
					HP->resize(_interface::text_size::big);
					HP->render(*window);
				}
				if (mltAboutMP->visible) {
					MP->resize(_interface::text_size::big);
					MP->render(*window);
				}
				if (mltAboutEnemy->visible) {					

					for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
						(*it_sp)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 3));
						it_sp++;
					}
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->update(timer / 4.5);
					it_ic++;
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 6));
					it_ic++;
				}
				for (list<Meteor*>::iterator it = Expl_list->begin(); it != Expl_list->end();) {
					if ((*it)->end) {
						delete* it;
						it = Expl_list->erase(it);
					} else {
						(*it)->update(timer);
						(*it)->render(*window, (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 0), (pointer_cast(((char*)(ptr_global_memory) + block_memory_sprite), Sprite) + 1));
						it++;
					}
				}
				mltAboutHP->render(*window);
				mltAboutMP->render(*window);
				mltAboutEnemy->render(*window);
				mltAboutIce->render(*window);
				mltAboutIce2->render(*window);
				mltAboutIce3->render(*window);
				mltEnd->render(*window);

				mlt->render(*window);
				Camera->setView(*window);
				men->render(*window, Camera);
				st_men->render(*window, Camera);
				message_settings->render(*window, Camera);
				message_end->render(*window, Camera);
				window->display();
			}

			for (list<Meteor*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
				delete* it_expl;
				it_expl = Expl_list->erase(it_expl);
			}
			delete Expl_list;
			delete men, st_men;
			delete HP, MP;
			delete mlt, mltAboutHP, mltAboutMP, mltAboutEnemy, mltAboutIce, mltAboutIce2, mltAboutIce3,  mltEnd, backgroundcl;
			for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
				delete *it_sp;
				it_sp = Spman->erase(it_sp);
			}
			for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
				delete *it_ic;
				it_ic = Ice->erase(it_ic);
			}
			delete Castle, Spman;
			delete triggerForEnemy;
			delete MainWrd;
			delete message_settings, message_end;
			delete Camera;

			return lvlnum::exitGame;	
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
		void ExecuteCommand(const char* command) {
			std::string find_str(command);

			if (find_str.find("-moremane") != std::string::npos) {
				barmp_max = 99999;
				barmp = 99999;
			}

			if (find_str.find("-morehealth") != std::string::npos) {
				barhp = 99999;
			}
		}

		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		void StartApp() {
			int out;
			out = this->Run();
			while (out != lvlnum::exitGame) {
				switch (out) {
				case lvlnum::lvlRun: out = this->LvlRun(); break;
				case lvlnum::lvlTraining: out = this->lvlTraining(); break;
				case lvlnum::menuGame: out = this->Run(); break;
				default: break;
				}
			}
		}
};

int main(int argc, char* argv[]){
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

	Game *gamez = new Game();

	if (argc >= 2) { //Проверяем наличие ключей запуска
		for (int i = 1; argc > i; i++) {
			gamez->ExecuteCommand(argv[i]);
		}
	}

	try {
		gamez->StartApp(); //Запускаем приложение
	} catch(...) {
		
	}

	delete gamez;
	return 0;
}