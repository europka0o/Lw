#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include "Interf.h"
#include <iostream>
#include <list>

#if defined _WIN32 || defined _WIN64 //для windows

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
		Image *ptr_on_image_world;
		Texture *ptr_on_texture_castle;
		Texture *ptr_on_texture_expl;
		Texture *ptr_on_texture_sprman; //"Img/sprman.png"
		Texture *ptr_on_texture_castl_destr; //"Img/CastlDestr.png"
		Texture *ptr_on_texture_gg; //"Img/gg.png"
		Texture *ptr_on_texture_ice; //"Img/ice.png"
		sf::Vector2i pos;
		sf::Vector2f realPos;
		uint32_t CENTER_SCREEN_X;
		uint32_t CENTER_SCREEN_Y;
	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Game() {
			config = new configuration;
			if (!config->loadSettings()) {
				config->createSettings(screen_width, screen_height, true, 30, true);
			}

			window = new RenderWindow(VideoMode(config->screenWidth, config->screenHeight), "Little World", config->fullScreen ? 8 : 7);
			window->setVerticalSyncEnabled(config->verticalSync);
			
			ptr_on_image_world = new Image;
			ptr_on_image_world->loadFromFile("Img/ground.png");
			ptr_on_texture_expl = new Texture;
			ptr_on_texture_expl->loadFromFile("Img/exp.png");
			ptr_on_texture_castle = new Texture;
			ptr_on_texture_castle->loadFromFile("Img/castle.png");
			ptr_on_texture_sprman = new Texture;
			ptr_on_texture_sprman->loadFromFile("Img/sprman.png");
			ptr_on_texture_castl_destr = new Texture;
			ptr_on_texture_castl_destr->loadFromFile("Img/CastlDestr.png");
			ptr_on_texture_gg = new Texture;
			ptr_on_texture_gg->loadFromFile("Img/gg.png");
			ptr_on_texture_ice = new Texture;
			ptr_on_texture_ice->loadFromFile("Img/ice.png");

			CENTER_SCREEN_X = config->screenWidth / 2;
			CENTER_SCREEN_Y = config->screenHeight / 2;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		~Game() {
			delete config, window; 
			delete ptr_on_image_world, ptr_on_texture_castle, ptr_on_texture_expl, ptr_on_texture_sprman, ptr_on_texture_castl_destr, ptr_on_texture_gg, ptr_on_texture_ice;
		}
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		int Run() {
			_interface::main_menu *main_men = new _interface::main_menu(config, Color(38, 38, 38, 255));
			_interface::settings_menu *st_men = new _interface::settings_menu(config, Color::Color(38, 38, 38, 255), Color::Yellow);
			st_men->blackout_visible = true;

			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			while (window->isOpen()) {

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
								//TODO
								//this->LvlRun();
								//goto Exit;
								delete main_men, st_men, message_settings;
								return lvlnum::lvlRun;
							}
						}

						if (main_men->btStartTren->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								//TODO
								//this->lvlTraining();
								//goto Exit;
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

						if (st_men->btSave->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->saveSettings(config);
								message_settings->active = true;
							}
						}

						if (st_men->combScreen->isAction(realPos.x, realPos.y)) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								st_men->combScreen->next();
							}
							else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
								st_men->combScreen->back();
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

				
				window->clear();
				main_men->render(window);
				st_men->render(window);
				message_settings->render(window);
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
			list<ObjectAnimated*> *Expl_list = new list<ObjectAnimated*>;

			list<Character*> *Pers = new list<Character*>;
			list<DestroerCastle*> *DC = new list<DestroerCastle*>;
			list<Spearman*> *Spman = new list<Spearman*>;
			list<IceBall*> *Ice = new list<IceBall*>;

			MainWrd = new World(ptr_on_image_world, 40, 60);
			mlt = new _interface::multiline_text(100, 200, Color::Black, Color::Yellow);
			mlt->resize(_interface::text_size::normal);
			Camera = new Camer(CENTER_SCREEN_X, CENTER_SCREEN_Y, config->screenWidth, config->screenHeight);

			Castle = new ObjectStatic(ptr_on_texture_castle, 0, 0); //замок
			Castle->setRect(IntRect(0, 0, 400, 1500));
			Castle->rect_collis->setBounds(IntRect(0, 0, 400, 1500));
			int numlvl = 0; //номер волны
			bool change_numlvl = false; //смена волны
			//int sd = 0;
			float timer; //глобальное время
			float barhp = 200; //здоровье
			float barmp = 150; //мана
			float barmp_max = 150;
			int mp_need_cast_expl = 20; //нужное кол-во маны для взрыва
			int mp_need_cast_ice = 100; //нужное кол-во маны для призыва кристала

			HP = new _interface::bar(5, 5, barhp, 0, L"HP:", Color::White, Color::Red, Color::Black); //полоса здоровья
			MP = new _interface::bar(5, HP->getSize().left + HP->getSize().width + 5, barmp, 0, L"MP:", Color::White, Color::Blue, Color::Black); //полоса маны

			_interface::message *message_end = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Захватчики разрушили ваш замок, вы проиграли", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_vic = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Вы победили всех нападающих противников! Поздравляем!", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			_interface::settings_menu *st_men = new _interface::settings_menu(config, Color::Color(38, 38, 38, 255), Color::Yellow); //меню настроек
			men = new _interface::menu(Camera, Color::Color(38, 38, 38, 255), Color::Yellow); //главное меню
			men->active = false;
			st_men->blackout_visible = false;
			
			lvlInfo = new _interface::text(0, 0, L"Волна 1", Color::Black, Color::Yellow); //надпись с номером волны
			lvlInfo->resize(_interface::text_size::medium);
			lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);
			

			mlt->add(L"Тревога!");
			mlt->add(L"На наш замок нападают");

			float Second = 0; //накапливает время в секундах
			float NacopSec = 0; //накапливает время в секундах
			float NacopSec2 = 0; //накапливает время в секундах

