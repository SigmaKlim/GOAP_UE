#pragma once
#include <unordered_map>
#include <string>
template <typename t_obj>
class Catalogue
{
public:
    Catalogue() : NameIterator(names), ObjectIterator(objects) {}
    ~Catalogue();
    bool                AddItem     (const std::string& name, const t_obj& object);
    bool                Contains    (const std::string& name) const;
    //It is guaranteed that id is similar to element index in simple array
    const t_obj*        GetItem     (size_t id) const;
    const t_obj*        GetItem     (const std::string& name) const;
    const std::string*  GetName     (size_t id) const;
    const size_t*       GetId       (const std::string& name) const;
    bool                RemoveItem  (const std::string& name);
    size_t              Size        () const;
    struct NamesRangeLoop
    {
        NamesRangeLoop(const std::vector<const std::string*>& names) : Names(names) {}
        auto begin() const
        {
            return Names.begin();
        }
        auto end() const
        {
            return Names.end();
        }
        const std::vector<const std::string*>& Names;
    } NameIterator; // for range-loop by names
    struct ObjectRangeLoop
    {
        ObjectRangeLoop(const std::vector<t_obj>& objects) : Objects(objects) {}
        auto begin() const
        {
            return Objects.begin();
        }
        auto end() const
        {
            return Objects.end();
        }
        const std::vector<t_obj>& Objects;
    } ObjectIterator; // for range-loop by objects
private:
    std::unordered_map<std::string, size_t> nameIdMap;
    std::vector<const std::string*> names;
    std::vector<t_obj> objects;
    size_t size = 0;
};


template <typename t_obj>
Catalogue<t_obj>::~Catalogue()
{
}

template <typename t_obj>
bool Catalogue<t_obj>::AddItem(const std::string& name, const t_obj& object)
{
    auto insertion = nameIdMap.insert(std::make_pair(name, size));
    if (insertion.second == false)
        return false;
    names.push_back(&insertion.first->first);
    objects.push_back(object);
    size++;
    return true;
}

template <typename t_obj>
bool Catalogue<t_obj>::Contains(const std::string& name) const
{
    return nameIdMap.find(name) != nameIdMap.end();
}

template <typename t_obj>
const t_obj* Catalogue<t_obj>::GetItem(size_t id) const
{
    if (id >= size)
        return nullptr;
    return &objects[id];
}

template <typename t_obj>
const t_obj* Catalogue<t_obj>::GetItem(const std::string& name) const
{
    auto* id = GetId(name);
    if (id == nullptr)
        return nullptr;
    return GetItem(*id);
}

template <typename t_obj>
const std::string* Catalogue<t_obj>::GetName(size_t id) const
{
    if (id >= size)
        return nullptr;
    return names[id];
}

template <typename t_obj>
const size_t* Catalogue<t_obj>::GetId(const std::string& name) const
{
    auto search = nameIdMap.find(name);
    if (search == nameIdMap.end())
        return nullptr;
    return &search->second;
}

template <typename t_obj>
bool Catalogue<t_obj>::RemoveItem(const std::string& name)
{
    auto search = nameIdMap.find(name);
    if (search == nameIdMap.end())
        return false;
    size_t id = search->second;
    objects.erase(objects.begin() + id);
    names.erase(names.begin() + id);
    nameIdMap.erase(search);
    return true;
}

template <typename t_obj>
size_t Catalogue<t_obj>::Size() const
{
    return size;
}

