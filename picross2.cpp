#include <iostream>
#include "sdlandnet.hpp"

// Constants
//{
// The current version of the program.
constexpr int VERSION[System::VERSION_LENGTH] = {2, 0, 0, 1};

// Window title
constexpr const char* TITLE = "Picross 2 by Chigozie Agomo";

// Numerical Constants
//{
constexpr int DIGITS = 10;
constexpr int BOARD_SIZE = 9;
constexpr int MAX_HINT = (BOARD_SIZE + (BOARD_SIZE % 2)) / 2;
constexpr int EMPTY = 0;
constexpr int FILLED = 1;
constexpr int MARKED = -1;
constexpr int MAX_TIME = 100;
//}

// Image locations
//{
constexpr const char* BACKGROUND_IMAGE = "data/background.bmp";
constexpr const char* TITLE_IMAGE = "data/title.bmp";
constexpr const char* PLAY_IMAGE = "data/play.bmp";
constexpr const char* HELP_IMAGE = "data/help.bmp";
constexpr const char* HELP_SCREEN_IMAGE = "data/helpscreen.bmp";
constexpr const char* BLANK_SQUARE_IMAGE = "data/blanksquare.bmp";
constexpr const char* FILLED_SQUARE_IMAGE = "data/filledsquare.bmp";
constexpr const char* MARKED_SQUARE_IMAGE = "data/markedsquare.bmp";
constexpr const char* QUIT_IMAGE = "data/quit.bmp";
constexpr const char* HINT_IMAGE = "data/hint.bmp";
constexpr const char* SOLVE_IMAGE = "data/solve.bmp";
constexpr const char* NEW_IMAGE = "data/new.bmp";
constexpr const char* RESET_IMAGE = "data/reset.bmp";
constexpr const char* TIME_IMAGE = "data/time.bmp";
constexpr const char* HINTS_IMAGE = "data/hints.bmp";
constexpr const char* COLON_IMAGE = "data/colon.bmp";
constexpr const char* NUMBER_IMAGES[DIGITS] = {
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
constexpr const char* NUMBER_SQUARE_IMAGES[DIGITS] = {
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
//}

// Sprite size ratio constants
//{
constexpr double BUTTON_X = 0.9;
constexpr double BUTTON_WIDTH = 0.1;
constexpr double BUTTON_HEIGHT = 0.075;
constexpr double TITLE_X = 0.5;
constexpr double TITLE_Y = 0.2;
constexpr double TITLE_WIDTH = 0.75;
constexpr double TITLE_HEIGHT = 0.3;
constexpr double PLAY_X = 0.5;
constexpr double PLAY_Y = 0.65;
constexpr double PLAY_WIDTH = 0.2;
constexpr double PLAY_HEIGHT = 0.15;
constexpr double HELP_X = 0.5;
constexpr double HELP_Y = 0.85;
constexpr double HELP_WIDTH = 0.2;
constexpr double HELP_HEIGHT = 0.15;
constexpr double QUIT_X = 0.925;
constexpr double QUIT_Y = 0.925;
constexpr double HELP_SCREEN_X = 0.5;
constexpr double HELP_SCREEN_Y = 0.5;
constexpr double HELP_SCREEN_WIDTH = 0.9;
constexpr double HELP_SCREEN_HEIGHT = 0.8;
constexpr double GRID_X = 0.325;
constexpr double GRID_Y = 0.5;
constexpr double GRID_WIDTH = 0.6;
constexpr double GRID_HEIGHT = 0.95;
constexpr double SQUARE_WIDTH = GRID_WIDTH / (BOARD_SIZE + MAX_HINT);
constexpr double SQUARE_HEIGHT = GRID_HEIGHT / (BOARD_SIZE + MAX_HINT);
constexpr double NEW_Y = 0.5;
constexpr double RESET_Y = 0.6;
constexpr double HINT_Y = 0.7;
constexpr double SOLVE_Y = 0.8;
constexpr double QUIT2_Y = 0.9;
constexpr double TIME_X = BUTTON_X;
constexpr double TIME_Y = 0.1;
constexpr double TIME_WIDTH =  BUTTON_WIDTH;
constexpr double TIME_HEIGHT = BUTTON_HEIGHT;
constexpr double COLON_X = TIME_X;
constexpr double COLON_Y = 0.2;
constexpr double COLON_WIDTH = 0.02;
constexpr double COLON_HEIGHT = TIME_HEIGHT;
constexpr double NUMBER_WIDTH = 0.04;
constexpr double NUMBER_HEIGHT = COLON_HEIGHT;
constexpr double HINTS_X = TIME_X;
constexpr double HINTS_Y = 0.3;
constexpr double HINTS_WIDTH = TIME_WIDTH;
constexpr double HINTS_HEIGHT = TIME_HEIGHT;
constexpr double MINUTE_X2 = COLON_X - COLON_WIDTH / 2 - NUMBER_WIDTH / 2;
constexpr double MINUTE_X1 = MINUTE_X2 - NUMBER_WIDTH;
constexpr double SECOND_X1 = COLON_X + COLON_WIDTH / 2 + NUMBER_WIDTH / 2;
constexpr double SECOND_X2 = SECOND_X1 + NUMBER_WIDTH;
constexpr double HINT_COUNT_Y = 0.4;
constexpr double TEN_X = HINTS_X - NUMBER_WIDTH / 2;
constexpr double UNIT_X = HINTS_X + NUMBER_WIDTH / 2;
//}

// Song location
constexpr const char* SONG = "data/song.wav";

// Song length lower bound (for requeuing)
constexpr int SONG_LENGTH = 56;
//}

/* The Picross Board
   Contains the pattern to be found, the player's pattern, and the hints
 */
class Board {
	public:
		/* Initialises the RNG and produces a grid with hints.
		 */
		Board(const Sprite& sprite):
            generator()
        {
			// The puzzle is initialised.
			new_puzzle();
			
			// The surfaces are initialised
            resize(sprite);
		}
				
		/* Prints the target board to display (including hints).
		 */
		void print_target() {
			std::cout << '\n';
			
			// Column hints are printed first
			for (int i = 0; i < MAX_HINT; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					std::cout << "  ";
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; ++j) {
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
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					if (rows[i][j]) {
						std::cout << rows[i][j] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; ++j) {
					std::cout << (target[i][j] ? 'O' : ' ') << ' ';
				}
				
				std::cout << ' ' << i << '\n';
			}
			
			std::cout << '\n';
			
			for (int i = 0; i < MAX_HINT; ++i) {
				std::cout << "  ";
			}
			
			std::cout << ' ';
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				std::cout << i << ' ';
			}
			
			std::cout << "\n\n";
		}
		
		/* Prints the player's board to display (including hints).
		 */
		void print_board() {
			std::cout << '\n';
			
			// Column hints are printed first
			for (int i = 0; i < MAX_HINT; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					std::cout << "  ";
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; ++j) {
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
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					if (rows[i][j]) {
						std::cout << rows[i][j] << ' ';
					}
					
					else {
						std::cout << "  ";
					}
				}
				
				std::cout << ' ';
				
				for (int j = 0; j < BOARD_SIZE; ++j) {
					std::cout << (board[i][j] ? (board[i][j] > 0 ? 'O' : 'X') : ' ') << ' ';
				}
				
				std::cout << ' ' << i << '\n';
			}
			
			std::cout << '\n';
			
			for (int i = 0; i < MAX_HINT; ++i) {
				std::cout << "  ";
			}
			
			std::cout << ' ';
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				std::cout << i << ' ';
			}
			
			std::cout << "\n\n";
		}
		
		/* Returns true if the player's board matches the target.
           A simple equality match is insufficient, as EMPTY and MARKED should match.
		 */
		bool solved() {
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0 ; j < BOARD_SIZE; ++j) {
					if (
                        target[i][j] == FILLED && board[i][j] != FILLED
                        || target[i][j] != FILLED && board[i][j] == FILLED
                    ) {
                        return false;
					}
				}
			}
			
			return true;
		}
		
		/* Fills in a cell of the board.
		   Duplicate commands erase the square.
		 */
		void fill(int command, int i, int j) {
			board[j][i] = board[j][i] == command ? 0 : command;
		}
			
		/* Blits the target to the given sprite.
		 */
		void blit_target(Sprite& sprite) {
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < BOARD_SIZE; ++j) {
					switch (board[i][j]) {
						case FILLED:
                            sprite.blit(
                                *filled_square,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
                            
							break;
							
						default:
                            sprite.blit(
                                *blank_square,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
					}
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					sprite.blit(
						*number_squares[rows[i][j]],
						x + (i + MAX_HINT) * width,
						y + j * height
					);
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					sprite.blit(
						*number_squares[columns[i][j]],
						x + j * width,
						y + (i + MAX_HINT) * height
					);
				}
			}
		}
		
		/* Blits the board to the given sprite.
		 */
		void blit_board(Sprite& sprite) {
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < BOARD_SIZE; ++j) {
					switch (board[i][j]) {
						case FILLED:
							sprite.blit(
								*filled_square,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
							break;
							
						case MARKED:
							sprite.blit(
								*marked_square,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
							break;
							
						default:
							sprite.blit(
								*blank_square,
								x + (i + MAX_HINT) * width,
								y + (j + MAX_HINT) * height
							);
					}
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					sprite.blit(
						*number_squares[rows[i][j]],
						x + (i + MAX_HINT) * width,
						y + j * height
					);
				}
			}
			
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < MAX_HINT; ++j) {
					sprite.blit(
						*number_squares[columns[i][j]],
						x + j * width,
						y + (i + MAX_HINT) * height
					);
				}
			}
		}

		/* Makes a new puzzle.
		 */
		void new_puzzle() {
            // The board, row hints, and column hints are (re)initialised.
            board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>();
            rows = std::array<std::array<int, MAX_HINT>, BOARD_SIZE>();
            columns = std::array<std::array<int, MAX_HINT>, BOARD_SIZE>();
            
			// Target board and player board initialised
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < BOARD_SIZE; ++j) {
					target[i][j] = generator.get_int(EMPTY, FILLED);
				}
			}
            
			// Row hints initialised.
			for (int i = 0; i < BOARD_SIZE; ++i) {
				int position = MAX_HINT - 1;
				
				for (int j = BOARD_SIZE - 1; j >= 0; --j) {
					if (target[i][j]) {
						++rows[i][position];
					}
					
					else if (rows[i][position]) {
						--position;
					}
				}
			}
			
			// Column hints initialised.
			for (int i = 0; i < BOARD_SIZE; ++i) {
				int position = MAX_HINT - 1;
				
				for (int j = BOARD_SIZE - 1; j >= 0; --j) {
					if (target[j][i]) {
						++columns[i][position];
					}
					
					else if (columns[i][position]) {
						--position;
					}
				}
			}
		}
		
		/* Resets the puzzle.
		 */
		void reset_puzzle() {
			// Player board initialised
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < BOARD_SIZE; ++j) {
					board[i][j] = 0;
				}
			}
		}

		/* Checks if the board was clicked.
           If it was, the cell's state is cycled.
		 */
		void click_check(const Point& point, bool left_click, bool act) {
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0; j < BOARD_SIZE; ++j) {
					Rectangle rect(
                        x + (i + MAX_HINT) * width,
                        y + (j + MAX_HINT) * height,
                        width,
                        height
					);
					
					if (rect.contains(point)) {
                        if (act && i == x_index && j == y_index) {
                            int command = left_click ? 1 : -1;
                            board[i][j] += command;
                            
                            if (board[i][j] > FILLED) {
                                board[i][j] = MARKED;
                            }
                            
                            else if (board[i][j] < MARKED) {
                                board[i][j] = FILLED;
                            }
                        }
                        
                        else if (!act) {
                            x_index = i;
                            y_index = j;
                        }
						
						return;
					}
				}
			}
            
            x_index = -1;
            y_index = -1;
		}
		
		/* Solves a single sqaure.
		   The square is chosen randomly.
		 */
		void hint() {
            if (solved()) {
                return;
            }
            
            // Loop until a square that is incorrect is found.
            for (
                // The first square to be checked is chosen at random.
                int index = generator.get_int(0, BOARD_SIZE * BOARD_SIZE - 1);;
                index = (index + 1) % (BOARD_SIZE * BOARD_SIZE - 1)
            ) {
                int i = index / BOARD_SIZE;
                int j = index % BOARD_SIZE;
                
                if (
                    target[i][j] == FILLED && board[i][j] != FILLED
                    || target[i][j] != FILLED && board[i][j] == FILLED
                ) {
                    board[i][j] = target[i][j];
                    return;
                }
            }
        }
		
		/* Solves the entire puzzle.
		 */
		void solve() {
			for (int i = 0; i < BOARD_SIZE; ++i) {
				for (int j = 0 ; j < BOARD_SIZE; ++j) {
					board[i][j] = target[i][j];
				}
			}
		}
		
        /* Sets the size of this object's sprites according to the size of the given sprite.
           Calculates the dimensions for blitting.
         */
        void resize(const Sprite& sprite) noexcept {
            blank_square = std::make_unique<Sprite>(
                BLANK_SQUARE_IMAGE, sprite, SQUARE_WIDTH, SQUARE_HEIGHT
            );
            
            filled_square = std::make_unique<Sprite>(
                FILLED_SQUARE_IMAGE, sprite, SQUARE_WIDTH, SQUARE_HEIGHT
            );
            
            marked_square = std::make_unique<Sprite>(
                MARKED_SQUARE_IMAGE, sprite, SQUARE_WIDTH, SQUARE_HEIGHT
            );
			
			for (int i = 0; i < DIGITS; ++i) {
				number_squares[i] = std::make_unique<Sprite>(
					NUMBER_SQUARE_IMAGES[i], sprite, SQUARE_WIDTH, SQUARE_HEIGHT
				);
			}
            
            // The dimensions are calculated.
			width = GRID_WIDTH * sprite.width();
			height = GRID_HEIGHT * sprite.height();
			x = GRID_X * sprite.width() - width / 2;
			y = GRID_Y * sprite.height() - height / 2;
			width = SQUARE_WIDTH * sprite.width();
			height = SQUARE_HEIGHT * sprite.height();
        }
        
	private:
		std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> target; // The pattern to be found.
		std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE> board; // The current pattern.
		std::array<std::array<int, MAX_HINT>, BOARD_SIZE> rows; // The row hints.
		std::array<std::array<int, MAX_HINT>, BOARD_SIZE> columns; // The column hints.
        
		RNG generator; // Pseudo-Random Number Generator.
        
		std::unique_ptr<Sprite> blank_square; // The sprite for a square that hasn't been filled yet.
		std::unique_ptr<Sprite> filled_square; // The sprite for a square that has been filled.
		std::unique_ptr<Sprite> marked_square; // The sprite for a square that has been marked as empty.
        
        // The digits 0 - 9 (used in for row/col hints).
		std::array<std::unique_ptr<Sprite>, DIGITS> number_squares;
        
		int x; // x co-ordinate of the board's left edge.
		int y; // y co-ordinate of the board's right edge.
		int width; // Width of a board square.
		int height; // Hieght of a board square.
        int x_index = -1; // The index of the last clicked column.
        int y_index = -1; // The index of the last clicked row.
};

