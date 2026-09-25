#include <state/Example.h>  // Included from library shared_static
#include "Example.h"

namespace client {

void Example::setY (int y) {
    // Create an object from "shared" library
    state::Example x {};

    this->y = y;
}

}

