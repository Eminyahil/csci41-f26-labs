// Lab 06 PUBLIC self-check.
//
// Passing all of these does NOT mean full marks: your grade comes from a
// larger private set built the same way against your src/int_set.cpp.
//
// What it DOES mean is that every KIND of case the private tests check is
// represented here at least once -- including the empty set, growth,
// duplicate handling, deep copying and self-assignment. If one of these
// categories is broken, you will find out here rather than after submitting.

#include "int_set.h"

#include <exception>
#include <iostream>
#include <string>

static int passed = 0, total = 0;

template <typename F>
static void check(const std::string& name, F f) {
    ++total;
    try {
        if (f()) { ++passed; std::cout << "[PASS] " << name << "\n"; return; }
        std::cout << "[FAIL] " << name << "\n";
    } catch (const std::exception& e) {
        std::cout << "[FAIL] " << name << "  (threw: " << e.what() << ")\n";
    }
}

int main() {
    check("a new set is empty", [] {
        IntSet s; return s.size() == 0 && s.isEmpty() && !s.contains(1); });

    check("insert then contains", [] {
        IntSet s; return s.insert(7) && s.contains(7) && s.size() == 1; });

    check("insert rejects a duplicate", [] {
        IntSet s; s.insert(7);
        return s.insert(7) == false && s.size() == 1; });

    check("remove an absent value returns false", [] {
        IntSet s; s.insert(1);
        return s.remove(99) == false && s.size() == 1; });

    check("remove a present value", [] {
        IntSet s; s.insert(1); s.insert(2);
        return s.remove(1) && !s.contains(1) && s.contains(2) && s.size() == 1; });

    check("remove from an empty set", [] {
        IntSet s; return s.remove(5) == false && s.isEmpty(); });

    check("clear empties it and it stays usable", [] {
        IntSet s; s.insert(1); s.insert(2);
        s.clear();
        return s.isEmpty() && s.size() == 0 && s.insert(3) && s.size() == 1; });

    check("it grows past its initial capacity", [] {
        IntSet s(2);
        for (int i = 0; i < 20; ++i) s.insert(i);
        if (s.size() != 20) return false;
        if (s.capacity() < 20) return false;
        for (int i = 0; i < 20; ++i) if (!s.contains(i)) return false;
        return true; });

    check("copy constructor makes a DEEP copy", [] {
        IntSet a; a.insert(1); a.insert(2);
        IntSet b = a;                       // copy constructor
        b.insert(3);
        return a.size() == 2 && !a.contains(3)
            && b.size() == 3 &&  b.contains(3); });

    check("assignment makes a DEEP copy", [] {
        IntSet a; a.insert(1);
        IntSet b; b.insert(99);
        b = a;                              // operator=
        b.insert(5);
        return a.size() == 1 && !a.contains(5) && !b.contains(99)
            && b.size() == 2; });

    check("self-assignment does not destroy the set", [] {
        IntSet s; s.insert(1); s.insert(2);
        s = s;
        return s.size() == 2 && s.contains(1) && s.contains(2); });

    check("a set built with a given capacity reports it", [] {
        IntSet s(8); return s.capacity() == 8 && s.isEmpty(); });

    std::cout << "\nPublic checks passed: " << passed << "/" << total << "\n";
    return (passed == total) ? 0 : 1;
}
