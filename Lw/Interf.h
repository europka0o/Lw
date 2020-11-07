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
/// ����������� ������������� ����� � ������
/// </summary>
/// <typeparam name="arg">�������� ������</typeparam>
/// <param name="input">������������� �����</param>
/// <returns>������</returns>
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
	int screen_H, screen_W; //���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ ������ � ��������, ������ ������ � ��������
public:
	/// <summary>
	/// �����������
	/// </summary>
	/// <param name="x">������� �� ��� X</param>
	/// <param name="y">������ �� ��� Y</param>
	/// <param name="screen_width">������ ���� � ��������</param>
	/// <param name="screen_height">������ ���� � ��������</param>
	/// <returns></returns>
	Camer(int x, int y, int screen_width, int screen_height); //���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ ������ � ��������, ������ ������ � ��������
	Camer(); //����������� �� ���������
	~Camer(); //����������
	View *Vid;
	/// <summary>
	/// ���������� ������
	/// </summary>
	/// <param name="x">����� ������� �� ��� X</param>
	/// <param name="y">����� ������� �� ��� Y</param>
	void __fastcall move(int x, int y); //�������� ������
	/// <summary>
	/// ������������� ���
	/// </summary>
	/// <param name="zoom">�������� � �������� ����, ��� �� ��������� ����� 1</param>
	void setZoom(float zoom); //��������� ������� �����������
	/// <summary>
	/// ���������� ������� 
	/// </summary>
	/// <returns>��������� axes_i</returns>
	axes_i getPosition() noexcept; //���������� ���������� ������ �� ��� X � Y
	/// <summary>
	/// ���������� ������ ���� ������ � �������� 
	/// </summary>
	/// <returns>���������� �������� � int</returns>
	int getScreenWidth() noexcept; //���������� ������ ������ � ��������
	/// <summary>
	/// ���������� ������ ���� ������ � �������� 
	/// </summary>
	/// <returns>���������� �������� � int</returns>
	int getScreenHeight() noexcept; //���������� ������ ������ � ��������
	/// <summary>
	/// ���������� ������ � �������: ������, ������, ������� �������� �����������
	/// </summary>
	/// <returns>��������� FloatRect</returns>
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
		/// �����������
		/// </summary>
		/// <param name="rect">�������</param>
		/// <returns></returns>
		Collision(IntRect rect);
		~Collision();
		/// <summary>
		/// ������������� ������� �������� �� ���� X � Y
		/// </summary>
		/// <param name="x">��� X</param>
		/// <param name="y">��� Y</param>
		void __fastcall setPosition(int x, int y);
		/// <summary>
		/// ������������� ������� �������� �� ���� X � Y
		/// </summary>
		/// <param name="xy">��������� axes_i</param>
		void setPosition(axes_i xy);
		/// <summary>
		/// ���������� ������� �������� �� ���� X � Y
		/// </summary>
		/// <returns>��������� axes_i</returns>
		axes_i getPosition();
		/// <summary>
		/// ���������� ������ � �������: ������, ������, ������� �������� �����������
		/// </summary>
		/// <returns>��������� IntRect</returns>
		IntRect getBounds();
		/// <summary>
		/// ������������� ����� ������ � ��������
		/// </summary>
		/// <param name="rect">��������� IntRect � ������ �����������</param>
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
		/// �����������
		/// </summary>
		/// <param name="i">��������� �� �����������</param>
		/// <param name="X">���������� �� ��� X</param>
		/// <param name="Y">���������� �� ��� Y</param>
		/// <returns></returns>
		ObjectStatic(Image* i, float X, float Y);
		~ObjectStatic();
		/// <summary>
		/// ���������� ������� 
		/// </summary>
		/// <returns>��������� axes_i</returns>
		virtual axes_i getPosition();
		/// <summary>
		/// ���������� ������ � �������: ������, ������, ������� �������� �����������
		/// </summary>
		/// <returns>��������� IntRect</returns>
		virtual FloatRect getSize();
		/// <summary>
		/// ������������� ����� ������ � ��������
		/// </summary>
		/// <param name="bound">��������� IntRect � ������ �����������</param>
		virtual void setRect(IntRect bound);
		/// <summary>
		/// ������������� ������� �������� �� ���� X � Y
		/// </summary>
		/// <param name="x">��� X</param>
		/// <param name="y">��� Y</param>
		virtual void __fastcall setPosition(int x, int y);
		/// <summary>
		/// ������������� ������� �������� �� ���� X � Y
		/// </summary>
		/// <param name="xy">��������� axes_i</param>
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
		/// �����������
		/// </summary>
		/// <param name="i">��������� �� �����������</param>
		/// <param name="X">���������� �� ��� X</param>
		/// <param name="Y">���������� �� ��� Y</param>
		/// <returns></returns>
		ObjectAnimated(Image* i, float X, float Y);
		~ObjectAnimated();
		/// <summary>
		/// ���������� �������
		/// </summary>
		/// <param name="time">�����, ��� ������ ����������, ��� ������� ���������� ����������</param>
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
		small = 24,   // ���������� ��� ��� X = 0.01 ��� Y = 7
		normal = 30,  // ���������� ��� ��� X = 1 ��� Y = 8
		medium = 48,  // ���������� ��� ��� X = 2 ��� Y = 12
		big = 68      // ���������� ��� ��� X = 2 ��� Y = 17
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

	#define repoz_x(type, X, size, indent) repoz_x <type>(X, size, indent) //��������� ����������� ������� ������ � �������� � ����������� ��� ������������ �������� ��������� ��� ��� X
	#define repoz_y(type, Y, size, indent) repoz_y <type>(Y, size, indent) //��������� ����������� ������� ������ � �������� � ����������� ��� ������������ �������� ��������� ��� ��� Y
	

	class gradient {
		private:
			axes_i pos;
			std::vector<sf::Color> colors;
			sf::VertexArray *rect;
			sf::FloatRect *rect_pos;
		public:
			bool visible;
			/// <param name="rt">������ � ��������� ��������</param>
			/// <param name="gd">����������� ���������</param>
			/// <param name="first">������ ���� ���������</param>
			/// <param name="second">������ ���� ���������</param>
			gradient(FloatRect rt, gradient_direction gd, Color first, Color second);
			~gradient();
			/// <summary>
			/// ���������� ������� ���������
			/// </summary>
			/// <returns>��������� FloatRect</returns>
			FloatRect getSize();
			/// <summary>
			/// ���������� �������
			/// </summary>
			/// <returns>��������� axes_i</returns>
			axes_i getPosition();
			/// <summary>
			/// ������������� ������� ������� �� ���� X � Y
			/// </summary>
			/// <param name="xy">��������� � ������������ �� ���� X � Y</param>
			void setPosition(axes_i xy);
			/// <summary>
			/// ������������� ������� ������� �� ���� X � Y
			/// </summary>
			/// <param name="x">���������� �� ��� X</param>
			/// <param name="x">���������� �� ��� Y</param>
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
			void freeze(Camer *camera, axes_i xy); //������������ ������� ���������� ������������ ������
			void freeze(Camer *camera, int x, int y); //������������ ������� ���������� ������������ ������
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
			axes_i pos; //���������� ������� �� ��� Y, ���������� ������� �� ��� X
			RectangleShape* bevel; //������������� ��� ������� 
			Font* font_main;
			FloatRect rect_tx;
		public:
			bool visible; //��������� �������
			bool visible_bevel; //��������� �������������� ��� ������� 
			Text* label;
			Color* bevel_cl;
			Color* label_cl;
			text(int x, int y, std::wstring txt, Color lbcol, Color bvcol); //���������� ������� �� ��� X, ���������� ������� �� ��� Y, �����, ���� ������, ���� ��������������
			~text();
			void setString(std::wstring txt) noexcept; //������� ����� 
			void setFont(String txt) noexcept; //���� � ���� ��� ������
			FloatRect getSize();
			axes_i getPosition() noexcept; //���������� ���������� ������� �� ��� X � Y
			void __fastcall setPosition(int x, int y) noexcept; //������������� ������� ������� �� ���� X, Y 
			void setPosition(axes_i xy) noexcept; //������������� ������� ������� �� ���� X, Y 
			void resize(int size) noexcept; //������ ������ ������� � ��������
			void freeze(Camer* camera, axes_i xy); //������������ ������� ���������� ������������ ������
			void freeze(Camer* camera, int x, int y); //������������ ������� ���������� ������������ ������
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
			int max_bar, min_bar, curr_bar; //���������� ������� �� ��� Y, ���������� ������� �� ��� X, ���� �������� ������, ��� �������� ������, ������� �������� ������ 
			RectangleShape *main; //������������� ��� ��������
			RectangleShape *bevel; //������
			Font *font_main;
			Text *label;
			FloatRect rect_br;
		public:
			bool visible; //��������� �������
			/// <summary>
			/// �����������
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <param name="br_ma">������������ �������� ������</param>
			/// <param name="br_mi">����������� �������� ������</param>
			/// <param name="name">����� ����� �������</param>
			/// <param name="mcol">���� ������</param>
			/// <param name="bcol">���� �������������� ��� �������</param>
			/// <param name="tcol">���� ������</param>
			/// <returns></returns>
			bar(int x, int y, int br_ma, int br_mi, std::wstring name, Color mcol, Color bcol, Color tcol); 
			bar(); //����������� �� ���������
			~bar(); //����������
			/// <summary>
			/// �������� ��������� ������
			/// </summary>
			/// <param name="arg">����� �������� ������</param>
			/// <returns></returns>
			void __fastcall changeBar(int arg) noexcept; //�������� ��������� ������ 
			void render(RenderWindow &wd) noexcept;
			void render(RenderWindow *wd) noexcept;
			/// <summary>
			/// ������������� ������� 
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept; //������������� ������� ������� �� ���� X, Y 
			/// <summary>
			/// ������������� ������� ������� �� ���� X � Y
			/// </summary>
			/// <param name="xy">��������� � ������������ �� ���� X � Y</param>
			void setPosition(axes_i xy) noexcept; //������������� ������� ������� �� ���� X, Y 
			/// <summary>
			/// �������� ������ ������
			/// </summary>
			/// <param name="size">����� ������ ������ � ��������</param>
			/// <returns></returns>
			void resize(int size) noexcept; //������ ������ ������� � �������� 
			/// <summary>
			/// ������������ ������� ������� ������������ ������
			/// </summary>
			/// <param name="camera">��������� �� ������</param>
			/// <param name="xy">��������� � ������������ ���������</param>
			void freeze(Camer* camera, axes_i xy); //������������ ������� ���������� ������������ ������
			/// <summary>
			/// ������������ ������� ������� ������������ ������
			/// </summary>
			/// <param name="camera">��������� �� ������</param>
			/// <param name="x">���������� �� ��� X</param>
			/// <param name="y">���������� �� ��� Y</param>
			void freeze(Camer* camera, int x, int y); //������������ ������� ���������� ������������ ������
			/// <summary>
			/// ���������� �������
			/// </summary>
			/// <returns>��������� axes_i</returns>
			axes_i getPosition() noexcept; //���������� ���������� ������� �� ��� X � Y
			/// <summary>
			/// ���������� ������� ���������
			/// </summary>
			/// <returns>��������� FloatRect</returns>
			FloatRect getSize();
	};

	class min_bar {
		private:
			axes_i pos;
			int max_br, min_br, curr_br; //���������� ������� �� ��� Y, ���������� ������� �� ��� X, ���� �������� ������, ��� �������� ������, ������� �������� ������ 
			RectangleShape* main; //������������� ��� ��������
			RectangleShape* bevel; //������
			FloatRect rect_br;
		public:
			bool visible;
			/// <summary>
			/// �����������
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <param name="br_ma">������������ �������� ������</param>
			/// <param name="br_mi">����������� �������� ������</param>
			/// <param name="mcol">���� ������</param>
			/// <param name="bcol">���� �������������� ��� �������</param>
			/// <returns></returns>
			min_bar(int x, int y, int br_ma, int br_mi, Color mcol, Color bcol);
			~min_bar();
			/// <summary>
			/// ������������� ������� 
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <returns></returns>
			void __fastcall setPosition(int x, int y) noexcept; //������������� ������� ������� �� ���� X, Y 
			/// <summary>
			/// ������������� ������� ������� �� ���� X � Y
			/// </summary>
			/// <param name="xy">��������� � ������������ �� ���� X � Y</param>
			void setPosition(axes_i xy) noexcept;
			/// <summary>
			/// ���������� �������
			/// </summary>
			/// <returns>��������� axes_i</returns>
			axes_i getPosition();
			/// <summary>
			/// ���������� ������� ���������
			/// </summary>
			/// <returns>��������� FloatRect</returns>
			FloatRect getSize();
			/// <summary>
			/// �������� ��������� ������
			/// </summary>
			/// <param name="arg">����� �������� ������</param>
			/// <returns></returns>
			void changeBar(int arg) noexcept;
			void render(RenderWindow& wd) noexcept;
			void render(RenderWindow* wd) noexcept;

	};

	class multiline_text {
		private:
			int SIZE_MSTX, max_length; //����������� �������(���-�� ����� � ���), ������������ ����� ������ � �������� 
			axes_f pos; //���������� ������� �� ��� Y, ���������� ������� �� ��� X
			RectangleShape *bevel; //������������� ��� ������� 
			Font *font_main;
		public:
			Text* mass; //��������� �� ������ �����
			Text* ptr; //��������� �� ������ ������� ������� ����� (���������)
			bool visible; //��������� �������
			bool visible_bevel; //��������� �������������� ��� �������
			Color *bevel_cl;
			Color *label_cl;
			/// <summary>
			/// �����������
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <param name="lbcol">���� ������</param>
			/// <param name="bvcol">���� ��������������� �������������� ��� �������</param>
			/// <returns></returns>
			multiline_text(float x, float y, Color lbcol, Color bvcol); //���������� ������� �� ��� X, ���������� ������� �� ��� Y, ���� ������, ���� ��������������
			~multiline_text() noexcept;
			/// <summary>
			/// �������� ������ ������
			/// </summary>
			/// <param name="size">������ ������ � ��������</param>
			/// <returns></returns>
			void resize(int size) noexcept; //������ ������ ������� � ��������
			/// <summary>
			/// ���������� ����� ����� ������
			/// </summary>
			/// <param name="txt">����� �� ����� �����</param>
			/// <returns></returns>
			void add(std::wstring txt) noexcept; //��������� ����� ������ � ������
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
			/// �����������
			/// </summary>
			/// <param name="x">������� �� ��� X</param>
			/// <param name="y">������� �� ��� Y</param>
			/// <param name="cl">����</param>
			/// <param name="cf">��������� �� ��������� ��������</param>
			/// <returns></returns>
			background_color(int x, int y, Color cl, configuration *cf);
			~background_color();
			/// <summary>
			/// �������� ����
			/// </summary>
			/// <param name="cl">����� ����</param>
			void setColor(Color cl);
			void render(RenderWindow& wd) noexcept;
			void render(RenderWindow* wd) noexcept;
	};

}; //����� ������������ ���� 

