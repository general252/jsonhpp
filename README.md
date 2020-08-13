# jsonhpp
C++类对象Serialization/Deserialization

依赖 [nlohmann/json](https://github.com/nlohmann/json)


#### 简单示例
```
#include "json_object.hpp"
#include <iostream>
#include <string>

class One : public JsonBaseObject {
public:
    // 通过 JsonBaseObject 继承
    virtual void addJsonData() {
        JsonAddField(aliyun);
    }
    virtual void parseJsonData() {
        JsonGetField(aliyun);
    }

public:
    std::string aliyun;

};

int main()
{
    One object_src;
    object_src.aliyun = "o1 aliyun";

    std::string strJson = object_src.toJsonString(4);
    printf("src: %s\n", strJson.data());

    One object_dst;
    object_dst.parseJsonString(strJson);

    printf("dst: %s\n", object_dst.toJsonString(4).data());
    return 0;
}
```



#### 对象、容器
```
#include "json_object.hpp"
#include <list>
#include <vector>
#include <stdio.h>


class One : public JsonBaseObject {
public:
    // 通过 JsonBaseObject 继承
    virtual void addJsonData() {
        JsonAddField(aliyun);
    }
    virtual void parseJsonData() {
        JsonGetField(aliyun);
    }

public:
    std::string aliyun;

};

class DemoOne : public JsonBaseObject
{
public:

    // 通过 JsonBaseObject 继承
    virtual void addJsonData() {
        JsonAddField(id);
        JsonAddField(name);
        JsonAddField(listName);
        JsonAddObject(object);
        JsonAddObjectArray(oneList);
    }
    virtual void parseJsonData() {
        JsonGetField(id);
        JsonGetField(name);
        JsonGetArray(listName);
        JsonGetObject(object);
        JsonGetObjectArray(oneList);
    }

public:
    int id;
    std::string name;
    One object;
    std::list<std::string> listName;
    std::vector<One> oneList;
};

class DemoTwo : public JsonBaseObject
{
public:
    // 通过 JsonBaseObject 继承
    virtual void addJsonData() {
        JsonAddField(count);
        JsonAddObject(ca);
    }
    virtual void parseJsonData() {
        JsonGetField(count);
        JsonGetObject(ca);
    }

public:
    int count;
    DemoOne ca;
};


int main()
{
    DemoTwo objDemo;
    objDemo.count = 100;
    objDemo.ca.id = 1;
    objDemo.ca.listName.push_back("tony");
    objDemo.ca.listName.push_back("tina");
    objDemo.ca.listName.push_back("tang");
    objDemo.ca.name = "my name";
    objDemo.ca.object.aliyun = "object aliyun";
    One o1;
    o1.aliyun = "o1 aliyun";
    objDemo.ca.oneList.push_back(o1);

    One o2;
    o2.aliyun = "o2 aliyun";
    objDemo.ca.oneList.push_back(o2);


    std::string strJson = objDemo.toJsonString(4);
    printf("src: %s\n", strJson.data());

    DemoTwo objDemo2;
    objDemo2.parseJsonString(strJson);

    printf("dst: %s\n", objDemo2.toJsonString(4).data());
    return 0;
}

```
