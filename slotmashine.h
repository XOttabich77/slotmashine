#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <sstream>

#include "slot.h"

const int wheels = 5;


class SlotMashine {
    static LRESULT CALLBACK s_OnEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT OnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

public:
    SlotMashine();
    void Run();
    ~SlotMashine();

private:
    WNDCLASS windowClass_;
    HINSTANCE instance_;
    HWND button_quit_, button_start_, button_stop_;
    HWND button_bet_plus_, button_bet_minus_, button_line_;
    HWND window_;
    int credit_ = 2000;
    int bet_ = 10, line_ = 10;
    double shag[wheels] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    std::ostringstream msg_;
    sf::RenderWindow wheel_[wheels];
    sf::RenderWindow text_window_;
    sf::Texture texture_[wheels];
    sf::Sprite cell_[wheels];
    sf::Font font_;
    sf::Text info_;
    char status_;
    slot::Slot slot_[wheels];

    void DoButton(MSG& message);
    void Spin();
    void IsStopmashine();
    void IsSpinOff();
    void Calculation();
    void Tablo();
};