﻿#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace sf; 

#ifndef _INTERFACE_G
#define _INTERFACE_G

enum class settings_save_code {
	sound_or_sound_volume_or_verticalS = 1,
	other = 0
};

typedef struct settings {
	bool fullScreen;
	bool verticalSync;
	bool sound;
	unsigned int screenHeight;
	unsigned int screenWidth;
	int textSize;
	int soundVolume;
	int saveSettings();
	int loadSettings();
	int createSettings(int ScreenW = 1600, int ScreenH = 900, bool VertS = false, int TxtS = 24, bool FullS = true, bool Sound = true, int SoundV = 50);
	//1280 x 720
	//1600 x 900
	//1600 x 1200
	//1920 x 1080
	//2560 x 1440
} configuration;

/// <summary>
/// Преобразует целочисленное число к строке
/// </summary>
/// <typeparam name="arg">Выходная строка</typeparam>
/// <param name="input">Целочисленное число</param>
/// <returns>Строка</returns>
template<typename arg>
arg IntToStr(int input);

#define intToStr(type, input) IntToStr<type>(input)

enum direcrion8{
	non,
	right = 1,
	left,
	up,
	down,
	upAndRight,
	upAndLeft,
	downAndRight,
	downAndLeft
};

template<typename type>
struct axes {
	type x, y;
	axes();
	axes(type _x, type _y);
};

typedef axes<int> axes_i;
typedef axes<float> axes_f;

class Camer {
private:
	axes_i pos;
	int screen_H, screen_W; //Высота и ширана вьюпорта камеры
public:
	/// <summary>
	/// Конструктор
	/// </summary>
	/// <param name="x">Позиция по оси X</param>
	/// <param name="y">Позиция по оси Y</param>
	/// <param name="screen_width">Ширина вьюпорта в пикселях</param>
	/// <param name="screen_height">Высота вьюпорта в пикселях</param>
	/// <returns></returns>
	Camer(int x, int y, int screen_width, int screen_height);
	Camer(); //Конструктор по умолчанию
	~Camer(); //Деструктор
	View *Vid;
	/// <summary>
	/// Перемещает камеру
	/// </summary>
	/// <param name="x">Новая позиция по оси X</param>
	/// <param name="y">Новая позиция по оси Y</param>
	void __fastcall move(int x, int y); //Передвигает камеру на новую позицию по осям X, Y
	/// <summary>
	/// Устанавливает зум камеры
	/// </summary>
	/// <param name="zoom">Прибавка к текущему зуму, зум по умолчанию равен 1</param>
	void setZoom(float zoom);
	/// <summary>
	/// Возвращает позицию камеры
	/// </summary>
	/// <returns>Структура axes_i</returns>
	axes_i getPosition() noexcept;
	/// <summary>
	/// Возвращает ширину вьюпорта камеры в пикселях  
	/// </summary>
	/// <returns>Количество пикселей в int</returns>
	int getScreenWidth() noexcept;
	/// <summary>
	/// Возвращает высоту вьюпорта камеры в пикселях 
	/// </summary>
	/// <returns>Количество пикселей в int</returns>
	int getScreenHeight() noexcept;
	/// <summary>
	/// Возвращает данные о хитбоке: ширина, высота, функции проверки пересечения
	/// </summary>
	/// <returns>Структура FloatRect</returns>
	FloatRect getBounds();
	void setView(RenderWindow &wd);
	void setView(RenderWindow *wd);
};

