#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#define DEEPTH_OF_WIEW 1500

///////////////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////

double toRadian(double grad) {
	return grad * 3.1415 / 180;
}

///////////////////////////////////////////////STRUCTS//////////////////////////////////////////////////////

class trigonometry {

private:
	float sinArray[361], cosArray[361];

public:

	void setUp() {
		for (int i = 0; i <= 360; i++) {
			sinArray[i] = sin(toRadian(i));
			cosArray[i] = cos(toRadian(i));
		}
	}

	float getSin(int angle) {
		return sinArray[abs(angle % 360)];
	}

	float getCos(int angle) {
		return cosArray[abs(angle % 360)];
	}
};

struct spriteObject {

	int getAngle() {
		if (!angleArray.empty()) return angleArray[angleArray.size() / 2];
		else return -1;
	}

	int num, lengh;
	std::vector<int> angleArray;
};

struct heapFiller {
	int lengh, num, angle;
	spriteObject sprite;
	std::string type;
};

class BinaryHeap                                                    // куча рассто€ний до объедков
{
private:
	std::vector<heapFiller> list;

public:
	int heapSize()
	{
		return list.size();
	}

	void add(heapFiller value)
	{
		list.push_back(value);
		int i = heapSize() - 1;
		int parent = (i - 1) / 2;

		while (i > 0 && list[parent].lengh < list[i].lengh)
		{
			std::swap(list[i], list[parent]);


			i = parent;
			parent = (i - 1) / 2;
		}
	}

	void heapify(int i)
	{
		int leftChild;
		int rightChild;
		int largestChild;

		while (true)
		{
			leftChild = 2 * i + 1;
			rightChild = 2 * i + 2;
			largestChild = i;

			if (leftChild < heapSize() && list[leftChild].lengh > list[largestChild].lengh)
			{
				largestChild = leftChild;
			}

			if (rightChild < heapSize() && list[rightChild].lengh > list[largestChild].lengh)
			{
				largestChild = rightChild;
			}

			if (largestChild == i)
			{
				break;
			}

			std::swap(list[i], list[largestChild]);
			i = largestChild;
		}
	}

	bool isEmpty() {
		if (list.empty()) return true;
		else return false;
	}

	heapFiller getMax()
	{
		heapFiller result = list[0];
		list[0] = list[heapSize() - 1];
		list.erase(list.begin() + heapSize() - 1);
		return result;
	}

	void clear() {
		list.clear();
	}
};

struct patch {
	patch() {
		startx = 0;
		starty = 0;
	}

	patch(int xValue, int yValue, int dirValue) {
		startx = xValue;
		starty = xValue;
		dir = dirValue;
		lengh = DEEPTH_OF_WIEW / cos(dirValue);
	}

	void setPatch(int xValue, int yValue, int dirValue) {
		startx = xValue;
		starty = xValue;
		dir = dirValue;
		// lengh = 650 / cos(toRadian(45 - dirValue));
		lengh = DEEPTH_OF_WIEW;
	}
	int startx, starty, dir, lengh;
};

struct player {

	player(int xValue, int yValue, int dirValue) {
		x = xValue;
		y = xValue;
		direction = dirValue;
	}

	void setPatchs() {
		for (int i = 0; i < 90; i++) {
			view[i].setPatch(x, y, i);
		}
	}

	double x, y, direction, speed = 15;
	patch view[90];
};

