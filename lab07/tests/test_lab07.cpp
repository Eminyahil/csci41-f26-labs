// Lab 07 PUBLIC self-check.
//
// These are NOT your grade. Your grade comes from a larger private set
// built the same way against your src/playlist.cpp. But every KIND of case
// the private set checks appears here at least once -- the empty playlist,
// the playlist of one, wrapping, deep copies, self-assignment -- so a
// broken category shows up here before you submit.
//
// The harness is tests/test_framework.hpp. You write TEST, CHECK and
// CHECK_EQ; it writes main() for you.
//
// A test that THROWS fails by itself and the run continues, so an
// unimplemented function reports itself instead of stopping everything.
//
// A test that SEGFAULTS cannot be caught. If this run stops partway, the
// last name printed is the test that died, and you have undefined
// behaviour. Rebuild with -fsanitize=address and it will name the line.

#include "test_framework.hpp"

#include "playlist.h"

#include <string>
#include <vector>

typedef std::vector<std::string> VS;

// Build A -> B -> C with the cursor on A.
//
// addBeforeCurrent inserts immediately behind the cursor, which on a ring
// means "at the end of the forward order from the cursor". So adding B then
// C in that order appends them.
static CircularPlaylist abc() {
    CircularPlaylist p;
    p.addAfterCurrent("A");      // {A}, cursor on A
    p.addBeforeCurrent("B");     // A -> B
    p.addBeforeCurrent("C");     // A -> B -> C
    return p;
}

// ---- the boundaries, first -------------------------------------------

TEST("a new playlist is empty") {
    CircularPlaylist p;
    CHECK(p.empty());
    CHECK_EQ(p.size(), 0);
    CHECK_EQ(p.current(), std::string(""));
}

TEST("EMPTY: next, prev and removeCurrent all return false") {
    CircularPlaylist p;
    CHECK(!p.next());
    CHECK(!p.prev());
    CHECK(!p.removeCurrent());
    CHECK(p.empty());
}

TEST("EMPTY: contains is false and snapshotForward is empty") {
    CircularPlaylist p;
    CHECK(!p.contains("A"));
    CHECK(p.snapshotForward(5).empty());
}

TEST("snapshotForward with count <= 0 is empty, even when there are songs") {
    CircularPlaylist p;
    p.addAfterCurrent("A");
    CHECK(p.snapshotForward(0).empty());
    CHECK(p.snapshotForward(-3).empty());
}

TEST("ONE song: the cursor lands on it") {
    CircularPlaylist p;
    p.addAfterCurrent("Solo");
    CHECK(!p.empty());
    CHECK_EQ(p.size(), 1);
    CHECK_EQ(p.current(), std::string("Solo"));
}

TEST("ONE song: next and prev both succeed and stay put") {
    CircularPlaylist p;
    p.addAfterCurrent("Solo");
    CHECK(p.next());
    CHECK_EQ(p.current(), std::string("Solo"));
    CHECK(p.prev());
    CHECK_EQ(p.current(), std::string("Solo"));
}

TEST("ONE song: removing it empties the playlist") {
    CircularPlaylist p;
    p.addAfterCurrent("Solo");
    CHECK(p.removeCurrent());
    CHECK(p.empty());
    CHECK_EQ(p.size(), 0);
    CHECK_EQ(p.current(), std::string(""));
    CHECK(!p.next());
}

TEST("addBeforeCurrent on an EMPTY playlist behaves like addAfterCurrent") {
    CircularPlaylist p;
    p.addBeforeCurrent("First");
    CHECK_EQ(p.size(), 1);
    CHECK_EQ(p.current(), std::string("First"));
}

// ---- inserting -------------------------------------------------------

TEST("addAfterCurrent inserts after the cursor and does NOT move it") {
    CircularPlaylist p;
    p.addAfterCurrent("A");
    p.addAfterCurrent("B");
    CHECK_EQ(p.current(), std::string("A"));
    CHECK_EQ(p.size(), 2);
    CHECK(p.snapshotForward(2) == VS({"A", "B"}));
}

TEST("addBeforeCurrent inserts before the cursor and does NOT move it") {
    CircularPlaylist p;
    p.addAfterCurrent("A");
    p.addBeforeCurrent("Z");
    CHECK_EQ(p.current(), std::string("A"));
    CHECK(p.snapshotForward(2) == VS({"A", "Z"}));
}

