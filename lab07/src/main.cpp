// CSCI 41 -- Lab 07 demo program. Not graded; here so you can watch your
// playlist behave. Run it with:   ./build/lab07
#include "playlist.h"

#include <iostream>

static void show(const CircularPlaylist& p, const char* label) {
    std::cout << label << "  size=" << p.size()
              << "  current=\"" << p.current() << "\"\n    next 6: ";
    std::vector<std::string> s = p.snapshotForward(6);
    for (std::size_t i = 0; i < s.size(); ++i)
        std::cout << s[i] << (i + 1 < s.size() ? " -> " : "");
    if (s.empty()) std::cout << "(nothing)";
    std::cout << "\n";
}

int main() {
    CircularPlaylist p;
    show(p, "empty          ");

    p.addAfterCurrent("Alpha");
    show(p, "add Alpha      ");

    // Each of these goes in just after the cursor, so they come out
    // in the reverse of the order you add them.
    p.addAfterCurrent("Beta");
    p.addAfterCurrent("Gamma");
    show(p, "add Beta, Gamma");

    p.addBeforeCurrent("Omega");
    show(p, "add Omega before");

    p.next();
    show(p, "next           ");
    p.prev();
    p.prev();
    show(p, "prev twice     ");

    std::cout << "contains(\"Beta\")  = " << (p.contains("Beta") ? "yes" : "no") << "\n";
    std::cout << "contains(\"Delta\") = " << (p.contains("Delta") ? "yes" : "no") << "\n";

    std::cout << "removeCurrent()   = " << (p.removeCurrent() ? "true" : "false") << "\n";
    show(p, "after remove   ");

    // A copy must have its own nodes, or both destructors free the same ring.
    CircularPlaylist q = p;
    q.addAfterCurrent("OnlyInCopy");
    show(p, "original       ");
    show(q, "the copy       ");
    std::cout << "the copy changed and the original did not: that is a deep copy.\n";

    p.clear();
    show(p, "after clear    ");
    return 0;
}
