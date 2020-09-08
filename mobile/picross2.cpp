#include <iostream>
#include <random>
#include <ctime>
#include <SDL2/SDL.h>

// Constants
//{
// The current version of the program.
constexpr int VERSION[] = {1, 2, 0, 1};

#define DIGITS 10
#define BOARD_SIZE 9
#define MAX_HINT (BOARD_SIZE + (BOARD_SIZE % 2)) / 2

// Window title
const char* const TITLE = "Picross 2 by Chigozie Agomo";

// Image locations
const char* const BACKGROUND_IMAGE = "data/background.bmp";
const char* const TITLE_IMAGE = "data/title.bmp";
const char* const PLAY_IMAGE = "data/play.bmp";
const char* const HELP_IMAGE = "data/help.bmp";
const char* const HELP_SCREEN_IMAGE = "data/helpscreen.bmp";
const char* const BLANK_SQUARE_IMAGE = "data/blanksquare.bmp";
const char* const FILLED_SQUARE_IMAGE = "data/filledsquare.bmp";
const char* const MARKED_SQUARE_IMAGE = "data/markedsquare.bmp";
const char* const QUIT_IMAGE = "data/quit.bmp";
const char* const HINT_IMAGE = "data/hint.bmp";
const char* const SOLVE_IMAGE = "data/solve.bmp";
const char* const NEW_IMAGE = "data/new.bmp";
const char* const RESET_IMAGE = "data/reset.bmp";
const char* const TIME_IMAGE = "data/time.bmp";
const char* const HINTS_IMAGE = "data/hints.bmp";
const char* const COLON_IMAGE = "data/colon.bmp";
const char* const NUMBER_IMAGES[DIGITS] = {
	"data/0.bmp",
	"data/1.bmp",
	"data/2.bmp",
	"data/3.bmp",
	"data/4.bmp",
	"data/5.bmp",
	"data/6.bmp",
	"data/7.bmp",
	"data/8.bmp",
	"data/9.bmp"
};
const char* const NUMBER_SQUARE_IMAGES[DIGITS] = {
	"data/0square.bmp",
	"data/1square.bmp",
	"data/2square.bmp",
	"data/3square.bmp",
	"data/4square.bmp",
	"data/5square.bmp",
	"data/6square.bmp",
	"data/7square.bmp",
	"data/8square.bmp",
	"data/9square.bmp"
};

// Sprite size ratio constants
const double BUTTON_X = 0.9;
const double BUTTON_WIDTH = 0.1;
const double BUTTON_HEIGHT = 0.075;
const double TITLE_X = 0.5;
const double TITLE_Y = 0.2;
const double TITLE_WIDTH = 0.75;
const double TITLE_HEIGHT = 0.3;
const double PLAY_X = 0.5;
const double PLAY_Y = 0.65;
const double PLAY_WIDTH = 0.2;
const double PLAY_HEIGHT = 0.15;
const double HELP_X = 0.5;
const double HELP_Y = 0.85;
const double HELP_WIDTH = 0.2;
const double HELP_HEIGHT = 0.15;
const double QUIT_X = 0.925;
const double QUIT_Y = 0.925;
const double HELP_SCREEN_X = 0.5;
const double HELP_SCREEN_Y = 0.5;
const double HELP_SCREEN_WIDTH = 0.9;
const double HELP_SCREEN_HEIGHT = 0.8;
const double GRID_X = 0.325;
const double GRID_Y = 0.5;
const double GRID_WIDTH = 0.6;
const double GRID_HEIGHT = 0.95;
#define SQUARE_WIDTH GRID_WIDTH / (BOARD_SIZE + MAX_HINT)
#define SQUARE_HEIGHT GRID_HEIGHT / (BOARD_SIZE + MAX_HINT)
const double NEW_Y = 0.5;
const double RESET_Y = 0.6;
const double HINT_Y = 0.7;
const double SOLVE_Y = 0.8;
const double QUIT2_Y = 0.9;
#define TIME_X BUTTON_X
const double TIME_Y = 0.1;
#define TIME_WIDTH BUTTON_WIDTH
#define TIME_HEIGHT BUTTON_HEIGHT
#define COLON_X TIME_X
const double COLON_Y = 0.2;
const double COLON_WIDTH = 0.02;
#define COLON_HEIGHT TIME_HEIGHT
const double NUMBER_WIDTH = 0.04;
#define NUMBER_HEIGHT COLON_HEIGHT
#define HINTS_X TIME_X
const double HINTS_Y = 0.3;
#define HINTS_WIDTH TIME_WIDTH
#define HINTS_HEIGHT TIME_HEIGHT
#define MINUTE_X2 COLON_X - COLON_WIDTH / 2 - NUMBER_WIDTH / 2
#define MINUTE_X1 MINUTE_X2 - NUMBER_WIDTH
#define SECOND_X1 COLON_X + COLON_WIDTH / 2 + NUMBER_WIDTH / 2
#define SECOND_X2 SECOND_X1 + NUMBER_WIDTH
const double HINT_COUNT_Y = 0.4;
#define TEN_X HINTS_X - NUMBER_WIDTH / 2
#define UNIT_X HINTS_X + NUMBER_WIDTH / 2