class Collision {
	private:
		axes_i pos;
		IntRect rect_collis;
		RectangleShape *main;
	public:
		bool active, visible_deb;
		/// <summary>
		/// Конструктор
		/// </summary>
		/// <param name="rect">Хитбокс</param>
		/// <returns></returns>
		Collision(const IntRect &rect);
		~Collision();
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="x">Ось X</param>
		/// <param name="y">Ось Y</param>
		void __fastcall setPosition(int x, int y);
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="xy">Структура axes_i с координатами по осям X и Y</param>
		void setPosition(const axes_i &xy);
		/// <summary>
		/// Возвращает позицию хитбокса по осям X и Y
		/// </summary>
		/// <returns>Структура axes_i</returns>
		axes_i getPosition();
		/// <summary>
		/// Возвращает данные о хитбоке: ширина, высота, функции проверки пересечения
		/// </summary>
		/// <returns>Структура IntRect</returns>
		IntRect getBounds();
		/// <summary>
		/// Устанавливает новые данные о хитбоксе
		/// </summary>
		/// <param name="rect">Струткура IntRect с новыми параметрами</param>
		void setBounds(const IntRect &rect);
		void render(RenderWindow& wd);
		void render(RenderWindow* wd);
};

typedef Collision Trigger;

class BaseCharacter {
	protected:
		axes_f pos;
		Sprite *sprt;
		FloatRect fl_rect;
		float frame, timer_cooldown;
		bool zeroing;
	public:
		BaseCharacter();
		BaseCharacter(Texture* ptr_texture, float x, float y, int _hp);
		BaseCharacter(Texture* ptr_texture, const axes_f &xy, int _hp);
		~BaseCharacter();
		bool cooldown, isDead, visible;
		int health;
		virtual axes_f getPosition();
		virtual void __fastcall setPosition(float x, float y);
		virtual void __fastcall setPosition(const axes_f &xy);
		virtual FloatRect getSize();
		virtual void render(RenderWindow& wd);
		virtual void render(RenderWindow* wd);
};

class ObjectStatic {
	protected:
		axes_i pos;
		Sprite *sprt;
	public:
		Collision *rect_collis;
		/// <summary>
		/// Конструктор
		/// </summary>
		/// <param name="i">Указатель на текстуру</param>
		/// <param name="X">Координаты по оси X</param>
		/// <param name="Y">Координаты по оси Y</param>
		/// <returns></returns>
		ObjectStatic(Texture* ptr_texture, float X, float Y);
		~ObjectStatic();
		/// <summary>
		/// Возвращает позицию
		/// </summary>
		/// <returns>Структура axes_i</returns>
		virtual axes_i getPosition();
		/// <summary>
		/// Возвращает данные о хитбоке: ширина, высота, функции проверки пересечения
		/// </summary>
		/// <returns>Структура IntRect</returns>
		virtual FloatRect getSize();
		/// <summary>
		/// Устанавливает новые данные о хитбоксе
		/// </summary>
		/// <param name="bound">Струткура IntRect с новыми параметрами</param>
		virtual void setRect(const IntRect &bound);
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="x">Ось X</param>
		/// <param name="y">Ось Y</param>
		virtual void __fastcall setPosition(int x, int y);
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="xy">Структура axes_i с координатами по осям X и Y</param>
		virtual void setPosition(const axes_i &xy);
		virtual void render(RenderWindow &wd);
		virtual void render(RenderWindow *wd);
};

class ObjectAnimated : public ObjectStatic {
	protected:
		float frame;
	public:
		bool end;
		/// <summary>
		/// Конструктор
		/// </summary>
		/// <param name="i">Указатель на текстуру</param>
		/// <param name="X">Координаты по оси X</param>
		/// <param name="Y">Координаты по оси Y</param>
		/// <returns></returns>
		ObjectAnimated(Texture* ptr_texture, float X, float Y);
		~ObjectAnimated();
		/// <summary>
		/// Обновление объекта
		/// </summary>
		/// <param name="time">Время, чем больше значение переменной, тем быстрее происходит обновление</param>
		virtual void __fastcall update(float time);
};

namespace _interface {

	class BaseInerface {
		protected:
			axes_i pos;
			FloatRect fl_rect;
		public:
			BaseInerface();
			BaseInerface(int x, int y, const FloatRect &rect);
			BaseInerface(const axes_i &xy, const FloatRect &rect);
			~BaseInerface();
			bool visible;
			virtual axes_i getPosition();
			virtual void setPosition(const axes_i &xy);
			virtual void __fastcall setPosition(int x, int y);
			virtual FloatRect getSize();
			virtual void render(RenderWindow &wd);
			virtual void render(RenderWindow *wd);
	};

