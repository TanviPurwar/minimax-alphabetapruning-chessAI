#ifndef GAMES_CHESS_AI_HPP
#define GAMES_CHESS_AI_HPP

#include "impl/chess.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "player.hpp"

#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/attr_wrapper.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional #includes here
// <<-- /Creer-Merge: includes -->>
#include <chrono>

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This is the header file for building your Chess AI
/// </summary>
class AI : public Base_ai
{
public:
    /// <summary>
    /// This is a reference to the Game object itself, it contains all the information about the current game
    /// </summary>
    Game game;

    /// <summary>
    /// This is a pointer to your AI's player. This AI class is not a player, but it should command this Player.
    /// </summary>
    Player player;

    //<<-- Creer-Merge: class variables -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional class variables here.
    //<<-- /Creer-Merge: class variables -->>

    /// <summary>
    /// struct that contains information for chess board state along with default constructer to initialize values
    /// </summary>
    struct GameState {
        char board[8][8]; // Board layout: 'p', 'P', 'r', 'R', etc. or '-' for empty
        bool isWhiteTurn; // True = White's turn, False = Black's
        std::string enPassantSquare; // e.g. "e3" or "-" if not available
        bool whiteCanCastleKingside;
        bool whiteCanCastleQueenside;
        bool blackCanCastleKingside;
        bool blackCanCastleQueenside;
        bool whiteInCheck;
        bool blackInCheck;
        std::string encodedActionLeadingToCurrentState;
    };

    /// <summary>
    /// maps piece to whether it is sliding or not
    /// </summary>
    static std::unordered_map<char, bool> isSlidingPiece;
    
    /// <summary>
    /// maps piece to direction it wants to move
    /// </summary>
    static std::unordered_map<char, std::vector<std::pair<int, int>>> moveDirections;

    /// <summary>
    /// Piece values: pawn=1, knight=3, bishop=3, rook=5, queen=9, king=100
    /// </summary>
    static std::unordered_map<char, int> pieceValues;

    static int timeLimitMillis;
    static std::chrono::steady_clock::time_point startTime;

    /// <summary>
    /// This returns your AI's name to the game server.
    /// Replace the string name.
    /// </summary>
    /// <returns>The name of your AI.</returns>
    virtual std::string get_name() const override;

    /// <summary>
    /// This is automatically called when the game first starts, once the game objects are created
    /// </summary>
    virtual void start() override;

    /// <summary>
    /// This is automatically called when the game ends.
    /// </summary>
    /// <param name="won">true if you won, false otherwise</param>
    /// <param name="reason">An explanation for why you either won or lost</param>
    virtual void ended(bool won, const std::string& reason) override;

    /// <summary>
    /// This is automatically called the game (or anything in it) updates
    /// </summary>
    virtual void game_updated() override;

    /// <summary>
    /// This is called every time it is this AI.player's turn to make a move.
    /// </summary>
    /// <returns>A string in Universal Chess Interface (UCI) or Standard Algebraic Notation (SAN) formatting for the move you want to make. If the move is invalid or not properly formatted you will lose the game.</returns>
    std::string make_move();

    // <<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional methods here.
    // <<-- /Creer-Merge: methods -->>

    /// <summary>
    /// generates the state of board from FEN string
    /// </summary>
    /// <returns> GameState struct describing state of board </returns>
    GameState parseFEN(std::string currFEN);

    /// <summary>
    /// sees if move leads to check
    /// </summary>
    /// <returns> bool for in check or not </returns>
    bool isInCheck(GameState& state, bool forWhite);

    /// <summary>
    /// get ordered list of moves (implementation of move ordering)
    /// 1. Checkmate (not implemented)
    /// 2. Checks 
    /// 3. Captures 
    /// 4. Castling (not implemented)
    /// 5. pawn promotion 
    /// 6. Forward positional moves
    /// 7. lateral or backward moves 
    /// </summary
    /// <returns> priority based ordered vector containing each valid move </returns>
    std::vector<std::vector<std::vector<int>>> getOrderedValidMoves(GameState sCurrent);

    /// <summary>
    /// check if certain time has passed during minimax search
    /// </summary>
    ///<returns> boolean letting us know when to cut off search </returns>
    bool timeExceeded();

    /// <summary>
    /// transition function for board state and action
    /// </summary>
    /// <returns> new game state due to action </returns>
    GameState transitionFunction(GameState sCurrent, std::vector<int> move);

    /// <summary>
    /// terminates at max depth
    /// </summary>
    /// <returns> boolean for if max depth is reached </returns>
    bool cutOffTest(GameState sCurrent, int depth);

    /// <summary>
    /// evaluates heval of the board.
    /// </summary>
    /// <returns> evaluation score for board state </returns>
    int hEval(GameState sCurrent);

    /// <summary>
    /// calculates max value for maximizing player with alpha beta pruning
    /// </summary>
    /// <returns> value for maximising and action leading to that state </returns>
    std::pair<int, std::string> maxValue(GameState sCurrent, int depth, int alpha, int beta);

    /// <summary>
    /// calculates min value for minimizing player with alpha beta pruning
    /// </summary>
    /// <returns> value for minimising  and action leading to that state </returns>
    std::pair<int, std::string> minValue(GameState sCurrent, int depth, int alpha, int beta);

    /// <summary>
    /// performs Iterative-Deepening Depth-Limited MiniMax with Alpha Beta Pruning (TL-ID-DL-MM-AB) for each player's turn 
    /// </summary>
    /// <returns> coordinates containing best move initial -> final </returns>
    std::string IterativeDeepeningHeuristicDepthLimitedMinimaxWithAlphaBetaPruning(GameState sCurrent);

    /// <summary>
    /// print board state for debugging
    /// </summary>
    void printGameState(const GameState& state);

    // ####################
    // Don't edit these!
    // ####################
    /// \cond FALSE
    virtual std::string invoke_by_name(const std::string& name,
                                       const std::unordered_map<std::string, Any>& args) override;
    virtual void set_game(Base_game* ptr) override;
    virtual void set_player(std::shared_ptr<Base_object> obj) override;
    virtual void print_win_loss_info() override;
    /// \endcond
    // ####################
    // Don't edit these!
    // ####################

};

} // CHESS

} // cpp_client

#endif // GAMES_CHESS_AI_HPP
