#pragma once
#include "Map.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace sf;

#ifndef _INTERFACE_G
#define _INTERFACE_G

typedef struct settings {
	bool fullScreen;
	bool verticalSync;
	unsigned int screenHeight;
	unsigned int screenWidth;
	int textSize;
	int saveSettings();
	int loadSettings();
	int createSettings(int ScreenW, int ScreenH, bool VertS, int TxtS, bool FullS);
	//1280 x 720
	//1600 x 900
	//1600 x 1200
	//1920 x 1080
	//2048 x 1152
	//2560 x 1440
} configuration;

/// <summary>
/// Преобразует целочисленное число к строке
/// </summary>
/// <typeparam name="arg">Выходная строка</typeparam>
/// <param name="input">Целочисленное число</param>
/// <returns>Строка</returns>
template<typename arg>
arg intToStr(int input) {
	std::ostringstream output;
	output << input;
	return output.str();
}

#define intToStr(type, input) intToStr<type>(input)

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
	int screen_H, screen_W; //координата объекта по оси X, координата объекта по оси Y, ширина камеры в пикселях, высота камеры в пикселях
public:
	/// <summary>
	/// Конструктор
	/// </summary>
	/// <param name="x">Позиция по оси X</param>
	/// <param name="y">озиция по оси Y</param>
	/// <param name="screen_width">Ширина вида в пикселях</param>
	/// <param name="screen_height">Высота вида в пикселях</param>
	/// <returns></returns>
	Camer(int x, int y, int screen_width, int screen_height); //координата объекта по оси X, координата объекта по оси Y, высота камеры в пикселях, ширина камеры в пикселях
	Camer(); //конструктор по умолчанию
	~Camer(); //деструктор
	View *Vid;
	/// <summary>
	/// Перемещает камеру
	/// </summary>
	/// <param name="x">Новая позиция по оси X</param>
	/// <param name="y">Новая позиция по оси Y</param>
	void __fastcall move(int x, int y); //движение камеры
	/// <summary>
	/// Устанавливает зум
	/// </summary>
	/// <param name="zoom">Прибавка к текущему зуму, зум по умолчанию равен 1</param>
	void setZoom(float zoom); //изменение размера изображения
	/// <summary>
	/// Возвращает позицию 
	/// </summary>
	/// <returns>Структура axes_i</returns>
	axes_i getPosition() noexcept; //возвращает координаты камеры по оси X и Y
	/// <summary>
	/// Возвращает ширину вида камеры в пикселях 
	/// </summary>
	/// <returns>Количество пикселей в int</returns>
	int getScreenWidth() noexcept; //возвращает высоту камеры в пикселях
	/// <summary>
	/// Возвращает высоту вида камеры в пикселях 
	/// </summary>
	/// <returns>Количество пикселей в int</returns>
	int getScreenHeight() noexcept; //возвращает ширину камеры в пикселях
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
		Collision(IntRect rect);
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
		/// <param name="xy">Структура axes_i</param>
		void setPosition(axes_i xy);
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
		void setBounds(IntRect rect);
		void render(RenderWindow& wd);
		void render(RenderWindow* wd);
};

typedef Collision Trigger;

class BaseCharacter {
	protected:
		axes_f pos;
		Texture *texture;
		Sprite *sprt;
		FloatRect fl_rect;
		float frame, timer_cooldown;
		bool zeroing;
	public:
		BaseCharacter();
		BaseCharacter(Image *i, int x, int y, int _hp);
		BaseCharacter(Image* i, axes_f xy, int _hp);
		~BaseCharacter();
		bool cooldown, isDead, visible;
		int health;
		virtual axes_f getPosition();
		virtual void __fastcall setPosition(int x, int y);
		virtual void __fastcall setPosition(axes_f xy);
		virtual FloatRect getSize();
		virtual void render(RenderWindow& wd);
		virtual void render(RenderWindow* wd);
};

