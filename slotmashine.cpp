#include "slotmashine.h"

LRESULT CALLBACK SlotMashine::s_OnEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE) {
        LPCREATESTRUCT pCS = (LPCREATESTRUCT)lParam;
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (DWORD)pCS->lpCreateParams); //«апоминаем переданный указатель на класс
    }
    SlotMashine* pThis = (SlotMashine*)::GetWindowLongPtr(hwnd, GWLP_USERDATA); //берем его 
    if (pThis) {
        return pThis->OnEvent(hwnd, uMsg, wParam, lParam); // вызываем обычный (не static метод)
    }
    else {
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT SlotMashine::OnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(handle, message, wParam, lParam);
}

SlotMashine::SlotMashine() {
    status_ = 'P';
    msg_.str("New Game, Let`s go!");

    instance_ = GetModuleHandle(NULL);
    windowClass_.style = 0;
    windowClass_.lpfnWndProc = &s_OnEvent;
    windowClass_.cbClsExtra = 0;
    windowClass_.cbWndExtra = 0;
    windowClass_.hInstance = instance_;
    windowClass_.hIcon = NULL;
    windowClass_.hCursor = 0;
    windowClass_.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    windowClass_.lpszMenuName = NULL;
    windowClass_.lpszClassName = TEXT("Slot0001");
    RegisterClass(&windowClass_);

    window_ = CreateWindow(TEXT("Slot0001"), TEXT("SlotMashine"), WS_SYSMENU | WS_VISIBLE, 100, 200, 890, 660, NULL, NULL, instance_, this);
    button_quit_ = CreateWindow(TEXT("BUTTON"), TEXT("Quit"), WS_CHILD | WS_VISIBLE, 600, 500, 80, 40, window_, NULL, instance_, NULL);
    button_start_ = CreateWindow(TEXT("BUTTON"), TEXT("Start"), WS_CHILD | WS_VISIBLE, 400, 500, 80, 40, window_, NULL, instance_, NULL);
    button_stop_ = CreateWindow(TEXT("BUTTON"), TEXT("Stop"), WS_CHILD | WS_VISIBLE, 500, 500, 80, 40, window_, NULL, instance_, NULL);
    button_bet_plus_ = CreateWindow(TEXT("BUTTON"), TEXT("BET +"), WS_CHILD | WS_VISIBLE, 400, 400, 80, 40, window_, NULL, instance_, NULL);
    button_bet_minus_ = CreateWindow(TEXT("BUTTON"), TEXT("BET -"), WS_CHILD | WS_VISIBLE, 500, 400, 80, 40, window_, NULL, instance_, NULL);
    button_line_ = CreateWindow(TEXT("BUTTON"), TEXT("LINE +"), WS_CHILD | WS_VISIBLE, 600, 400, 80, 40, window_, NULL, instance_, NULL);      
   
    wheel_[0].create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 20, 50, 150, 300, window_, NULL, instance_, NULL));
    wheel_[1].create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 190, 50, 150, 300, window_, NULL, instance_, NULL));
    wheel_[2].create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 360, 50, 150, 300, window_, NULL, instance_, NULL));
    wheel_[3].create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 530, 50, 150, 300, window_, NULL, instance_, NULL));
    wheel_[4].create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 700, 50, 150, 300, window_, NULL, instance_, NULL));
    text_window_.create(CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 20, 400, 350, 200, window_, NULL, instance_, NULL));

    texture_[0].loadFromFile("resources/cherry.jpg");
    texture_[1].loadFromFile("resources/monkey.jpg");
    texture_[2].loadFromFile("resources/banan.jpg");

    cell_[0].setTexture(texture_[0]);
    cell_[1].setTexture(texture_[1]);
    cell_[2].setTexture(texture_[2]);

    font_.loadFromFile("resources/courier-bold.ttf");
    info_.setFont(font_);
    info_.setCharacterSize(24);
}

void SlotMashine::Run() {
    MSG message;
    message.message = static_cast<UINT>(~WM_QUIT);
    while (message.message != WM_QUIT) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
            DoButton(message);
        }
        else {
            Spin();
            IsSpinOff();
            IsStopmashine();
            Calculation();
            Tablo();
        }
    }
}

SlotMashine::~SlotMashine() {
    for (int k = 0; k < wheels; ++k) {
        wheel_[k].close();
    }
    text_window_.close();
    DestroyWindow(window_);
    UnregisterClass(TEXT("Slot0001"), instance_);
}

