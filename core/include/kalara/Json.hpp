#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <variant>

namespace kalara::core {

class JsonValue {
public:
    enum class Type { Null, Bool, Number, String, Array, Object };

    using ArrayType = std::vector<JsonValue>;
    using ObjectType = std::unordered_map<std::string, JsonValue>;

    JsonValue() : m_type(Type::Null) {}
    JsonValue(bool val) : m_type(Type::Bool), m_bool(val) {}
    JsonValue(double val) : m_type(Type::Number), m_number(val) {}
    JsonValue(int val) : m_type(Type::Number), m_number(static_cast<double>(val)) {}
    JsonValue(uint64_t val) : m_type(Type::Number), m_number(static_cast<double>(val)) {}
    JsonValue(const char* val) : m_type(Type::String), m_string(val) {}
    JsonValue(std::string_view val) : m_type(Type::String), m_string(val) {}
    JsonValue(std::string val) : m_type(Type::String), m_string(std::move(val)) {}
    JsonValue(ArrayType arr) : m_type(Type::Array), m_array(std::move(arr)) {}
    JsonValue(ObjectType obj) : m_type(Type::Object), m_object(std::move(obj)) {}

    [[nodiscard]] Type type() const noexcept { return m_type; }
    [[nodiscard]] bool is_null() const noexcept { return m_type == Type::Null; }
    [[nodiscard]] bool is_bool() const noexcept { return m_type == Type::Bool; }
    [[nodiscard]] bool is_number() const noexcept { return m_type == Type::Number; }
    [[nodiscard]] bool is_string() const noexcept { return m_type == Type::String; }
    [[nodiscard]] bool is_array() const noexcept { return m_type == Type::Array; }
    [[nodiscard]] bool is_object() const noexcept { return m_type == Type::Object; }

    [[nodiscard]] bool as_bool(bool def = false) const noexcept { return is_bool() ? m_bool : def; }
    [[nodiscard]] double as_number(double def = 0.0) const noexcept { return is_number() ? m_number : def; }
    [[nodiscard]] const std::string& as_string(const std::string& def = "") const noexcept { return is_string() ? m_string : def; }
    [[nodiscard]] const ArrayType& as_array() const;
    [[nodiscard]] const ObjectType& as_object() const;

    [[nodiscard]] bool contains(const std::string& key) const noexcept;
    [[nodiscard]] const JsonValue& operator[](const std::string& key) const;
    [[nodiscard]] JsonValue& operator[](const std::string& key);

    [[nodiscard]] std::string dump(int indent = 2) const;
    static JsonValue parse(std::string_view json_str);

private:
    Type m_type{Type::Null};
    bool m_bool{false};
    double m_number{0.0};
    std::string m_string{};
    ArrayType m_array{};
    ObjectType m_object{};
};

} // namespace kalara::core