class ObjectStatic {
	protected:
		axes_i pos;
		Texture *texture;
		Sprite *sprt;
		FloatRect react_obj_stat;
	public:
		Collision *rect_collis;
		/// <summary>
		/// Конструктор
		/// </summary>
		/// <param name="i">Указатель на изображение</param>
		/// <param name="X">Координаты по оси X</param>
		/// <param name="Y">Координаты по оси Y</param>
		/// <returns></returns>
		ObjectStatic(Image* i, float X, float Y);
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
		virtual void setRect(IntRect bound);
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="x">Ось X</param>
		/// <param name="y">Ось Y</param>
		virtual void __fastcall setPosition(int x, int y);
		/// <summary>
		/// Устанавливает позицию хитбокса по осям X и Y
		/// </summary>
		/// <param name="xy">Структура axes_i</param>
		virtual void setPosition(axes_i xy);
		virtual void render(RenderWindow &wd);
		virtual void render(RenderWindow *wd);
};

class ObjectAnimated : public ObjectStatic {
	private:
		float frame;
	public:
		bool end, cooldown;
		/// <summary>
		/// Конструктор
		/// </summary>
		/// <param name="i">Указатель на изображение</param>
		/// <param name="X">Координаты по оси X</param>
		/// <param name="Y">Координаты по оси Y</param>
		/// <returns></returns>
		ObjectAnimated(Image* i, float X, float Y);
		~ObjectAnimated();
		/// <summary>
		/// Обновление объекта
		/// </summary>
		/// <param name="time">Время, чем больше переменная, тем быстрее происходит обновление</param>
		void __fastcall update(float time);
};

namespace _interface {

	class BaseInerface {
		protected:
			axes_i pos;
			FloatRect fl_rect;
		public:
			BaseInerface();
			~BaseInerface();
			bool visible;
			virtual axes_i getPosition();
			virtual void setPosition(axes_i xy);
			virtual void __fastcall setPosition(int x, int y);
			virtual FloatRect getSize();
	};

	enum gradient_direction {
		leftOnRight,
		rightOnLeft,
		topOnBottom,
		bottomOnTop
	};

	enum text_size { 
		small = 24,   // отклонение для оси X = 0.01 для Y = 7
		normal = 30,  // отклонение для оси X = 1 для Y = 8
		medium = 48,  // отклонение для оси X = 2 для Y = 12
		big = 68      // отклонение для оси X = 2 для Y = 17
	};
	
	template<typename arg>
	arg repoz_x(arg X, arg size, arg indent = 0) {
		arg A = 0;
		A = size - X;
		X = X - A;
		return X + indent;
	}

	template<typename arg>
	arg repoz_y(arg Y, arg size, arg indent = 0) {
		arg A = 0;
		A = size - Y;
		Y = Y - A;
		return Y + indent;
	}

	#define repoz_x(type, X, size, indent) repoz_x <type>(X, size, indent) //вычисляет погрешность позиции текста в пикселях и выравнивает его относительно заданных координат для оси X
	#define repoz_y(type, Y, size, indent) repoz_y <type>(Y, size, indent) //вычисляет погрешность позиции текста в пикселях и выравнивает его относительно заданных координат для оси Y
	

	class gradient {
		private:
			axes_i pos;
			std::vector<sf::Color> colors;
			sf::VertexArray *rect;
			sf::FloatRect *rect_pos;
		public:
			bool visible;
			/// <param name="rt">Размер и положение хитбокса</param>
			/// <param name="gd">Направление градиента</param>
			/// <param name="first">Первый цвет градиента</param>
			/// <param name="second">Второй цвет градиента</param>
			gradient(FloatRect rt, gradient_direction gd, Color first, Color second);
			~gradient();
			/// <summary>
			/// Возвращает хитбокс градиента
			/// </summary>
			/// <returns>Структура FloatRect</returns>
			FloatRect getSize();
			/// <summary>
			/// Возвращает позицию
			/// </summary>
			/// <returns>Структура axes_i</returns>
			axes_i getPosition();
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="xy">Структура с координатами по осям X и Y</param>
			void setPosition(axes_i xy);
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="x">Координаты по оси X</param>
			/// <param name="x">Координаты по оси Y</param>
			void __fastcall setPosition(int x, int y);
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
	};

