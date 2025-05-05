#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <cstdlib>
#include <windows.h>
#include "template.h"
#include <SDL_mouse.h>
#include "template.h"
#include <fstream>
#include <windows.h> 
#include <mmsystem.h> 
#pragma comment(lib, "winmm.lib") 
#define WIN32_LEAN_AND_MEAN


namespace Tmpl8
{
	int screenX = ScreenWidth, screenY = ScreenHeight; // Get screen sizes
	int px = screenX / 2 - 18, py = screenY / 2 - 18; // Place player in center of screen
	int gemX[10], gemY[10]; // 10 values for coordinates of gems
	
	Sprite player(new Surface("assets/spaceship.tga"), 4); // Player sprite
	Surface menuBackground("assets/menu_background.png"); // Menu background sprite
	Surface howtoBackground("assets/howtoplay_background.png"); // How to play background sprite
	Surface spaceBackground("assets/space_background.png"); // Space background sprite

	Sprite titleLogo(new Surface("assets/gameTitle.png"), 1); // Title logo sprite
	Sprite playSprite(new Surface("assets/playButton.png"), 1); // Play button sprite
	Sprite howtoSprite(new Surface("assets/howtoplayButton.png"), 1); // How to play button sprite
	Sprite quitSprite(new Surface("assets/quitButton.png"), 1); // Quit button sprite

	 bool timerStarted = false;

	//Runs when program initializes
	void Game::Init()
	{
		gem.Init();

		// Load highscore from file
		std::ifstream highscoreFile("highscore.txt");
		if (highscoreFile.is_open())
		{
			highscoreFile >> highscore;
			highscoreFile.close();
		}
	}

	// Runs when programs shuts down
	void Game::Shutdown()
	{
		// Save highscore to file
		std::ofstream highscoreFile("highscore.txt");
		if (highscoreFile.is_open())
		{
			highscoreFile << highscore;
			highscoreFile.close();
		}
	}

	// Runs every tick
	void Game::Tick(float deltaTime)
	{
		switch (state)
		{
		case GameState::menuState: // This is the state of the menu screen
			MenuScreen(deltaTime);
			break;
		case GameState::howtoState: // This is the state of the menu screen
			HowtoScreen(deltaTime);
			break;
		case GameState::gameState: // This is the state of the game screen
			GameScreen(deltaTime);
			break;
		case GameState::scoreState: // This is the state of the score screen
			ScoreScreen(deltaTime);
			break;
		}

	}

	// Runs when in menu screen
	void Game::MenuScreen(float deltaTime)
	{
		menuBackground.CopyTo(screen, 0, 0); // Render background image
		titleLogo.Draw(screen, 90, 100); // Render title image
		playSprite.Draw(screen, 100, 250); // Render play button image
		howtoSprite.Draw(screen, 100, 300); // Render play button image
		quitSprite.Draw(screen, 100, 350); // Render quit button image
	}

	void Game::HowtoScreen(float deltaTime)
	{
		howtoBackground.CopyTo(screen, 0, 0); // Render background image
	}

