#ifndef H_EVENT
#define H_EVENT

#include "level.h"
#include "common.h"

typedef enum { EVENT_NONE, EVENT_TIMER, EVENT_LEVEL, EVENT_BUTTON, EVENT_BUILD_SELECTOR } EventType;

typedef enum { MOUSE_CLICK, MOUSE_HOVER } MouseEventType;

typedef enum { UI_SAVE, UI_LOAD, UI_CLEAR, UI_EXIT, UI_THEME } ButtonFunction; 

typedef struct {
	EventType type;
} TimerEvent;

typedef struct {
	EventType type;
    MouseEventType mouse_event_type;
	int row;
    int col;
} LevelEvent;

typedef struct {
	EventType type;
    MouseEventType mouse_event_type;
    CellType cell_type;
    Owner owner;
} BuildSelectorEvent;

typedef struct {
	EventType type;
    MouseEventType mouse_event_type;
	ButtonFunction function;
} ButtonEvent;

typedef union {
    EventType type;
	TimerEvent timer_event;
    LevelEvent level_event;
    ButtonEvent button_event;
    BuildSelectorEvent build_selector_event;
} Event;

#endif