/* Class for the game timer.
 */
class Clock {
	public:
		/* Initialises the sprites for the numbers and the colon.
		 */
		Clock(const Sprite& sprite) {
			resize(sprite);
            start();
		}
		
		/* Starts the timer
		 */
		void start() {
			start_ = Timer::time();
			now = -1;
		}
		
		/* Blits the timer to the given sprite.
		 */
		void blit_to(Sprite& sprite) {
			// The time is split into minutes and seconds
			int minutes = now / 60;
			int seconds = now % 60;
			
			// If too much time has passed, no time is displayed
			if (minutes < MAX_TIME) {
				// The minute-second separator is blitted
				colon->blit_to(sprite);
				
				// The digits to be used are calculated
				int m1 = minutes / 10;
				int m2 = minutes % 10;
				int s1 = seconds / 10;
				int s2 = seconds % 10;
                
                sprite.blit(*numbers[m1], MINUTE_X1, COLON_Y);
                sprite.blit(*numbers[m2], MINUTE_X2, COLON_Y);
                sprite.blit(*numbers[s1], SECOND_X1, COLON_Y);
                sprite.blit(*numbers[s2], SECOND_X2, COLON_Y);
			}
		}
		
		/* Updates the current time.
		   Returns true if the time changed.
		 */
		bool update() {
			// The time elapsed since the start is obtained
			int now_ = Timer::time() - start_;
			
			// If the time changed, the time is updated and true is returned.
			if (now < now_) {
				now = now_;
				return true;
			}
			
			// Else, false is returned.
			return false;
		}
		