// Song location
const char* const SONG = "data/song.wav";

// Song length lower bound (for requeuing)
const int SONG_LENGTH = 56;

// Other constants
const int BACKGROUND_COLOUR[3] = {0x0, 0x0, 0x0};

const int SURFACE_DEPTH = 32;

const Uint32 SURFACE_MASKS[2][4] = {
	{
		0xff000000,
		0x00ff0000,
		0x0000ff00,
		0x000000ff
	},
	{
		0x000000ff,
		0x0000ff00,
		0x00ff0000,
		0xff000000
	}
};

const int LEFT_CLICK = SDL_BUTTON(SDL_BUTTON_LEFT);
const int RIGHT_CLICK = SDL_BUTTON(SDL_BUTTON_RIGHT);
//}

/* Gives a relative time in seconds
 */
double get_time() {
	return (double) clock() / CLOCKS_PER_SEC;
}

/* A wrapper class for the window and its surface
 */
class Display {
	public:
		/* Sets the display mode (display's height and width)
		   Creates a window and stores its surface
		 */
		Display() {
			SDL_GetDesktopDisplayMode(0, &display_mode);
			
			window = SDL_CreateWindow(
				TITLE, 0, 0,
				display_mode.w, display_mode.h, SDL_WINDOW_SHOWN
			);
			
			window_surface = SDL_GetWindowSurface(window);
		}
		
		/* Destroys the window
		 */
		~Display() {
			SDL_DestroyWindow(window);
		}
		
		/* Returns the width of the window
		 */
		int get_width() {
			return window_surface->w;
		}
		
		/* Returns the height of the window
		 */
		int get_height() {
			return window_surface->h;
		}
		
		/* Updates the window's surface
		 */
		void update() {
			SDL_UpdateWindowSurface(window);
		}
		
		/* Returns the window's surface
		 */
		SDL_Surface* get_surface() {
			return window_surface;
		}
			
	private:
		SDL_DisplayMode display_mode; // Display dimensions
		SDL_Window* window; // The window
		SDL_Surface* window_surface; // The window's surface
};

/* A wrapper class for surfaces
 */
class Surface {
	public:
		/* Loads the surface and fits it to the given dimenions
		   Initialises a rectangle for the surface
		 */
		Surface(const char* source, int x, int y, int width, int height) {
			SDL_Surface* raw_surface = SDL_LoadBMP(source);
			
			surface = SDL_CreateRGBSurface(
				0, width, height, SURFACE_DEPTH,
				SURFACE_MASKS[byte_order][0], SURFACE_MASKS[byte_order][1],
				SURFACE_MASKS[byte_order][2], SURFACE_MASKS[byte_order][3]
			);
			
			SDL_BlitScaled(raw_surface, NULL, surface, NULL);
			SDL_FreeSurface(raw_surface);
			
			rect.x = x;
			rect.y = y;
			rect.w = width;
			rect.h = height;
		}
		
		/* Alternative constructor for use with ratios
		   Takes in the display's width and height in addition to the ratios as arguments
		 */
		Surface(const char* source, int dw, int dh, double x, double y, double w, double h) {
			int width = w * dw;
			int height = h * dh;
			int rx = x * dw - width / 2;
			int ry = y * dh - height / 2;
			
			SDL_Surface* raw_surface = SDL_LoadBMP(source);
			
			surface = SDL_CreateRGBSurface(
				0, width, height, SURFACE_DEPTH,
				SURFACE_MASKS[byte_order][0], SURFACE_MASKS[byte_order][1],
				SURFACE_MASKS[byte_order][2], SURFACE_MASKS[byte_order][3]
			);
			
			SDL_BlitScaled(raw_surface, NULL, surface, NULL);
			SDL_FreeSurface(raw_surface);
			
			rect.x = rx;
			rect.y = ry;
			rect.w = width;
			rect.h = height;
		}
		
