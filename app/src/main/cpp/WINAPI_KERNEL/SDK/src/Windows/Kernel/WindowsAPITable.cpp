//
// Created by konek on 7/18/2019.
//

#include <Windows/Kernel/WindowsAPITable.h>
#include <cassert> // assert

Table::Table() {
    this->Page.table = this;
    assert(this->table.size() == 0 && this->Page.count() == 0);
}

Table::Table(size_t page_size) {
    this->page_size = page_size;
    this->Page.table = this;
    assert(this->table.size() == 0 && this->Page.count() == 0);
}

Table::~Table() {
    this->Page.removeAll();
}

Table::Iterator::Iterator(Table *t) {
    table = t;
    tableIsEmpty = t == nullptr || table->table.empty();
}

void Table::Iterator::skip(size_t amount) {
    for (int i = 0; i < amount; ++i) this->index++;
}

bool Table::Iterator::hasNext() {
    if (tableIsEmpty) return false;
    size_t page_size = this->table->page_size;
    for (; this->page <= this->table->Page.count(); this->page++) {
        size_t idx = ((page_size * this->page) - page_size);
        for (; idx < page_size * this->page; idx++) {
            if (idx >= this->index) {
                this->index = idx;
                if (this->table->table[this->index] != nullptr)
                    return true;
            }
        }
    }
    return false;
}

Object* Table::Iterator::next() {
    return tableIsEmpty ? nullptr : this->table->table.at(this->index++);
}

size_t Table::Iterator::getIndex() const {
    return this->index-1;
}

Table::Iterator Table::getIterator() {
    return Iterator(this);
}

bool Table::hasFreeIndex() {
    int page = 1;
    size_t index;
    for (; page <= Page.count(); page++) {
        index = ((page_size * page) - page_size);
        for (; index < page_size * page; index++)
            if (table[index] == nullptr)
                return true;
    }
    return false;
}

tl::optional<size_t> Table::nextFreeIndex() {
    int page = 1;
    size_t index;
    for (; page <= this->Page.count(); page++) {
        index = ((page_size * page) - page_size);
        for (; index < page_size * page; index++)
            if (this->table[index] == nullptr)
                return index;
    }
    return tl::nullopt;
}

bool Table::hasObject(Object *object) {
    if (object == nullptr) return false;
    int page = 1;
    size_t index;
    for (; page <= this->Page.count(); page++) {
        index = ((page_size * page) - page_size);
        for (; index < page_size * page; index++)
            if (this->table[index] != nullptr)
                if (*this->table[index] == *object) return true;
    }
    return false;
}

tl::optional<size_t> Table::findObject(Object *object) {
    int page = 1;
    size_t index;
    for (; page <= this->Page.count(); page++) {
        index = ((page_size * page) - page_size);
        for (; index < page_size * page; index++)
            if (this->table[index] != nullptr && object != nullptr)
                if (*this->table[index] == *object) return index;
    }
    return tl::nullopt;
}

Object * Table::objectAt(size_t index) {
    if (index < this->table.size()) return this->table[index];
    return nullptr;
}

Object *Table::add(ObjectType type, ObjectFlag flags) {
    return this->add(type, flags, AnyOpt());
}

Object *Table::add(Object *object) {
    if (object != nullptr) return this->add(*object);
    return nullptr;
}

Object *Table::add(Object &object) {
    if (!this->hasFreeIndex()) this->Page.add();
    auto x = this->nextFreeIndex();
    if (!x) return nullptr;
    size_t i = x.value();
    this->table[i] = new Object();
    this->table[i]->inherit(object);
    return this->table[i];
}

void Table::DELETE(size_t index) {
    if (this->table[index] != nullptr) {
        this->table[index]->clean();
        delete this->table[index];
        this->table[index] = nullptr;
    }
}

void Table::remove(Object *object) {
    auto o = this->findObject(object);
    if (o) this->DELETE(o.value());
}

void Table::remove(Object &object) {
    this->remove(&object);
}

size_t Table::objectCount() {
    size_t count = 0;
    int page = 1;
    size_t index;
    for (; page <= Page.count(); page++) {
        index = ((page_size * page) - page_size);
        for (; index < page_size * page; index++)
            if (table[index] != nullptr) count++;
    }
    return count;
}

void Table::Page::add() {
    this->table->table.resize(this->table->table.size() + this->table->page_size);
    this->zero(this->indexToPageIndex(this->table->table.size()));
}

void Table::Page::remove() {
    if (count() != 0) {
        this->clean(this->indexToPageIndex(this->table->table.size()));
        this->table->table.resize(this->table->table.size() - this->table->page_size);
    }
}

void Table::Page::removeAll() {
    while (count() > 0) this->remove();
}

void Table::Page::allocate(size_t size) {
    int pn = this->count();
    int pr = this->indexToPageIndex(size);
    if (pn == pr) return;
    if (pn > pr) for (int x = pr; x < pn; x++) this->clean(x);
    this->table->table.resize(roundUp<size_t>(size, this->table->page_size));
    if (pn < pr) for (int x = pn; x < pr; x++) this->zero(x);
}

int Table::Page::count() {
    if (this->table->table.size() == 0) return 0;
    return this->indexToPageIndex(this->table->table.size());
}

int Table::Page::indexToPageIndex(size_t index) {
    return static_cast<int>(roundDown<size_t>(index, this->table->page_size) /
                            this->table->page_size);
}

void Table::Page::clean(int page) {
    size_t page_size = this->table->page_size;
    size_t index = ((page_size * page) - page_size);
    for (; index < page_size * page; index++)
        this->table->DELETE(index);
}

void Table::Page::zero(int page) {
    size_t page_size = this->table->page_size;
    size_t index = ((page_size * page) - page_size);
    for (; index < page_size * page; index++)
        this->table->table[index] = nullptr;
}