	// Runs when in game screen
	void Game::GameScreen(float deltaTime) 
	{
		screen->Clear(0);

		if (!timerStarted || timer.IsFinished()) { // Reset timer if not started or finished
			timer.Reset(); // Reset timer when starting the game
			timerStarted = true;
			score.Reset(); // Reset score when starting the game

			// Resets the player to correct position and orientation
			px = screenX / 2 - player.GetWidth() / 2; 
			py = screenY / 2 - player.GetHeight() / 2;
			player.SetFrame(0);
		}

		timer.Update();

		spaceBackground.CopyTo(screen, 0, 0);

		for (int i = 0; i < 5; i++)
		{
			if (!gem.CheckCollision(px, py, player.GetWidth(), player.GetHeight())) // Check if the player hasn't collided with the gem
			{
				gem.Draw(screen, gem.x[i], gem.y[i]); // If no collision, draw the gem at its position
			}
			else
			{
				score.IncrementScore(); // Increment score when collision occurs
				PlaySound(TEXT("assets/gempickup.wav"), NULL, SND_FILENAME | SND_ASYNC);

			}
		}

		player.Draw(screen, px, py); // Draw player

		const float playerSpeed = 0.5f; // Constant movement speed player

		// Movement and border logic. Also includes player speed
		if (GetAsyncKeyState('W') & 0x8000 && py > 0) py -= playerSpeed * deltaTime, player.SetFrame(0);
		if (GetAsyncKeyState('A') & 0x8000 && px > 0) px -= playerSpeed * deltaTime, player.SetFrame(3);
		if (GetAsyncKeyState('S') & 0x8000 && py < screenY - player.GetHeight()) py += playerSpeed * deltaTime, player.SetFrame(2);
		if (GetAsyncKeyState('D') & 0x8000 && px < screenX - player.GetWidth()) px += playerSpeed * deltaTime, player.SetFrame(1);

		// Print time left on screen
		char timeText[15]; // 14 digits at most
		sprintf(timeText, "Time: %ds", timer.GetDuration());
		screen->Print(timeText, 620, 10, Pixel(255 << 16) + (255 << 8) + (255));

		// Print score on screen
		char scoreText[15]; // 14 digits at most
		sprintf(scoreText, "Score: %d", score.GetScore());
		screen->Print(scoreText, 10, 10, Pixel(255 << 16) + (255 << 8) + (255));

		// If-statement to switch the state to scoreState when the timer has finished
		if (timer.IsFinished()) 
		{
			state = GameState::scoreState;
		}

	}

	// Runs when in score screen
	void Game::ScoreScreen(float deltaTime) 
	{
		screen->Clear(0);

		// If-statement to check if the current score is higher than the current score
		if (score.GetScore() > highscore) {
			highscore = score.GetScore(); // Update highscore if necessary
		}

		// Print score on screen
		char scoreText[15]; // 14 digits at most
		sprintf(scoreText, "Score: %d", score.GetScore());
		screen->Print(scoreText, 300, 200, Pixel(255 << 16) + (255 << 8) + (255));

		// Print highscore on screen
		char highscoreText[15]; // 14 digits at most
		sprintf(highscoreText, "Highscore: %d", highscore);
		screen->Print(highscoreText, 300, 240, Pixel(255 << 16) + (255 << 8) + (255));
	}

	// Runs when mouse is pressed down
	void Game::MouseDown(int button) 
	{
		if (state == GameState::menuState) {
			// Check for button clicks only in the menu state
			if (button == SDL_BUTTON_LEFT) {
				// Check if the mouse coordinates are within the play button area
				if (mousex > 100 && mousex < 100 + playSprite.GetWidth() &&
					mousey > 250 && mousey < 250 + playSprite.GetHeight()) {
					PlaySound(TEXT("assets/button.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play button sound
					state = GameState::gameState; // Change to game state
				}

				// Check if the mouse coordinates are within the how to play button area
				else if (mousex > 100 && mousex < 100 + howtoSprite.GetWidth() &&
					mousey > 300 && mousey < 300 + howtoSprite.GetHeight()) {
					PlaySound(TEXT("assets/button.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play button sound
					state = GameState::howtoState; // Change to how to play state
				}

				// Check if the mouse coordinates are within the quit button area
				else if (mousex > 100 && mousex < 100 + quitSprite.GetWidth() &&
					mousey > 350 && mousey < 350 + quitSprite.GetHeight()) {
					PlaySound(TEXT("assets/button.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play button sound
					exit(0); // Quit the program
				}
			}
		}

		else if (state == GameState::howtoState) {
			// Check for mouse clicks during the how to play screen
			if (button == SDL_BUTTON_LEFT) {
				PlaySound(TEXT("assets/button.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play button sound
				// Change state to menuState if clicked anywhere on the screen
				state = GameState::menuState;
			}
		}

		else if (state == GameState::scoreState) {
			// Check for mouse clicks during the score screen
			if (button == SDL_BUTTON_LEFT) {
				PlaySound(TEXT("assets/button.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play button sound
				// Change state to menuState if clicked anywhere on the screen
				state = GameState::menuState;
			}
		}
	}
	
};