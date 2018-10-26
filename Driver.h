//
// Created by blazeq on 26/10/2018.
//

#ifndef OPENGL_DRIVER_H
#define OPENGL_DRIVER_H


#include "Application.h"
#include <memory>

class Driver {
public:
    template<typename T, class... Args>
    static bool run(Args&&... args) {
        auto ptr = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        return Driver::create()->runApplication(*ptr);
    }

    virtual ~Driver() = default;

private:
    static std::unique_ptr<Driver> create();
    virtual bool runApplication(Application& application) = 0;
};

#endif //OPENGL_DRIVER_H
