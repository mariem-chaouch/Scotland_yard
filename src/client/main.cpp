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
    Example example;
    ExampleA exampleA;
    exampleA.setX(53);
    example.setA(exampleA);

    cout << "It works !" << endl;

    return 0;
}

// LCOV_EXCL_END
