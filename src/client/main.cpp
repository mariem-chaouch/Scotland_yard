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
    /*
    Example example;
    ExampleA exampleA;
    exampleA.setX(53);
    example.setA(exampleA);*/
    Connection c1(Taxi);
    Connection c2(Bus);
    Connection c3(Train);
    Connection c4(Boat);

    assert(c1.getTransport() == Taxi);
    assert(c2.getTransport() == Bus);
    assert(c3.getTransport() == Train);
    assert(c4.getTransport() == Boat);

    std::cout << "Tous les tests Connection sont réussis !" << std::endl;

    return 0;
}

// LCOV_EXCL_END