TEST("repeated addAfterCurrent comes out in reverse order") {
    CircularPlaylist p;
    p.addAfterCurrent("A");
    p.addAfterCurrent("B");
    p.addAfterCurrent("C");
    CHECK(p.snapshotForward(3) == VS({"A", "C", "B"}));
}

// ---- moving ----------------------------------------------------------

TEST("the A -> B -> C fixture really is in that order") {
    CircularPlaylist p = abc();
    CHECK_EQ(p.current(), std::string("A"));
    CHECK(p.snapshotForward(3) == VS({"A", "B", "C"}));
}

TEST("next and prev wrap all the way round") {
    CircularPlaylist p = abc();
    const std::string start = p.current();
    CHECK(p.next()); CHECK(p.next()); CHECK(p.next());
    CHECK_EQ(p.current(), start);
    CHECK(p.prev());
    CHECK(p.prev()); CHECK(p.prev()); CHECK(p.prev());
    CHECK(p.next());
    CHECK_EQ(p.current(), start);
}

TEST("snapshotForward wraps past the end") {
    CircularPlaylist p = abc();
    VS s = p.snapshotForward(5);
    CHECK_EQ(static_cast<int>(s.size()), 5);
    CHECK_EQ(s[0], s[3]);
    CHECK_EQ(s[1], s[4]);
}

TEST("snapshotForward does not move the cursor") {
    CircularPlaylist p = abc();
    const std::string before = p.current();
    p.snapshotForward(7);
    CHECK_EQ(p.current(), before);
}

// ---- removing --------------------------------------------------------

TEST("removeCurrent moves the cursor to the song that followed") {
    CircularPlaylist p;
    p.addAfterCurrent("A");
    p.addAfterCurrent("B");          // A -> B, cursor on A
    CHECK(p.removeCurrent());
    CHECK_EQ(p.current(), std::string("B"));
    CHECK_EQ(p.size(), 1);
    CHECK(!p.contains("A"));
}

TEST("removing everything, one at a time, ends empty") {
    CircularPlaylist p = abc();
    CHECK(p.removeCurrent());
    CHECK(p.removeCurrent());
    CHECK(p.removeCurrent());
    CHECK(p.empty());
    CHECK(!p.removeCurrent());
}

// ---- searching -------------------------------------------------------

TEST("contains finds what is there and not what is not") {
    CircularPlaylist p = abc();
    CHECK(p.contains("A"));
    CHECK(p.contains("B"));
    CHECK(p.contains("C"));
    CHECK(!p.contains("D"));
}

// ---- clear -----------------------------------------------------------

TEST("clear empties it, and it still works afterwards") {
    CircularPlaylist p = abc();
    p.clear();
    CHECK(p.empty());
    CHECK_EQ(p.size(), 0);
    p.addAfterCurrent("Fresh");
    CHECK_EQ(p.size(), 1);
    CHECK_EQ(p.current(), std::string("Fresh"));
}

// ---- the Rule of Three -----------------------------------------------

TEST("the copy constructor makes a DEEP copy") {
    CircularPlaylist a = abc();
    CircularPlaylist b = a;
    b.addAfterCurrent("OnlyInB");
    CHECK(b.contains("OnlyInB"));
    CHECK(!a.contains("OnlyInB"));
    CHECK_EQ(a.size(), 3);
    CHECK_EQ(b.size(), 4);
}

TEST("the copy's cursor is on the same song") {
    CircularPlaylist a = abc();
    a.next();
    CircularPlaylist b = a;
    CHECK_EQ(b.current(), a.current());
    CHECK(b.snapshotForward(3) == a.snapshotForward(3));
}

TEST("assignment makes a DEEP copy") {
    CircularPlaylist a = abc();
    CircularPlaylist b;
    b.addAfterCurrent("Discarded");
    b = a;
    CHECK_EQ(b.size(), 3);
    CHECK(!b.contains("Discarded"));
    b.addAfterCurrent("OnlyInB");
    CHECK(!a.contains("OnlyInB"));
}

TEST("self-assignment does not destroy the playlist") {
    CircularPlaylist a = abc();
    a = a;
    CHECK_EQ(a.size(), 3);
    CHECK(a.contains("A"));
    CHECK(a.contains("B"));
    CHECK(a.contains("C"));
}

TEST("copying an EMPTY playlist is fine") {
    CircularPlaylist a;
    CircularPlaylist b = a;
    CHECK(b.empty());
    CHECK_EQ(b.size(), 0);
    b.addAfterCurrent("X");
    CHECK_EQ(b.size(), 1);
    CHECK(a.empty());
}