	//Направление градиента
	enum gradient_direction {
		leftOnRight,
		rightOnLeft,
		topOnBottom,
		bottomOnTop
	};

	//Размер текста в пикселях
	enum text_size { 
		small = 24,   
		normal = 30,  
		medium = 48, 
		big = 68
	};
	
	template<typename arg>
	arg repoz_X(arg X, arg size, arg indent = 0);

	template<typename arg>
	arg repoz_Y(arg Y, arg size, arg indent = 0);

	#define repoz_x(type, X, size, indent) repoz_X <type>(X, size, indent) //Вычисляет погрешность позиции текста в пикселях и выравнивает его относительно заданных координат для оси X
	#define repoz_y(type, Y, size, indent) repoz_Y <type>(Y, size, indent) //Вычисляет погрешность позиции текста в пикселях и выравнивает его относительно заданных координат для оси Y
	

	class gradient : public BaseInerface {
		private:
			std::vector<sf::Color> colors;
			sf::VertexArray *rect;
		public:
			/// <param name="rt">Размер и положение хитбокса</param>
			/// <param name="gd">Направление градиента</param>
			/// <param name="first">Первый цвет градиента</param>
			/// <param name="second">Второй цвет градиента</param>
			gradient(const FloatRect &rt, gradient_direction gd, Color first, Color second);
			~gradient();
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="xy">Структура axes_i с координатами по осям X и Y</param>
			void setPosition(const axes_i &xy) override;
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="x">Координаты по оси X</param>
			/// <param name="x">Координаты по оси Y</param>
			void __fastcall setPosition(int x, int y) override;
			void render(RenderWindow &wd) noexcept override;
			void render(RenderWindow *wd) noexcept override;
	};

	class button : public BaseInerface {
		private:
			Font *font;
			Text *txt;
			Color *main_cl, *txt_cl, *active_cl;
			RectangleShape *main, *active_bvl;
		public:
			bool active;
			button(int x, int y, const std::wstring &text, Color maincl, Color textcl, Color activecl);
			~button();
			void setPosition(const axes_i &xy) override;
			void __fastcall setPosition(int x, int y) override;
			void render(RenderWindow &wd) noexcept override;
			void render(RenderWindow *wd) noexcept override;
			void resize(int size);
			void freeze(Camer *camera, const axes_i &xy); //Замораживает позицию компонента относительно камеры
			void freeze(Camer *camera, int x, int y); //Замораживает позицию компонента относительно камеры
			bool __fastcall isAction(int x, int y);
			bool isAction(const axes_i &xy);
	};

	class combo_box : public BaseInerface {
		private:
			struct cell {
				cell(Text txt, int val);
				Text text;
				int value;
			};
			Font *font;
			RectangleShape *main;
			Color *text_cl, *main_cl;
			std::vector<cell*> mass_text;
			std::vector<cell*>::iterator it;
		public:
			bool visible_main, active;
			combo_box(int x, int y, Color maincl, Color textcl);
			~combo_box();
			void setPosition(const axes_i &xy) override;
			void __fastcall setPosition(int x, int y) override;
			void add(const std::wstring &st, int val);
			void next();
			void back();
			bool __fastcall isAction(int x, int y);
			bool isAction(const axes_i &xy);
			std::wstring getText();
			int getValue();
			void render(RenderWindow &wd);
			void render(RenderWindow *wd);
	};

	class check_box : public BaseInerface {
		private:
			Color *main_cl, *border_cl, *check_cl;
			RectangleShape *main, *border, *check;
		public:
			bool isCheck;
			check_box(int x, int y, Color maincl, Color bordercl, Color checkcl);
			~check_box();
			void setPosition(const axes_i &xy) override;
			void __fastcall setPosition(int x, int y) override;
			void render(RenderWindow  &wd) noexcept override;
			void render(RenderWindow *wd) noexcept override;
			void invers(bool operation);
			void invers();
	};

