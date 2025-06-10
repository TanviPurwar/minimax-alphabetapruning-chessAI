// AI
// This is where you build your AI
// Tanvi Purwar (tphtf)
// bonus implemented -> move ordering, time heuristic

#include "ai.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
// <<-- /Creer-Merge: includes -->>
#include <climits>
#include <chrono>

namespace cpp_client
{

namespace chess
{

std::chrono::steady_clock::time_point AI::startTime;
int AI::timeLimitMillis = 1000; // 2 seconds

/// <summary>
/// maps piece to direction it wants to move
/// </summary>
std::unordered_map<char, std::vector<std::pair<int, int>>> AI::moveDirections = {
    {'R', {{0,1}, {0,-1}, {1,0}, {-1,0}}},
    {'N', {{2,1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}}},
    {'B', {{1,1}, {-1,1}, {1,-1}, {-1,-1}}},
    {'Q', {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}},
    {'K', {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}},
    {'r', {{0,1}, {0,-1}, {1,0}, {-1,0}}},
    {'n', {{2,1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}}},
    {'b', {{1,1}, {-1,1}, {1,-1}, {-1,-1}}},
    {'q', {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}},
    {'k', {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}}
};

/// <summary>
/// maps piece to whether it is sliding or not
/// </summary>
std::unordered_map<char, bool> AI::isSlidingPiece = {
    {'R', true},
    {'N', false},
    {'B', true},
    {'Q', true},
    {'K', false},
    {'r', true},
    {'n', false},
    {'b', true},
    {'q', true},
    {'k', false}
};

/// <summary>
/// Piece values: pawn=1, knight=3, bishop=3, rook=5, queen=9, king=100
/// </summary>
std::unordered_map<char, int> AI::pieceValues = {
    {'P', 1}, {'N', 3}, {'B', 3}, {'R', 5}, {'Q', 9}, {'K', 100}, // White
    {'p', 1}, {'n', 3}, {'b', 3}, {'r', 5}, {'q', 9}, {'k', 100}  // Black
};

/// <summary>
/// sInit with values initialized in default constructor
/// </summary>
//GameState AI::sInit;

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Tanvi Purwar (tphtf)";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
    //<<-- /Creer-Merge: ended -->>
}

/// <summary>
/// This is called every time it is this AI.player's turn to make a move.
/// </summary>
/// <returns>A string in Universal Chess Interface (UCI) or Standard Algebraic Notation (SAN) formatting for the move you want to make. If the move is invalid or not properly formatted you will lose the game.</returns>
std::string AI::make_move()
{
    // <<-- Creer-Merge: makeMove -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // Put your game logic here for make_move here
    // <<-- /Creer-Merge: makeMove -->>
    
    std::string currFEN = game->fen;

    GameState sCurrent = parseFEN(currFEN);
    //printGameState(sCurrent);

    std::string moveFinal = IterativeDeepeningHeuristicDepthLimitedMinimaxWithAlphaBetaPruning(sCurrent);
    return moveFinal;

    /*if(player->color == "white"){
        std::cout << "White turn: " << game->fen << std::endl;
        return "c2c4";
    }
    else if(player->color == "black"){
        std::cout << "Black turn: " << game->fen << std::endl;
        return "d7d5";
    }*/
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional methods here for your AI to call
//<<-- /Creer-Merge: methods -->>

/// <summary>
/// generates the state of board from FEN string
/// </summary>
/// <returns> GameState struct describing state of board </returns>
AI::GameState AI::parseFEN(std::string currFEN){

    // split fen string by spaces
    std::vector<std::string> parts;
    std::string current;

    for (char ch : currFEN) {
        if (ch == ' ') {
            if (!current.empty()) {
                parts.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
    }
    // Add the last part
    if (!current.empty()) {
        parts.push_back(current);
    }

    GameState state;

    // 1. board
    int r = 0, c = 0;
    for (char ch : parts[0]) {
        if (ch == '/') {
            r++;
            c = 0;
        }
        else if (isdigit(ch)) {
            int empty = ch - '0';  
            for (int i = 0; i < empty; ++i) {
                state.board[r][c] = '-';
                c++;
            }
        }
        else {
            state.board[r][c] = ch;
            c++;
        }
    }   

    // 2. Turn
    state.isWhiteTurn = (parts[1] == "w");

    // 3. Castling
    state.whiteCanCastleKingside = parts[2].find('K') != std::string::npos;
    state.whiteCanCastleQueenside = parts[2].find('Q') != std::string::npos;
    state.blackCanCastleKingside = parts[2].find('k') != std::string::npos;
    state.blackCanCastleQueenside = parts[2].find('q') != std::string::npos;

    // 4. En passant
    state.enPassantSquare = parts[3];

    // 5. Check status → updated later
    state.whiteInCheck = false;
    state.blackInCheck = false;

    // 6. bestAction (default for now)
    //state.bestAction = "0000"; delete?

    return state;
}

/// <summary>
/// sees if move leads to self check
/// </summary>
bool AI::isInCheck(GameState& state, bool forWhite){
    int kingRow = -1, kingCol = -1;
    char kingChar = forWhite ? 'K' : 'k';

    // find king position
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (state.board[r][c] == kingChar) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }
    // check if any enemy piece attacks king's square
    GameState temp = state;
    temp.isWhiteTurn = !forWhite; // enemy's turn

    auto enemyMoves = getOrderedValidMoves(temp);

    for (const auto& category : enemyMoves) {
        for (const auto& move : category) {
            int targetRow = move[2];
            int targetCol = move[3];
            if (targetRow == kingRow && targetCol == kingCol) {
                return true; // king is under threat
            }
        }
    }
    return false; // no threats found
}

/// <summary>
/// generates ordered list of all valid moves
/// </summary>
std::vector<std::vector<std::vector<int>>> AI::getOrderedValidMoves(GameState sCurrent){

    std::vector<std::vector<int>> forwardMoves; // r, c, r', c'
    std::vector<std::vector<int>> pawnForwardMoves; // r, c, r', c'
    std::vector<std::vector<int>> lateralMoves; // r, c, r', c'
    std::vector<std::vector<int>> backwardMoves; // r, c, r', c'
    std::vector<std::vector<int>> captureMoves; // r, c, r', c'
    std::vector<std::vector<int>> promoteMoves; // r, c, r', c'
    std::vector<std::vector<int>> promoteAndCaptureMoves; // r, c, r', c'

    if(sCurrent.isWhiteTurn == true){
        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){
                char piece = sCurrent.board[r][c];
                if(isupper(piece)){
                    if(piece != 'P'){
                        for(const auto& move : moveDirections[piece]){
                            bool done = false;
                            int step = 1;
                            while(!done){
                                int rNew = r + move.first*step;
                                int cNew = c + move.second*step;
                                if(rNew >= 0 && rNew < 8 && cNew >= 0 && cNew < 8){ // check if in bounds
                                    std::vector<int> m = {r, c, rNew, cNew};
                                    if(sCurrent.board[rNew][cNew] == '-'){ // check if empty
                                        if(rNew < r){
                                            forwardMoves.push_back(m);
                                        }
                                        else if(rNew == r){
                                            lateralMoves.push_back(m);
                                        }
                                        else if(rNew > r){
                                            backwardMoves.push_back(m);
                                        }
                                    }
                                    else{
                                        if(islower(sCurrent.board[rNew][cNew])){ // check if enemy piece present
                                            captureMoves.push_back(m);
                                        }
                                        done = true;
                                    }
                                }
                                else{
                                    done = true;
                                }    
                                if(!isSlidingPiece[piece]){
                                    done = true;
                                }
                                step = step + 1;
                            }
                        }
                    }
                    else if(piece == 'P'){
                        //Captures
                        if(c < 7 && r > 0 && sCurrent.board[r - 1][c + 1] != '-' && islower(sCurrent.board[r - 1][c + 1])){
                            if (r - 1 == 0){
                                promoteAndCaptureMoves.push_back({r, c, r - 1, c + 1}); // promotion by capture
                            }else{
                                captureMoves.push_back({r, c, r - 1, c + 1});
                            }
                        }
                        if(c > 0 && r > 0 && sCurrent.board[r - 1][c - 1] != '-' && islower(sCurrent.board[r - 1][c - 1])){
                            if (r - 1 == 0) {
                                promoteAndCaptureMoves.push_back({r, c, r - 1, c - 1}); // promotion by capture
                            } else {
                                captureMoves.push_back({r, c, r - 1, c - 1});
                            }
                        }
                        // En Passant capture for white
                        if(!sCurrent.enPassantSquare.empty() && sCurrent.enPassantSquare != "-"){
                            int epRow = 8 - (sCurrent.enPassantSquare[1] - '0'); // convert rank
                            int epCol = sCurrent.enPassantSquare[0] - 'a';       // convert file

                            if(r == 3 && abs(c - epCol) == 1 && epRow == 2){ // white pawn must be on rank 5 (r==3)
                                captureMoves.push_back({r, c, epRow, epCol});
                            }
                        }
                        // Forward Moves 
                        if(r > 0 && sCurrent.board[r - 1][c] == '-'){
                            if(r - 1 == 0){
                                promoteMoves.push_back({r, c, r - 1, c}); // promotion by forward move
                            }else{
                                pawnForwardMoves.push_back({r, c, r - 1, c}); // in forward moves priority to pawns
                            }
                            // 2-step move from starting rank
                            if(r == 6 && sCurrent.board[r - 2][c] == '-'){
                                forwardMoves.push_back({r, c, r - 2, c}); 
                            }
                        }
                    }
                }
            }
        }
    }
    else if(sCurrent.isWhiteTurn == false){
        for (int r = 0; r < 8; ++r){
            for (int c = 0; c < 8; ++c){
                char piece = sCurrent.board[r][c];
                if(islower(piece)){
                    if(piece != 'p'){
                        for(const auto& move : moveDirections[piece]){
                            bool done = false;
                            int step = 1;
                            while(!done){
                                int rNew = r + move.first*step;
                                int cNew = c + move.second*step;
                                if(rNew >= 0 && rNew < 8 && cNew >= 0 && cNew < 8){ // check if in bounds
                                    std::vector<int> m = {r, c, rNew, cNew};
                                    if(sCurrent.board[rNew][cNew] == '-'){ // check if empty
                                        if(rNew > r){
                                            forwardMoves.push_back(m);
                                        }
                                        else if(rNew == r){
                                            lateralMoves.push_back(m);
                                        }
                                        else if(rNew < r){
                                            backwardMoves.push_back(m);
                                        }
                                    }
                                    else{
                                        if(isupper(sCurrent.board[rNew][cNew])){ // check if enemy piece present
                                            captureMoves.push_back(m);
                                        }
                                        done = true;
                                    }
                                }
                                else{
                                    done = true;
                                }    
                                if(!isSlidingPiece[piece]){
                                    done = true;
                                }
                                step = step + 1;
                            }
                        }
                    }
                    else if(piece == 'p'){
                        //Captures
                        if(c < 7 && r < 7 && sCurrent.board[r + 1][c + 1] != '-' && isupper(sCurrent.board[r + 1][c + 1])){
                            if (r + 1 == 7){
                                promoteAndCaptureMoves.push_back({r, c, r + 1, c + 1}); // promotion by capture
                            }else{
                                captureMoves.push_back({r, c, r + 1, c + 1});
                            }
                        }
                        if(c > 0 && r < 7 && sCurrent.board[r + 1][c - 1] != '-' && isupper(sCurrent.board[r + 1][c - 1])){
                            if (r + 1 == 7){
                                promoteAndCaptureMoves.push_back({r, c, r + 1, c - 1}); // promotion by capture
                            }else{
                                captureMoves.push_back({r, c, r + 1, c - 1});
                            }
                        }
                        // En Passant capture for black
                        if(!sCurrent.enPassantSquare.empty() && sCurrent.enPassantSquare != "-"){
                            int epRow = 8 - (sCurrent.enPassantSquare[1] - '0'); // convert rank
                            int epCol = sCurrent.enPassantSquare[0] - 'a';       // convert file

                            if(r == 4 && abs(c - epCol) == 1 && epRow == 5){ // black pawn must be on rank 4
                                captureMoves.push_back({r, c, epRow, epCol});
                            }
                        }
                        // Forward Moves 
                        if(r < 7 && sCurrent.board[r + 1][c] == '-'){
                            if (r + 1 == 7){
                                promoteMoves.push_back({r, c, r + 1, c});
                            }else{
                                pawnForwardMoves.push_back({r, c, r + 1, c}); // in forward moves priority to pawns
                                // 2-step move from starting rank
                                if(r == 1 && sCurrent.board[r + 2][c] == '-'){
                                    forwardMoves.push_back({r, c, r + 2, c});
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // order is preserved hence ensuring move ordering
    return {captureMoves, promoteAndCaptureMoves, promoteMoves, pawnForwardMoves, forwardMoves, lateralMoves, backwardMoves};
}

/// <summary>
/// check if certain time has passed during minimax search
/// </summary>
bool AI::timeExceeded(){
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    return elapsed >= timeLimitMillis;
}

/// <summary>
/// transition function for board state and action
/// </summary>
AI::GameState AI::transitionFunction(GameState sCurrent, std::vector<int> move){
    
    GameState sNew = sCurrent; // Deep copy

    int r1 = move[0], c1 = move[1], r2 = move[2], c2 = move[3];
    char movedPiece = sCurrent.board[r1][c1];
    //char capturedPiece = sCurrent.board[r2][c2];

    // Clear previous en passant state
    sNew.enPassantSquare = "-";

    // Handle en passant capture (pawn captures empty square diagonally)
    if ((movedPiece == 'P' || movedPiece == 'p') && c1 != c2 && sCurrent.board[r2][c2] == '-') {
        // White capturing black en passant
        if (movedPiece == 'P' && r1 == 3 && r2 == 2) {
            sNew.board[r2 + 1][c2] = '-'; // remove captured pawn
        }
        // Black capturing white en passant
        else if (movedPiece == 'p' && r1 == 4 && r2 == 5) {
            sNew.board[r2 - 1][c2] = '-'; // remove captured pawn
        }
    }

    // Move piece
    sNew.board[r2][c2] = movedPiece;
    sNew.board[r1][c1] = '-';

    // Handle pawn promotion (auto-promote to queen)
    if (movedPiece == 'P' && r2 == 0) {
        sNew.board[r2][c2] = 'Q';
    }
    else if (movedPiece == 'p' && r2 == 7) {
        sNew.board[r2][c2] = 'q';
    }

    // Set en passant square if pawn moved two squares forward
    if (movedPiece == 'P' && r1 == 6 && r2 == 4) {
        sNew.enPassantSquare = std::string(1, 'a' + c1) + "3";
    }
    else if (movedPiece == 'p' && r1 == 1 && r2 == 3) {
        sNew.enPassantSquare = std::string(1, 'a' + c1) + "6";
    }

    // Flip turn
    sNew.isWhiteTurn = !sCurrent.isWhiteTurn;

    // Create UCI move string
    std::string moveUCI;
    moveUCI += static_cast<char>('a' + c1);
    moveUCI += static_cast<char>('8' - r1);
    moveUCI += static_cast<char>('a' + c2);
    moveUCI += static_cast<char>('8' - r2);

    // Append promotion suffix if applicable
    if ((movedPiece == 'P' && r2 == 0) || (movedPiece == 'p' && r2 == 7)) {
        moveUCI += 'q'; // always promote to queen
    }

    sNew.encodedActionLeadingToCurrentState = moveUCI;

    return sNew;
}

/// <summary>
/// terminates at max depth
/// </summary>
bool AI::cutOffTest(GameState sCurrent, int depth){
    return depth == 0 || timeExceeded(); // add other conditions later? delete
}

/// <summary>
/// evaluates heval of the board.
/// </summary>
int AI::hEval(GameState sCurrent){
    
    int whiteScore = 0, blackScore = 0;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char piece = sCurrent.board[r][c];
            if (piece == '-') continue;

            if (isupper(piece))
                whiteScore += pieceValues[piece];
            else
                blackScore += pieceValues[piece];
        }
    }
    // Final score: white maximises, black minimises
    return whiteScore - blackScore;
}

/// <summary>
/// calculates max value for maximizing player with alpha beta pruning
/// </summary>
std::pair<int, std::string> AI::maxValue(GameState sCurrent, int depth, int alpha, int beta){

    if(cutOffTest(sCurrent, depth)){
        return {hEval(sCurrent), sCurrent.encodedActionLeadingToCurrentState};
    }
    
    std::vector<std::vector<std::vector<int>>> orderedMoves = getOrderedValidMoves(sCurrent);
    int v = INT_MIN;
    std::string bestAction = "0000"; // Default move delete or change?

    // iterate over promoteAndCaptureMoves ,captureMoves, promoteMoves, forwardMoves, lateralMoves, backwardMoves in order;
    for (const auto& moveCategory : orderedMoves){
        for (const auto& move : moveCategory){

            if (timeExceeded()) {
                return {hEval(sCurrent), sCurrent.encodedActionLeadingToCurrentState};
            }

            GameState sNew = transitionFunction(sCurrent, move);
            // Skip illegal moves that result in self-check
            if (isInCheck(sNew, sNew.isWhiteTurn)) {
                continue;
            }
    
            int childVal = minValue(sNew, depth-1, alpha, beta).first;
            if(childVal > v){ // to track v = max(v, min(minValue(sNew, depth-1, alpha, beta)));
                v = childVal;
                bestAction = sNew.encodedActionLeadingToCurrentState;
            }
            if(v >= beta){
                return{v, bestAction};
            }
            alpha = std::max(alpha, v);
        }
    }
    return {v, bestAction};
}

/// <summary>
/// calculates min value for minimizing player with alpha beta pruning
/// </summary>
std::pair<int, std::string> AI::minValue(GameState sCurrent, int depth, int alpha, int beta){

    if(cutOffTest(sCurrent, depth)){
        return {hEval(sCurrent), sCurrent.encodedActionLeadingToCurrentState};
    }
    
    std::vector<std::vector<std::vector<int>>> orderedMoves = getOrderedValidMoves(sCurrent);
    int v = INT_MAX;
    std::string bestAction = "0000"; // Default move delete or change?

    // iterate over promoteAndCaptureMoves ,captureMoves, promoteMoves, forwardMoves, lateralMoves, backwardMoves in order;
    for (const auto& moveCategory : orderedMoves){
        for (const auto& move : moveCategory){

            if (timeExceeded()) {
                return {hEval(sCurrent), sCurrent.encodedActionLeadingToCurrentState};
            }

            GameState sNew = transitionFunction(sCurrent, move);
            // Skip illegal moves that result in self-check
            if (isInCheck(sNew, sNew.isWhiteTurn)) {
                continue;
            }

            int childVal = maxValue(sNew, depth-1, alpha, beta).first;
            if(childVal < v){ // to track  v = min(v, min(minValue(sNew, depth-1, alpha, beta)));
                v = childVal;
                bestAction = sNew.encodedActionLeadingToCurrentState;
            }
            if(v <= alpha){
                return{v, bestAction};
            }
            beta = std::min(beta, v);
        }
    }
    return {v, bestAction};
}

/// <summary>
/// performs Iterative-Deepening Depth-Limited MiniMax with Alpha Beta Pruning (TL-ID-DL-MM-AB) for each player's turn 
/// </summary>
std::string AI::IterativeDeepeningHeuristicDepthLimitedMinimaxWithAlphaBetaPruning(GameState sCurrent){

    startTime = std::chrono::steady_clock::now();
    std::string bestAction = "0000"; // fallback move

    int maxAllowedDepth = 7;
    int depth = 0;

    for(depth = 0; depth < maxAllowedDepth; ++depth){
        if (timeExceeded()) break;

        std::pair<int, std::string> result;
        if (sCurrent.isWhiteTurn) {
            result = maxValue(sCurrent, depth, INT_MIN, INT_MAX);
        } else {
            result = minValue(sCurrent, depth, INT_MIN, INT_MAX);
        }

        if (!timeExceeded()) {
            bestAction = result.second;
        } else {
            break; // depth not completed, use last best move
        }
    }
    return bestAction;
}

/// <summary>
/// print board state for debugging
/// </summary>
void AI::printGameState(const GameState& state) {
    std::cout << "Board:\n";
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            std::cout << state.board[r][c] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "Turn: " << (state.isWhiteTurn ? "White" : "Black") << '\n';

    std::cout << "Castling Rights: ";
    if (state.whiteCanCastleKingside)  std::cout << "K";
    if (state.whiteCanCastleQueenside) std::cout << "Q";
    if (state.blackCanCastleKingside)  std::cout << "k";
    if (state.blackCanCastleQueenside) std::cout << "q";
    if (!state.whiteCanCastleKingside && !state.whiteCanCastleQueenside &&
        !state.blackCanCastleKingside && !state.blackCanCastleQueenside)
        std::cout << "-";
    std::cout << '\n';

    std::cout << "En Passant: " << state.enPassantSquare << '\n';
    std::cout << "White in Check: " << (state.whiteInCheck ? "Yes" : "No") << '\n';
    std::cout << "Black in Check: " << (state.blackInCheck ? "Yes" : "No") << '\n';
    std::cout << "Action leading to state: " << state.encodedActionLeadingToCurrentState << '\n';
}

} // chess

} // cpp_client
