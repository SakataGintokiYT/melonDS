/*
    Copyright 2016-2017 StapleButter

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#ifndef WX_MAIN_H
#define WX_MAIN_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <SDL2/SDL.h>

enum
{
    ID_OPENROM = 1,
    ID_EXIT,

    ID_RUN,
    ID_PAUSE,
    ID_RESET,

    ID_INPUTCONFIG,
};

class EmuThread;

class wxApp_melonDS : public wxApp
{
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

    SDL_Window* sdlwin;
    SDL_Renderer* sdlrend;
    SDL_Texture* sdltex;

    SDL_Joystick* joy;
    SDL_JoystickID joyid;
    u8 axismask;

    wxMutex* texmutex;
    void* texpixels;
    int texstride;

    int emustatus;
    EmuThread* emuthread;
    wxMutex* emustatuschangemutex;
    wxCondition* emustatuschange;
    wxMutex* emustopmutex;
    wxCondition* emustop;

private:
    wxDECLARE_EVENT_TABLE();

    void OnClose(wxCloseEvent& event);
    void OnCloseFromMenu(wxCommandEvent& event);
    void OnOpenROM(wxCommandEvent& event);

    void OnInputConfig(wxCommandEvent& event);

    void ProcessSDLEvents();

    void OnPaint(wxPaintEvent& event);
    void OnIdle(wxIdleEvent& event);
};

class EmuThread : public wxThread
{
public:
    EmuThread(MainFrame* parent);
    ~EmuThread();

    void EmuRun() { emustatus = 1; }
    void EmuPause() { emustatus = 2; }
    void EmuExit() { emustatus = 0; }

protected:
    virtual ExitCode Entry();
    void ProcessEvents();

    MainFrame* parent;

    SDL_Window* sdlwin;
    SDL_Renderer* sdlrend;
    SDL_Texture* sdltex;

    void* texpixels;
    int texstride;

    int joyid;
    u32 axismask;

    int emustatus;
};

#endif // WX_MAIN_H