	class text : public BaseInerface {
		private:
			RectangleShape* bevel; //Прямоугольник под текстом 
			Font* font_main;
		public:
			bool visible_bevel; //Видимость прямоугольника под текстом 
			Text* label;
			Color* bevel_cl;
			Color* label_cl;
			text(int x = 0, int y = 0, const std::wstring& txt = L"NON:", Color lbcol = Color::White, Color bvcol = Color::Black);
			~text();
			void setString(const std::wstring &txt) noexcept; //Задает текст 
			void setFont(const String &txt) noexcept; //Путь к фону для текста
			void __fastcall setPosition(int x, int y) noexcept override; //Устанавливает позицию объекта по осям X, Y
			void setPosition(const axes_i &xy) noexcept override; //Устанавливает позицию объекта по осям X, Y 
			void resize(int size = normal) noexcept; //Задает размер объекта в пикселях
			void freeze(Camer* camera, const axes_i &xy); //Замораживает позицию компонента относительно камеры
			void freeze(Camer* camera, int x, int y); //Замораживает позицию компонента относительно камеры
			void render(RenderWindow& wd) noexcept override;
			void render(RenderWindow* wd) noexcept override;
	};
	
	class message {
		private:
			axes_i pos;
			Color *main_cl, *border_cl;
			RectangleShape *main, *border;
			text *txInfo, *txMess;
			gradient *grFirst, *grSecond;
		public:
			bool active;
			button *btOk;
			message(int x, int y, const std::wstring& txt, Color maincl, Color bordercl, Color textcl);
			~message();
			void render(RenderWindow &wd, Camer *camera) noexcept;
			void render(RenderWindow *wd, Camer *camera) noexcept;
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
	};

	class menu {
		private:
			axes_i pos;
			Color *main_cl, *border_cl;
			RectangleShape *main, *border, *blackout;
			text *txMenu;
			gradient *grFirst, *grSecond;
		public:
			bool active, blackout_visible;
			button *btExit, *btContinue, *btOptions;
			menu(Camer* camera, Color maincl, Color bordercl);
			~menu();
			void render(RenderWindow &wd, Camer *camera) noexcept;
			void render(RenderWindow *wd, Camer *camera) noexcept;
	};

	class settings_menu {
		private:
			axes_i pos;
			Color *main_cl, *border_cl;
			RectangleShape *main, *border, *blackout;
			text *txMenuSettings;
			gradient *grFirst, *grSecond;
		public:
			bool active, blackout_visible;
			button *btBack, *btSave;
			text *txVertS, *txFullS, *txScreen, *txSound, *txSoundV;
			check_box *cbVertS, *cbFullS, *cbSound;
			combo_box *combScreen, *combSoundV;
			settings_menu(configuration *cf, Camer *camera, Color maincl, Color bordercl);
			settings_menu(configuration *cf, Color maincl, Color bordercl);
			~settings_menu();
			void backSettings(configuration *cf);
			int saveSettings(configuration *cf);
			void render(RenderWindow &wd, Camer *camera) noexcept;
			void render(RenderWindow *wd, Camer *camera) noexcept;
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
	};

	class main_menu {
		private:
			Color *main_cl;
			RectangleShape *main;
			text *txMainMenu;
			gradient *grFirst, *grSecond;
		public:
			button *btExit, *btStart, *btOptions, *btStartTren;
			main_menu(configuration *cf, Color maincl);
			~main_menu();
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
	};