		/* Frees the surface associated with the object
		 */
		~Surface() {
			SDL_FreeSurface(surface);
		}
		
		/* Returns true if the point in the surface's rect
		 */
		bool in_rect(const SDL_Point* point) {
			return SDL_PointInRect(point, &rect);
		}
		
		/* Blits the surface to another
		 */
		void blit(SDL_Surface* surf) {
			SDL_BlitSurface(surface, NULL, surf, &rect);
		}
		
		/* Sets the rectangle's co-ordinates and then blits
		 */
		void blit(SDL_Surface* surf, double x, double y) {
			rect.x = x * surf->w - rect.w / 2;
			rect.y = y * surf->h - rect.h / 2;
			
			blit(surf);
		}
		
		/* Sets the rectangle's x co-ordinate and then blits
		 */
		void blit_x(SDL_Surface* surf, double x) {
			rect.x = x * surf->w - rect.w / 2;
			
			blit(surf);
		}

		/* Sets the rectangle's co-ordinates exactly before blitting
		 */
		void blit(SDL_Surface* surf, int x, int y) {
			rect.x = x;
			rect.y = y;
			blit(surf);
		}

	private:
		static const int byte_order = SDL_BYTEORDER != SDL_BIG_ENDIAN; // Byte ordering
		SDL_Surface* surface; // Contains the pixel data for blitting
		SDL_Rect rect; // The rectangle showing the position and size of the surface
};

/* Class that deals with queuing the music
 */
class MusicQueuer {
	public:
		/* Loads the music, plays it, and stores the time of playing
		 */
		MusicQueuer(const char* source, int length) {
			SDL_LoadWAV(source, &audio_spec, &audio_buffer, &audio_length);
			audio_device = SDL_OpenAudioDevice(NULL, false, &audio_spec, NULL, 0);
			SDL_PauseAudioDevice(audio_device, false);
			SDL_QueueAudio(audio_device, audio_buffer, audio_length);
			double last_queue = get_time();
			song_length = length;
		}
		
		/* Closes the audio device and frees the song
		 */
		~MusicQueuer() {
			SDL_CloseAudioDevice(audio_device);
			SDL_FreeWAV(audio_buffer);
		}
		
		/* Requeues the song if it is time
		 */
		void requeue() {
			if (get_time() > last_queue + song_length) {
				SDL_QueueAudio(audio_device, audio_buffer, audio_length);
				last_queue = get_time();
			}
		}		
	
	private:
		SDL_AudioSpec audio_spec;
		Uint8* audio_buffer;
		Uint32 audio_length;
		SDL_AudioDeviceID audio_device;
		double last_queue;
		int song_length;
};

/* The Picross Board
   Contains the pattern to be found, the player's pattern, and the hints
 */
class Board {
	public:
		/* Initialises the RNG and produces a grid with hints
		 */
		Board(Display* display) {
			// RNG initialised
			generator = new std::mt19937(time(NULL));
			distribution = new std::uniform_int_distribution<int>(0, 1);
			distribution2 = new std::uniform_int_distribution<int>(0, BOARD_SIZE * BOARD_SIZE - 1);
			
			new_puzzle();
			
			// Surfaces are initialised
			blank_square = new Surface(
				BLANK_SQUARE_IMAGE, display->get_width(), display->get_height(),
				0, 0, SQUARE_WIDTH, SQUARE_HEIGHT
			);
			
			filled_square = new Surface(
				FILLED_SQUARE_IMAGE, display->get_width(), display->get_height(),
				0, 0, SQUARE_WIDTH, SQUARE_HEIGHT
			);
			
			marked_square = new Surface(
				MARKED_SQUARE_IMAGE, display->get_width(), display->get_height(),
				0, 0, SQUARE_WIDTH, SQUARE_HEIGHT
			);
			
			for (int i = 0; i < DIGITS; i++) {
				number_squares[i] = new Surface(
					NUMBER_SQUARE_IMAGES[i], display->get_width(), display->get_height(),
					0, 0, SQUARE_WIDTH, SQUARE_HEIGHT
				);
			}
			
			width = GRID_WIDTH * display->get_width();
			height = GRID_HEIGHT * display->get_height();
			x = GRID_X * display->get_width() - width / 2;
			y = GRID_Y * display->get_height() - height / 2;
			width = SQUARE_WIDTH * display->get_width();
			height = SQUARE_HEIGHT * display->get_height();
		}
		
