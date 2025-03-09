#include <functional>
#include <unordered_map>
#include <memory>

#include "Entity.hpp"

class EntityFactory
{
private:
    using CreatorFunc = std::function<std::shared_ptr<sas::Entity>()>;
    std::unordered_map<std::string, CreatorFunc> registry;

public:
    static EntityFactory &instance()
    {
        static EntityFactory factory;
        return factory;
    }

    void registerEntity(const std::string &name, CreatorFunc creator)
    {
        registry[name] = std::move(creator);
    }

    std::shared_ptr<sas::Entity> create(const std::string &name) const
    {
        if (auto it = registry.find(name); it != registry.end())
        {
            return it->second();
        }
        throw std::runtime_error("Unknown entity type: " + name);
    }
};

// Helper macro for registration
#define REGISTER_ENTITY(TYPE) \
    static bool TYPE##_registered = [] { \
        EntityFactory::instance().registerEntity(#TYPE, [] { return std::make_unique<TYPE>(); }); \
        return true; }();