	class button {
		private:
			axes_i pos;
			Font *font;
			Text *txt;
			Color *main_cl, *txt_cl, *active_cl;
			RectangleShape *main, *active_bvl;
			FloatRect rect_bt;
		public:
			bool visible, active;
			button(int x, int y, std::wstring text, Color maincl, Color textcl, Color activecl);
			~button();
			axes_i getPosition();
			FloatRect getSize();
			void setPosition(axes_i xy);
			void __fastcall setPosition(int x, int y);
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
			void resize(int size);
			void freeze(Camer *camera, axes_i xy); //замораживает позицию компонента относительно камеры
			void freeze(Camer *camera, int x, int y); //замораживает позицию компонента относительно камеры
			bool __fastcall isAction(int x, int y);
			bool isAction(axes_i xy);
	};

	class combo_box {
		private:
			struct cell {
				cell(Text txt, int val);
				Text text;
				int value;
			};
			axes_i pos;
			Font *font;
			RectangleShape *main;
			Color *text_cl, *main_cl;
			std::vector<cell*> mass_text;
			std::vector<cell*>::iterator it;
			FloatRect rect_cmb;
		public:
			bool visible, visible_main, active;
			combo_box(int x, int y, Color maincl, Color textcl);
			~combo_box();
			axes_i getPosition();
			void setPosition(axes_i xy);
			void __fastcall setPosition(int x, int y);
			void add(std::wstring st, int val);
			void next();
			void back();
			bool __fastcall isAction(int x, int y);
			bool isAction(axes_i xy);
			std::wstring getText();
			int getValue();
			FloatRect getSize();
			void render(RenderWindow &wd);
			void render(RenderWindow *wd);
	};

	class check_box {
		private:
			axes_i pos;
			Color *main_cl, *border_cl, *check_cl;
			RectangleShape *main, *border, *check;
			FloatRect rect_cb;
		public:
			bool visible, isCheck;
			check_box(int x, int y, Color maincl, Color bordercl, Color checkcl);
			~check_box();
			axes_i getPosition();
			FloatRect getSize();
			void setPosition(axes_i xy);
			void __fastcall setPosition(int x, int y);
			void render(RenderWindow  &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
			void invers(bool operation);
			void invers();
	};

	class text {
		private:
			axes_i pos; //координата объекта по оси Y, координата объекта по оси X
			RectangleShape* bevel; //прямоугольник под текстом 
			Font* font_main;
			FloatRect rect_tx;
		public:
			bool visible; //видимость объекта
			bool visible_bevel; //видимость прямоугольника под текстом 
			Text* label;
			Color* bevel_cl;
			Color* label_cl;
			text(int x, int y, std::wstring txt, Color lbcol, Color bvcol); //координата объекта по оси X, координата объекта по оси Y, текст, цвет текста, цвет прямоугольника
			~text();
			void setString(std::wstring txt) noexcept; //задаетс текст 
			void setFont(String txt) noexcept; //путь к фону для текста
			FloatRect getSize();
			axes_i getPosition() noexcept; //возвращает координаты объекта по оси X и Y
			void __fastcall setPosition(int x, int y) noexcept; //устанавливает позицию объекта по осям X, Y 
			void setPosition(axes_i xy) noexcept; //устанавливает позицию объекта по осям X, Y 
			void resize(int size) noexcept; //задает размер объекта в пикселях
			void freeze(Camer* camera, axes_i xy); //замораживает позицию компонента относительно камеры
			void freeze(Camer* camera, int x, int y); //замораживает позицию компонента относительно камеры
			void render(RenderWindow& wd) noexcept; // 
			void render(RenderWindow* wd) noexcept;
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
			message(int x, int y, std::wstring txt, Color maincl, Color bordercl, Color textcl);
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
			text *txVertS, *txFullS, *txScreen;
			check_box *cbVertS, *cbFullS;
			combo_box *combScreen;
			settings_menu(configuration *cf,Camer *camera, Color maincl, Color bordercl);
			settings_menu(configuration *cf, Color maincl, Color bordercl);
			~settings_menu();
			void backSettings(configuration *cf);
			void saveSettings(configuration *cf);
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

