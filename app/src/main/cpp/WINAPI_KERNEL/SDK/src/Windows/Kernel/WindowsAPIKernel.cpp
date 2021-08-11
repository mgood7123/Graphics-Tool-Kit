//
// Created by konek on 7/19/2019.
//

#include "../../../include/Windows/Kernel/WindowsAPIKernel.h"

Kernel::Kernel() {
    this->table = new WindowsAPITable();
    handles = 0;
}

Kernel::Kernel(size_t page_size) {
    this->table = new WindowsAPITable(page_size);
    handles = 0;
}

Kernel::Kernel(const Kernel &x) {
    this->table = new WindowsAPITable();
    table->table = x.table->table;
    handles = x.handles;
    const_cast<Kernel&>(x).handles = 0;
}

Kernel::Kernel(Kernel &&x) {
    this->table = new WindowsAPITable();
    std::swap(table->table, x.table->table);
    handles = x.handles;
    x.handles = 0;
}

Kernel & Kernel::operator=(const Kernel &x) {
    table->table = x.table->table;
    handles = x.handles;
    const_cast<Kernel&>(x).handles = 0;
    return *this;
}

Kernel & Kernel::operator=(Kernel &&x) {
    std::swap(table->table, x.table->table);
    handles = x.handles;
    x.handles = 0;
    return *this;
}

Kernel::~Kernel() {
    if (handles != 0) {
        Log::Error_And_Throw("handles have not been deallocated");
    }
    delete this->table;
}