class Character : public BaseCharacter { //TODO dx, dy, speed, 
	private:
		int direction, last_direction; //������ �����������, ������ �����������, ��������� ������� �� ��� X, ��������� ������� �� ��� Y, ����������� �������� ��������� 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Character(Image* ptr_on_img, float X_POS, float Y_POS, int hp); //���� � �������, ���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ �����������, ������ �����������,
		~Character(); //����������
		void __fastcall setPosition(float X, float Y) noexcept; //������������� ������� ������� �� ���� X, Y
		void setPosition(axes_f XY) noexcept; //������������� ������� ������� �� ���� X, Y
		void setImage(Image *ptr_on_img);
		float getPositionX_forCamer() noexcept; //���������� ����� ���������� ������� �� ��� X
		float getPositionY_forCamer() noexcept; //���������� ����� ���������� ������� �� ��� Y
		void __fastcall move(float time, int direct) noexcept; //�������������� ��������� �������
		void __fastcall move(float time) noexcept; //�������������� ��������� �������
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};	

class DestroerCastle : public BaseCharacter {
	private:
		int direction, last_direction; //������ �����������, ������ �����������, ��������� ������� �� ��� X, ��������� ������� �� ��� Y, ����������� �������� ��������� 
		_interface::min_bar* HP;
	public:
		DestroerCastle(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //���� � �������, ���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ �����������, ������ �����������,
		~DestroerCastle(); //����������
		void __fastcall setPosition(float X, float Y) noexcept; //������������� ������� ������� �� ���� X, Y
		void setPosition(axes_f XY) noexcept; //������������� ������� ������� �� ���� X, Y
		//float getPositionX_forCamer() noexcept; //���������� ����� ���������� ������� �� ��� X
		//float getPositionY_forCamer() noexcept; //���������� ����� ���������� ������� �� ��� Y
		void __fastcall move(float time, int direct) noexcept; //�������������� ��������� �������
		//void move(float time) noexcept; //�������������� ��������� �������
		void __fastcall attack(float time);
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};

class Spearman : public BaseCharacter {
	private:
		int direction, last_direction; //������ �����������, ������ �����������, ��������� ������� �� ��� X, ��������� ������� �� ��� Y, ����������� �������� ��������� 
		_interface::min_bar* HP;
	public:
		Collision* rect_collis;
		Spearman(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //���� � �������, ���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ �����������, ������ �����������,
		~Spearman(); //����������
		void __fastcall setPosition(float X, float Y) noexcept; //������������� ������� ������� �� ���� X, Y
		void setPosition(axes_f XY) noexcept; //������������� ������� ������� �� ���� X, Y
		void __fastcall move(float time, int direct) noexcept; //�������������� ��������� �������
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
		IceBall(Image *ptr_on_img, float X_POS, float Y_POS, int hp); //���� � �������, ���������� ������� �� ��� X, ���������� ������� �� ��� Y, ������ �����������, ������ �����������,
		~IceBall(); //����������
		void __fastcall setPosition(float X, float Y) noexcept; //������������� ������� ������� �� ���� X, Y
		void setPosition(axes_f XY) noexcept; //������������� ������� ������� �� ���� X, Y
		void __fastcall update(float time) noexcept; //�������������� ��������� �������
		bool isCooldown(float time);
		void render(RenderWindow& wd) noexcept;
		void render(RenderWindow* wd) noexcept;
};

#endif