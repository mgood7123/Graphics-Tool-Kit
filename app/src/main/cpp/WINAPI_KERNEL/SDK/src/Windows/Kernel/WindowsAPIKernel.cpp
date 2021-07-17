//
// Created by konek on 7/19/2019.
//

#include "../../../include/Windows/Kernel/WindowsAPIKernel.h"

Kernel::Kernel() {
    this->table = new WindowsAPITable();
}

Kernel::Kernel(const Kernel &x) {
    this->table = new WindowsAPITable();
    table->table = x.table->table;
}

Kernel::Kernel(Kernel &&x) {
    this->table = new WindowsAPITable();
    std::swap(table->table, x.table->table);
}

Kernel & Kernel::operator=(const Kernel &x) {
    table->table = x.table->table;
    return *this;
}

Kernel & Kernel::operator=(Kernel &&x) {
    std::swap(table->table, x.table->table);
    return *this;
}

Kernel::~Kernel() {
    delete this->table;
}
