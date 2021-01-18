#ifndef _DEFS_H_
#define _DEFS_H_
#include <iostream>
#include "../SDL2-2.0.14/include/SDL.h"

#define FPS 60
#define _dt (1000/FPS)/1000
//dt_30_60_120_144{0.0333,0.0167,0.0083,0.00694}

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define RAD(a) (a*3.14/180)

struct Mouse { int x, y, offsx, offsy, stateL, stateR; };

struct fPoint { float x, y; };

bool PointInRect(SDL_Point p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }
bool fPointInRect(fPoint p, SDL_Rect r) { return (p.x > r.x && p.x < r.x + r.w && p.y > r.y && p.y < r.y + r.h); }

struct Timer
{
	Uint32 time;
	Timer() { Start(); };
	void Start() { time = SDL_GetTicks(); };
	/*inline*/Uint32 msRead() { return time; };
	/*inline*/float sRead() { return float(time / 1000); };
};

#if 0
template<class tdata>
struct ListItem
{
	tdata data;
	ListItem<tdata>* next = nullptr;
	ListItem<tdata>* prev = nullptr;

	inline ListItem(const tdata& _data) : data(_data) {}
	~ListItem() {}
};

template<class tdata>
class List
{
public:
	ListItem<tdata>* start;
	ListItem<tdata>* end;
private:
	unsigned int size;
public:
	List() { init(); }
	~List() { clear(); }

	void init() { start = end = nullptr; size = 0; }

	unsigned int Count() const { return size; }

	ListItem<tdata>* add(const tdata& item)
	{
		ListItem<tdata>* dataItem = new ListItem<tdata>(item);

		if (start == nullptr)
		{
			start = end = dataItem;
		}
		else
		{
			dataItem->prev = end;
			end->next = dataItem;
			end = dataItem;
		}

		++size;
		return(dataItem);
	}

	bool del(ListItem<tdata>* item)
	{
		if (item != nullptr)
		{
			if (item->prev != nullptr)
			{
				item->prev->next = item->next;
				if (item->next != nullptr) item->next->prev = item->prev;
				else end = item->prev;
			}
			else
			{
				if (item->next != nullptr)
				{
					item->next->prev = nullptr;
					start = item->next;
				}
				else start = end = nullptr;
			}

			RELEASE(item);
			--size;
			return true;
		}
		return false;
	}

	void clear()
	{
		while (start != nullptr)
		{
			ListItem<tdata>* p_next = start->next;
			RELEASE(start);
			start = p_next;
		}
		init();
	}

	tdata& operator [](const unsigned int index)
	{
		for (ListItem<tdata>* p_item = start, long pos = 0;
			p_item != nullptr && pos < index;
			p_item = p_item->next; pos++);
		return p_item->data;
	}

	const tdata& operator [](const unsigned int index) const
	{
		for (ListItem<tdata>* p_item = start, long pos = 0;
			p_item != nullptr && pos < index;
			p_item = p_item->next; pos++);
		return (p_item->data != nullptr) ? p_item->data : nullptr;//assert
	}
};
#endif

#endif
