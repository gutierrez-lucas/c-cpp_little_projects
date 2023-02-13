/**
 * Hex game implementation (use g++ -std=c++11 hexgame.cpp)
 * By RT
 * 12th Dec, 2020
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <vector>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

enum class Player: short { B, R };
enum class SquareVal: short {
  Empty = 0,
  B = 1,
  R = 2,
};

// Game board
class Board {
  private:
    vector< vector<SquareVal> > board;

  public:
    // init empty board with size 
    Board(int n = 11): board(n, vector<SquareVal>(n, SquareVal::Empty)) {}

    friend ostream& operator<<(ostream& out, const Board& b);

    SquareVal getSquare(int r, int c) const {
      return board[r][c];
    }

    void setSquare(int i, int j, SquareVal val) {
      board[i][j] = val;
    }

    int size() const {
      return board.size();
    }

    bool isWithinBoard(int r, int c) const {
      return r >= 0 && r < board.size()
          && c >= 0 && c < board.size();
    }

    bool isAdjacent(int r1, int c1, int r2, int c2) const {
      // if both sqaures are within board and not the same sqaure
      if (isWithinBoard(r1, c1) && isWithinBoard(r2, c2) && !(r1 == r2 && c1 == c2)) {
        // if the euclidean distance is 1 or sqrt(2)
        int dist_pow = (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
        if (dist_pow == 1) return true;
        if (dist_pow == 2 && ((r1 - r2) + (c1 - c2)) == 0) return true;
      }

      return false;
    }

    vector< tuple<int, int> > getNeighbors(int r, int c, SquareVal val) const {
      tuple<int, int> positions[] = {
        make_tuple(r, c-1),
        make_tuple(r, c+1),
        make_tuple(r-1, c),
        make_tuple(r+1, c),
        make_tuple(r+1, c-1),
        make_tuple(r-1, c+1),
      };

      vector< tuple<int, int> > neighbors;
      for (auto pos: positions) {
        auto [r, c] = pos; // C++17 structured binding
        if (isWithinBoard(r, c) && getSquare(r, c) == val) {
          neighbors.push_back(pos);
        }
      }

      return neighbors;
    }

    ~Board(){}
};

// Game controller
class Game {
  private:
    Player player; // which player's turn
    Board board;
    bool won;

  public:
    // new game starts with player B's turn
    Game(int n = 11): board(n), player(Player::B), won(false) {}

    //return `true` if move is valid
    bool move(int r, int c) {
      if (validate(r, c)) {
        SquareVal val = player == Player::B ? SquareVal::B : SquareVal::R;
        board.setSquare(r, c, val);
        checkWon(r, c, val);

        // switch player
        player = player == Player::B ? Player::R : Player::B;

        return true;
      }

      return false;
    }

    bool validate(int r, int c) const {
      return board.isWithinBoard(r, c)
          && board.getSquare(r, c) == SquareVal::Empty;
    }

    // if game is won, the curr move is the last move
    // check the connected graph of last move has a path
    void checkWon(int r, int c, SquareVal val) {
      int n = board.size();
      // prevent recursion loop, visit once
      vector< vector<bool> >visited(n, vector<bool>(n, false));
      // memo if four boundary is reached
      vector<bool>memo(4, false); // 0: N, 1: S, 2: W, 3: E

      traverseNeighbors(r, c, val, visited, memo);

      if (memo[0] && memo[1] && val == SquareVal::B) {
        cout << "Player B has won" << endl;
        won = true;
      } else if (memo[2] && memo[3] && val == SquareVal::R) {
        cout << "Player R has won" << endl;
        won = true;
      }
    }

    void traverseNeighbors(int r, int c, SquareVal val, vector< vector<bool> > &visited, vector<bool> &memo) {
      visited[r][c] = true;
      if (r == 0) memo[0] = true;
      if (r == board.size()-1) memo[1] = true;
      if (c == 0) memo[2] = true;
      if (c == board.size()-1) memo[3] = true;

      vector< tuple<int, int> > neighbors = board.getNeighbors(r, c, val);
      for (auto neighbor: neighbors){
        auto [nr, nc] = neighbor;
        if (!visited[nr][nc]) {
          traverseNeighbors(nr, nc, val, visited, memo);
        }
      }
    }

    bool isWon() const {
      return won;
    }

    void printBoard() {
      cout << board;
    }

    Player getPlayer() const {
      return player;
    }

    ~Game(){}
};

// print board
ostream& operator<<(ostream& out, const Board& b) {
  int row_indent = 1;
  // print header row
  out << setw(4) << setfill(' ') << ' ';
  for (int i = 0; i < b.size(); i ++){
    out << i << "   ";
  }
  out << endl;
  for (auto const &row: b.board) {
    // set row number
    out << setw(2) << setfill(' ') << setiosflags(ios_base::left) << row_indent-1;

    out << setw(row_indent * 2) << setfill(' ') << ' ';
    int col = 0;
    for (auto const &val: row) {
      char label = val == SquareVal::Empty ? '.' : (val == SquareVal::R ? 'X' : 'O');
      string delimeter = ++col == b.size() ? "" : " - ";
      out << label << delimeter;
    }
    out << endl;
    // print links to next row
    if (row_indent < b.size()){
      out << setw(row_indent * 2 + 2) << setfill(' ') << ' ';
      out << " \\";
      for (int i = 1; i < b.size(); i ++){
        out << " / \\";
      }
      out << endl;

      row_indent++;
    }
  }

  return out;
}

ostream& operator<<(ostream& out, const Player& p) {
  char label = p == Player::B ? 'B' : 'R';
  out << label;

  return out;
}

void readint(string title, int &i) {
  cout << title << endl;
  while(!(cin >> i)){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid integer, please try again: ";
  }
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main(void) {
  Game g = Game(11);

  cout << "Game start - Player B place 'O' to connect North & South sides: " << endl;
  g.printBoard();
  while (!g.isWon()) {
    cout << "Player " << g.getPlayer() << "'s turn: " << endl;

    int r, c;
    readint("Enter row: ", r);
    readint("Enter col: ", c);

    if (!g.move(r, c)) {
      cout << "Invalid input" << endl;
    } else {
      g.printBoard();
    }
  }

  return 1;
}