        /* Sets the size of the colon and numbers according to the size of the given sprite.
         */
        void resize(const Sprite& sprite) noexcept {
            colon = std::make_unique<Button>(
                Sprite(COLON_IMAGE, sprite, COLON_WIDTH, COLON_HEIGHT),
                sprite, COLON_X, COLON_Y
            );
            
			for (int i = 0; i < DIGITS; ++i) {
				numbers[i] = std::make_unique<Sprite>(NUMBER_IMAGES[i], sprite, NUMBER_WIDTH, NUMBER_HEIGHT);
			}
        }
        
	private:
		std::unique_ptr<Button> colon; // The separator of the minutes and seconds.
		std::array<std::unique_ptr<Sprite>, DIGITS> numbers; // The digits 0 - 9.
		double start_; // The time that the timer was started at.
		int now; // The current time (in seconds rounded down).
};

/* Class for the hint counter.
 */
class HintCounter {
	public:
		/* Creates sprites for the numbers and sets their size based on the given sprite.
		 */
		HintCounter(const Sprite& sprite) noexcept {
            resize(sprite);
            reset();
		}
	
		/* Sets the number of hints to zero.
		 */
		void reset() noexcept {
			hints = 0;
		}
		
		/* Sets the number of hints to the maximum.
		 */
		void max() noexcept {
			hints = BOARD_SIZE * BOARD_SIZE;
		}
		