int main()
{

	trigonometry trigon;
	trigon.setUp();

	sf::RenderWindow window(sf::VideoMode(1440, 900), "SFMLworks");
	window.setFramerateLimit(60);                                       // задаю окно
	window.setTitle("game");

	window.setMouseCursorVisible(false);

	sf::Texture playerTexture, background, milos;

	playerTexture.loadFromFile("img\\player.png");
	background.loadFromFile("img\\background.png");
	milos.loadFromFile("img\\milos1.png");

	sf::Sprite playerSprite(playerTexture), backgroundSprite(background), milosSprite(milos);
	playerSprite.setOrigin(90, 124);


	sf::RectangleShape wall1(sf::Vector2f(400, 30));
	wall1.setPosition(0, 0);
	wall1.setFillColor(sf::Color(225, 0, 0, 70));
	sf::FloatRect wall1Colision(wall1.getGlobalBounds());

	sf::RectangleShape wall2(sf::Vector2f(400, 30));
	wall2.setPosition(0, 430);
	wall2.setFillColor(sf::Color(225, 0, 0, 70));
	sf::FloatRect wall2Colision(wall2.getGlobalBounds());

	sf::RectangleShape wall3(sf::Vector2f(30, 400));
	wall3.setPosition(0, 30);
	wall3.setFillColor(sf::Color(225, 0, 0, 70));
	sf::FloatRect wall3Colision(wall3.getGlobalBounds());


	milosSprite.setOrigin(598, 450);
	sf::RectangleShape sprite1Box(sf::Vector2f(10, 10));
	sprite1Box.setPosition(215, 215);
	sprite1Box.setFillColor(sf::Color(225, 0, 0, 70));
	sf::FloatRect sprite1BoxCollision(sprite1Box.getGlobalBounds());

	std::vector<sf::FloatRect> collisionArray;                          // создаю массив коллизий стен
	collisionArray.push_back(wall1Colision);
	collisionArray.push_back(wall2Colision);
	collisionArray.push_back(wall3Colision);

	sf::RectangleShape psWall(sf::Vector2f(16, 900));
	psWall.setOrigin(0, 450);
	psWall.setFillColor(sf::Color(255, 255, 255));

	sf::ConvexShape viewVision;
	viewVision.setPointCount(91);
	viewVision.setFillColor(sf::Color(0, 0, 0, 0));
	viewVision.setOutlineThickness(3);
	viewVision.setOutlineColor(sf::Color(250, 150, 100));

	sf::Font font;
	// if (!font.loadFromFile("fonts\\font.ttf")) {}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(40);
	text.setFillColor(sf::Color::Black);
	text.setPosition(200, 10);
	text.setStyle(sf::Text::Bold);              // шрифт

	//////////////////////////////////////////////////VAR///////////////////////////////////////////////////////

	player GAYmer(215, 215, 20);
	GAYmer.setPatchs();
	int mouseDiference;

	int lenghArray[90];

	float viewWallHeight = 0;

	sf::FloatRect castBox;
	sf::CircleShape castRect(2, 4);
	float castLengh;
	bool checkWall;
	bool checkSprite1;

	BinaryHeap heap;
	heapFiller filler;

	spriteObject tmpSpriteObject;

	float drawWallLengh;

	////////////////////////////////////////////ќбработка кадра/////////////////////////////////////////////////

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)                        // зкрытие
				window.close();
		}

		heap.clear();
		tmpSpriteObject.angleArray.clear();

		for (int i = 0; i < 90; i++) {                                  // работа с лучами

			checkSprite1 = false;

			castLengh = GAYmer.view[i].lengh;                       // получение наибольшей длины луча

			checkWall = true;
			for (int k = 0; k < GAYmer.view[i].lengh; k += 5) {         // нахождение расто€ни€ до преп€тстви€

				int casty = GAYmer.y + k * trigon.getSin(GAYmer.direction - 45 + i);
				int castx = GAYmer.x + k * trigon.getCos(GAYmer.direction - 45 + i);

				castRect.setPosition(castx, casty);
				castBox = castRect.getGlobalBounds();

				for (int j = 0; j < collisionArray.size() && checkWall; j++) {
					if (castBox.intersects(collisionArray[j])) {
						filler.angle = i;
						filler.lengh = k;
						filler.type = "wall";

						heap.add(filler);

						checkWall = false;
					}
				}

				if (castBox.intersects(sprite1BoxCollision) && !checkSprite1) {
					checkSprite1 = true;

					tmpSpriteObject.angleArray.push_back(i);
					tmpSpriteObject.lengh = k;

				}

			}

		}

		if (tmpSpriteObject.getAngle() != -1) {
			filler.num = 1;
			filler.angle = tmpSpriteObject.getAngle();
			filler.lengh = tmpSpriteObject.lengh;
			filler.type = "sprite";
			heap.add(filler);
		}

		mouseDiference = (sf::Mouse::getPosition().x - 800) / 4;
		if (mouseDiference < 0) GAYmer.direction += mouseDiference;
		else if (mouseDiference > 0) GAYmer.direction += mouseDiference;
		sf::Mouse::setPosition(sf::Vector2i(800, 400));                 // управление мышью

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			GAYmer.y += GAYmer.speed * trigon.getSin(GAYmer.direction);
			GAYmer.x += GAYmer.speed * trigon.getCos(GAYmer.direction);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			GAYmer.y -= GAYmer.speed * trigon.getSin(GAYmer.direction);
			GAYmer.x -= GAYmer.speed * trigon.getCos(GAYmer.direction); // управление клавиатурой
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			GAYmer.y += GAYmer.speed * trigon.getSin(GAYmer.direction - 90);
			GAYmer.x += GAYmer.speed * trigon.getCos(GAYmer.direction - 90);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			GAYmer.y += GAYmer.speed * trigon.getSin(GAYmer.direction + 90);
			GAYmer.x += GAYmer.speed * trigon.getCos(GAYmer.direction + 90);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {         // закрытие
			window.close();
		}

		window.clear();                                                 // очистка окна  
		window.draw(backgroundSprite);

		while (!heap.isEmpty()) {
			filler = heap.getMax();

			heap.heapify(0);

			if (filler.type == "wall") {
				int patchy = GAYmer.y + filler.lengh * trigon.getSin(GAYmer.direction - 45 + filler.angle);
				int patchx = GAYmer.x + filler.lengh * trigon.getCos(GAYmer.direction - 45 + filler.angle);

				psWall.setSize(sf::Vector2f(16, 900));

				viewWallHeight = filler.lengh * 1.732 / 3;              // отрисовка стены

				psWall.setPosition(filler.angle * 16, 450);
				psWall.setScale(1, 110 / viewWallHeight);

				drawWallLengh = filler.lengh;
				uint8_t tmp_color = (DEEPTH_OF_WIEW - drawWallLengh) / DEEPTH_OF_WIEW * 200;
				psWall.setFillColor(sf::Color(tmp_color, tmp_color, tmp_color));
				window.draw(psWall);
			}

			if (filler.type == "sprite") {
				int patchy = GAYmer.y + filler.lengh * trigon.getSin(GAYmer.direction - 45 + filler.angle);
				int patchx = GAYmer.x + filler.lengh * trigon.getCos(GAYmer.direction - 45 + filler.angle);

				milosSprite.setScale(sf::Vector2f(16, 900));

				viewWallHeight = filler.lengh * 1.732 / 3;              // отрисовка стены

				milosSprite.setPosition(filler.angle * 16, 450);
				milosSprite.setScale(110 / viewWallHeight, 110 / viewWallHeight);

				drawWallLengh = filler.lengh;

				uint8_t tmp_color = (DEEPTH_OF_WIEW - drawWallLengh) / DEEPTH_OF_WIEW * 200;
				milosSprite.setColor(sf::Color(tmp_color, tmp_color, tmp_color));
				window.draw(milosSprite);
			}
		}

		window.display();

	}

	return 0;
}