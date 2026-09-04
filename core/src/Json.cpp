#include "kalara/Json.hpp"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <stdexcept>

namespace kalara::core {

static const JsonValue s_null_json;
static const JsonValue::ArrayType s_empty_array;
static const JsonValue::ObjectType s_empty_object;

const JsonValue::ArrayType& JsonValue::as_array() const {
    return is_array() ? m_array : s_empty_array;
}

const JsonValue::ObjectType& JsonValue::as_object() const {
    return is_object() ? m_object : s_empty_object;
}

bool JsonValue::contains(const std::string& key) const noexcept {
    if (!is_object()) return false;
    return m_object.find(key) != m_object.end();
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    if (!is_object()) return s_null_json;
    auto it = m_object.find(key);
    return (it != m_object.end()) ? it->second : s_null_json;
}

JsonValue& JsonValue::operator[](const std::string& key) {
    if (!is_object()) {
        m_type = Type::Object;
        m_object.clear();
    }
    return m_object[key];
}

std::string JsonValue::dump(int indent) const {
    std::stringstream ss;
    switch (m_type) {
        case Type::Null: ss << "null"; break;
        case Type::Bool: ss << (m_bool ? "true" : "false"); break;
        case Type::Number: ss << m_number; break;
        case Type::String:
            ss << '"';
            for (char c : m_string) {
                if (c == '"' || c == '\\') ss << '\\';
                ss << c;
            }
            ss << '"';
            break;
        case Type::Array:
            ss << '[';
            for (size_t i = 0; i < m_array.size(); ++i) {
                if (i > 0) ss << ", ";
                ss << m_array[i].dump(indent);
            }
            ss << ']';
            break;
        case Type::Object:
            ss << "{\n";
            size_t count = 0;
            for (const auto& [k, v] : m_object) {
                ss << std::string(indent, ' ') << '"' << k << "\": " << v.dump(indent + 2);
                if (++count < m_object.size()) ss << ',';
                ss << '\n';
            }
            ss << "}";
            break;
    }
    return ss.str();
}

class JsonParser {
public:
    explicit JsonParser(std::string_view str) : m_str(str) {}

    JsonValue parse_val() {
        skip_ws();
        if (m_pos >= m_str.size()) return {};

        char c = m_str[m_pos];
        if (c == 'n') { m_pos += 4; return {}; }
        if (c == 't') { m_pos += 4; return true; }
        if (c == 'f') { m_pos += 5; return false; }
        if (c == '"') return parse_str();
        if (c == '[') return parse_arr();
        if (c == '{') return parse_obj();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_num();
        return {};
    }

private:
    void skip_ws() {
        while (m_pos < m_str.size() && std::isspace(static_cast<unsigned char>(m_str[m_pos]))) {
            m_pos++;
        }
    }

    std::string parse_str() {
        m_pos++; // skip quote
        std::string res;
        while (m_pos < m_str.size() && m_str[m_pos] != '"') {
            if (m_str[m_pos] == '\\' && m_pos + 1 < m_str.size()) m_pos++;
            res += m_str[m_pos++];
        }
        if (m_pos < m_str.size()) m_pos++;
        return res;
    }

    double parse_num() {
        size_t next_pos = 0;
        double val = std::stod(std::string(m_str.substr(m_pos)), &next_pos);
        m_pos += next_pos;
        return val;
    }

    JsonValue parse_arr() {
        m_pos++; // skip [
        JsonValue::ArrayType arr;
        while (m_pos < m_str.size()) {
            skip_ws();
            if (m_pos < m_str.size() && m_str[m_pos] == ']') { m_pos++; break; }
            arr.push_back(parse_val());
            skip_ws();
            if (m_pos < m_str.size() && m_str[m_pos] == ',') m_pos++;
        }
        return JsonValue(arr);
    }

    JsonValue parse_obj() {
        m_pos++; // skip {
        JsonValue::ObjectType obj;
        while (m_pos < m_str.size()) {
            skip_ws();
            if (m_pos < m_str.size() && m_str[m_pos] == '}') { m_pos++; break; }
            std::string key = parse_str();
            skip_ws();
            if (m_pos < m_str.size() && m_str[m_pos] == ':') m_pos++;
            obj[key] = parse_val();
            skip_ws();
            if (m_pos < m_str.size() && m_str[m_pos] == ',') m_pos++;
        }
        return JsonValue(obj);
    }

    std::string_view m_str;
    size_t m_pos{0};
};

JsonValue JsonValue::parse(std::string_view json_str) {
    JsonParser parser(json_str);
    return parser.parse_val();
}

} // namespace kalara::core
