#pragma once

namespace ge
{

class model_loader
{
public:
    static ref<model> load(const string& path);
};

}
