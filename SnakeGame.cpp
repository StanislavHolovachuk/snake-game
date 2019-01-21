#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>

using namespace sf;

bool startGame(int &score);//��������

int N = 36, M = 24;
int size = 16;
int score = 4;
int w = size*N;
int h = size*M;
int dir, num = 4;
int next_poison = 0; // �������� ����� ������ �� ������
float timer = 0, delay = 0.1;//����� ������(������ �) �� delay(��������) - ������� � ���������� �� ��������� ����

struct Snake
{
	int x, y;
}  s[864];

struct Fruit
{
	int x, y;
} f;

struct Poison
{
	int x, y;
} p;

struct Bonus
{
	int x, y;
} b;

void SetLevel(RenderWindow & window) {
	Texture t_choose, t_first, t_second, t_third;

	t_choose.loadFromFile("images/choose.png");
	t_first.loadFromFile("images/first.png");
	t_second.loadFromFile("images/second.png");
	t_third.loadFromFile("images/third.png");

	Sprite choose(t_choose);
	Sprite first(t_first);
	Sprite second(t_second);
	Sprite third(t_third);

	bool isSL = 1;
	int menuSLnum = 0;

	choose.setPosition(160, 50);
	first.setPosition(230, 110);
	second.setPosition(230, 150);
	third.setPosition(230, 190);

	//////////////// ���� ������ в���///////////////////
	while (isSL)
	{
		choose.setColor(Color::Color(196, 150, 15));
		first.setColor(Color::Cyan);
		second.setColor(Color::Magenta);
		third.setColor(Color::Red);

		menuSLnum = 0;
		window.clear(Color(46, 84, 5));

		if (IntRect(220, 100, 120, 30).contains(Mouse::getPosition(window))) { first.setColor(Color::Color(196, 150, 15)); menuSLnum = 1; }
		if (IntRect(220, 140, 120, 30).contains(Mouse::getPosition(window))) { second.setColor(Color::Color(196, 150, 15)); menuSLnum = 2; }
		if (IntRect(220, 180, 120, 30).contains(Mouse::getPosition(window))) { third.setColor(Color::Color(196, 150, 15)); menuSLnum = 3; }


		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuSLnum == 1) { delay = 0.17;  isSL = false; }
			if (menuSLnum == 2) { delay = 0.1;  isSL = false; }
			if (menuSLnum == 3) { delay = 0.07; isSL = false; }
		}

		window.draw(choose);
		window.draw(first);
		window.draw(second);
		window.draw(third);

		window.display();
	}
}

void menu(RenderWindow & window) {

	Image icon;
	icon.loadFromFile("images/icon32.png");
	window.setIcon(32, 32, icon.getPixelsPtr()); //������ ���� ����
	
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground, menuInst;

	menuTexture1.loadFromFile("images/1ukr.png");
	menuTexture2.loadFromFile("images/2ukr.png");
	menuTexture3.loadFromFile("images/3ukr.png");
	aboutTexture.loadFromFile("images/ab_ukr.png");
	menuBackground.loadFromFile("images/bg.png");
	menuInst.loadFromFile("images/instr_ukr.png");

	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground), menuInstruction(menuInst);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menu3.setPosition(100, 150);
	menuBg.setPosition(335, 0);
	menuInstruction.setPosition(100, 210);

	//////////////// ���� ///////////////////
	while (isMenu)
	{
		menu1.setColor(Color::Cyan);
		menu2.setColor(Color::Magenta);
		menu3.setColor(Color::Red);
		menuNum = 0;
		window.clear(Color(46, 84, 5));

		if (IntRect(80, 20, 120, 30).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Color(196, 150, 15)); menuNum = 1; } //������ ����
		if (IntRect(80, 80, 120, 30).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Color(196, 150, 15)); menuNum = 2; }
		if (IntRect(80, 130, 120, 30).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Color(196, 150, 15)); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) { SetLevel(window);  isMenu = false; }
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.draw(menuInstruction);

		window.display();
	}
}

void loseScreen(RenderWindow & window, Text &text, Event &e) {
	Texture menuLS;

	menuLS.loadFromFile("images/1r.png");
	Sprite LS(menuLS);

	bool isLS = 1;
	int menuLSNum = 0;
	LS.setPosition(230, 90);

	//////////////// ���� ��������///////////////////
	while (isLS)
	{
		LS.setColor(Color::Cyan);
		menuLSNum = 0;
		window.clear(Color(46, 84, 5));

		if (IntRect(200, 80, 120, 30).contains(Mouse::getPosition(window))) { LS.setColor(Color::Color(196, 150, 15)); menuLSNum = 1; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuLSNum == 1) { window.close(); isLS = false;  startGame(score); } //������ ����� ������ - ������ � ���� - �������� ���							
		}

		text.setString("��� ������� ����� ���� �����: " + std::to_string(score) + " \n\t\t\t��������� �� ���!");
		text.setFillColor(Color::Color(196, 150, 15));
		text.setPosition(135, 150);

		window.draw(text);
		window.draw(LS);

		window.display();
	}
}