		/* Increments the number of hints.
		 */
		HintCounter& operator++() noexcept {
			++hints;
            return *this;
		}
		
        /* Blits the hint value to the given sprite.
         */
		void blit_to(Sprite& sprite) const noexcept {
			int tens = hints / 10;
			int units = hints % 10;
            
            sprite.blit(*numbers[tens], TEN_X, HINT_COUNT_Y);
            sprite.blit(*numbers[units], UNIT_X, HINT_COUNT_Y);
		}
        
        /* Sets the size of the numbers according to the size of the given sprite.
         */
        void resize(const Sprite& sprite) noexcept {
			for (int i = 0; i < DIGITS; ++i) {
				numbers[i] = std::make_unique<Sprite>(NUMBER_IMAGES[i], sprite, NUMBER_WIDTH, NUMBER_HEIGHT);
			}
        }
		
	private:
		std::array<std::unique_ptr<Sprite>, DIGITS> numbers; // The digits 0 - 9.
		int hints; // The number of hints used this round.
};

/* Starts a game of Picross.
 */
bool game(Display& display, Button& background) noexcept {
	// The game sprites are initialised.
    Button hint(Sprite(HINT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, HINT_Y);
    Button solve(Sprite(SOLVE_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, SOLVE_Y);
    Button quit(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, QUIT2_Y);
    Button new_(Sprite(NEW_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, NEW_Y);
    Button reset(Sprite(RESET_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, RESET_Y);
    Button hints(Sprite(HINTS_IMAGE, display, HINTS_WIDTH, HINTS_HEIGHT), display, HINTS_X, HINTS_Y);
    Button time_(Sprite(TIME_IMAGE, display, TIME_WIDTH, TIME_HEIGHT), display, TIME_X, TIME_Y);
    
    // Handles events.
    Event event;
    
    // True when the game loop should end.
    bool end = false;
    
    // True if the corresponding button was clicked.
    bool hint_clicked = false;
    bool solve_clicked = false;
    bool quit_clicked = false;
    bool new_clicked = false;
    bool reset_clicked = false;
    
    // The game elements are initialised just before the
    //   loop (for the timer to start as late as possible).
    Board board(display);
	HintCounter hint_counter(display);
	Clock timer(display);
		
	// Initialisation loop.
	while (!end) {
        bool round_end = false;
		bool show_result = true;
        
        // Main game loop.
        while (!end && !round_end) {
            // If the board was solved, the round ends.
            if (board.solved()) {
                board.solve();
                round_end = true;
            }
            
            // The timer is updated.
            timer.update();
            
            // The sprites are displayed.
            background.blit_to(display);
            board.blit_board(display);
            hint_counter.blit_to(display);
            timer.blit_to(display);
            hint.blit_to(display);
            solve.blit_to(display);
            quit.blit_to(display);
            new_.blit_to(display);
            reset.blit_to(display);
            hints.blit_to(display);
            time_.blit_to(display);
            display.update();
            
            // Real-time event handling loop.
            while (!end && !round_end && event.poll()) {
                // Check event type.
                switch (event.type()) {
                    // End the game.
                    case Event::TERMINATE: //{
                        return true;
                    //}
                    
                    // Resize (and rotation) handling.
                    case Event::RESIZE: //{
                        // The display and sprites are resized.
                        display.resize(event.window_width(), event.window_height());
                        board.resize(display);
                        timer.resize(display);
                        hint_counter.resize(display);
                        
                        background = Button(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
                        hint = Button(Sprite(HINT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, HINT_Y);
                        solve = Button(Sprite(SOLVE_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, SOLVE_Y);
                        quit = Button(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, QUIT2_Y);
                        new_ = Button(Sprite(NEW_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, NEW_Y);
                        reset = Button(Sprite(RESET_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, RESET_Y);
                        hints = Button(Sprite(HINTS_IMAGE, display, HINTS_WIDTH, HINTS_HEIGHT), display, HINTS_X, HINTS_Y);
                        time_ = Button(Sprite(TIME_IMAGE, display, TIME_WIDTH, TIME_HEIGHT), display, TIME_X, TIME_Y);
                        
                        break;
                    //}
                    
                    // Mouse clicks (and screen touches).
                    case Event::LEFT_CLICK: //{
                        if (hint.get_rectangle().contains(event.click_position())) {
                            hint_clicked = true;
                        }
                        
                        else if (solve.get_rectangle().contains(event.click_position())) {
                            solve_clicked = true;
                        }
                        
                        else if (quit.get_rectangle().contains(event.click_position())) {
                            quit_clicked = true;
                        }
                        
                        else if (new_.get_rectangle().contains(event.click_position())) {
                            new_clicked = true;
                        }
                        
                        else if (reset.get_rectangle().contains(event.click_position())) {
                            reset_clicked = true;
                        }
                        
                        // The board is notified of the click (or touch).
                        else {
                            board.click_check(event.click_position(), true, false);
                        }
                        
                        break;
                    //}
                    
                    // Mouse (and screen) releases.
                    case Event::LEFT_UNCLICK: //{
                        // If hint was clicked, a square is filled in.
                        if (hint_clicked && hint.get_rectangle().contains(event.click_position())) {
                            board.hint();
                            ++hint_counter;
                        }
                        
                        // Else, if solve was clicked, the solution is displayed.
                        else if (solve_clicked && solve.get_rectangle().contains(event.click_position())) {
                            board.solve();
                            hint_counter.max();
                            round_end = true;
                        }
                        
                        // Else, if quit was clicked, the main menu is returned to.
                        else if (quit_clicked && quit.get_rectangle().contains(event.click_position())) {
                            end = true;
                        }
                        
                        // Else, if new was clicked, a new puzzle is made.
                        else if (new_clicked && new_.get_rectangle().contains(event.click_position())) {
                            board.new_puzzle();
                            round_end = true;
                            show_result = false;
                        }
                        
                        // Else, if reset was clicked, the puzzle is reset.
                        else if (reset_clicked && reset.get_rectangle().contains(event.click_position())) {
                            board.reset_puzzle();
                            round_end = true;
                            show_result = false;
                        }
                        
                        // Else, the board is notified of the release.
                        else {
                            board.click_check(event.click_position(), true, true);
                        }
                        
                        // The click statuses are reset.
                        hint_clicked = false;
                        solve_clicked = false;
                        quit_clicked = false;
                        new_clicked = false;
                        reset_clicked = false;
                        
                        break;
                    //}
                    
                    // Right clicks.
                    case Event::RIGHT_CLICK: //{
                        board.click_check(event.click_position(), false, false);
                        break;
                    //}
                    
                    // Right click releases.
                    case  Event::RIGHT_UNCLICK: //{
                        board.click_check(event.click_position(), false, true);
                        break;
                    //}
                }
            }
        }
        
        // Wait for the player to quit, get a new puzzle, or reset the puzzle.
        while (!end && show_result) {
            // The sprites are displayed.
            background.blit_to(display);
            board.blit_board(display);
            hint_counter.blit_to(display);
            timer.blit_to(display);
            hint.blit_to(display);
            solve.blit_to(display);
            quit.blit_to(display);
            new_.blit_to(display);
            reset.blit_to(display);
            hints.blit_to(display);
            time_.blit_to(display);
            display.update();
            
            // Wait for an event.
            event.wait();
            
            // Check event type.
            switch (event.type()) {
                // End the game.
                case Event::TERMINATE: //{
                    return true;
                //}
                
                // Resize (and rotation) handling.
                case Event::RESIZE: //{
                    // The display and sprites are resized.
                    display.resize(event.window_width(), event.window_height());
                    board.resize(display);
                    timer.resize(display);
                    hint_counter.resize(display);
                    
                    background = Button(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
                    hint = Button(Sprite(HINT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, HINT_Y);
                    solve = Button(Sprite(SOLVE_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, SOLVE_Y);
                    quit = Button(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, QUIT2_Y);
                    new_ = Button(Sprite(NEW_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, NEW_Y);
                    reset = Button(Sprite(RESET_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, BUTTON_X, RESET_Y);
                    hints = Button(Sprite(HINTS_IMAGE, display, HINTS_WIDTH, HINTS_HEIGHT), display, HINTS_X, HINTS_Y);
                    time_ = Button(Sprite(TIME_IMAGE, display, TIME_WIDTH, TIME_HEIGHT), display, TIME_X, TIME_Y);
                    
                    break;
                //}
                
                // Mouse clicks (and screen touches).
                case Event::LEFT_CLICK: //{
                    if (quit.get_rectangle().contains(event.click_position())) {
                        quit_clicked = true;
                    }
                    
                    else if (new_.get_rectangle().contains(event.click_position())) {
                        new_clicked = true;
                    }
                    
                    else if (reset.get_rectangle().contains(event.click_position())) {
                        reset_clicked = true;
                    }
                    
                    break;
                //}
                
                // Mouse (and screen) releases.
                case Event::LEFT_UNCLICK: //{
                    // If quit was clicked, the main menu is returned to.
                    if (quit_clicked && quit.get_rectangle().contains(event.click_position())) {
                        end = true;
                    }
                    
                    // Else, if new was clicked, a new puzzle is made.
                    else if (new_clicked && new_.get_rectangle().contains(event.click_position())) {
                        board.new_puzzle();
                        show_result = false;
                    }
                    
                    // Else, if reset was clicked, the puzzle is reset.
                    else if (reset_clicked && reset.get_rectangle().contains(event.click_position())) {
                        board.reset_puzzle();
                        show_result = false;
                    }
                    
                    // The click statuses are reset.
                    quit_clicked = false;
                    new_clicked = false;
                    reset_clicked = false;
                    
                    break;
                //}
            }
        }
		
        #ifdef PICROSS_2_V1
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
					++hint_counter;
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
        #endif
        
        // The timer and hint counter are reset.
		timer.start();
		hint_counter.reset();
	}
    
    return false;
}

/* Displays the help screen.
 */
bool help(Display& display, const Sprite& snapshot) noexcept {
    Button help_screen(
        Sprite(HELP_SCREEN_IMAGE, display, HELP_SCREEN_WIDTH, HELP_SCREEN_HEIGHT),
        display, HELP_SCREEN_X, HELP_SCREEN_Y
    );
	
    // Handles events.
    Event event;
    
    // True when the help screen should be closed.
    bool end = false;
    
	while (!end) {
        // The help screen is displayed.
        help_screen.blit_to(display);
        display.update();
            
        // Wait for an event.
        event.wait();
        
        // Handle the event.
        switch (event.type()) {
            // End the game.
            case Event::TERMINATE: //{
                return true;
            //}
            
            // Resize (and rotation) handling.
            case Event::RESIZE: //{
                // The display is reinitialised.
                display.resize(event.window_width(), event.window_height());
                
                // The help screen is resized.
                help_screen = Button(
                    Sprite(HELP_SCREEN_IMAGE, display, HELP_SCREEN_WIDTH, HELP_SCREEN_HEIGHT),
                    display, HELP_SCREEN_X, HELP_SCREEN_Y
                );
                
                // The display is returned to its former state.
                display.blit(snapshot);
                
                break;
            //}
            
            // A click (or touch) exits the help screen.
            case Event::LEFT_UNCLICK: //{
                end = true;
                break;
            //}
        }
	}
    
    return false;
}

/* Initialises SDL and loads the main menu.
 */
int main(int argc, char** argv) {
	// SDL is initialised for video and audio
	System::initialise(System::VIDEO | System::AUDIO);
    
    // The version of this program and the version of
    //   the SDL and Net Utilities library are displayed.
    std::string version_string = System::version(VERSION);
    std::cout
        << "\nPicross 2 by Chigozie Agomo.\nVersion: "
        << version_string
        << "\n\nPowered by:\n"
        << System::info()
        << "\n\n"
    ;
	
    {
        // The display is initialised.
        Display display;
        
        // The main menu sprites are initialised.
        Button background(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
        Button title(Sprite(TITLE_IMAGE, display, TITLE_WIDTH, TITLE_HEIGHT), display, TITLE_X, TITLE_Y);
        Button play_mode(Sprite(PLAY_IMAGE, display, PLAY_WIDTH, PLAY_HEIGHT), display, PLAY_X, PLAY_Y);
        Button help_mode(Sprite(HELP_IMAGE, display, HELP_WIDTH, HELP_HEIGHT), display, HELP_X, HELP_Y);
        Button quit(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, QUIT_X, QUIT_Y);
        
        // The song is loaded and played
        AudioThread music_queuer(SONG, SONG_LENGTH);
        
        // Handles events.
        Event event;
        
        // True when the game should end.
        bool end = false;
        
        // True if the corresponding button was clicked.
        bool play_clicked = false;
        bool help_clicked = false;
        bool quit_clicked = false;
        
        // Main menu loop
        while (!end) {
            // The menu's sprites are displayed.
            background.blit_to(display);
            title.blit_to(display);
            play_mode.blit_to(display);
            help_mode.blit_to(display);
            quit.blit_to(display);
            display.update();
            
            // Wait for an event.
            event.wait();
            
            // Handle the event.
            switch (event.type()) {
                // End the game.
                case Event::TERMINATE: //{
                    end = true;
                    break;
                //}
                
                // Resize (and rotation) handling.
                case Event::RESIZE: //{
                    // The display is resized.
                    display.resize(event.window_width(), event.window_height());
                    
                    // The sprites are resized.
                    background = Button(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
                    title = Button(Sprite(TITLE_IMAGE, display, TITLE_WIDTH, TITLE_HEIGHT), display, TITLE_X, TITLE_Y);
                    play_mode = Button(Sprite(PLAY_IMAGE, display, PLAY_WIDTH, PLAY_HEIGHT), display, PLAY_X, PLAY_Y);
                    help_mode = Button(Sprite(HELP_IMAGE, display, HELP_WIDTH, HELP_HEIGHT), display, HELP_X, HELP_Y);
                    quit = Button(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, QUIT_X, QUIT_Y);
                    
                    break;
                //}
                
                // Mouse clicks (and screen touches).
                case Event::LEFT_CLICK: //{
                    if (play_mode.get_rectangle().contains(event.click_position())) {
                        play_clicked = true;
                    }
                    
                    else if (help_mode.get_rectangle().contains(event.click_position())) {
                        help_clicked = true;
                    }
                    
                    else if (quit.get_rectangle().contains(event.click_position())) {
                        quit_clicked = true;
                    }
                    
                    break;
                //}
                
                // Mouse (and screen) releases.
                case Event::LEFT_UNCLICK: //{
                    // If play was clicked, the game begins.
                    if (play_clicked && play_mode.get_rectangle().contains(event.click_position())) {
                        end = game(display, background);
                    
                        // The sprites are resized.
                        background = Button(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
                        title = Button(Sprite(TITLE_IMAGE, display, TITLE_WIDTH, TITLE_HEIGHT), display, TITLE_X, TITLE_Y);
                        play_mode = Button(Sprite(PLAY_IMAGE, display, PLAY_WIDTH, PLAY_HEIGHT), display, PLAY_X, PLAY_Y);
                        help_mode = Button(Sprite(HELP_IMAGE, display, HELP_WIDTH, HELP_HEIGHT), display, HELP_X, HELP_Y);
                        quit = Button(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, QUIT_X, QUIT_Y);
                    }
                    
                    // Else, if help was clicked, the help menu is displayed.
                    else if (help_clicked && help_mode.get_rectangle().contains(event.click_position())) {
                        // The display's current appearance is stored.
                        Sprite snapshot(display);
                        end = help(display, snapshot);
                        
                        // The sprites are resized.
                        background = Button(Sprite(BACKGROUND_IMAGE, display.width(), display.height()));
                        title = Button(Sprite(TITLE_IMAGE, display, TITLE_WIDTH, TITLE_HEIGHT), display, TITLE_X, TITLE_Y);
                        play_mode = Button(Sprite(PLAY_IMAGE, display, PLAY_WIDTH, PLAY_HEIGHT), display, PLAY_X, PLAY_Y);
                        help_mode = Button(Sprite(HELP_IMAGE, display, HELP_WIDTH, HELP_HEIGHT), display, HELP_X, HELP_Y);
                        quit = Button(Sprite(QUIT_IMAGE, display, BUTTON_WIDTH, BUTTON_HEIGHT), display, QUIT_X, QUIT_Y);
                    }
                    
                    // Else, if quit was clicked, the game shuts down.
                    else if (quit_clicked && quit.get_rectangle().contains(event.click_position())) {
                        end = true;
                    }
                    
                    // The click statuses are reset.
                    play_clicked = false;
                    help_clicked = false;
                    quit_clicked = false;
                    
                    break;
                //}
            }
        }
	}
	
	// The library is shut down.
    System::terminate();
    return 0;
}

/* CHANGELOG:
     v2.0.0.1:
       Hide hints upon solving the puzzle.
     v2:
       Use modern sdlandnet and reverse changelog order.
	 v1.2.0.1:
	   The display update is now paired with the rendering.
	 v1.2:
	   The display is only rendered to after a click or after a seocnd passes.
	 v1.1:
	   Left click and right click now cycle through cell states rather than setting them.
	 v1:
	   Release.
 */