		/* Frees all dynamically allocated memory associated with the board
		 */
		~Board() {
			delete generator;
			delete distribution;
			delete distribution2;
			delete blank_square;
			delete filled_square;
			delete marked_square;
			
			for (int i = 0; i < DIGITS; i++) {
				delete number_squares[i];
			}
		}
		
		/* Prints the target board to display (including hints)
		 */
		void print_target() {
			std::cout << '\n';
			
			// Column hints are printed first
			for (int i = 0; i < MAX_HINT; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					std::cout << "  ";
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; j++) {
					if (columns[j][i]) {
						std::cout << columns[j][i] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << '\n';
			}
			
			std::cout << '\n';
			
			// Row hints and the target board are then printed
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					if (rows[i][j]) {
						std::cout << rows[i][j] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; j++) {
					std::cout << (target[i][j] ? 'O' : ' ') << ' ';
				}
				
				std::cout << ' ' << i << '\n';
			}
			
			std::cout << '\n';
			
			for (int i = 0; i < MAX_HINT; i++) {
				std::cout << "  ";
			}
			
			std::cout << ' ';
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				std::cout << i << ' ';
			}
			
			std::cout << "\n\n";
		}
		
		/* Prints the player's board to display (including hints)
		 */
		void print_board() {
			std::cout << '\n';
			
			// Column hints are printed first
			for (int i = 0; i < MAX_HINT; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					std::cout << "  ";
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; j++) {
					if (columns[j][i]) {
						std::cout << columns[j][i] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << '\n';
			}
			
			std::cout << '\n';
			
			// Row hints and the target board are then printed
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					if (rows[i][j]) {
						std::cout << rows[i][j] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; j++) {
					std::cout << (board[i][j] ? (board[i][j] > 0 ? 'O' : 'X') : ' ') << ' ';
				}
				
				std::cout << ' ' << i << '\n';
			}
			
			std::cout << '\n';
			
			for (int i = 0; i < MAX_HINT; i++) {
				std::cout << "  ";
			}
			
			std::cout << ' ';
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				std::cout << i << ' ';
			}
			
			std::cout << "\n\n";
		}
		
		/* Returns true if the player's board matches the target
		 */
		bool solved() {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0 ; j < BOARD_SIZE; j++) {
					if (target[i][j]) {
						if (board[i][j] != 1) {
							return false;
						}
					}
					
					else {
						if (board[i][j] == 1) {
							return false;
						}
					}
				}
			}
			
			return true;
		}
		
		/* Fills in the board
		   Duplicate commands erase the square
		 */
		void fill(int command, int i, int j) {
			board[j][i] = board[j][i] == command ? 0 : command;
		}
			
		/* Blits the target to the surface
		 */
		void blit_target(SDL_Surface* surface) {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					switch (board[i][j]) {
						case 1:
							filled_square->blit(
								surface,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
							break;
							
						default:
							blank_square->blit(
								surface,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
					}
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					number_squares[rows[i][j]]->blit(
						surface,
						x + (i + MAX_HINT) * width,
						y + j * height
					);
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					number_squares[columns[i][j]]->blit(
						surface,
						x + j * width,
						y + (i + MAX_HINT) * height
					);
				}
			}
		}
		
		/* Blits the board to the surface
		 */
		void blit_board(SDL_Surface* surface) {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					switch (board[i][j]) {
						case 1:
							filled_square->blit(
								surface,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
							break;
							
						case -1:
							marked_square->blit(
								surface,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
							break;
							
						default:
							blank_square->blit(
								surface,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
					}
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					number_squares[rows[i][j]]->blit(
						surface,
						x + (i + MAX_HINT) * width,
						y + j * height
					);
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					number_squares[columns[i][j]]->blit(
						surface,
						x + j * width,
						y + (i + MAX_HINT) * height
					);
				}
			}
		}

		/* Makes a new puzzle
		 */
		void new_puzzle() {
			// Target board and player board initialised
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					target[i][j] = (*distribution)(*generator);
					board[i][j] = 0;
				}
			}
			
			//Row and column hints set to zero
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < MAX_HINT; j++) {
					rows[i][j] = 0;
					columns[i][j] = 0;
				}
			}
			
			// Row hints initialised
			for (int i = 0; i < BOARD_SIZE; i++) {
				int position = MAX_HINT - 1;
				
				for (int j = BOARD_SIZE - 1; j > -1; j--) {
					if (target[i][j]) {
						rows[i][position]++;
					}
					
					else if (rows[i][position]) {
						position--;
					}
				}
			}
			
			// Column hints initialised
			for (int i = 0; i < BOARD_SIZE; i++) {
				int position = MAX_HINT - 1;
				
				for (int j = BOARD_SIZE - 1; j > -1; j--) {
					if (target[j][i]) {
						columns[i][position]++;
					}
					
					else if (columns[i][position]) {
						position--;
					}
				}
			}
		}
		
		/* Resets the puzzle
		 */
		void reset_puzzle() {
			// Player board initialised
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					board[i][j] = 0;
				}
			}
		}

		/* Checks if the board was clicked
		 */
		void click_check(const SDL_Point* point, bool left_click) {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0; j < BOARD_SIZE; j++) {
					SDL_Rect rect = {
							.x = x + (i + MAX_HINT) * width,
							.y = y + (j + MAX_HINT) * height,
							.w = width,
							.h = height
					};
					
					if (SDL_PointInRect(point, &rect)) {
						int command = left_click ? 1 : -1;
						board[i][j] += command;
						
						if (board[i][j] > 1) {
							board[i][j] -= 3;
						}
						
						else if (board[i][j] < -1) {
							board[i][j] += 3;
						}
						
						return;
					}
				}
			}
		}
		
		/* Solves a single sqaure
		   The square is chosen randomly
		 */
		void hint() {
			// The square to be revealed is chosen
			int index = (*distribution2)(*generator);

			for (int i = 0;; i++) {
				// The choice loops through the board
				if (i == BOARD_SIZE) {
					i = 0;
				}
				
				for (int j = 0 ; j < BOARD_SIZE; j++) {
					// If the square hasn't been marked correctly and the index
					//   is zero, then it is solved, else the index is decremented
					if (target[j][i]) {
						if (board[j][i] != 1) {
							if (!index--) {
								board[j][i] = 1;
								return;
							}
						}
					}
					
					else {
						if (board[j][i] != -1) {
							if (!index--) {
								board[j][i] = -1;
								return;
							}
						}
					}
				}
			}
		}
		
		/* Solves the entire puzzle
		 */
		void solve() {
			for (int i = 0; i < BOARD_SIZE; i++) {
				for (int j = 0 ; j < BOARD_SIZE; j++) {
					board[i][j] = target[i][j];
				}
			}
		}
		
	private:
		int target[BOARD_SIZE][BOARD_SIZE]; // The pattern to be found
		int board[BOARD_SIZE][BOARD_SIZE]; // The player's pattern
		int rows[BOARD_SIZE][MAX_HINT]; // The row hints
		int columns[BOARD_SIZE][MAX_HINT]; // The column hints
		std::mt19937* generator; // RNG
		std::uniform_int_distribution<int>* distribution; // Uniform distribution
		std::uniform_int_distribution<int>* distribution2; // Uniform distribution
		Surface* blank_square;
		Surface* filled_square;
		Surface* marked_square;
		Surface* number_squares[DIGITS];
		int x; // x co-ordinate of the board's left edge
		int y; // y co-ordinate of the board's right edge
		int width; // Width of a board square
		int height; // Hieght of a board square
};

