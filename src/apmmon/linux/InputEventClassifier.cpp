/*
 * Copyright (c) 2018 David 'Mokon' Bond <mokon@mokon.net>
 * Author: David 'Mokon' Bond <mokon@mokon.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <linux/input.h>
#include <linux/InputEventClassifier.hpp>
#include <linux/InputEventCodes.hpp>
#include <stdexcept>

#include <iostream>

namespace apm {

InputEventClassifier::InputEventClassifier()
{
    populateInputEventCodes();
}

void
InputEventClassifier::populateInputEventCodes()
{
    for (auto& pair : rawKeyInputEventCodes)
    {
        if (not inputEventCodes.emplace(pair.code, pair.description).second)
        {
            throw std::runtime_error("duplicate raw input event code");
        }
    }
}

Event
InputEventClassifier::get(const struct input_event& inputEvent)
{
    if (inputEvent.type == EV_KEY)
    {
        return Event(
            Time::from(inputEvent.time),
            getKey(inputEvent.code),
            getKeyModifier(inputEvent.value),
            inputEventCodes[inputEvent.code]);
    }
    // TODO handle mouse events
    else if (inputEvent.type == EV_MSC)
    {
        /*return Event(
            Time::from(inputEvent.time),
            getKey(inputEvent.code),
            getKeyModifier(inputEvent.value),
            inputEventCodes[inputEvent.code]);*/
        return Event::makeIgnoreEvent();
    }
    else if (inputEvent.type == EV_SYN)
    {
        return Event::makeIgnoreEvent();
    }
    return Event();
}

KeyModifier
InputEventClassifier::getKeyModifier(Value value)
{
    switch (value)
    {
        case 0:
            return KeyModifier::Release;
        case 1:
            return KeyModifier::Press;
        case 2:
            return KeyModifier::Hold;
        default:
            return KeyModifier::Unknown;
    }
}