//------------------------------------------------------------------------------------------------------------------------Звуки и музыка
			SoundBuffer shootBuffer;//создаём буфер для звука																		
			shootBuffer.loadFromFile("Sounds/expls.ogg");//загружаем в него звук													
			Sound expl_sound(shootBuffer);//создаем звук и загружаем в него звук из буфера											
			
			SoundBuffer game_overBuffer;
			game_overBuffer.loadFromFile("Sounds/game_over.ogg");
			Sound game_over_sound(game_overBuffer);

			SoundBuffer victoryBuffer;
			victoryBuffer.loadFromFile("Sounds/victory.ogg");
			Sound victory_sound(victoryBuffer);

			SoundBuffer attackBuffer;
			attackBuffer.loadFromFile("Sounds/attack.ogg");
			list<Sound*> *attack_sound = new list<Sound*>;
			
			SoundBuffer attackIceBuffer;
			attackIceBuffer.loadFromFile("Sounds/attack_ice.ogg");
			list<Sound*> *attack_ice_sound = new list<Sound*>;
		
			Music *music_main_theme = new Music;//создаем объект музыки																
			music_main_theme->openFromFile("Sounds/main_theme.ogg");//загружаем файл												
			music_main_theme->play();																								
//------------------------------------------------------------------------------------------------------------------------------------
			
			while (window->isOpen()) {
				
				music_main_theme->setLoop(true);

				timer = clock.getElapsedTime().asMicroseconds(); //получаем время в микросек
				Second = clock.getElapsedTime().asSeconds(); //получаем время в секундах
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
					if (!message_end->active && !message_vic->active) {
						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left && !men->active) { //если нажата левая кнопка мыши
							if (barmp >= mp_need_cast_expl) {
								Expl_list->push_back(new ObjectAnimated(ptr_on_texture_expl, realPos.x - 200, realPos.y - 200));
								expl_sound.play();
								barmp -= mp_need_cast_expl;
							}
						}

						if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right && !men->active) { //если нажата правая кнопка мыши
							if (barmp >= mp_need_cast_ice) {
								if (Ice->empty()) {
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 100));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 100));
								}
								barmp -= mp_need_cast_ice;
							}
						}

						if (men->active && !st_men->active) { //меню паузы
							if (men->btContinue->isAction(realPos.x, realPos.y)) { //если нажата кнопка продолжить 
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									men->active = false;
								}
							}

							if (men->btOptions->isAction(realPos.x, realPos.y)) { //если нажата кнопка настройки
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = true;
								}
							}

							if (men->btExit->isAction(realPos.x, realPos.y)) { //если нажата кнопка выйти
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
									//TODO/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//this->Run();////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									//goto Exit;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
									return lvlnum::menuGame;
								}
							}
						} else if (st_men->active && !message_settings->active) { //меню настроек
							if (st_men->btBack->isAction(realPos.x, realPos.y)) { //если нажата кнопка назад
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->active = false;
									st_men->backSettings(config);
								}
							}

							if (st_men->cbFullS->getSize().contains(realPos.x, realPos.y)) { //если нажат чекбокс на весь экран
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbFullS->invers();
								}
							}

							if (st_men->cbVertS->getSize().contains(realPos.x, realPos.y)) { //если нажат чекбокс верикальная синхронизация
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->cbVertS->invers();
								}
							}

							if (st_men->btSave->isAction(realPos.x, realPos.y)) { //если нажата кнопка сохранить
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->saveSettings(config);
									message_settings->active = true;
								}
							}

							if (st_men->combScreen->isAction(realPos.x, realPos.y)) { 
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) { //если нажата левая кнопка мыши на комбобоксе разрешение экрана
									st_men->combScreen->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) { //если нажата правая кнопка мыши на комбобоксе разрешение экрана
									st_men->combScreen->back();
								}
							}

						} else {
							if (message_settings->btOk->isAction(realPos.x, realPos.y)) { //если нажата кнопка ок в сообщении
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									message_settings->active = false;
									st_men->active = false;
								}
							}
						}
					} else {
						if (message_end->btOk->isAction(realPos.x, realPos.y) && message_end->active) { //если нажата кнопка ок в сообщении о проигрыше
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
								//TODO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//this->Run();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//goto Exit;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								return lvlnum::menuGame;
							}
						}

						if (message_vic->btOk->isAction(realPos.x, realPos.y) && message_vic->active) {
							if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
								for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
								//TODO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//this->Run();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//goto Exit;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
						case 1: //первая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 2");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 920, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 950, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 740, 990, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 920, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 905, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 950, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 740, 400, 45));				
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 180, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 260, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 340, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 420, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 580, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 660, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 740, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 920, 190, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 905, 280, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 900, 370, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 460, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 950, 550, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 740, 640, 45));
							}

							change_numlvl = false;
							break;
						case 2: //вторая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 3");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 4; //mp_need_cast_expl = 16

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 1200, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 920, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 20, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 70, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 120, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 220, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 270, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 320, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 370, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 420, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 470, 45));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 180, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 260, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 340, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 420, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 580, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 660, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 180, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 260, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 340, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 420, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 580, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 660, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 170, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 240, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 310, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 380, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 520, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 590, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 660, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 730, 45));
							}

							change_numlvl = false;
							break;
						case 3: //третья волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 4");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 4; //mp_need_cast_expl = 12

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 1000, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 920, 45));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 90, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 130, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 210, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 290, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 330, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 370, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 410, 45));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 500, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 550, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1910, 550, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 600, 45));
							}

							change_numlvl = false;
							break;
						case 4: //четвертая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 5");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 2; //mp_need_cast_expl = 10

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 1200, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 120, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 240, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 410, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 550, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 810, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 930, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 970, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 1100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 110, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 220, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 405, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 575, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 809, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 915, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1890, 970, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 2000, 1005, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1950, 1130, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 2100, 600, 650));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 400, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 90, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 130, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 170, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 210, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1890, 390, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 2000, 430, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1950, 450, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 2100, 400, 650));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 27, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 55, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 240, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 300, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 980, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 995, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 990, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 970, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1040, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1905, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1900, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1908, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1920, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1890, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 2000, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1950, 500, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 2100, 450, 650));
							}
		
							change_numlvl = false;
							break;
						case 5: //пятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 6");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 85

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 57, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 95, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 230, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 1200, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 230, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 560, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 920, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 990, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 1000, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 750, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 1050, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 1200, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1870, 700, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 57, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 95, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 230, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 400, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 450, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1870, 450, 500));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 29, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 114, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 57, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 95, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 230, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 600, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1300, 500, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 200, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1870, 500, 500));
							}

							change_numlvl = false;
							break;
						case 6: //шестая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 7");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_expl -= 1; //mp_need_cast_expl = 9
							
							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 750, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 1050, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 1200, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1500, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1700, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 750, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 1050, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1500, 1200, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 600, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1500, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1700, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 450, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 300, 500));
							} else { // если любое другое
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 20, 200, 400));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1000, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1050, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1100, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1150, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1200, 550, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1500, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1700, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1650, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1600, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1550, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 1500, 500, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1900, 400, 500));
							}

							change_numlvl = false;
							break;
						case 7: //седьмая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 8");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 70

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 750, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 1050, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 1200, 45));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 1000, 700, 1000));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 450, 45));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 1000, 250, 1000));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 110, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 115, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 120, 500, 45));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 1000, 400, 1000));
							}

							change_numlvl = false;
							break;
						case 8: //восьмая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Волна 9");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);
							
							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 650, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 850, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 650, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 850, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 1050, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 300, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 350, 600, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 800, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 300, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 350, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 300, 200, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 350, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 400, 500));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 105, 550, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 305, 550, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 300, 100, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 350, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 500, 500));
							}

							change_numlvl = false;
							break;
						case 9: //девятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Последняя волна");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 100, 700, 1000));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 500, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 500, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 570, 900, 500));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 100, 200, 1000));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 500, 150, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 200, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 570, 250, 500));
							} else { // если любое другое
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 100, 300, 1000));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 500, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 700, 400, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 570, 600, 500));
							}

							change_numlvl = false;
							break;
						case 10: //десятая волна
							lvlInfo->visible = false;
							lvlInfo->setString(L"Победа!");
							lvlInfo->resize(_interface::text_size::medium);
							lvlInfo->setPosition((config->screenWidth / 2) - lvlInfo->getSize().width / 2, (config->screenHeight / 2) - lvlInfo->getSize().height / 2);

							mp_need_cast_ice -= 15; //mp_need_cast_ice = 55
							mp_need_cast_expl -= 1; //mp_need_cast_expl = 8

							if (config->screenWidth == 2560 && config->screenHeight == 1440) { //если разрешение экрана 2560 х 1440
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 700, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 1000, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 1100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 400, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 400, 700, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 1000, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 400, 1100, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 800, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 500, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 600, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 800, 700, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 800, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 900, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 1000, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 800, 1100, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1300, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1400, 800, 500));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2000, 100, 1000));
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2700, 700, 1000));
							} else if (config->screenWidth == 1280 && config->screenHeight == 720) { //если разрешение 1280 х 720
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 400, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 400, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 400, 450, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 800, 50, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 800, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 800, 450, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1300, 100, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1400, 400, 500));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2000, 50, 1000));
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2700, 300, 1000));
							} else { // если любое другое
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140, 500, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 400, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 400, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 400, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 400, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 400, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 400, 500, 45));

								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 130 + 800, 100, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 150, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 200, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 250, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 10 + 800, 300, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 40 + 800, 350, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 70 + 800, 400, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 100 + 800, 450, 45));
								Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 140 + 800, 500, 45));

								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1300, 300, 500));
								Pers->push_back(new Character(ptr_on_texture_gg, config->screenWidth + 1400, 500, 500));

								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2000, 100, 1000));
								DC->push_back(new DestroerCastle(ptr_on_texture_castl_destr, config->screenWidth + 2700, 560, 1000));
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
							victory_sound.play();
							numlvl++;
						}
					} else {
						NacopSec2 = 0;
					}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Регистрация попаданий
					if (!men->active) {
						for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
							if ((*it_expl)->end) {
								delete* it_expl;
								it_expl = Expl_list->erase(it_expl);
							} else {

								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects((*it_sp)->rect_collis->getBounds()) && !(*it_expl)->cooldown) {
										(*it_sp)->health -= 30;
										if ((*it_sp)->health <= 0) { (*it_sp)->health = 0; }
									}
									it_sp++;
								}

								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects(IntRect((*it_dc)->getSize())) && !(*it_expl)->cooldown) {
										(*it_dc)->health -= 25;
										if ((*it_dc)->health <= 0) { (*it_dc)->health = 0; }
									}
									it_dc++;
								}

								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									if ((*it_expl)->rect_collis->getBounds().intersects(IntRect((*it_p)->getSize())) && !(*it_expl)->cooldown) {
										(*it_p)->health -= 25;
										if ((*it_p)->health <= 0) { (*it_p)->health = 0; }
									}
									it_p++;
								}

								(*it_expl)->update(timer);

								(*it_expl)->cooldown = true;
								it_expl++;
							}

						}

						for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
							if ((*it_ic)->isDead) {
								delete* it_ic;
								it_ic = Ice->erase(it_ic);
							} else {

								for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
									if ((*it_ic)->getSize().intersects((*it_sp)->getSize())) {
										if (!(*it_ic)->isCooldown(Second)) {
											(*it_sp)->health -= 5;
											(*it_ic)->cooldown = true;
										}
									}
									it_sp++;
								}

								for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
									if ((*it_ic)->getSize().intersects((*it_dc)->getSize())) {
										if (!(*it_ic)->isCooldown(Second)) {
											(*it_dc)->health -= 5;
											(*it_ic)->cooldown = true;
										}
									}
									it_dc++;
								}

								for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
									if ((*it_ic)->getSize().intersects((*it_p)->getSize())) {
										if (!(*it_ic)->isCooldown(Second)) {
											(*it_p)->health -= 5;
											(*it_ic)->cooldown = true;
										}
									}
									it_p++;
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

								if ((*it_sp)->getSize().intersects(FloatRect(Castle->rect_collis->getBounds())) && (*it_sp)->health != 0) {
									(*it_sp)->attack(timer / 2);
									if (!(*it_sp)->isCooldown(Second)) {
										barhp -= 3;
										(*it_sp)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->play();
									}
								} else {

									if (Ice->empty()) {
										(*it_sp)->move(timer / 2, direcrion8::left);
									} else {
										if ((*it_sp)->getSize().intersects(FloatRect(Ice->back()->rect_collis->getBounds())) && Ice->back()->health != 0) {
											(*it_sp)->attack(timer / 2);
											if (!(*it_sp)->isCooldown(Second)) {
												Ice->back()->health -= 3;
												(*it_sp)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
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

								if ((*it_p)->getSize().intersects(FloatRect(Castle->rect_collis->getBounds())) && (*it_p)->health != 0) {
									(*it_p)->attack(timer / 1.5);
									if (!(*it_p)->isCooldown(Second)) {
										barhp -= 10;
										(*it_p)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->play();
									}
								} else {
									if (Ice->empty()) {
										(*it_p)->move(timer / 3.5, direcrion8::left);
									} else {
										if ((*it_p)->getSize().intersects(FloatRect(Ice->back()->rect_collis->getBounds())) && Ice->back()->health != 0) {
											(*it_p)->attack(timer / 2);
											if (!(*it_p)->isCooldown(Second)) {
												Ice->back()->health -= 10;
												(*it_p)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
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
								if ((*it_dc)->getSize().intersects(FloatRect(Castle->rect_collis->getBounds())) && (*it_dc)->health != 0) {
									(*it_dc)->attack(timer / 4.2);
									if (!(*it_dc)->isCooldown(Second)) {
										barhp -= 20;
										(*it_dc)->cooldown = true;
										attack_sound->push_back(new Sound(attackBuffer));
										attack_sound->back()->play();
									}
								} else {
									if (Ice->empty()) {
										(*it_dc)->move(timer / 3.5, direcrion8::left);
									} else {
										if ((*it_dc)->getSize().intersects(FloatRect(Ice->back()->rect_collis->getBounds())) && Ice->back()->health != 0) {
											(*it_dc)->attack(timer / 2);
											if (!(*it_dc)->isCooldown(Second)) {
												Ice->back()->health -= 20;
												(*it_dc)->cooldown = true;
												attack_ice_sound->push_back(new Sound(attackIceBuffer));
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
							Castle->setRect(IntRect(400, 0, 400, 1500));
						} if (barhp <= 75) {
							Castle->setRect(IntRect(400 * 2, 0, 400, 1500));
						} if (barhp <= 0) {
							Castle->setRect(IntRect(400 * 3, 0, 400, 1500));
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
				MainWrd->render(window);
				
				for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
					//(*it_sp)->rect_collis->visible_deb = true;
					//if (Camera->getBounds().intersects(FloatRect((*it_sp)->rect_collis->getBounds()))) {
						(*it_sp)->render(window);
					//}
					it_sp++;
				}

				for (list<DestroerCastle*>::iterator it_dc = DC->begin(); it_dc != DC->end();) {
					//if (Camera->getBounds().intersects(FloatRect((*it_dc)->getSize()))) {
						(*it_dc)->render(window);
					//}
					it_dc++;
				}
				for (list<Character*>::iterator it_p = Pers->begin(); it_p != Pers->end();) {
					//(*it_p)->rect_collis->visible_deb = true;
					//if (Camera->getBounds().intersects(FloatRect((*it_p)->getSize()))) {
						(*it_p)->render(window);
					//}
					it_p++;
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->render(window);
					it_ic++;
				}
				Castle->render(window);
				HP->render(window);
				MP->render(window);
				for (list<ObjectAnimated*>::iterator it = Expl_list->begin(); it != Expl_list->end();) {
					(*it)->render(window);
					it++;
				}

				mlt->render(window);
				lvlInfo->render(window);
				Camera->setView(window);
				men->render(window, Camera);
				st_men->render(window, Camera);
				message_settings->render(window, Camera); //Camer
				message_end->render(window, Camera); //Camer
				message_vic->render(window, Camera);
				window->display();
			}

			for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
			mlt->add(L"Добро пожаловать!");
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
			mltAboutIce->add(L"Хорошо!");
			mltAboutIce->add(L"Но в настоящем бою одним только взыров не обойдешься,");
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
			mltEnd->add(L"Отлично!");
			mltEnd->add(L"Вы прошли обучение и теперь можете перейти в меню!");
			mltEnd->add(L"Нажмите правую кнопку мыши чтобы продолжить");
			mltEnd->visible = false;
			mltEnd->resize(_interface::text_size::normal);

			ObjectStatic *Castle;
			_interface::menu *men;
			list<ObjectAnimated*> *Expl_list;
			list<Spearman*> *Spman = new list<Spearman*>;

			list<IceBall*> *Ice = new list<IceBall*>;

			Collision *triggerForEnemy = new Collision(IntRect(config->screenWidth - 500, 0, 20, config->screenHeight));

			MainWrd = new World(ptr_on_image_world, 60, 60);
			HP = new _interface::bar(5, 5, 100, 0, L"HP:", Color::White, Color::Red, Color::Black);
			MP = new _interface::bar(5, HP->getSize().left + HP->getSize().width + 5, 100, 0, L"MP:", Color::White, Color::Blue, Color::Black);
			Camera = new Camer(CENTER_SCREEN_X, CENTER_SCREEN_Y, config->screenWidth, config->screenHeight);

			Castle = new ObjectStatic(ptr_on_texture_castle, 0, 0);
			Castle->setRect(IntRect(0, 0, 400, 1500));
			Castle->rect_collis->setBounds(IntRect(0, 0, 400, 1500));
			//int sd = 0;
			float timer;
			float barhp = 100;
			float barmp = 100;

			_interface::message *message_end = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Замок разрушен, вы проиграли", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);
			_interface::message *message_settings = new _interface::message(config->screenWidth / 2, config->screenHeight / 2, L"Изменения вступят в силу после перезапуска игры", Color(38, 38, 38, 255), Color::Yellow, Color::Yellow);

			_interface::settings_menu *st_men = new _interface::settings_menu(config, Color::Color(38, 38, 38, 255), Color::Yellow);
			men = new _interface::menu(Camera, Color::Color(38, 38, 38, 255), Color::Yellow);
			men->active = false;
			st_men->blackout_visible = false;
			Expl_list = new list<ObjectAnimated*>;

			float Second = 0;

			SoundBuffer shootBuffer;//создаём буфер для звука
			shootBuffer.loadFromFile("Sounds/expls.ogg");//загружаем в него звук
			Sound expl_sound(shootBuffer);//создаем звук и загружаем в него звук из буфера

			//Spman->push_back(new Spearman("Img/sprman.png", 2600, 100, 45));

			while (window->isOpen()) {

				timer = clock.getElapsedTime().asMicroseconds(); //получаем время в микросек
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
							for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
							//TODO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							//this->Run();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 60));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 60));
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
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 60));
								} else {
									Ice->back()->health = 0;
									Ice->push_back(new IceBall(ptr_on_texture_ice, realPos.x - 55, realPos.y - 65, 60));
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
								Expl_list->push_back(new ObjectAnimated(ptr_on_texture_expl, realPos.x - 200, realPos.y - 200));
								expl_sound.play();
								barmp -= 10;
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

							if (st_men->btSave->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->saveSettings(config);
									message_settings->active = true;
								}
							}

							if (st_men->combScreen->isAction(realPos.x, realPos.y)) {
								if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
									st_men->combScreen->next();
								} else if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Right) {
									st_men->combScreen->back();
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
								for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
								//TODO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//this->Run();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								//goto Exit;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								return lvlnum::menuGame;
							}
						}
					}

					if (window->isOpen()) { break; }
				}

				if (Keyboard::isKeyPressed(Keyboard::Enter)) {
					for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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
					//TODO///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//this->Run();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//goto Exit;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					return lvlnum::menuGame;
				}

				if (mltAboutEnemy->visible) {

					for (list<ObjectAnimated*>::iterator it = Expl_list->begin(); it != Expl_list->end();) {
							for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
								if ((*it)->rect_collis->getBounds().intersects((*it_sp)->rect_collis->getBounds()) && !(*it)->cooldown) {
									(*it_sp)->health -= 30;
									if ((*it_sp)->health <= 0) { (*it_sp)->health = 0; }
								}
								it_sp++;
							}
							(*it)->cooldown = true;
							it++;
					}

					for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
						(*it_sp)->move(timer / 2, direcrion8::non);
						if ((*it_sp)->isDead) {
							delete* it_sp;
							it_sp = Spman->erase(it_sp);
							mltAboutEnemy->visible = false;
							mltAboutIce->visible = true;
							//backgroundcl->visible = false;
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
								if ((*it_sp)->getSize().intersects(FloatRect(triggerForEnemy->getBounds()))) {
									mltAboutEnemy->visible = true;
									backgroundcl->visible = true;
								}
								if ((*it_sp)->getSize().intersects(FloatRect(Castle->rect_collis->getBounds())) && (*it_sp)->health != 0) {
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
							Castle->setRect(IntRect(400, 0, 400, 1500));
						} if (barhp <= 25) {
							Castle->setRect(IntRect(400 * 2, 0, 400, 1500));
						} if (barhp <= 0) {
							Castle->setRect(IntRect(400 * 3, 0, 400, 1500));
							barhp = 0;
							message_end->active = true;
						}
						barmp >= 100 ? barmp = 100 : barmp += timer / 100;
						MP->changeBar(barmp);
						MP->freeze(Camera, 5, 40);

						if (EnemyActive) { //----------------------------------------------Скрипт с врагом
							Spman->push_back(new Spearman(ptr_on_texture_sprman, config->screenWidth + 20, config->screenHeight / 2, 45));
							EnemyActive = false;
						}	
					}

				}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Отрисовка
				window->clear();
				MainWrd->render(window);
				
				for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
					(*it_sp)->render(window);
					it_sp++;
				}
				Castle->render(window);
				if (!mltAboutHP->visible) {
					#undef small
					HP->resize(_interface::text_size::small);
					HP->render(window);
					#define small char
				}
				if (!mltAboutMP->visible) {
					#undef small
					MP->resize(_interface::text_size::small);
					MP->render(window);
					#define small char
				}
				backgroundcl->render(window);
				if (mltAboutHP->visible) {
					HP->resize(_interface::text_size::big);
					HP->render(window);
				}
				if (mltAboutMP->visible) {
					MP->resize(_interface::text_size::big);
					MP->render(window);
				}
				if (mltAboutEnemy->visible) {
					for (list<Spearman*>::iterator it_sp = Spman->begin(); it_sp != Spman->end();) {
						(*it_sp)->render(window);
						it_sp++;
					}
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->update(timer / 4.5);
					it_ic++;
				}
				for (list<IceBall*>::iterator it_ic = Ice->begin(); it_ic != Ice->end();) {
					(*it_ic)->render(window);
					it_ic++;
				}
				for (list<ObjectAnimated*>::iterator it = Expl_list->begin(); it != Expl_list->end();) {
					if ((*it)->end) {
						delete* it;
						it = Expl_list->erase(it);
					} else {
						(*it)->update(timer);
						(*it)->render(window);
						it++;
					}
				}
				mltAboutHP->render(window);
				mltAboutMP->render(window);
				mltAboutEnemy->render(window);
				mltAboutIce->render(window);
				mltAboutIce2->render(window);
				mltAboutIce3->render(window);
				mltEnd->render(window);

				mlt->render(window);
				Camera->setView(window);
				men->render(window, Camera);
				st_men->render(window, Camera);
				message_settings->render(window, Camera); //Camer
				message_end->render(window, Camera); //Camer
				window->display();
			}

			for (list<ObjectAnimated*>::iterator it_expl = Expl_list->begin(); it_expl != Expl_list->end();) {
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

int main(){
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

	Game *gamez = new Game();

	try {
		gamez->StartApp();
	} catch(...) {
		
	}

	delete gamez;
	return 0;
}