	class bar : public BaseInerface {
		private:
			int max_bar, min_bar, curr_bar; //Максимальное значение полосы, минимальное значение полосы, текущее значение полосы
			RectangleShape *main; //Прямоугольник под объектом
			RectangleShape *bevel; //Полоса
			Font *font_main;
			Text *label;
		public:
			/// <summary>
			/// Конструктор
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <param name="br_ma">Максимальное значение полосы</param>
			/// <param name="br_mi">Минимальное значение полосы</param>
			/// <param name="name">Текст перед полосой</param>
			/// <param name="mcol">Цвет полосы</param>
			/// <param name="bcol">Цвет прямоугольника под полосой</param>
			/// <param name="tcol">Цвет текста</param>
			/// <returns></returns>
			bar(int x = 0, int y = 0, int br_ma = 100, int br_mi = 0, const std::wstring& name = L"NON:", Color mcol = Color::White, Color bcol = Color::Red, Color tcol = Color::Black);
			bar(); //Конструктор по умолчанию
			~bar(); //Деструктор
			/// <summary>
			/// Изменяет состояние полосы
			/// </summary>
			/// <param name="arg">Новое значение полосы</param>
			/// <returns></returns>
			void __fastcall changeBar(int arg) noexcept; 
			void render(RenderWindow &wd) noexcept override;
			void render(RenderWindow *wd) noexcept override;
			/// <summary>
			/// Устанавливает новую позицию объекта 
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept override;
			/// <summary>
			/// Устанавливает новую позицию объекта
			/// </summary>
			/// <param name="xy">Структура axes_i с координатами по осям X и Y</param>
			void setPosition(const axes_i &xy) noexcept override;
			/// <summary>
			/// Изменяет размер текста
			/// </summary>
			/// <param name="size">Новый размер текста в пикселях</param>
			/// <returns></returns>
			void resize(int size = normal) noexcept; 
			/// <summary>
			/// Замораживает позицию объекта относительно камеры
			/// </summary>
			/// <param name="camera">Указатель на камеру</param>
			/// <param name="xy">Структура с координатами заморозки</param>
			void freeze(Camer* camera, const axes_i &xy); 
			/// <summary>
			/// Замораживает позицию объекта относительно камеры
			/// </summary>
			/// <param name="camera">Указатель на камеру</param>
			/// <param name="x">Координаты по оси X</param>
			/// <param name="y">Координаты по оси Y</param>
			void freeze(Camer* camera, int x, int y);
	};

	class min_bar : public BaseInerface {
		private:
			int max_br, min_br, curr_br; //Максимальное значение полосы, минимальное значение полосы, текущее значение полосы 
			RectangleShape* main; //Прямоугольник под объектом
			RectangleShape* bevel; //Полоса
		public:
			/// <summary>
			/// Конструктор
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <param name="br_ma">Максимальное значение полосы</param>
			/// <param name="br_mi">Минимальное значение полосы</param>
			/// <param name="mcol">Цвет полосы</param>
			/// <param name="bcol">Цвет прямоугольника под полосой</param>
			/// <returns></returns>
			min_bar(int x, int y, int br_ma, int br_mi, Color mcol, Color bcol);
			~min_bar();
			/// <summary>
			/// Устанавливает позицию объекта по осям X, Y
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept override;
			/// <summary>
			/// Устанавливает позицию объекта по осям X, Y
			/// </summary>
			/// <param name="xy">Структура axes_i с координатами по осям X и Y</param>
			void setPosition(const axes_i &xy) noexcept override;
			/// <summary>
			/// Изменяет состояние полосы
			/// </summary>
			/// <param name="arg">Новое значение полосы</param>
			/// <returns></returns>
			void changeBar(int arg) noexcept;
			void render(RenderWindow& wd) noexcept override;
			void render(RenderWindow* wd) noexcept override;

	};

