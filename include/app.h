#ifndef __APP_H__
#define __APP_H__

#include "test.pb.h"
#include "spdlog/spdlog.h"
#include <unistd.h>
#include <iostream>
#include <string>

class App {
public:
    App() {
        m_person.set_name("xiongyi");
        m_person.set_age(30);
    }
    ~App() {}
    void run() {
        while (true) {
            // 序列化为二进制字符串
            std::string serialized_data;
            if (!m_person.SerializeToString(&serialized_data)) {
                spdlog::error("Failed to serialize person.");
                return;
            }
            spdlog::info("Serialized data");
            // 反序列化
            Person new_person;
            if (!new_person.ParseFromString(serialized_data)) {
                spdlog::error("Failed to parse serialized data.");
                return;
            }
            // 输出反序列化后的数据
            spdlog::info("------------------------------");
            spdlog::info("Deserialized data:");
            spdlog::info("Name: {}", new_person.name());
            spdlog::info("Age: {}", new_person.age());
            sleep(2);
        }
    }
private:
    Person m_person;
};

#endif //__APP_H__