void Tick(Music &FON_MUSIC, Sound &GameOver, Sound &bite, Sound &poison, Sound &bonus, RenderWindow &window, Text &text, Event &e) {

	//��� ���� ������� �� �������
	for (int i = num;i>0;--i)
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	//����� ���������
	if (dir == 0) s[0].y += 1;	// DOWN --0---(����: ���������� �+1)
	if (dir == 1) s[0].x -= 1;	// LEFT --1---(����: ���������� �-1)
	if (dir == 2) s[0].x += 1;	// RIGHT -2---(������: ���������� �+1)
	if (dir == 3) s[0].y -= 1;	// UP --- 3---(�����: ���������� �-1)

								//������ ����� "�����" ����� ��� ������ ������
	if ((s[0].x == f.x) && (s[0].y == f.y))
	{
		num++;
		score++;
		next_poison++;//���� ����=7, ����������� ������
		bite.play();
		//� ������ ������� ������ ������

		f.x = rand() % (N - 2) + 1;
		f.y = rand() % (M - 2) + 1;

		for (int n = 0; n < num; n++) {		//��� ����� ��� ������ ��� ����� �� ������� �� �����
			if ((p.x == s[n].x && p.y == s[n].y) || (p.x == s[n].x && p.y == s[n].y) || (b.x == s[n].x && b.y == s[n].y))
			{
				f.x = rand() % (N - 2) + 1;
				f.y = rand() % (M - 2) + 1;
			}
		}
	}

	///////////////////��������� ������
	if (next_poison % 5 == 0 && next_poison != 0)
	{
		p.x = rand() % (N - 2) + 1;
		p.y = rand() % (M - 2) + 1;
		for (int n = 0; n < num; n++) {		//��� ����� ��� ������ ��� ����� �� ������� �� �����
			if ((p.x == s[n].x && p.y == s[n].y) || (p.x == s[n].x && p.y == s[n].y) || (b.x == s[n].x && b.y == s[n].y))
			{
				p.x = rand() % (N - 2) + 1;
				p.y = rand() % (M - 2) + 1;
			}
		}
		next_poison++;
	}
	p.y--;// ��� ������(�����) ������� 

		  //����� ������ ������ �� ����
	if (true)
	{
		if (p.x > N) { p.x = 0; }
		if (p.x < 0) { p.x = N; }
		if (p.y > M) { p.y = 0; }
		if (p.y < 0) { p.y = M; }
	}


	//��� ������ ���� � �����
	if ((s[0].x == p.x) && (s[0].y == p.y) || (s[0].x == p.x) && (s[0].y == p.y + 1) || (s[0].x == p.x) && (s[0].y == p.y - 1) || (s[0].x == p.x + 1) && (s[0].y == p.y) || (s[0].x == p.x - 1) && (s[0].y == p.y))
	{
		if (num > 5) {
			num -= 5;
			score -= 5;
		}
		if (num <= 5) {
			num = 1;
			score = 0;
		}

		p.x = N;
		p.y = M;
		poison.play();
	}

	////////////��������� ������
	if (next_poison % 8 == 0 && next_poison != 0)
	{
		b.x = rand() % (N - 2) + 1;
		b.y = rand() % (M - 2) + 1;
		for (int n = 0; n < num; n++) {		//��� ����� ��� ������ ��� ����� �� ������� �� �����
			if ((p.x == s[n].x && p.y == s[n].y) || (p.x == s[n].x && p.y == s[n].y) || (b.x == s[n].x && b.y == s[n].y))
			{
				b.x = rand() % (N - 2) + 1;
				b.y = rand() % (M - 2) + 1;
			}
		}
		next_poison++;
	}

	b.x--;// ��� ������ ��������

		  //����� ������ ������ �� ����
	if (true)
	{
		if (b.x > N) { b.x = 0; }
		if (b.x < 0) { b.x = N; }
		if (b.y > M) { b.y = 0; }
		if (b.y < 0) { b.y = M; }
	}

	if ((s[0].x == b.x) && (s[0].y == b.y) || (s[0].x == b.x) && (s[0].y == b.y + 1) || (s[0].x == b.x) && (s[0].y == b.y - 1) || (s[0].x == b.x + 1) && (s[0].y == b.y) || (s[0].x == b.x - 1) && (s[0].y == b.y)) // ����� �� ���������� � ��������� �������
	{
		num += 3;
		score += 3;

		b.x = N; //�������
		b.y = M;
		bonus.play();
	}

	//����� ������ �쳿 �� ��� ����
	if (s[0].x >= N) { s[0].x = 0; }
	if (s[0].x < 0) { s[0].x = N; }
	if (s[0].y >= M) { s[0].y = 0; }
	if (s[0].y < 0) { s[0].y = M; }

	//�������� � �����
	for (int i = 1;i<num;i++)
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			num = 4; // ��������� �������
				GameOver.play();
			FON_MUSIC.stop();
			loseScreen(window, text, e);
		}
}