Key
InputEventClassifier::getKey(Code key)
{
    switch (key)
    {
        case KEY_RESERVED:
            return Key::Other;
        case KEY_ESC:
            return Key::Esc;
        case KEY_1:
            return Key::Num1;
        case KEY_2:
            return Key::Num2;
        case KEY_3:
            return Key::Num3;
        case KEY_4:
            return Key::Num4;
        case KEY_5:
            return Key::Num5;
        case KEY_6:
            return Key::Num6;
        case KEY_7:
            return Key::Num7;
        case KEY_8:
            return Key::Num8;
        case KEY_9:
            return Key::Num9;
        case KEY_0:
            return Key::Num0;
        case KEY_MINUS:
            return Key::Minus;
        case KEY_EQUAL:
            return Key::Equal;
        case KEY_BACKSPACE:
            return Key::Backspace;
        case KEY_TAB:
            return Key::Tab;
        case KEY_Q:
            return Key::Q;
        case KEY_W:
            return Key::W;
        case KEY_E:
            return Key::E;
        case KEY_R:
            return Key::R;
        case KEY_T:
            return Key::T;
        case KEY_Y:
            return Key::Y;
        case KEY_U:
            return Key::U;
        case KEY_I:
            return Key::I;
        case KEY_O:
            return Key::O;
        case KEY_P:
            return Key::P;
        case KEY_LEFTBRACE:
            return Key::LeftBrace;
        case KEY_RIGHTBRACE:
            return Key::RightBrace;
        case KEY_ENTER:
            return Key::Enter;
        case KEY_LEFTCTRL:
            return Key::LeftCtrl;
        case KEY_A:
            return Key::A;
        case KEY_S:
            return Key::S;
        case KEY_D:
            return Key::D;
        case KEY_F:
            return Key::F;
        case KEY_G:
            return Key::G;
        case KEY_H:
            return Key::H;
        case KEY_J:
            return Key::J;
        case KEY_K:
            return Key::K;
        case KEY_L:
            return Key::L;
        case KEY_SEMICOLON:
            return Key::SemiColon;
        case KEY_APOSTROPHE:
            return Key::Apostrophe;
        case KEY_GRAVE:
            return Key::Grave;
        case KEY_LEFTSHIFT:
            return Key::LeftShift;
        case KEY_BACKSLASH:
            return Key::Backslash;
        case KEY_Z:
            return Key::Z;
        case KEY_X:
            return Key::X;
        case KEY_C:
            return Key::C;
        case KEY_V:
            return Key::V;
        case KEY_B:
            return Key::B;
        case KEY_N:
            return Key::N;
        case KEY_M:
            return Key::M;
        case KEY_COMMA:
            return Key::Comma;
        case KEY_DOT:
            return Key::Dot;
        case KEY_SLASH:
            return Key::Slash;
        case KEY_RIGHTSHIFT:
            return Key::RightShift;
        case KEY_KPASTERISK:
            return Key::KpAsterisk;
        case KEY_LEFTALT:
            return Key::LeftAlt;
        case KEY_SPACE:
            return Key::Space;
        case KEY_CAPSLOCK:
            return Key::CapsLock;
        case KEY_F1:
            return Key::F1;
        case KEY_F2:
            return Key::F2;
        case KEY_F3:
            return Key::F3;
        case KEY_F4:
            return Key::F4;
        case KEY_F5:
            return Key::F5;
        case KEY_F6:
            return Key::F6;
        case KEY_F7:
            return Key::F7;
        case KEY_F8:
            return Key::F8;
        case KEY_F9:
            return Key::F9;
        case KEY_F10:
            return Key::F10;
        case KEY_NUMLOCK:
            return Key::NumLock;
        case KEY_SCROLLLOCK:
            return Key::ScrollLock;
        case KEY_KP7:
            return Key::Kp7;
        case KEY_KP8:
            return Key::Kp8;
        case KEY_KP9:
            return Key::Kp9;
        case KEY_KPMINUS:
            return Key::Kpminus;
        case KEY_KP4:
            return Key::Kp4;
        case KEY_KP5:
            return Key::Kp5;
        case KEY_KP6:
            return Key::Kp6;
        case KEY_KPPLUS:
            return Key::Kpplus;
        case KEY_KP1:
            return Key::Kp1;
        case KEY_KP2:
            return Key::Kp2;
        case KEY_KP3:
            return Key::Kp3;
        case KEY_KP0:
            return Key::Kp0;
        case KEY_KPDOT:
            return Key::Kpdot;
        case KEY_ZENKAKUHANKAKU:
            return Key::Zenkakuhankaku;
        case KEY_102ND:
            return Key::Key102nd;
        case KEY_F11:
            return Key::F11;
        case KEY_F12:
            return Key::F12;
        case KEY_RO:
            return Key::Ro;
        case KEY_KATAKANA:
            return Key::Katakana;
        case KEY_HIRAGANA:
            return Key::Hiragana;
        case KEY_HENKAN:
            return Key::Henkan;
        case KEY_KATAKANAHIRAGANA:
            return Key::Katakanahiragana;
        case KEY_MUHENKAN:
            return Key::Muhenkan;
        case KEY_KPJPCOMMA:
            return Key::Kpjpcomma;
        case KEY_KPENTER:
            return Key::Kpenter;
        case KEY_RIGHTCTRL:
            return Key::Rightctrl;
        case KEY_KPSLASH:
            return Key::Kpslash;
        case KEY_SYSRQ:
            return Key::Sysrq;
        case KEY_RIGHTALT:
            return Key::Rightalt;
        case KEY_LINEFEED:
            return Key::Linefeed;
        case KEY_HOME:
            return Key::Home;
        case KEY_UP:
            return Key::Up;
        case KEY_PAGEUP:
            return Key::Pageup;
        case KEY_LEFT:
            return Key::Left;
        case KEY_RIGHT:
            return Key::Right;
        case KEY_END:
            return Key::End;
        case KEY_DOWN:
            return Key::Down;
        case KEY_PAGEDOWN:
            return Key::Pagedown;
        case KEY_INSERT:
            return Key::Insert;
        case KEY_DELETE:
            return Key::Delete;
        case KEY_MACRO:
            return Key::Macro;
        case KEY_MUTE:
            return Key::Mute;
        case KEY_VOLUMEDOWN:
            return Key::Volumedown;
        case KEY_VOLUMEUP:
            return Key::Volumeup;
        case KEY_POWER:
            return Key::Power;
        case KEY_KPEQUAL:
            return Key::Kpequal;
        case KEY_KPPLUSMINUS:
            return Key::Kpplusminus;
        case KEY_PAUSE:
            return Key::Pause;
        case KEY_SCALE:
            return Key::Scale;
        case KEY_KPCOMMA:
            return Key::Kpcomma;
        case KEY_HANGEUL:
            return Key::Hangeul;
        case KEY_HANJA:
            return Key::Hanja;
        case KEY_YEN:
            return Key::Yen;
        case KEY_LEFTMETA:
            return Key::Leftmeta;
        case KEY_RIGHTMETA:
            return Key::Rightmeta;
        case KEY_COMPOSE:
            return Key::Compose;
        case KEY_STOP:
            return Key::Stop;
        case KEY_AGAIN:
            return Key::Again;
        case KEY_PROPS:
            return Key::Props;
        case KEY_UNDO:
            return Key::Undo;
        case KEY_FRONT:
            return Key::Front;
        case KEY_COPY:
            return Key::Copy;
        case KEY_OPEN:
            return Key::Open;
        case KEY_PASTE:
            return Key::Paste;
        case KEY_FIND:
            return Key::Find;
        case KEY_CUT:
            return Key::Cut;
        case KEY_HELP:
            return Key::Help;
        case KEY_MENU:
            return Key::Menu;
        case KEY_CALC:
            return Key::Calc;
        case KEY_SETUP:
            return Key::Setup;
        case KEY_SLEEP:
            return Key::Sleep;
        case KEY_WAKEUP:
            return Key::Wakeup;
        case KEY_FILE:
            return Key::File;
        case KEY_SENDFILE:
            return Key::Sendfile;
        case KEY_DELETEFILE:
            return Key::Deletefile;
        case KEY_XFER:
            return Key::Xfer;
        case KEY_PROG1:
            return Key::Prog1;
        case KEY_PROG2:
            return Key::Prog2;
        case KEY_WWW:
            return Key::Www;
        case KEY_MSDOS:
            return Key::Msdos;
        case KEY_SCREENLOCK:
            return Key::Screenlock;
        case KEY_DIRECTION:
            return Key::Direction;
        case KEY_CYCLEWINDOWS:
            return Key::Cyclewindows;
        case KEY_MAIL:
            return Key::Mail;
        case KEY_BOOKMARKS:
            return Key::Bookmarks;
        case KEY_COMPUTER:
            return Key::Computer;
        case KEY_BACK:
            return Key::Back;
        case KEY_FORWARD:
            return Key::Forward;
        case KEY_CLOSECD:
            return Key::Closecd;
        case KEY_EJECTCD:
            return Key::Ejectcd;
        case KEY_EJECTCLOSECD:
            return Key::Ejectclosecd;
        case KEY_NEXTSONG:
            return Key::Nextsong;
        case KEY_PLAYPAUSE:
            return Key::Playpause;
        case KEY_PREVIOUSSONG:
            return Key::Previoussong;
        case KEY_STOPCD:
            return Key::Stopcd;
        case KEY_RECORD:
            return Key::Record;
        case KEY_REWIND:
            return Key::Rewind;
        case KEY_PHONE:
            return Key::Phone;
        case KEY_ISO:
            return Key::Iso;
        case KEY_CONFIG:
            return Key::Config;
        case KEY_HOMEPAGE:
            return Key::Homepage;
        case KEY_REFRESH:
            return Key::Refresh;
        case KEY_EXIT:
            return Key::Exit;
        case KEY_MOVE:
            return Key::Move;
        case KEY_EDIT:
            return Key::Edit;
        case KEY_SCROLLUP:
            return Key::Scrollup;
        case KEY_SCROLLDOWN:
            return Key::Scrolldown;
        case KEY_KPLEFTPAREN:
            return Key::Kpleftparen;
        case KEY_KPRIGHTPAREN:
            return Key::Kprightparen;
        case KEY_NEW:
            return Key::New;
        case KEY_REDO:
            return Key::Redo;
        case KEY_F13:
            return Key::F13;
        case KEY_F14:
            return Key::F14;
        case KEY_F15:
            return Key::F15;
        case KEY_F16:
            return Key::F16;
        case KEY_F17:
            return Key::F17;
        case KEY_F18:
            return Key::F18;
        case KEY_F19:
            return Key::F19;
        case KEY_F20:
            return Key::F20;
        case KEY_F21:
            return Key::F21;
        case KEY_F22:
            return Key::F22;
        case KEY_F23:
            return Key::F23;
        case KEY_F24:
            return Key::F24;
        case KEY_PLAYCD:
            return Key::Playcd;
        case KEY_PAUSECD:
            return Key::Pausecd;
        case KEY_PROG3:
            return Key::Prog3;
        case KEY_PROG4:
            return Key::Prog4;
        case KEY_DASHBOARD:
            return Key::Dashboard;
        case KEY_SUSPEND:
            return Key::Suspend;
        case KEY_CLOSE:
            return Key::Close;
        case KEY_PLAY:
            return Key::Play;
        case KEY_FASTFORWARD:
            return Key::Fastforward;
        case KEY_BASSBOOST:
            return Key::Bassboost;
        case KEY_PRINT:
            return Key::Print;
        case KEY_HP:
            return Key::Hp;
        case KEY_CAMERA:
            return Key::Camera;
        case KEY_SOUND:
            return Key::Sound;
        case KEY_QUESTION:
            return Key::Question;
        case KEY_EMAIL:
            return Key::Email;
        case KEY_CHAT:
            return Key::Chat;
        case KEY_SEARCH:
            return Key::Search;
        case KEY_CONNECT:
            return Key::Connect;
        case KEY_FINANCE:
            return Key::Finance;
        case KEY_SPORT:
            return Key::Sport;
        case KEY_SHOP:
            return Key::Shop;
        case KEY_ALTERASE:
            return Key::Alterase;
        case KEY_CANCEL:
            return Key::Cancel;
        case KEY_BRIGHTNESSDOWN:
            return Key::Brightnessdown;
        case KEY_BRIGHTNESSUP:
            return Key::Brightnessup;
        case KEY_MEDIA:
            return Key::Media;
        case KEY_SWITCHVIDEOMODE:
            return Key::Switchvideomode;
        case KEY_KBDILLUMTOGGLE:
            return Key::Kbdillumtoggle;
        case KEY_KBDILLUMDOWN:
            return Key::Kbdillumdown;
        case KEY_KBDILLUMUP:
            return Key::Kbdillumup;
        case KEY_SEND:
            return Key::Send;
        case KEY_REPLY:
            return Key::Reply;
        case KEY_FORWARDMAIL:
            return Key::Forwardmail;
        case KEY_SAVE:
            return Key::Save;
        case KEY_DOCUMENTS:
            return Key::Documents;
        case KEY_BATTERY:
            return Key::Battery;
        case KEY_BLUETOOTH:
            return Key::Bluetooth;
        case KEY_WLAN:
            return Key::Wlan;
        case KEY_UWB:
            return Key::Uwb;
        case KEY_UNKNOWN:
            return Key::Unknown;
        case KEY_VIDEO_NEXT:
            return Key::Video_next;
        case KEY_VIDEO_PREV:
            return Key::Video_prev;
        case KEY_BRIGHTNESS_CYCLE:
            return Key::Brightness_cycle;
        case KEY_BRIGHTNESS_AUTO:
            return Key::Brightness_auto;
        case KEY_DISPLAY_OFF:
            return Key::Display_off;
        case KEY_WWAN:
            return Key::Wwan;
        case KEY_RFKILL:
            return Key::Rfkill;
        case KEY_MICMUTE:
            return Key::Micmute;
        default:
            return Key::Other;
    }
}

} // namespace apm
