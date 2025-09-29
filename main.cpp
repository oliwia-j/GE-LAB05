#include <SFML/Graphics.hpp>

const sf::Keyboard::Key controls[4] = {
	sf::Keyboard::A, // Player1 UP
	sf::Keyboard::Z, // Player1 DOWN
	sf::Keyboard::Up, // PLayer2 UP
	sf::Keyboard::Down // Player2 DOWN
};

// Parameters
sf::Time timeStep = sf::seconds(0.017f); // 60fps
const int gameWidth = 800;
const int gameHeight = 600;
sf::Font font;
sf::Text text;
int player1Points = 0;
int player2Points = 0;

bool isPlayer1Serving = true;
const float velocityMultiplier = 1.1f; // Ball speed up for every paddle hit (10%)

const sf::Vector2f paddleSize(25.f, 100.f);
const float paddleOffsetWall = 10.f;
const float paddleSpeed = 400.f;

const float ballRadius = 10.f;
sf::Vector2f ballVelocity;
const float initialVelocityX = 100.f; // Horizontal velocity
const float initialVelocityY = 60.f; // Vertical velocity

// Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];

// Reset paddles and ball position and velocity
void reset() {
	// Reset paddle position
	paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
	paddles[1].setPosition(gameWidth - paddleSize.x / 2.f - paddleOffsetWall, gameHeight / 2.f);
	// Reset ball position
	ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

	ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

// Initialise all the objects needed for the game.
void init() {
	// Load font-face from res dir
	font.loadFromFile(".\\Debug\\res\\fonts\\Roboto\\static\\Roboto-Regular.ttf");
	// Set text element to use font
	text.setFont(font);
	// Set character size to 24 pixels
	text.setCharacterSize(24);
	text.setColor(sf::Color(255, 255, 255));

	// Set size and origin of paddles
	for (sf::RectangleShape& p : paddles) {
		p.setSize(paddleSize);
		p.setOrigin(paddleSize / 2.f);
	}
	// Set size and origin of ball
	ball.setRadius(ballRadius);
	ball.setOrigin(ballRadius / 2.f, ballRadius / 2.f);

	reset();
	/*
	// Reset paddle position
	paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
	paddles[1].setPosition(gameWidth - paddleSize.x / 2.f - paddleOffsetWall, gameHeight / 2.f);
	// Reset ball position
	ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);

	ballVelocity = {(isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
	*/
}

// Update score
void score() {
	text.setString(std::to_string(player1Points) + " : " + std::to_string(player2Points));
	text.setPosition((gameWidth * .5f) - (text.getLocalBounds().width * .5f), 0);
}

// Update Everything
void update(float dt) {
	// Handle paddle movement
	float directionP1 = 0.0f;
	float directionP2 = 0.0f;
	if (sf::Keyboard::isKeyPressed(controls[0])) {
		directionP1--;
	}
	if (sf::Keyboard::isKeyPressed(controls[1])) {
		directionP1++;
	}
	if (sf::Keyboard::isKeyPressed(controls[2])) {
		directionP2--;
	}
	if (sf::Keyboard::isKeyPressed(controls[3])) {
		directionP2++;
	}

	// Prevent paddles from going beyond screen
	if (!(paddles[0].getPosition().y == gameHeight && directionP1 > 0)) {
		paddles[0].move(sf::Vector2f(0.f, directionP1 * paddleSpeed * dt));
	}
	else if (!(paddles[0].getPosition().y == 0 && directionP1 < 0)) {
		paddles[0].move(sf::Vector2f(0.f, directionP1 * paddleSpeed * dt));
	}

	if (!(paddles[1].getPosition().y == gameHeight && directionP2 > 0)) {
		paddles[1].move(sf::Vector2f(0.f, directionP2 * paddleSpeed * dt));
	}
	else if (!(paddles[1].getPosition().y == 0 && directionP2 < 0)) {
		paddles[1].move(sf::Vector2f(0.f, directionP2 * paddleSpeed * dt));
	}

	ball.move(ballVelocity * dt);

	// Check ball collision
	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;
	if (by > gameHeight) { /* bottom wall */
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(sf::Vector2f(0.f, -10.f));
	} else if(by < 0) { /* top wall */
		ballVelocity.x *= velocityMultiplier;
		ballVelocity.y *= -velocityMultiplier;
		ball.move(sf::Vector2f(0.f, 10.f));
	}
	else if (bx > gameWidth) { /* right wall  */
		// Point for Player1
		isPlayer1Serving = true;
		player1Points++;
		score();
		reset();
	}
	else if (bx < 0) { /* left wall  */
		// Point for Player2
		isPlayer1Serving = false;
		player2Points++;
		score();
		reset();
	}
	else if (
		/* ball is inline or behind paddle AND */
		(bx < paddleSize.x + paddleOffsetWall) &&
		/* ball is below top edge of paddle AND */
		(by > paddles[0].getPosition().y - (paddleSize.y * 0.5)) &&
		/* ball is above bottom edge of paddle */
		(by < paddles[0].getPosition().y + (paddleSize.y * 0.5))
		) { /* bounce off left paddle */
		ballVelocity.x *= -velocityMultiplier;
		ballVelocity.y *= velocityMultiplier;
		ball.move(sf::Vector2f(+10.f, 0.f));
	}
	else if (
		/* ball is inline or in front of paddle AND */
		(bx > gameWidth - paddleSize.x - paddleOffsetWall) &&
		/* ball is below top edge of paddle AND */
		(by > paddles[1].getPosition().y - (paddleSize.y * 0.5)) &&
		/* ball is above bottom edge of paddle */
		(by < paddles[1].getPosition().y + (paddleSize.y * 0.5))
		) { /* bounce off right paddle */
		ballVelocity.x *= -velocityMultiplier;
		ballVelocity.y *= velocityMultiplier;
		ball.move(sf::Vector2f(-10.f, 0.f));
	}
}

// Draw Everything
void render(sf::RenderWindow& window) {
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
	window.draw(text);
}

// Free up the memory if necessary
void clean() {
	
}

int main() {
	// Create the window
	sf::RenderWindow window(sf::VideoMode({ gameWidth, gameHeight }), "PONG");
	// Enable/disable vsync
	//window.setVerticalSyncEnabled(true);
	// Initialise and load
	init();
	while (window.isOpen()) {
		// Calculate dt
		static sf::Clock clock;
		const float dt = clock.restart().asSeconds();
		window.clear();
		update(dt);
		render(window);
		// Wait for the time_step to finish before displaying the next frame
		sf::sleep(timeStep);
		// Wait for Vsync
		window.display();
	}
	// Unload and shutdown
	clean();
}