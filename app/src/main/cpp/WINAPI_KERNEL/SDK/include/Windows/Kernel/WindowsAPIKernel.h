//
// Created by konek on 7/19/2019.
//

#ifndef MEDIA_PLAYER_PRO_WINDOWSAPIKERNEL_H
#define MEDIA_PLAYER_PRO_WINDOWSAPIKERNEL_H

#include "../WindowsAPIDefinitions.h"
#include "WindowsAPIHandle.h"
#include "WindowsAPIObject.h"
#include "WindowsAPITable.h"
#include <cassert>
#include <cstdint>

class Kernel {
    size_t handles;
    public:
        Kernel();

        Kernel(size_t page_size);

        Kernel(const Kernel &x);
        Kernel(Kernel &&x);

        Kernel & operator=(const Kernel &x);

        Kernel & operator=(Kernel &&x);

        ~Kernel();

        WindowsAPITable *table = nullptr;

        bool validateHandle(HANDLE hObject);

        HANDLE newHandle(ObjectType type);

        HANDLE newHandle(ObjectType type, ResourceType resource);

        HandleClass *getHandle(HANDLE handle);

        void removeHandle(HANDLE handle);

        Object * newObject();

        Object *newObject(ObjectFlag flags);
        Object *newObject(ObjectType type, ObjectFlag flags);

        template <typename T>
        Object * newObject(ObjectType type, ObjectFlag flags, T * resource) {
            return this->table->add(type, flags, resource);
        }

        template <typename T>
        Object * newObject(ObjectFlag flags, T * resource) {
            return this->table->add(ObjectTypeNone, flags, resource);
        }

        template <typename T>
        Object * newObject(T * resource) {
            return this->table->add(ObjectTypeNone, ObjectFlagNone, resource);
        }

        template <typename T>
        Object * newObject(ObjectType type, ObjectFlag flags, T && resource) {
            return this->table->add(type, flags, resource);
        }

        template <typename T>
        Object * newObject(ObjectFlag flags, T && resource) {
            return this->table->add(ObjectTypeNone, flags, resource);
        }

        template <typename T>
        Object * newObject(T && resource) {
            return this->table->add(ObjectTypeNone, ObjectFlagNone, resource);
        }

        size_t objectCount() const;

        void deleteObject(Object *object);

        void deleteObject(Object &object);

        template <typename T>
        void deleteObject(const T & resource) {
            return this->table->remove(resource);
        }

    pid PID_LAST = 0;

};

#endif //MEDIA_PLAYER_PRO_WINDOWSAPIKERNEL_H