void SlotMashine::DoButton(MSG& message) {
    if (message.message == 514) {
        if (reinterpret_cast<HWND>(message.hwnd) == button_start_) {
            if (credit_ >= bet_ * line_) {
                status_ = 'S';
                credit_ -= bet_ * line_;
                for (int i = 0; i < wheels; ++i)shag[i] = 5 + slot::Rand(19);
            }
        }
        else if (reinterpret_cast<HWND>(message.hwnd) == button_quit_) {
            PostQuitMessage(0);
        }
        else if (reinterpret_cast<HWND>(message.hwnd) == button_stop_) {
            if (status_ == 'S') status_ = 'B';
            for (int i = 0; i < wheels; ++i)shag[i] = 0;
        }
        else if (reinterpret_cast<HWND>(message.hwnd) == button_bet_plus_) {
            if (line_ * (bet_ + 10) <= credit_) bet_ += 10;
        }
        else if (reinterpret_cast<HWND>(message.hwnd) == button_bet_minus_) {
            if (bet_ >= 20) bet_ -= 10;
        }
        else if (reinterpret_cast<HWND>(message.hwnd) == button_line_) {
            if (line_ == 10) line_ = 1;
            else if ((line_ + 1) * bet_ <= credit_) ++line_;
        }
    }
}

void SlotMashine::Spin() {
    for (int w = 0; w < wheels; ++w) {
        wheel_[w].clear();
        float offset = slot_[w].GetOffset();
        for (int i = 0; i < 4; ++i) {
            int k = slot_[w].GetCell(i);
            cell_[k].setPosition(0.f, 0.f + i * 100 - offset);
            wheel_[w].draw(cell_[k]);
        }
        shag[w] -= 0.005;
        slot_[w].AddOffSet(shag[static_cast<int>(w)]);
        wheel_[w].display();
    }
}
void SlotMashine::IsStopmashine() {
    bool stop = true;
    if (status_ == 'B') {
        for (int i = 0; i < wheels; ++i) {
            if (slot_[i].GetOffset() != 0) stop = false;
        }
        if (stop) status_ = 'W';
    }
}
void SlotMashine::IsSpinOff() {
    bool stop = true;
    if (status_ == 'S') {
        for (int i = 0; i < wheels; ++i) {
            if (shag[i] >= 0) stop = false;
        }
        if (stop) status_ = 'B';
    }
}
void SlotMashine::Calculation() {
    if (status_ == 'W') {
        int priz = 0;
        std::string lines = "xxxxx xxxxx";
        //line1-3                                
        bool win;
        for (int k = 0; k < (line_ > 3 ? 3 : line_); ++k) {
            win = true;
            lines[k] = 'L';
            for (int i = 0; i < wheels - 1; ++i) if (slot_[i].GetCell(k) != slot_[i + 1].GetCell(k)) win = false;
            if (win) {
                priz += bet_ * 333;
                lines[k] = 'W';
            }
        }
        //line4
        if (line_ > 3) {
            lines[3] = 'L';
            if (slot_[0].GetCell(0) == slot_[1].GetCell(0) && slot_[3].GetCell(2) == slot_[4].GetCell(2) && slot_[2].GetCell(1) == slot_[4].GetCell(2)) {
                priz += bet_ * 222;
                lines[3] = 'W';
            }
        }
        //line5
        if (line_ > 4) {
            lines[4] = 'L';
            if (slot_[0].GetCell(2) == slot_[1].GetCell(2) && slot_[3].GetCell(0) == slot_[4].GetCell(0) && slot_[4].GetCell(0) == slot_[2].GetCell(1)) {
                priz += bet_ * 222;
                lines[4] = 'W';
            }
        }
        // wheel win 3 line 6-10
        for (int i = 0; i < line_ - wheels; ++i) {
            win = slot_[i].Win();
            lines[i + 6] = 'L';
            if (win) {
                priz += bet_ * 20;
                lines[6 + i] = 'W';
            }
        }
        status_ = 'P';
        msg_.str("");
        if (priz > 0) {
            msg_ << "YOU WINNNNNN!:  " << priz << '\n' << lines;
            credit_ += priz;
        }
        else if (credit_ == 0) {
            msg_ << "YOU LOSE ALLLLLL ! \n       Ha-Ha-Ha...";
            status_ = 'L';
        }
        else {
            msg_ << "YOU LOSERRRRRR!: -" << bet_ * line_ << '\n' << lines;
        }
        if (bet_ * line_ > credit_) {
            bet_ = 10; line_ = 1;
        }
    }
}

void SlotMashine::Tablo(){
    text_window_.clear();
    std::ostringstream osstr;
    osstr.str("");
    osstr << "Credit:  " << credit_;
    info_.setString(osstr.str());
    info_.setFillColor(sf::Color::White);
    info_.setPosition(20.0f, 10.0f);
    text_window_.draw(info_);

    osstr.str("");
    osstr << "Bet:  " << bet_ * line_;
    info_.setString(osstr.str());
    info_.setFillColor(sf::Color::Blue);
    info_.setPosition(20.0f, 40.0f);
    text_window_.draw(info_);

    osstr.str("");
    osstr << "Line:  " << line_;
    info_.setString(osstr.str());
    info_.setFillColor(sf::Color::Blue);
    info_.setPosition(20.0f, 70.0f);
    text_window_.draw(info_);

    info_.setString(msg_.str());
    info_.setFillColor(sf::Color::Red);
    info_.setPosition(20.0f, 100.0f);
    text_window_.draw(info_);

    text_window_.display();
}