bool startGame(int &score) {

	//�������� ����������
	f.x = 10;
	f.y = 10;
	//�������� ����������
	p.x = N;
	p.y = M;
	//�������� ����������
	b.x = N;
	b.y = M;

	dir = 3;
	score = 4;
	next_poison = 0;

	srand(time(0));

	//��������� ����
	RenderWindow window(VideoMode(w, h), "SnakeGame by Holovachuk S.V. (TI-72)");

	//SetLevel(window);
	menu(window);//������ ����

				 //�����
	Font font;
	font.loadFromFile("font/CyrilicOld.ttf");//�������� ������ ������ ���� ������

	Text text("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������)	//text.setColor(Color::Color(196, 150, 15));//��������� �����
	text.setStyle(Text::Bold | Text::Underlined);//������ � ������������ �����

	Text text_time("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������)	//text.setColor(Color::Color(196, 150, 15));//��������� �����
	text_time.setStyle(Text::Bold | Text::Underlined);//������ � ������������ �����

													  //�����
	SoundBuffer buffer_gameover;
	buffer_gameover.loadFromFile("wav/gameover.wav");  //��������� � ����� ������
	Sound GameOver;
	GameOver.setBuffer(buffer_gameover);// ���� ���� ����


	SoundBuffer buffer_bite;
	buffer_bite.loadFromFile("wav/bite.wav");  //��������� � ����� �������
	Sound bite;
	bite.setBuffer(buffer_bite);// ���� �����

	SoundBuffer buffer_poison;
	buffer_poison.loadFromFile("wav/poison.wav");  //��������� � ����� ������
	Sound poison;
	poison.setBuffer(buffer_poison);// ���� ������ ������

	SoundBuffer buffer_bonus;
	buffer_bonus.loadFromFile("wav/bonus.wav");  //��������� � ����� ������
	Sound bonus;
	bonus.setBuffer(buffer_bonus);// ���� ������



								  //������
	Music FON_MUSIC;
	FON_MUSIC.openFromFile("wav/snake_music.wav");//��������� ����
	FON_MUSIC.play();//������������� ������
	FON_MUSIC.setLoop(true); //�������� ������



							 //���
	Texture t1, t2, t3, t4, t5;
	t1.loadFromFile("images/11111.png");
	t2.loadFromFile("images/snake+grass.png");
	t3.loadFromFile("images/fruct.png");
	t4.loadFromFile("images/poison.png");
	t5.loadFromFile("images/bonus.png");

	Sprite sprite1(t1);
	Sprite sprite2(t2);
	Sprite sprite3(t3);
	Sprite sprite4(t4);
	Sprite sprite5(t5);

	//��������� ���� ��� ����� ����
	Clock clock;

	//�������� ���� ��������� 
	while (window.isOpen())
	{
		//���� ����
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//����
		Event e;

		//���� ���� �������, ��������
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}

		//���������
		if (Keyboard::isKeyPressed(Keyboard::Left) && dir != 2) dir = 1;
		if (Keyboard::isKeyPressed(Keyboard::Right) && dir != 1)  dir = 2;
		if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 0) dir = 3;
		if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 3) dir = 0;

		int abc = 0;

		//����� ��������� ���� 
		if (timer>delay)
		{
			timer = 0;
			Tick(FON_MUSIC, GameOver, bite, poison, bonus, window, text, e);
			abc++;
		}

		////// ���������  ///////
		window.clear();
		//��������� ����
		for (int i = 0; i<N; i++)
			for (int j = 0; j<M; j++)
			{
				sprite1.setPosition(i*size, j*size);
				window.draw(sprite1);
			}
		//������ ���
		for (int i = 0;i<num;i++)
		{
			sprite2.setPosition(s[i].x*size, s[i].y*size);
			window.draw(sprite2);
		}
		//������ �����
		sprite3.setPosition(f.x*size, f.y*size);
		window.draw(sprite3);

		//������ ��
		sprite4.setPosition(p.x*size, p.y*size);
		window.draw(sprite4);

		//������ �����
		sprite5.setPosition(b.x*size, b.y*size);
		window.draw(sprite5);

		if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//��� - �������
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//��� - �����

																	   //���� �������
		text.setString("�������:" + std::to_string(score));
		text.setFillColor(Color(196, 150, 15));

		window.draw(text);//������ �����

		window.display();
	}
}

void gameRunning() {//�-��� ����������� ��� , ���� ���������
	if (startGame(score)) {
		gameRunning();
	} //���� startGame() == true, �� ��������� ����� �-� GameRunning, ��� � ���� ����� ������� startGame
}

int main()
{
	gameRunning();//��������� ������ ���
	return 0;
}