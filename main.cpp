#include <sys/types.h>
#include <iostream>
#include <iterator>
#include <string>
#include <cinttypes>

/* the headers below do not
 * exist in windows machines */
//#include <sys/socket.h>
//#include <netdb.h>
//#include <arpa/inet.h>
//#include <netinet/in.h>

// oh my god this is horrifying
template <typename Iterator>
typename std::iterator_traits<Iterator>::value_type
getElement(Iterator it) {
    return *it;
}

int main() {
    std::string tst("This is a string.");
    for (auto it = tst.begin(); it < tst.end(); it++) {
        std::cout << getElement(it);
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
