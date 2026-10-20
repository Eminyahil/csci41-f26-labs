// A place to try things by hand. Not graded.
#include <iostream>

#include "int_set.h"

static void show(const char* label, const IntSet& s) {
    std::cout << label << "  size=" << s.size()
              << "  capacity=" << s.capacity()
              << "  empty=" << (s.isEmpty() ? "yes" : "no") << "\n";
}

int main() {
    IntSet s(2);                 // deliberately small, so it has to grow
    show("new set        ", s);

    for (int v : {4, 8, 15, 16, 23, 42}) s.insert(v);
    show("after 6 inserts", s);

    std::cout << "insert(15) again -> "
              << (s.insert(15) ? "true" : "false") << "  (already present)\n";
    std::cout << "contains(23)     -> " << (s.contains(23) ? "true" : "false") << "\n";
    std::cout << "remove(23)       -> " << (s.remove(23) ? "true" : "false") << "\n";
    std::cout << "contains(23)     -> " << (s.contains(23) ? "true" : "false") << "\n";

    IntSet copy = s;             // uses your copy constructor
    copy.insert(99);
    std::cout << "\nafter copying and inserting 99 into the COPY only:\n";
    show("  original     ", s);
    show("  copy         ", copy);
    std::cout << "  original contains 99? "
              << (s.contains(99) ? "YES - your copy is not deep" : "no, good") << "\n";
    return 0;
}
