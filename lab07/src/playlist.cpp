// CSCI 41 -- Lab 07: Linked Lists
//
// Implement every function below. Read include/playlist.h first: it states
// exactly what each one must do, including what happens on an empty
// playlist and on a playlist of one.
//
// Three rules that will save you an evening:
//
//   1. Test the EMPTY playlist and the playlist of ONE first. Almost every
//      circular-list bug lives in those two cases.
//   2. A ring has no nullptr, so you cannot stop a walk by looking for one.
//      Use do/while and stop when you are back at the node you started on.
//   3. Before you delete the node the cursor is on, move the cursor. A
//      cursor pointing at freed memory is the bug this lab is built to
//      teach you.
//
// Build it with sanitizers from the very first function:
//
//   g++ -std=c++17 -g -Wall -Wextra -fsanitize=address -Iinclude
//       src/playlist.cpp tests/test_lab07.cpp -o t && ./t
//
// That is how it is graded.
#include "playlist.h"

#include <stdexcept>

CircularPlaylist::CircularPlaylist() : current_(nullptr), size_(0) {
}

CircularPlaylist::CircularPlaylist(const CircularPlaylist& other)
    : current_(nullptr), size_(0) {
    copyFrom(other);
}

CircularPlaylist& CircularPlaylist::operator=(const CircularPlaylist& other) {
    if (this != &other) {
        CircularPlaylist temp(other);

        Node* oldCurrent = current_;
        int oldSize = size_;

        current_ = temp.current_;
        size_ = temp.size_;

        temp.current_ = oldCurrent;
        temp.size_ = oldSize;
    }
    return *this;
}

CircularPlaylist::~CircularPlaylist() {
    clear();
}

bool CircularPlaylist::empty() const {
    return size_ == 0;
}

int CircularPlaylist::size() const {
    return size_;
}

void CircularPlaylist::addAfterCurrent(const std::string& title) {
    Node* node = new Node{title, nullptr, nullptr};

    if (empty()) {
        node->next = node;
        node->prev = node;
        current_ = node;
    } else {
        Node* after = current_->next;

        node->prev = current_;
        node->next = after;

        current_->next = node;
        after->prev = node;
    }

    ++size_;
}

void CircularPlaylist::addBeforeCurrent(const std::string& title) {
    if (empty()) {
        addAfterCurrent(title);
        return;
    }

    Node* node = new Node{title, nullptr, nullptr};
    Node* before = current_->prev;

    node->next = current_;
    node->prev = before;

    before->next = node;
    current_->prev = node;

    ++size_;
}

bool CircularPlaylist::removeCurrent() {
    if (empty()) {
        return false;
    }

    Node* old = current_;

    if (size_ == 1) {
        current_ = nullptr;
    } else {
        Node* before = old->prev;
        Node* after = old->next;

        before->next = after;
        after->prev = before;

        current_ = after;
    }

    delete old;
    --size_;

    return true;
}

bool CircularPlaylist::next() {
    if (empty()) {
        return false;
    }

    current_ = current_->next;
    return true;
}

bool CircularPlaylist::prev() {
    if (empty()) {
        return false;
    }
    current_ = current_->prev;
    return true;
}

std::string CircularPlaylist::current() const {
    if (empty()) {
        return "";
    }

    return current_->data;
}

bool CircularPlaylist::contains(const std::string& title) const {
    if (empty()) {
        return false;
    }

    Node* start = current_;
    Node* cur = start;

    do {
        if (cur->data == title) {
            return true;
        }

        cur = cur->next;
    } while (cur != start);
    return false;
}

std::vector<std::string> CircularPlaylist::snapshotForward(int count) const {
    std::vector<std::string> result;

    if (empty() || count <= 0) {
        return result;
    }

    Node* node = current_;
        for (int i = 0; i < count; i++) {
            result.push_back(node->data);
            node = node->next;
        }
    return result;
}

void CircularPlaylist::clear() {
    if (empty()) {
        return;
    }

    Node* first = current_->next;

    current_->next = nullptr;

    while (first != nullptr) {
        Node* nextNode = first->next;
        delete first;
        first = nextNode;
    }

    current_ = nullptr;
    size_ = 0;
}

void CircularPlaylist::copyFrom(const CircularPlaylist& other) {
    if (this == &other || other.empty()) {
        return;
    }

    Node* cur = other.current_;

    try {
        do {
            addBeforeCurrent(cur->data);
            cur = cur->next;
        } while (cur != other.current_);
    } catch (...) {
        clear();
        throw;
    }
}
