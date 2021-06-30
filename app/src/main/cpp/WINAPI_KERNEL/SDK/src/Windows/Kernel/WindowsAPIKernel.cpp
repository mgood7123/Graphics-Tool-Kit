//
// Created by konek on 7/19/2019.
//

#include "../../../include/Windows/Kernel/WindowsAPIKernel.h"

Kernel::Kernel() {
    this->table = new WindowsAPITable();
}

Kernel::Kernel(const Kernel &x) {
    if (table == nullptr) {
        table = new WindowsAPITable();
    }
    if (x.table != nullptr) {
        table->table = x.table->table;
    }
}

Kernel::Kernel(Kernel &&x) {
}

Kernel & Kernel::operator=(const Kernel &x) {
    return *const_cast<Kernel*>(this);
}

Kernel & Kernel::operator=(Kernel &&x) {
    return *const_cast<Kernel*>(this);
}

Kernel::~Kernel() {
    delete this->table;
}
