#pragma once

template<class... Args>
class Action final
{
    std::vector<std::function<void(Args&...)>> observers;
    std::vector<std::function<void(Args&...)>>::iterator it;

    bool AllowsAddingListnersDuringInvokation() const { return addDuringInvoke; }
    bool addDuringInvoke;
    size_t addOffset;
    bool added = false;
public:

    void Invoke(Args&... args)
    {
        if (observers.empty()) return;

        it = observers.begin();
        addOffset = 0;
        while (it < observers.end() - addOffset)
        {
            added = false;
            it->operator()(args...);
            if (!added)
                ++it;
        }
    }

    void operator+=(const std::function<void(Args&...)>& func)
    {
        if (!addDuringInvoke)
            ++addOffset;

        auto diff = it - observers.begin();

        observers.push_back(func);
        it = observers.begin() + diff;
        added = true;
    }

    void operator-=(const std::function<void(Args&...)>& func)
    {
        auto diff = it - observers.begin();
        auto there = std::find_if(observers.begin(), observers.end(),
            [&func](const std::function<void(Args&...)>& f) {

                auto a = func.target<void(*)(Args&...)>();
                auto b = f.target<void(*)(Args&...)>();
                return a && b && *a == *b;
            });

        if (there == observers.end())
            return;

        it = observers.erase(there);

        auto temp = it - observers.begin();
        if (temp < diff)
        {
            it = observers.begin() + (diff - 1);
        }
    }

    Action(bool addListnersDuringInvoke = false)
        : addDuringInvoke(addListnersDuringInvoke), addOffset(0)
    {
        it = observers.begin();
    }
};

