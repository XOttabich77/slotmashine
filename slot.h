#pragma once
#include <random>
namespace slot {
    const int cells = 3;
    inline int Rand(int);

    class Slot {
    public:
        Slot();
        int AddOffSet(int i);
        int GetCell(size_t i) const;
        int GetOffset() const;
        bool Win() const;

    private:
        int cell_[4];
        int offset_ = 0;

        void InitCell();
        void SetNewCell();
    };

}