	class multiline_text {
		private:
			int SIZE_MSTX, max_length; //Размерность массива(кол-во строк в нем), максимальная длина текста в пикселях
			axes_f pos; //Структура axes_i с координатами по осям X и Y
			RectangleShape *bevel; //Прямоугольник под текстом  
			Font *font_main;
		public:
			using dyn_vector_text = std::vector<Text*>;
			dyn_vector_text mass_string;
			bool visible; //Видимость объекта
			bool visible_bevel; //Видимость прямоугольника под текстом
			Color *bevel_cl;
			Color *label_cl;
			/// <summary>
			/// Конструктор
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <param name="lbcol">Цвет текста</param>
			/// <param name="bvcol">Цвет прямоугольника под текстом</param>
			/// <returns></returns>
			multiline_text(float x = 0, float y = 0, Color lbcol = Color::White, Color bvcol = Color::Black);
			~multiline_text() noexcept;
			/// <summary>
			/// Изменяет размер текста
			/// </summary>
			/// <param name="size">Размер текста в пикселях</param>
			/// <returns></returns>
			void resize(int size = normal) noexcept;
			/// <summary>
			/// Добавление новой линии текста
			/// </summary>
			/// <param name="txt">Текст на новой линии</param>
			/// <returns></returns>
			void add(const std::wstring &txt) noexcept;
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
	};

	class background_color {
		private:
			axes_i pos;
			RectangleShape *background;
		public:
			bool visible;
			/// <summary>
			/// Конструктор
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <param name="cl">Цвет</param>
			/// <param name="cf">Указатель на структуру настроек</param>
			background_color(int x, int y, Color cl, configuration *cf);
			~background_color();
			/// <summary>
			/// Изменяет цвет
			/// </summary>
			/// <param name="cl">Новый Цвет</param>
			void setColor(Color cl);
			void render(RenderWindow& wd) noexcept;
			void render(RenderWindow* wd) noexcept;
	};

}; //Конец пространства имен _interface

class Character : public BaseCharacter { 
	private:
		int direction, last_direction; 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Character(Texture* ptr_texture, float X_POS, float Y_POS, int hp); 
		~Character(); 
		void __fastcall setPosition(float x, float y) noexcept override; 
		void setPosition(const axes_f &xy) noexcept override; 
		float getPositionX_forCamer() noexcept; 
		float getPositionY_forCamer() noexcept; 
		void __fastcall move(float time, int direct = 0) noexcept; 
		void __fastcall move(float time) noexcept; 
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept override;
		void render(RenderWindow* wd) noexcept override;
};	

class DestroerCastle : public BaseCharacter {
	private:
		int direction, last_direction; 
		_interface::min_bar* HP;
	public:
		DestroerCastle(Texture* ptr_texture, float X_POS, float Y_POS, int hp); 
		~DestroerCastle(); 
		void __fastcall setPosition(float x, float y) noexcept override; 
		void setPosition(const axes_f &xy) noexcept override; 
		void __fastcall move(float time, int direct) noexcept; 
		//void move(float time) noexcept; 
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept override;
		void render(RenderWindow* wd) noexcept override;
};

class Spearman : public BaseCharacter {
	private:
		int direction, last_direction; 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Spearman(Texture* ptr_texture, float X_POS, float Y_POS, int hp);
		~Spearman(); 
		void __fastcall setPosition(float x, float y) noexcept override;
		void setPosition(const axes_f &xy) noexcept override;
		void __fastcall move(float time, int direct) noexcept;
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept override;
		void render(RenderWindow* wd) noexcept override;
};

class IceBall : public BaseCharacter {
	private:
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		IceBall(Texture* ptr_texture, float X_POS, float Y_POS, int hp);
		~IceBall();
		void __fastcall setPosition(float x, float y) noexcept override;
		void setPosition(const axes_f &xy) noexcept override;
		void __fastcall update(float time) noexcept;
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept override;
		void render(RenderWindow* wd) noexcept override;
};

class Meteor : public ObjectAnimated {
	private:
		Sprite* sprt_meteor;
		Vector2f start_point, mouse_point;
		float current_len, full_len;
		void isReachedPoint(float time);
	public:
		bool cooldown, reached_point, is_sound_play;
		Meteor(Texture* ptr_texture, Texture* ptr_texture2, float X, float Y);
		~Meteor();
		void __fastcall update(float time) override final;
		void render(RenderWindow& wd) override final;
		void render(RenderWindow* wd) override final;
	};

#endif