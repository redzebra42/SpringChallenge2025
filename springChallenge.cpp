#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "springChallenge.h"

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int depth;
    cin >> depth; cin.ignore();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value;
            cin >> value; cin.ignore();
        }
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << "0" << endl;
}

struct State {
    vector<int> board;
    int turn;
    State(vector<int> b, int t) {
        board = b;
        turn = t;
    }
};

struct Node {
    State *state;
    vector<Node*> children;
};

struct Move {
    int coord;
    vector<vector<int>> selected_dice; //{{neighb1, neighb2, ...}, ...}
    Move(int c) {
        coord = c;
        selected_dice = {};
    }
};

vector<int> neighbouring_coords(int coord) {
    switch (coord)
    {
    case 0:
        return {1, 3};
        break;
    case 1:
        return {0, 2, 4};
        break;
    case 2:
        return {1, 5};
        break;
    case 3:
        return {0, 4, 6};
        break;
    case 4:
        return {1, 3, 5, 7};
        break;
    case 5:
        return {2, 4, 8};
        break;
    case 6:
        return {3, 7};
        break;
    case 7:
        return {4, 6, 8};
        break;
    case 8:
        return {5, 7};
        break;
    default:
        cerr << "coord out of bounds in neighbour" << endl;
        return {};
        break;
    }
}

Move *counting_neighbours(const State &state, int coord) {
    vector<int> neighb = neighbouring_coords(coord);
    remove_zeros(neighb);
    Move *move = new Move(coord);
    if (neighb.size() > 1) {
        if (neighb.size() == 2) {
            move->selected_dice = {{neighb[0], neighb[1]}};
        }
        else if (neighb.size() == 3) {
            move->selected_dice = {
                {neighb[0], neighb[1]},
                {neighb[0], neighb[2]},
                {neighb[1], neighb[2]},
                {neighb[0], neighb[1], neighb[3]}
            };
        }
        else if (neighb.size() == 4) {
            move->selected_dice = {
                {neighb[0], neighb[1]},
                {neighb[0], neighb[2]},
                {neighb[0], neighb[3]},
                {neighb[1], neighb[2]},
                {neighb[1], neighb[3]},
                {neighb[2], neighb[3]},
                {neighb[0], neighb[1], neighb[2]},
                {neighb[0], neighb[1], neighb[3]},
                {neighb[0], neighb[2], neighb[3]},
                {neighb[1], neighb[2], neighb[3]},
                {neighb[0], neighb[1], neighb[2], neighb[3]},
            };
        }
    }
    return move;
}

int sum(vector<int> v) {
    int s = 0;
    for (int i : v) {
        s += i;
    }
    return s;
}

bool exist_nice_sum(const Move &move, const State &state) {
    for (vector<int> dice : move.selected_dice) {
        if (sum(dice) <= 6) {
            return true;
        }
    }
    return false;
}

void remove_zeros(std::vector<int> &neighb) {
    // remove the 0s from neighb
    for (int i = 0; i < neighb.size(); i++) {
        if (neighb[i] == 0) {
            neighb.erase(neighb.begin() + i);
        }
    }
}

bool is_legal(const State &state, int coord) {
    if (coord >= 0 && coord < 9) {
        if (state.board[coord] == 0 && exist_nice_sum(*counting_neighbours(state, coord), state)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        cerr << "coord out of bounds" << endl;
        return false;
    }
}

vector<Move*> legal_moves(const State &state) {
    vector<Move*> res = {};
    for (int i=0; i<9; i++) {
        if (is_legal(state, i)) {
            res.push_back(counting_neighbours(state, i));
        }
    }
    return res;
}

void play_at(State &state, int coord, int value) {
    // we dont check if the move is legal to gain time
    state.board[coord] = value;
}

bool is_over(const State &state) {
    return size(legal_moves(state)) == 0;
}

int combinaison_value(vector<int> comb, const State &state) {
    int s = 0;
    for (int i : comb) {
        s += state.board[i];
    }
    return s;
}

int to_int(const State state) {
    //TODO
}

int state_sum(int s1, int s2) {
    //TODO
}

int all_possible_boards(const State &state, int max_depth) {
    if (is_over(state) || max_depth == 0) {
        return to_int(state);
    }
    else {
        int s = 0;
        for (Move *move : legal_moves(state)) {
            for (vector<int> combinaison : move->selected_dice) {
                State *new_state = new State(state.board, state.turn + 1);
                play_at(*new_state, move->coord, combinaison_value(combinaison, state));
                s = state_sum(s, all_possible_boards(*new_state, max_depth-1));
            }
        }
        return s;
    }
}