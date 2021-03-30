#pragma once

#include <iostream>
#include <list>
#include <vector>

#include "json.hpp"

/** 添加字段 */
#define JsonAddField(field) addField(#field, field)
/** 添加数组 */
#define JsonAddArray(field) addField(#field, field)
/** 添加对象(对象实现JsonBaseObject接口) */
#define JsonAddObject(object) addObject(#object, object)
/** 添加对象数组(对象实现JsonBaseObject接口) */
#define JsonAddObjectArray(objectArray) addObjectArray(#objectArray, objectArray)

/** 获取字段 */
#define JsonGetField(p) getField(#p, p)
/** 获取数组 */
#define JsonGetArray(p) getArray(#p, p)
/** 获取对象 */
#define JsonGetObject(p) getObject(#p, p)
/** 获取对象数组 */
#define JsonGetObjectArray(p) getObjectArray(#p, p)

/** Json序列化/反序列化 基础类 */
class JsonBaseObject {
public:
    JsonBaseObject() {}

    virtual ~JsonBaseObject() {}

    /** 添加需要序列化的字段 JsonAddField/JsonAddObject/JsonAddObjectArray */
    virtual void addJsonData() = 0;

    /** 获取本对象的json对象 */
    nlohmann::json toJson() {
        this->addJsonData();
        return _m_object_nlohmann_json;
    }

    /** 获取Json字符串 */
    std::string toJsonString(const int indent = -1, const char indent_char = ' ',
        const bool ensure_ascii = false) {
        try {
            this->toJson();
            return _m_object_nlohmann_json.dump(indent, indent_char, ensure_ascii);
        }
        catch (const std::exception& e) {
            this->exception = this->exception.append(e.what());
            this->exception = this->exception.append("\n\n");
            return std::string();
        }
    }

    /** 添加字段 */
    template <typename T>
    nlohmann::json addField(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value;
        return _m_object_nlohmann_json;
    }

    /** 添加对象 */
    template <typename T>
    nlohmann::json addObject(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value.toJson();
        return _m_object_nlohmann_json;
    }

    /** 添加std::list */
    template <typename T>
    nlohmann::json addObjectArray(const std::string& name, std::list<T>& value) {
        nlohmann::json arr;
        for (typename std::list<T>::iterator it = value.begin(); it != value.end();
            it++) {
            arr.push_back(it->toJson());
        }

        _m_object_nlohmann_json[name] = arr;
        return _m_object_nlohmann_json;
    }

    /** 添加std::vector */
    template <typename T>
    nlohmann::json addObjectArray(const std::string& name,
        std::vector<T>& value) {
        nlohmann::json arr;
        for (typename std::vector<T>::iterator it = value.begin();
            it != value.end(); it++) {
            arr.push_back(it->toJson());
        }

        _m_object_nlohmann_json[name] = arr;
        return _m_object_nlohmann_json;
    }

public:
    /** 添加需要反序列化的字段 JsonGetField/JsonGetObject/JsonGetObjectArray */
    virtual void parseJsonData() = 0;

    /** 设置本对象的json对象 */
    void fromJson(nlohmann::json& objJson) {
        _m_object_nlohmann_json = objJson;

        parseJsonData();
    }

    /** 异常信息 */
    const std::string& getException() { return this->exception; }

    /** 解析Json字符串 */
    bool parseJsonString(const std::string& data) {
        try {
            nlohmann::json obj = nlohmann::json::parse(data);
            fromJson(obj);

            if (this->exception.empty()) {
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            appendException("", std::string(e.what()));
            return false;
        }
    }

    /** 获取Json字段 */
    template <typename T>
    void getField(const std::string& name, T& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            value = obj;
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** 获取Json对象 */
    template <typename T>
    void getObject(const std::string& name, T& value) {
        try {
            value.fromJson(_m_object_nlohmann_json[name]);
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    template <typename T>
    void getArray(const std::string& name, std::list<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t = *it;

                value.push_back(t);
            }
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** 获取Json对象列表 */
    template <typename T>
    void getObjectArray(const std::string& name, std::list<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t;
                t.fromJson(*it);

                value.push_back(t);
            }

        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** 获取Json对象数组 */
    template <typename T>
    void getObjectArray(const std::string& name, std::vector<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t;
                t.fromJson(*it);

                value.push_back(t);
            }
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

private:
    void appendException(const std::string& name, const std::string& exception) {
        this->exception = this->exception.append("\n [");
        this->exception = this->exception.append(name);
        this->exception = this->exception.append("] ==>> ");
        this->exception = this->exception.append(exception);
        this->exception = this->exception.append("\n");
    }

private:
    /** nlohmann::json 对象 */
    nlohmann::json _m_object_nlohmann_json;
    std::string exception;
};