	class bar {
		private:
			axes_i pos;
			int max_bar, min_bar, curr_bar; //координата объекта по оси Y, координата объекта по оси X, макс значение полосы, мин значение полосы, текущее значение полосы 
			RectangleShape *main; //прямоугольник под объектом
			RectangleShape *bevel; //полоса
			Font *font_main;
			Text *label;
			FloatRect rect_br;
		public:
			bool visible; //видимость объекта
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
			bar(int x, int y, int br_ma, int br_mi, std::wstring name, Color mcol, Color bcol, Color tcol); 
			bar(); //конструктор по умолчанию
			~bar(); //деструктор
			/// <summary>
			/// Изменяет состояние полосы
			/// </summary>
			/// <param name="arg">Новое значение полосы</param>
			/// <returns></returns>
			void __fastcall changeBar(int arg) noexcept; //изменяет состояние полосы 
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
			/// <summary>
			/// Устанавливает позицию 
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept; //устанавливает позицию объекта по осям X, Y 
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="xy">Структура с координатами по осям X и Y</param>
			void setPosition(axes_i xy) noexcept; //устанавливает позицию объекта по осям X, Y 
			/// <summary>
			/// Изменяет размер текста
			/// </summary>
			/// <param name="size">Новый размер текста в пикселях</param>
			/// <returns></returns>
			void resize(int size) noexcept; //задает размер объекта в пикселях 
			/// <summary>
			/// Замораживает позицию объекта относительно камеры
			/// </summary>
			/// <param name="camera">Указатель на камеру</param>
			/// <param name="xy">Структура с координатами заморозки</param>
			void freeze(Camer* camera, axes_i xy); //замораживает позицию компонента относительно камеры
			/// <summary>
			/// Замораживает позицию объекта относительно камеры
			/// </summary>
			/// <param name="camera">Указатель на камеру</param>
			/// <param name="x">Координаты по оси X</param>
			/// <param name="y">Координаты по оси Y</param>
			void freeze(Camer* camera, int x, int y); //замораживает позицию компонента относительно камеры
			/// <summary>
			/// Возвращает позицию
			/// </summary>
			/// <returns>Структура axes_i</returns>
			axes_i getPosition() noexcept; //возвращает координаты объекта по оси X и Y
			/// <summary>
			/// Возвращает хитбокс градиента
			/// </summary>
			/// <returns>Структура FloatRect</returns>
			FloatRect getSize();
	};

	class min_bar {
		private:
			axes_i pos;
			int max_br, min_br, curr_br; //координата объекта по оси Y, координата объекта по оси X, макс значение полосы, мин значение полосы, текущее значение полосы 
			RectangleShape* main; //прямоугольник под объектом
			RectangleShape* bevel; //полоса
			FloatRect rect_br;
		public:
			bool visible;
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
			/// Устанавливает позицию 
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept; //устанавливает позицию объекта по осям X, Y 
			/// <summary>
			/// Устанавливает позицию объекта по осям X и Y
			/// </summary>
			/// <param name="xy">Структура с координатами по осям X и Y</param>
			void setPosition(axes_i xy) noexcept;
			/// <summary>
			/// Возвращает позицию
			/// </summary>
			/// <returns>Структура axes_i</returns>
			axes_i getPosition();
			/// <summary>
			/// Возвращает хитбокс градиента
			/// </summary>
			/// <returns>Структура FloatRect</returns>
			FloatRect getSize();
			/// <summary>
			/// Изменяет состояние полосы
			/// </summary>
			/// <param name="arg">Новое значение полосы</param>
			/// <returns></returns>
			void changeBar(int arg) noexcept;
			void render(RenderWindow& wd) noexcept;
			void render(RenderWindow* wd) noexcept;

	};