/* Class for the game timer
 */
class Timer {
	public:
		/* Initialises the surfaces for the numbers and the colon
		 */
		Timer(Display* display) {
			colon = new Surface(
				COLON_IMAGE, display->get_width(), display->get_height(),
				COLON_X, COLON_Y, COLON_WIDTH, COLON_HEIGHT
			);
			
			for (int i = 0; i < DIGITS; i++) {
				numbers[i] = new Surface(
					NUMBER_IMAGES[i], display->get_width(), display->get_height(),
					0, COLON_Y, NUMBER_WIDTH, NUMBER_HEIGHT
				);
			}
		}
		
		/* Deletes the surfaces allocated
		 */
		~Timer() {
			delete colon;
			
			for (int i = 0; i < DIGITS; i++) {
				delete numbers[i];
			}
		}
		
		/* Starts the timer
		 */
		void start() {
			start_ = get_time();
			now = -1;
		}
		
		/* Blits the timer to the surface
		 */
		void blit(SDL_Surface* surface) {
			// The time is split into minutes and seconds
			int minutes = now / 60;
			int seconds = now % 60;
			
			// If an hour has passed, no time is displayed
			if (minutes < 60) {
				// The minute-second separator is blitted
				colon->blit(surface);
				
				// The digits to be used are calculated
				int m1 = minutes / 10;
				int m2 = minutes % 10;
				int s1 = seconds / 10;
				int s2 = seconds % 10;
				
				numbers[m1]->blit_x(surface, MINUTE_X1);
				numbers[m2]->blit_x(surface, MINUTE_X2);
				numbers[s1]->blit_x(surface, SECOND_X1);
				numbers[s2]->blit_x(surface, SECOND_X2);
			}
		}
		
