//
// Created by blazeq on 27/10/2018.
//

template<GLenum name>
std::vector<typename info<name>::type> getv() {
    using value_type = typename info<name>::type;

    std::vector<value_type> values;
    values.resize(info<name>::paramCount);
    value_type* pos = values.data();
    if (std::is_same<value_type, GLint>::value) {
        glGetIntegerv(name, (GLint*)pos);
    } else if (std::is_same<value_type, GLboolean>::value) {
        glGetBooleanv(name, (GLboolean*)pos);
    } else if (std::is_same<value_type, GLfloat>::value) {
        glGetFloatv(name, (GLfloat*)pos);
    } else if (std::is_same<value_type, GLdouble>::value) {
        glGetDoublev(name, (GLdouble*)pos);
    }

    return values;
}

template<GLenum name>
typename info<name>::type get() {
    using value_type = typename info<name>::type;
    static_assert(info<name>::paramCount == 1);
    return getv<name>().front();
}

template<GLenum name>
std::string str() {
    auto values = getv<name>();

    std::stringstream out;
    out << info<name>::str();

    bool isBoolType = std::is_same<typename info<name>::type, GLboolean>::value;
    if (isBoolType) {
        for (int i = 0; i < values.size(); ++i) {
            auto strVal = values[i] ? "GL_TRUE" : "GL_FALSE";
            out << (i == 0 ? "{" : ", ") << strVal;
        }
    } else {
        for (int i = 0; i < values.size(); ++i) {
            out << (i == 0 ? "{" : ", ") << values[i];
        }
    }

    out << "}";

    return out.str();
}