	class multiline_text {
		private:
			int SIZE_MSTX, max_length; //размерность массива(кол-во строк в нем), максимальная длина текста в пикселях 
			axes_f pos; //координата объекта по оси Y, координата объекта по оси X
			RectangleShape *bevel; //прямоугольник под текстом 
			Font *font_main;
		public:
			Text* mass; //указатель на массив строк
			Text* ptr; //указатель на первый элемент массива строк (итерратор)
			bool visible; //видимость объекта
			bool visible_bevel; //видимость прямоугольника под текстом
			Color *bevel_cl;
			Color *label_cl;
			/// <summary>
			/// Конструктор
			/// </summary>
			/// <param name="x">Позиция по оси X</param>
			/// <param name="y">Позиция по оси Y</param>
			/// <param name="lbcol">Цвет текста</param>
			/// <param name="bvcol">Цвет ограничивающего прямоугольника под текстом</param>
			/// <returns></returns>
			multiline_text(float x, float y, Color lbcol, Color bvcol); //координата объекта по оси X, координата объекта по оси Y, цвет текста, цвет прямоугольника
			~multiline_text() noexcept;
			/// <summary>
			/// Изменяет размер текста
			/// </summary>
			/// <param name="size">Размер текста в пикселях</param>
			/// <returns></returns>
			void resize(int size) noexcept; //задает размер объекта в пикселях
			/// <summary>
			/// Добавление новой линии текста
			/// </summary>
			/// <param name="txt">Текст на новой линии</param>
			/// <returns></returns>
			void add(std::wstring txt) noexcept; //добавляет новую строку в массив
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
			/// <returns></returns>
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

}; //конец пространства имен 

class Character : public BaseCharacter { //TODO dx, dy, speed, 
	private:
		int direction, last_direction; //ширина изображения, высота изображения, положение спрайта по оси X, положение спрайта по оси Y, направление движения персонажа 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Character(Image* ptr_on_img, float X_POS, float Y_POS, int hp); //путь к спрайту, координата объекта по оси X, координата объекта по оси Y, ширина изображения, высота изображения,
		~Character(); //деструктор
		void __fastcall setPosition(float X, float Y) noexcept; //устанавливает позицию спрайта по осям X, Y
		void setPosition(axes_f XY) noexcept; //устанавливает позицию спрайта по осям X, Y
		void setImage(Image *ptr_on_img);
		float getPositionX_forCamer() noexcept; //возвращает центр координата спрайта по оси X
		float getPositionY_forCamer() noexcept; //возвращает центр координата спрайта по оси Y
		void __fastcall move(float time, int direct) noexcept; //перезаписывает положение спрайта
		void __fastcall move(float time) noexcept; //перезаписывает положение спрайта
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};	

class DestroerCastle : public BaseCharacter {
	private:
		int direction, last_direction; //ширина изображения, высота изображения, положение спрайта по оси X, положение спрайта по оси Y, направление движения персонажа 
		_interface::min_bar* HP;
	public:
		DestroerCastle(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //путь к спрайту, координата объекта по оси X, координата объекта по оси Y, ширина изображения, высота изображения,
		~DestroerCastle(); //деструктор
		void __fastcall setPosition(float X, float Y) noexcept; //устанавливает позицию спрайта по осям X, Y
		void setPosition(axes_f XY) noexcept; //устанавливает позицию спрайта по осям X, Y
		//float getPositionX_forCamer() noexcept; //возвращает центр координата спрайта по оси X
		//float getPositionY_forCamer() noexcept; //возвращает центр координата спрайта по оси Y
		void __fastcall move(float time, int direct) noexcept; //перезаписывает положение спрайта
		//void move(float time) noexcept; //перезаписывает положение спрайта
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};

class Spearman : public BaseCharacter {
	private:
		int direction, last_direction; //ширина изображения, высота изображения, положение спрайта по оси X, положение спрайта по оси Y, направление движения персонажа 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Spearman(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //путь к спрайту, координата объекта по оси X, координата объекта по оси Y, ширина изображения, высота изображения,
		~Spearman(); //деструктор
		void __fastcall setPosition(float X, float Y) noexcept; //устанавливает позицию спрайта по осям X, Y
		void setPosition(axes_f XY) noexcept; //устанавливает позицию спрайта по осям X, Y
		void __fastcall move(float time, int direct) noexcept; //перезаписывает положение спрайта
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};

class IceBall : public BaseCharacter {
	private:
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		IceBall(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //путь к спрайту, координата объекта по оси X, координата объекта по оси Y, ширина изображения, высота изображения,
		~IceBall(); //деструктор
		void __fastcall setPosition(float X, float Y) noexcept; //устанавливает позицию спрайта по осям X, Y
		void setPosition(axes_f XY) noexcept; //устанавливает позицию спрайта по осям X, Y
		void __fastcall update(float time) noexcept; //перезаписывает положение спрайта
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};

#endif