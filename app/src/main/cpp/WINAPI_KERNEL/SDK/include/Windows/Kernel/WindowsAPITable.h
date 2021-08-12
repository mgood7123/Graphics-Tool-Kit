//
// Created by konek on 7/18/2019.
//

#ifndef MEDIA_PLAYER_PRO_WINDOWSAPITABLE_H
#define MEDIA_PLAYER_PRO_WINDOWSAPITABLE_H

#include <vector>
#include "../WindowsAPIDefinitions.h"
#include "WindowsAPIObject.h"
#include "optional.h"

typedef class Table {
    public:
        std::vector<Object *> table;
        size_t page_size = 4096;

        Table();
        Table(size_t page_size);
        ~Table();

        class Iterator {
            Table * table;
            bool tableIsEmpty;
            size_t index = 0;
            size_t page = 1;
        public:
            Iterator(Table * t);
            void skip(size_t amount);
            bool hasNext();
            Object * next();
            int getIndex() const;
        };

        Iterator getIterator();

        bool hasFreeIndex();

        tl::optional<size_t> nextFreeIndex();

        bool hasObject(Object *object);

        tl::optional<size_t> findObject(Object *object);

        Object * objectAt(size_t index);

        Object *add(ObjectType type, ObjectFlag flags);

        template <typename T> Object * add(ObjectType type, ObjectFlag flags, T * resource) {
            if (this->Page.count() == 0 || !this->hasFreeIndex()) this->Page.add();
            auto x = this->nextFreeIndex();
            if (!x) return nullptr;
            size_t i = x.value();
            this->table[i] = new Object();
            this->table[i]->type = type;
            this->table[i]->flags = flags;
            this->table[i]->resource.store(resource, flags & ObjectFlagAutoDeallocateResource);
            return this->table[i];
        }

        template <typename T> Object * add(ObjectType type, ObjectFlag flags, const T & resource) {
            if (this->Page.count() == 0 || !this->hasFreeIndex()) this->Page.add();
            auto x = this->nextFreeIndex();
            if (!x) return nullptr;
            size_t i = x.value();
            this->table[i] = new Object();
            this->table[i]->type = type;
            this->table[i]->flags = flags;
            this->table[i]->resource.store(resource);
            return this->table[i];
        }

        template <typename T> Object * add(ObjectType type, ObjectFlag flags, T && resource) {
            if (this->Page.count() == 0 || !this->hasFreeIndex()) this->Page.add();
            auto x = this->nextFreeIndex();
            if (!x) return nullptr;
            size_t i = x.value();
            this->table[i] = new Object();
            this->table[i]->type = type;
            this->table[i]->flags = flags;
            this->table[i]->resource.store(resource);
            return this->table[i];
        }

        Object *add(Object *object);

        Object *add(Object &object);

        void DELETE(size_t index);

        template <typename T> Object * findObject(const T & resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                Object * obj = it.next();
                if (obj->resource.template get<T&>() == resource) {
                    return obj;
                }
            }
            return nullptr;
        }
    
        template <typename T> Object * findObject(T * resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                Object * obj = it.next();
                if (obj->resource.template get<T*>() == resource) {
                    return obj;
                }
            }
            return nullptr;
        }
    
        template <typename T> void remove(const T & resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                if (it.next()->resource.template get<T&>() == resource) {
                    DELETE(it.getIndex());
                }
            }
        }
    
        template <typename T> void remove(T * resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                if (it.next()->resource.template get<T*>() == resource) {
                    DELETE(it.getIndex());
                }
            }
        }

        template <typename T> void removeWithoutDeallocating(const T & resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                Object * obj = it.next();
                if (obj->resource.template get<T&>() == resource) {
                    obj->resource.data_is_allocated = false;
                    DELETE(it.getIndex());
                }
            }
        }

        template <typename T> void removeWithoutDeallocating(T * resource) {
            auto it = getIterator();
            while (it.hasNext()) {
                Object * obj = it.next();
                if (obj->resource.template get<T*>() == resource) {
                    obj->resource.data_is_allocated = false;
                    DELETE(it.getIndex());
                }
            }
        }
    
        void remove(Object *object);

        void remove(Object &object);

        class Page {
            public:
                Table *table;

                void add();

                void remove();

                void removeAll();

                void allocate(size_t size);

                int count();

                int indexToPageIndex(size_t index);

                void clean(int page);

            void zero(int page);

        } Page;

    size_t objectCount();
} WindowsAPITable;
#endif //MEDIA_PLAYER_PRO_WINDOWSAPITABLE_H
