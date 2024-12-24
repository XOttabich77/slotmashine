#include "slot.h"
namespace slot {

    int Rand(int s) {
        static std::random_device rd;
        static std::mt19937 rnd(rd());
        static std::normal_distribution<double> dist(0, 20);
        return std::abs(static_cast<int>(dist(rnd))) % s;
    }

    void Slot::InitCell() {
        for (int i = 0; i < 4; ++i) {
            cell_[i] = Rand(cells);
        }
    }
    void Slot::SetNewCell() {
        for (int i = 0; i < 3; ++i) {
            cell_[i] = cell_[i + 1];
        }
        cell_[3] = Rand(cells);
    }
    bool Slot::Win() const {
        return cell_[0] == cell_[1] && cell_[1] == cell_[2];
    }
    int Slot::GetOffset() const {
        return offset_;
    }
    Slot::Slot() {
        InitCell();
    }

    int Slot::AddOffSet(int i) {
        if ((i >= 100) || (i <= 0 && offset_ == 0)) {
            return offset_;
        }
        if (i <= 0 && offset_ != 0) {
            offset_ += 1;
            if (offset_ == 100) {
                offset_ = 0;
                SetNewCell();
            }
            return offset_;
        }
        offset_ += i;
        if (offset_ >= 100) {
            offset_ -= 100;
            SetNewCell();
        }
        return offset_;
    }
    int Slot::GetCell(size_t i) const {
        if (i > 3) {
            return -1;
        }
        return cell_[i];
    }
}