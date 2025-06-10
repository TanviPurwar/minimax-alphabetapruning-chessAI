# AI Chess Engine – C++ (Iterative Minimax with Alpha-Beta Pruning)

Course: Introduction to AI

This project is a a turn-based chess engine using depth-limited Minimax with alpha-beta pruning, iterative deepening, move ordering, and a custom time-limiting heuristic. Deployed on university’s proprietary game server; designed game-state transitions, evaluation logic, and rule-based move prioritization.

⚠️ This code is **not directly compilable or runnable** without the proprietary game server interface provided by the university.

## 🔧 Technologies

- **Language:** C++
- **Algorithms:** Minimax, Alpha-Beta Pruning, Iterative Deepening
- **Environment:** University’s proprietary turn-based chess server

## 🕹️ How It Works

1. Reads FEN-based or internal board state
2. Generates legal moves for the current player
3. Applies depth-limited Minimax with Alpha-Beta pruning
4. Uses iterative deepening to fit within the time limit
5. Returns the best move based on evaluation score

