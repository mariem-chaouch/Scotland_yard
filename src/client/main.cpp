#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

// LCOV_EXCL_START
void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>

using namespace std;
using namespace state;

int main(int argc,char* argv[])
{
    Board board;

    for (auto neighbor : board.getNeighbors(77))
    {
        cout << neighbor << endl;
    }
    cout << board.areConnected(77,79) << endl;
    cout << board.getStation(78).getId() << endl;
    cout << "It works !" << endl;

    return 0;
}

// LCOV_EXCL_END
