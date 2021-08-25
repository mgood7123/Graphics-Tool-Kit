//
// Created by Matthew Good on 25/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_APPLICATIONLOADER_H
#define GRAPHICAL_TOOL_KIT_APPLICATIONLOADER_H

#include <dlfcn.h>
#include <functional>
#include <string>
#include "Application.h"

class ApplicationLoader {

public:
    class SymbolTable {
        std::string path;
        bool self = false;
        void * symbol_table = nullptr;
    public:
        SymbolTable(const char * path, void * table);

        SymbolTable(const SymbolTable &x) = delete;
        SymbolTable(SymbolTable &&x) = delete;

        SymbolTable & operator=(const SymbolTable &x) = delete;

        SymbolTable & operator=(SymbolTable &&x) = delete;

        std::string getPath();

        void * get_symbol_table();

        template <typename T> T load_symbol(const char * name) {
            return reinterpret_cast<T>(load_symbol(name));
        }

        void * load_symbol(const char * name);
    };

private:

    Kernel symbols {4};
    SymbolTable * self = nullptr;

    SymbolTable * createSymbolTable(const char * path, void * symbolTable) {
        return symbols.newObject(
                ObjectFlagAutoDeallocateResource,
                new SymbolTable{path, symbolTable}
        )->resource.get<SymbolTable*>();
    }

public:
    class ApplicationInstancer {
        SymbolTable * symbolTable;
        size_t instanceCount = 0;
    public:
        ApplicationInstancer() {
        }

        ApplicationInstancer(SymbolTable * sym) {
            symbolTable = sym;
        }

        SymbolTable * getSymbolTable() {
            return symbolTable;
        }

        ApplicationInstance * createInstance(const char * name) {
            if (name == nullptr) {
                Log::Error_And_Throw("attempting to instantiate an application with a nullptr value");
            }

            auto sym_c = std::string("newInstance__") + name;
            ApplicationInstance::Application_Creator_T* c = symbolTable->load_symbol<ApplicationInstance::Application_Creator_T*>(sym_c.c_str());
            
            if (c == nullptr) return nullptr;

            auto sym_d = std::string("deleteInstance__") + name;
            ApplicationInstance::Application_Destroyer_T* d = symbolTable->load_symbol<ApplicationInstance::Application_Destroyer_T*>(sym_d.c_str());

            if (d == nullptr) return nullptr;

            ApplicationInstance * instance = c();
            instance->destroyerT = d;
            instanceCount++;
            return instance;
        }

        void destroyInstance(ApplicationInstance * instance) {
            if (instance == nullptr) {
                Log::Error_And_Throw("attempting to destroy a nullptr instance");
            }
            instance->destroyerT(instance);
            instanceCount--;
        }
        
        ApplicationInstancer(const ApplicationInstancer &x) = delete;
        ApplicationInstancer(ApplicationInstancer &&x) = delete;

        ApplicationInstancer & operator=(const ApplicationInstancer &x) = delete;

        ApplicationInstancer & operator=(ApplicationInstancer &&x) = delete;

        ~ApplicationInstancer() {
            if (instanceCount != 0) {
                Log::Error_And_Throw("failed to call 'destroyInstance' on ", instanceCount, " instance", instanceCount == 1 ? "" : "s");
            }
        }
    };
    
private:

    Kernel instances {4};
    
    ApplicationInstancer * createApplicationInstancer(const char * path, void * symbolTable) {
        return instances.newObject(
                ObjectFlagAutoDeallocateResource,
                new ApplicationInstancer{createSymbolTable(path, symbolTable)}
        )->resource.get<ApplicationInstancer*>();
    }

    ApplicationInstancer * self_instancer = nullptr;
public:

    ApplicationLoader() {
        void * sym = dlopen(NULL, RTLD_LAZY);
        if (sym == nullptr) {
            Log::Error_And_Throw("cannot open self: ", dlerror());
        }
        self_instancer = createApplicationInstancer(nullptr, sym);
        self = self_instancer->getSymbolTable();
    }

    ~ApplicationLoader() {
        unloadApplication(self_instancer);
        size_t count = getLoadedApplicationCount();
        if (count != 0) {
            Log::Error_And_Throw("failed to call 'unloadApplication' on ", count, " loaded application", count == 1 ? "s" : "");
        }
    }

    SymbolTable * getSelf() {
        return self;
    }

    ApplicationInstancer * getSelfApplicationInstancer() {
        return self_instancer;
    }

    size_t getLoadedApplicationCount() {
        return instances.objectCount();
    }

    ApplicationInstancer * findLoadedApplication(const char * path) {
        if (path == nullptr) {
            Log::Error_And_Throw("attempting to find an application with a nullptr path");
        }
        auto it = instances.table->getIterator();
        while (it.hasNext()) {
            Object * obj = it.next();
            ApplicationInstancer * ai = obj->resource.get<ApplicationInstancer*>();
            if (ai->getSymbolTable()->getPath() == path) {
                return ai;
            }
        }
        return nullptr;
    }

    ApplicationInstancer * loadApplicationFile(const char * path) {
        if (path == nullptr) {
            Log::Error_And_Throw("attempting to load an application with a nullptr path");
        }
        if (instances.objectCount() != 0) {
            ApplicationInstancer *ai = findLoadedApplication(path);
            if (ai != nullptr) {
                return ai;
            }
        }
        void *symbol_table = dlopen(path, RTLD_LAZY);
        if (symbol_table == nullptr) {
            Log::Error("could not load application from path '", path, "' : ", dlerror());
            return nullptr;
        }
        return createApplicationInstancer(path, symbol_table);
    }

    void unloadApplication(const char * path) {
        if (path == nullptr) {
            Log::Error_And_Throw("attempting to destroy an application with a nullptr path");
        }
        ApplicationInstancer * ai = findLoadedApplication(path);
        if (ai == nullptr) {
            Log::Error("cannot unload an application that has not been loaded");
            return;
        }
        unloadApplication(ai);
    }
    
    void unloadApplication(ApplicationInstancer * applicationInstancer) {
        if (applicationInstancer == nullptr) {
            Log::Error_And_Throw("attempting to destroy a nullptr application instance");
        }
        SymbolTable * symbolTable = applicationInstancer->getSymbolTable();
        dlclose(symbolTable->get_symbol_table());
        symbols.deleteObject(symbolTable);
        instances.deleteObject(applicationInstancer);
    }
};


#endif //GRAPHICAL_TOOL_KIT_APPLICATIONLOADER_H