		/* Updates the current time.
		   Returns true if the time changed.
		 */
		bool update() {
			// The time elapsed since the start is obtained
			int now_ = get_time() - start_;
			
			// If the time changed, the time is updated and true is returned.
			if (now < now_) {
				now = now_;
				return true;
			}
			
			// Else, false is returned.
			return false;
		}
		
	private:
		Surface* colon; // The separator of the minutes and seconds
		Surface* numbers[DIGITS]; // The digits 0 - 9
		double start_; // The time that the timer was started at.
		int now; // The current time (in seconds rounded down).
};

/* Class for the hint counter
 */
class HintCounter {
	public:
		/* Creates sprites for the numbers and sets their heights
		 */
		HintCounter(Display* display) {
			for (int i = 0; i < DIGITS; i++) {
				numbers[i] = new Surface(
					NUMBER_IMAGES[i], display->get_width(), display->get_height(),
					0, HINT_COUNT_Y, NUMBER_WIDTH, NUMBER_HEIGHT
				);
			}
		}
		
		/* Deletes the sprites for the numbers
		 */
		~HintCounter() {
			for (int i = 0; i < DIGITS; i++) {
				delete numbers[i];
			}
		}
	
		/* Sets the number of hints to zero
		 */
		void reset() {
			hints = 0;
		}
		
		/* Sets the number of hints to the maximum
		 */
		void max() {
			hints = BOARD_SIZE * BOARD_SIZE;
		}
		
		/* Increments the number of hints
		 */
		void operator++(int) {
			hints++;
		}
		
		void blit(SDL_Surface* surface) {
			int tens = hints / 10;
			int units = hints % 10;
			
			numbers[tens]->blit_x(surface, TEN_X);
			numbers[units]->blit_x(surface, UNIT_X);
		}
		
	private:
		Surface* numbers[DIGITS];
		int hints;
};

/* Starts a game of Picross
 */
