//
// Created by Matthew Good on 25/8/21.
//

#include "ApplicationLoader.h"

ApplicationLoader::SymbolTable::SymbolTable(const char *path, void *table) {
    this->path = path;
    symbol_table = table;
}

std::string ApplicationLoader::SymbolTable::getPath() {
    return path;
}

void *ApplicationLoader::SymbolTable::get_symbol_table() {
    return symbol_table;
}

void *ApplicationLoader::SymbolTable::load_symbol(const char *name) {
    void * symbol = dlsym(symbol_table, name);
    if (symbol == nullptr) {
        Log::Error("could not load symbol '", name, "' from path '", path, "' : ", dlerror());
    }
    return symbol;
}