void game(Display* display, MusicQueuer* music_queuer, Surface* background) {
	Board board(display);
	Timer timer(display);
	HintCounter hint_counter(display);
	
	Surface hint(
		HINT_IMAGE, display->get_width(), display->get_height(),
		BUTTON_X, HINT_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	Surface solve(
		SOLVE_IMAGE, display->get_width(), display->get_height(),
		BUTTON_X, SOLVE_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	Surface quit(
		QUIT_IMAGE, display->get_width(), display->get_height(),
		BUTTON_X, QUIT2_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	Surface new_(
		NEW_IMAGE, display->get_width(), display->get_height(),
		BUTTON_X, NEW_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	Surface reset(
		RESET_IMAGE, display->get_width(), display->get_height(),
		BUTTON_X, RESET_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	Surface hints(
		HINTS_IMAGE, display->get_width(), display->get_height(),
		HINTS_X, HINTS_Y, HINTS_WIDTH, HINTS_HEIGHT
	);
	
	Surface time_(
		TIME_IMAGE, display->get_width(), display->get_height(),
		TIME_X, TIME_Y, TIME_WIDTH, TIME_HEIGHT
	);
		
	// The mouse
	SDL_Point mouse;
	
	// Main game loop
	while (true) {
		timer.start();
		hint_counter.reset();
		bool new_puzzle = false;
		bool reset_puzzle = false;
		
		// Puzzle solving loop
		while (!new_puzzle && !reset_puzzle && !board.solved()) {
			// Music is requeued if necessary
			music_queuer->requeue();
			
			// The mouse is checked for a click
			if (SDL_GetMouseState(NULL, NULL) & LEFT_CLICK) {
				// The game waits for the mouse to be released
				while (SDL_GetMouseState(&mouse.x, &mouse.y) & LEFT_CLICK) {
					SDL_PumpEvents();
				}
				
				// If the player chose to quit, they are returned to the main menu
				if (quit.in_rect(&mouse)) {
					return;
				}
				
				// Else, if the player requested a hint, a square is filled in
				else if (hint.in_rect(&mouse)) {
					board.hint();
					hint_counter++;
				}
				
				// Else, if the player requested the solution, the solution is displayed
				else if (solve.in_rect(&mouse)) {
					board.solve();
					hint_counter.max();
				}
				
				// Else, if the player requested a new puzzle, a new puzzle is generated
				else if (new_.in_rect(&mouse)) {
					new_puzzle = true;
				}
				
				// Else, if the players requested for a reset, the puzzle is reset
				else if (reset.in_rect(&mouse)) {
					reset_puzzle = true;
				}
				
				// Else, the click is checked for whether it is on the board or not
				else {
					board.click_check(&mouse, true);
				}
				
				// The display is updated after a click.
				background->blit(display->get_surface());
				board.blit_board(display->get_surface());
				timer.blit(display->get_surface());
				hint_counter.blit(display->get_surface());
				hint.blit(display->get_surface());
				solve.blit(display->get_surface());
				quit.blit(display->get_surface());
				new_.blit(display->get_surface());
				reset.blit(display->get_surface());
				hints.blit(display->get_surface());
				time_.blit(display->get_surface());
				display->update();
			}
			
			// The right click is also checked
			else if (SDL_GetMouseState(NULL, NULL) & RIGHT_CLICK) {
				// The game waits for the mouse to be released
				while (SDL_GetMouseState(&mouse.x, &mouse.y) & RIGHT_CLICK) {
					SDL_PumpEvents();
				}
				
				// The click is checked for whether it is on the board or not
				board.click_check(&mouse, false);
				
				// The display is updated after a click.
				background->blit(display->get_surface());
				board.blit_board(display->get_surface());
				timer.blit(display->get_surface());
				hint_counter.blit(display->get_surface());
				hint.blit(display->get_surface());
				solve.blit(display->get_surface());
				quit.blit(display->get_surface());
				new_.blit(display->get_surface());
				reset.blit(display->get_surface());
				hints.blit(display->get_surface());
				time_.blit(display->get_surface());
				display->update();
			}
			
			// The display is updated, if the timer was updated.
			else if (timer.update()) {
				background->blit(display->get_surface());
				board.blit_board(display->get_surface());
				timer.blit(display->get_surface());
				hint_counter.blit(display->get_surface());
				hint.blit(display->get_surface());
				solve.blit(display->get_surface());
				quit.blit(display->get_surface());
				new_.blit(display->get_surface());
				reset.blit(display->get_surface());
				hints.blit(display->get_surface());
				time_.blit(display->get_surface());
				display->update();
			}
			
			// Events are updated
			SDL_PumpEvents();
		}
		
		// If the board was solved, the target is displayed (no marks)
		if (board.solved()) {
			board.blit_target(display->get_surface());
			display->update();
		}
		
		while (!new_puzzle && !reset_puzzle) {
			// The mouse is checked for a click
			if (SDL_GetMouseState(NULL, NULL) & LEFT_CLICK) {
				// The game waits for the mouse to be released
				while (SDL_GetMouseState(&mouse.x, &mouse.y) & LEFT_CLICK) {
					SDL_PumpEvents();
				}
				
				// The function returns if the player quits
				if (quit.in_rect(&mouse)) {
					return;
				}
				
				// A new puzzle will be generated
				else if (new_.in_rect(&mouse)) {
					new_puzzle = true;
				}
				
				// The puzzle will be reset
				else if (reset.in_rect(&mouse)) {
					reset_puzzle = true;
				}
			}
			
			// The music is requeued when necessary
			music_queuer->requeue();
			
			// Events are updated
			SDL_PumpEvents();
		}
		
		if (new_puzzle) {
			board.new_puzzle();
		}
		
		else {
			board.reset_puzzle();
		}
	}
}

/* Displays the help screen
 */
void help(Display* display, MusicQueuer* music_queuer) {
	Surface help_screen(
		HELP_SCREEN_IMAGE, display->get_width(), display->get_height(),
		HELP_SCREEN_X, HELP_SCREEN_Y, HELP_SCREEN_WIDTH, HELP_SCREEN_HEIGHT
	);
	
	help_screen.blit(display->get_surface());
	display->update();
	
	SDL_Point mouse;
	
	while (true) {
		// Clicking off of the help screen removes it
		if (SDL_GetMouseState(NULL, NULL) & LEFT_CLICK) {
			// Waits until the mouse is released
			while (SDL_GetMouseState(&mouse.x, &mouse.y) & LEFT_CLICK) {
				SDL_PumpEvents();
			}
			
			if (!help_screen.in_rect(&mouse)) {
				break;
			}
		}
			
		// Music is still requeued in the help screen
		music_queuer->requeue();
		
		// Events are updated
		SDL_PumpEvents();
	}
}

/* Initialises SDL and loads the main menu
 */
int main(int argc, char* argv[]) {
	// SDL is initialised for video and audio
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	
	// The display is initialised (the window and its surface)
	Display* display = new Display;
	
	// The main menu surfaces are initialised
	Surface* background = new Surface(
		BACKGROUND_IMAGE, 0, 0, display->get_width(), display->get_height()
	);
	
	Surface* title = new Surface(
		TITLE_IMAGE, display->get_width(), display->get_height(),
		TITLE_X, TITLE_Y, TITLE_WIDTH, TITLE_HEIGHT
	);
	
	Surface* play_mode = new Surface(
		PLAY_IMAGE, display->get_width(), display->get_height(),
		PLAY_X, PLAY_Y, PLAY_WIDTH, PLAY_HEIGHT
	);
	
	Surface* help_mode = new Surface(
		HELP_IMAGE, display->get_width(), display->get_height(),
		HELP_X, HELP_Y, HELP_WIDTH, HELP_HEIGHT
	);
	
	Surface* quit = new Surface(
		QUIT_IMAGE, display->get_width(), display->get_height(),
		QUIT_X, QUIT_Y, BUTTON_WIDTH, BUTTON_HEIGHT
	);
	
	// The menu's assets are displayed
	background->blit(display->get_surface());
	title->blit(display->get_surface());
	play_mode->blit(display->get_surface());
	help_mode->blit(display->get_surface());
	quit->blit(display->get_surface());
	
	display->update();
	
	// The song is loaded and played
	MusicQueuer* music_queuer = new MusicQueuer(SONG, SONG_LENGTH);
	
	// The mouse is declared
	SDL_Point mouse;
	
	// Main menu loop
	while (true) {
		// If the mouse is clicked, what was clicked is checked
		// If a display changing option was clicked, the main menu will
		//   be reblitted after the operation is completed
		if (SDL_GetMouseState(NULL, NULL) & LEFT_CLICK) {
			// The game waits for the mouse click to be released
			while (SDL_GetMouseState(&mouse.x, &mouse.y) & LEFT_CLICK) {
				SDL_PumpEvents();
			}
			
			// If play was clicked, the game begins
			if (play_mode->in_rect(&mouse)) {
				game(display, music_queuer, background);
				background->blit(display->get_surface());
				title->blit(display->get_surface());
				play_mode->blit(display->get_surface());
				help_mode->blit(display->get_surface());
				quit->blit(display->get_surface());
				display->update();
			}
			
			// Else, if help was clicked, the help menu is displayed
			else if (help_mode->in_rect(&mouse)) {
				help(display, music_queuer);
				background->blit(display->get_surface());
				title->blit(display->get_surface());
				play_mode->blit(display->get_surface());
				help_mode->blit(display->get_surface());
				quit->blit(display->get_surface());
				display->update();
			}
			
			// Else, if quit was clicked, the game shuts down
			else if (quit->in_rect(&mouse)) {
				break;
			}
		}
		
		// Music is queued if necessary
		music_queuer->requeue();
		
		// Events are updated
		SDL_PumpEvents();
	}
	
	// Wrapper objects are deleted
	delete music_queuer;
	delete background;
	delete title;
	delete play_mode;
	delete help_mode;
	delete quit;
	
	// SDL is shutdown
	SDL_Quit();
	
	return 0;
}

/* CHANGELOG:
	 v1:
	   Release.
	 v1.1:
	   Left click and right click now cycle through cell states rather than setting them.
	 v1.2:
	   The display is only rendered to after a click or after a seocnd passes.
	 v1.2.0.1:
	   The display update is now paired with the rendering.
 */